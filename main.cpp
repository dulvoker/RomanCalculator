#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <chrono>

using namespace std;

/* postfix generator */

bool isNumber(const string& s) {
    string::const_iterator it = s.begin();
    if (s.empty()) return false;

    if (*it == '-') {
        if (s.size() == 1) return false;
        ++it;
    }

    while (it != s.end() && isdigit(*it)) ++it;

    return it == s.end();
}

int precedence(const string& operation) {
    if (operation == "+" || operation == "-") return 1;
    if (operation == "*" || operation == "/") return 2;
    return 0;
}

vector<string> infixToPostfix(const vector<string>& infix) {
    stack<string> localStack = stack<string>();
    localStack.push("#");
    vector<string> postfix;

    for (const string& item: infix) {
        if (isNumber(item)) {
            postfix.push_back(item);
        } else if (item == "(") {
            localStack.push("(");
        } else if (item == ")") {
            while (localStack.top() != "#" && localStack.top() != "(") {
                postfix.push_back(localStack.top());
                localStack.pop();
            }
            localStack.pop();
        } else {
            if (precedence(item) > precedence(localStack.top())) {
                localStack.push(item);
            } else {
                while (localStack.top() != "#" && precedence(item) <= precedence(localStack.top())) {
                    postfix.push_back(localStack.top());
                    localStack.pop();
                }
                localStack.push(item);
            }
        }
    }

    while (localStack.top() != "#") {
        postfix.push_back(localStack.top());
        localStack.pop();
    }

    return postfix;
}

int evaluatePostfix(const vector<string>& postfix) {
    stack<int> localStack;

    for (string token : postfix) {
        if (isNumber(token)) {
            localStack.push(stoi(token));
        } else {
            int theLastOne = localStack.top();
            localStack.pop();
            int thePrevOfLastOne = localStack.top();
            localStack.pop();

            int temp;

            switch (token[0]) {
                case '*' :
                    temp = theLastOne * thePrevOfLastOne;
                    break;
                case '/' :
                    temp = thePrevOfLastOne / theLastOne;
                    break;
                case '+':
                    temp = thePrevOfLastOne + theLastOne;
                    break;
                case '-' :
                    temp = thePrevOfLastOne - theLastOne;
                    break;
            }

            localStack.push(temp);
        }
    }

    return localStack.top();
}

/* end */

