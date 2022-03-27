#include <string>
#include <stack>
#include <iostream>
#include <math.h>
using namespace std;

struct element
{
    string value;
    struct element *next;
};

void push(struct element **anchor, string value)
{
    struct element *new_element = new struct element;
    new_element->value = value;
    new_element->next = nullptr;
    if (*anchor == nullptr)
    {
        *anchor = new_element;
    }
    else
    {
        struct element *current = *anchor;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new_element;
    }
}

struct element *pop(struct element **anchor)
{
    struct element *current = *anchor;
    if (current == nullptr)
    {
        return nullptr;
    }else{
        // TODO: Fehler wenn nur ein Element in der Liste ist
        while(current->next->next != nullptr)
        {
            current = current->next;
        }
        struct element *temp = current->next;
        current->next = nullptr;
        return temp;
    }
}

struct element *top(struct element **head)
{
    struct element *current = *head;
    if (current == nullptr)
    {
        return nullptr;
    }
    else
    {
        while(current->next != nullptr)
        {
            current = current->next;
        }
        return current;
    }
}

bool is_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

bool is_number(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

string read_number(string &s, int i)
{
    string number = "";
    while (i < s.length() && is_number(s[i]))
    {
        number += s[i];
        i++;
    }
    return number;
}

int get_precedence(char c)
{
    if (c == '+' || c == '-')
        return 1;
    if (c == '*' || c == '/' || c == '%')
        return 2;
    if (c == '^')
        return 3;
    return 0;
}

stack<string> to_rpn(string &s){
    int i = 0, l = s.length();
    struct element *fin = nullptr;
    struct element *ops = nullptr;

    while (i < l){
        char token = s[i];

        if (is_number(token))
        {
            string number = read_number(s, i);
            push(&fin, number);
            i += number.length();
            continue;
        }
        if (is_operator(token))
        {
            if (ops != nullptr && get_precedence(token) <= get_precedence(ops.top()[0]))
            {
                
                push(&fin,top(&ops)->value);
                pop(&ops);
            }
            push(&ops,string(1, token));
            i++;
            continue;
        }
        if (token == '(')
        {
            push(&ops,string(1, token));
            i++;
            continue;
        }
        if (token == ')')
        {
            do {
                string op = top(&ops)->value;
                pop(&ops);
                if (op[0] == ')')
                    break;
                push(&fin,op);
            }while( ops != nullptr);
            i++;
            continue;
        }
        i++;
    }
    while (ops != nullptr)
    {
        fin.push(ops.top());
        ops.pop();
    }
    return fin;
}

string get_value_of_expression(string b, string a, char op)
{
    if (op == '+')
        return to_string(stod(b) + stod(a));
    if (op == '-')
        return to_string(stod(b) - stod(a));
    if (op == '*')
        return to_string(stod(b) * stod(a));
    if (op == '/')
        return to_string(stod(b) / stod(a));
    if (op == '%')
        return to_string(stoi(b) % stoi(a));
    if (op == '^')
        return to_string(pow(stod(b), stod(a)));
    return "";
}

string calculate_rpn(stack<string> &rpn)
{
    stack<string> values;
    while (!rpn.empty())
    {
        string token = rpn.top();
        rpn.pop();
        if (is_operator(token[0]))
        {
            string a = values.top();
            values.pop();
            string b = values.top();
            values.pop();
            values.push(get_value_of_expression(b, a, token[0]));
        }
        else
        {
            values.push(token);
        }
    }
    return values.top();
}

int main()
{
    cout << "test" << endl;
    string s = "2+3";
    stack<string> rpn = to_rpn(s);
    cout << calculate_rpn(rpn);
    return 0;
}