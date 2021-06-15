#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <stack>

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

/* end */

vector<std::string> to_evaluate;

/* parser */

string printRoman(int number)
{
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
    return word;
}

pair<int,bool> solution(string roman) {
    int sum = 0;
    int i = 0;
    while (roman[i] != '\0') {
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
            return make_pair(0,false);
        }
        i++;
    }
    if (roman != printRoman(sum)) {
        return make_pair(0,false);
    }
    return make_pair(sum,true);
}

string no_space(string equation) {
    string new_word;
    for (unsigned int i = 0; i < equation.size(); i++) {
        if (equation[i] != 32) {
            new_word += equation[i];
        }
    }
    return new_word;
};

bool digitize(string equation) {
    string temp;
    int open_braces = 0;
    int end_braces = 0;
    for (int i = 0; i < equation.size(); i++) {
        temp.push_back(equation[i]);
        if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '(' || equation[i] == ')' || i + 1 == equation.size()) {
            if (equation[i] == '(') {
                open_braces++;
            }
            if (equation[i] == ')') {
                end_braces++;
            }
            temp.pop_back();
            if (!temp.empty()) {
                to_evaluate.push_back(temp);
            }
            temp.clear();
            temp.push_back(equation[i]);
            to_evaluate.push_back(temp);
            temp.clear();
        }
    }
    for (auto & each : to_evaluate) {
        if (each.size() != 1) {
            if (!(solution(each).second)) {
                return false;
            }
            each = to_string(solution(each).first);
        }
    }
    if (open_braces != end_braces) {
        cout << endl << "Error ( Inappropriate braces! )" << endl;
        return false;
    }
    return true;
}

/* end */

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

            Node* leftNode = localStack.top();
            localStack.pop();

            Node* rightNode = localStack.top();
            localStack.pop();

            node->left = leftNode;
            node->right = rightNode;

            localStack.push(node);
        }
    }

    return localStack.top();
}

/* end */

int main() {
    string equation;
    cout << "Enter Expression :\n";
    char c;
    do {
        c = getchar();
        if(c != '\n'){
            equation += c;
        }
    } while (c != '\n');
    equation = no_space(equation);
    if (digitize(equation)) {
        for (string &every : to_evaluate) {
            cout << "[" << every << "] ";
        }
    }
    cout << endl;

    vector<string> postfix = infixToPostfix(to_evaluate);

    cout << "[";
    for (string &item : postfix) {
        cout << item << ", ";
    } cout << "]" << endl;

    cout << "here?" << endl;

    Node* root = postfixToBinaryExpressionTree(postfix);

    cout << "result = " << root->evaluate() << endl;
    return 0;
}