string intToRoman(int number)
{
    if (number == 0) {
        return "Z";
    }

    bool isNegative = false;

    if (number < 0) {
        isNegative = true;
        number *= -1;
    }

    int num[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
    string sym[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
    int i=12;
    string word;
    while(number>0)
    {
        int div = number/num[i];
        number = number%num[i];
        while(div--)
        {
            word += string(sym[i]);
        }
        i--;
    }
    return isNegative ? "-" + word : word;
}

int romanToInt(string roman) {
    int sum = 0;
    int i = 0;
    bool isNegative = false;

    if (roman[0] == '-') {
        isNegative = true;
        ++i;
    }

    while (roman[i] != '\0') {
        if (roman[i] == 'Z') return 0;

        if(roman[i] == 'I'){
            sum = sum + 1;
        }
        else if(roman[i] == 'V') {
            if (roman[i-1] == 'I') {
                sum = sum - 2;
            }
            sum = sum + 5;
        }
        else if(roman[i] == 'X') {
            if (roman[i-1] == 'I') {
                sum = sum - 2;
            }
            sum = sum + 10;
        }
        else if(roman[i] == 'L') {
            if (roman[i-1] == 'X') {
                sum = sum - 20;
            }
            sum = sum + 50;
        }
        else if(roman[i] == 'C') {
            if (roman[i-1] == 'X') {
                sum = sum - 20;
            }
            sum = sum + 100;
        }
        else if(roman[i] == 'D') {
            if (roman[i-1] == 'C') {
                sum = sum - 200;
            }
            sum = sum + 500;
        }
        else if(roman[i] == 'M') {
            if (roman[i-1] == 'C') {
                sum = sum - 200;
            }
            sum = sum + 1000;
        }
        else {
            throw std::runtime_error("Invalid roman: " + roman);
        }
        i++;
    }

    if (isNegative) {
        if (roman.substr(1) != intToRoman(sum)) {
            throw std::runtime_error("Invalid roman: " + roman);
        }
    } else {
        if (roman != intToRoman(sum)) {
            throw std::runtime_error("Invalid roman: " + roman);
        }
    }

    return isNegative ? -sum : sum;
}

/* Node class */

class Node {
    int value{};
    string operationType;
public:
    bool isOperation = false;
    Node* left = nullptr;
    Node* right = nullptr;

    explicit Node(string operationType) {
        this->isOperation = true;
        this->operationType = std::move(operationType);
        this->left = nullptr;
        this->right = nullptr;
    }

    explicit Node(int value) {
        this->isOperation = false;
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }

    int evaluate() {
        if (this->isOperation) {
            int leftVal = this->left->evaluate(),
                    rightVal = this->right->evaluate();

            switch (this->operationType[0]) {
                case '+':
                    return leftVal + rightVal;
                case '-':
                    return leftVal - rightVal;
                case '*':
                    return leftVal * rightVal;
                case '/':
                    if (rightVal == 0) {
                        throw std::runtime_error("Can't divide by 0");
                    }

                    return leftVal / rightVal;
            }
        }

        return this->value;
    }
};

/* end */

/* postfix to Binary Expression Tree */

Node* postfixToBinaryExpressionTree(const vector<string>& postfix) {
    stack<Node*> localStack;

    for (const string& item: postfix) {
        if (isNumber(item)) {
            int intVal = stoi(item);
            Node* node = new Node(intVal);
            localStack.push(node);
        } else {
            Node* node = new Node(item);

            Node* rightNode = localStack.top();
            localStack.pop();

            Node* leftNode = localStack.top();
            localStack.pop();

            node->left = leftNode;
            node->right = rightNode;

            localStack.push(node);
        }
    }

    return localStack.top();
}

/* end */

/* validation */

bool isOperationToken(string character) {
    if (character.size() != 1) {
        return false;
    }
    switch (character[0]) {
        case '+':
        case '-':
        case '*':
        case '/':
            return true;
    }

    return false;
}

string removeSpaces(string equation) {
    string new_word;
    for (unsigned int i = 0; i < equation.size(); i++) {
        if (equation[i] != 32) {
            new_word += equation[i];
        }
    }
    return new_word;
}

vector<string> tokenize(string rawEquation) {
    vector<string> tokens;
    string temp;

    temp.push_back(rawEquation[0]);

    if (isOperationToken(temp) && rawEquation[0] != '-') {
        throw std::logic_error("Invalid operations");
    }

    temp.clear();

    for (size_t i = 0; i < rawEquation.size(); ++i) {
        char currentChar = rawEquation[i];
        if (currentChar == '-' && temp.empty()) {
            if (i == 0) {
                temp = "-";
                continue;
            }
            if (i > 0 && !tokens.empty() &&
                (isOperationToken(tokens.back()) || tokens.back() == "(")) {
                temp = "-";
                continue;
            }
        }

        switch (currentChar) {
            case '-':
                // на случай унарного минуса
                // for unary minus cases
            case '+':
            case '*':
            case '/':
            case '(':
            case ')':
                if (!temp.empty()) {
                    tokens.push_back(temp);
                    temp.clear();
                }

                tokens.push_back(string(1, currentChar));
                continue;
        }

        temp.push_back(currentChar);
    }

    if (!temp.empty()) {
        tokens.push_back(temp);
    }

    return tokens;
}

vector<string> convertRomanTokensToInts(const vector<string>& tokens) {
    vector<string> result;

    for (string token: tokens) {
        if (token[0] == '-' && token.size() > 1) {
            int intRoman = romanToInt(token);
            result.push_back(to_string(intRoman));
            continue;
        }
        switch (token[0]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
                result.push_back(token);
                continue;
        }

        int intRoman = romanToInt(token);
        result.push_back(to_string(intRoman));
    }

    return result;
}

void validateEquation(const vector<string>& tokens) {
    vector<string> valid_tokens;

    int openingBraces = 0,
            closingBraces = 0;

    for (size_t i = 0; i < tokens.size(); i++) {

        string currentToken = tokens[i];

        if (currentToken == "(") {
            openingBraces++;
        }
        if (currentToken == ")") {
            closingBraces++;
        }

        if (isOperationToken(currentToken) && i != 0 &&
            isOperationToken(tokens[i + 1]) &&
            tokens[i+1][0] != '-') {
            throw std::logic_error("Invalid operations");
        }

        if (isOperationToken(currentToken) && i == tokens.size() - 1) {
            throw std::logic_error("Invalid operations");
        }
    }

    if (openingBraces != closingBraces) {
        throw openingBraces > closingBraces ?
              std::logic_error("You have more opening brace") :
              std::logic_error("You have more closing brace");
    }
}

/* end */

void measureExecutionTime() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    for (string line; getline(cin, line);) {
        try {
            const string& rawEquation = line;

            string spacelessEquation = removeSpaces(rawEquation); // O(len(str))
            vector<string> tokens = tokenize(spacelessEquation); // O(len(str))

            validateEquation(tokens); // O(num(tokens) ~> O(len(str))

            tokens = convertRomanTokensToInts(tokens); // O(num(tokens)/2) + O((num(tokens)/2 + 1) * X)

            vector<string> postfix = infixToPostfix(tokens);
            cout << intToRoman(evaluatePostfix(postfix)) << endl;


// Старое решение
//            Node* root = postfixToBinaryExpressionTree(postfix);
//            cout << intToRoman(root->evaluate()) << endl;
        } catch (exception& ex) {
            cout << "error: " + string(ex.what()) << endl;
        }
    }

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_double.count() << std::endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    /*
    for (string line; getline(cin, line);) {
        try {
            const string& rawEquation = line;

            string spacelessEquation = removeSpaces(rawEquation); // O(len(str))
            vector<string> tokens = tokenize(spacelessEquation); // O(len(str))

            validateEquation(tokens); // O(num(tokens) ~> O(len(str))

            tokens = convertRomanTokensToInts(tokens); // O(num(tokens)/2) + O((num(tokens)/2 + 1) * X)

            vector<string> postfix = infixToPostfix(tokens);

            Node* root = postfixToBinaryExpressionTree(postfix);

            cout << intToRoman(root->evaluate()) << endl;
        } catch (exception& ex) {
            cout << "error: " + string(ex.what()) << endl;
        }
    }
    */

    measureExecutionTime();

    return 0;
}