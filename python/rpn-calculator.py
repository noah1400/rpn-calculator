from math import factorial
def readNumber(string: str,index: int):
    number = ""
    while index < len(string) and isNumber(string[index]):
        number += string[index]
        index += 1
    return float(number), index


def isOperator(token):
    operators = ['+', '-', '*', '/', '%','^']
    return token in operators

def isNumber(token):
    return (token >= '0' and token <= '9') or token == "."



def toRpn(string: str):
    """
    Converts infix notation to reverse polish notation
    """
    precedence = {
        '(': 0,
        '-': 1,
        '+': 1,
        '*': 2,
        '/': 2,
        '%': 2,
        '^': 3,
    }

    i = 0
    fin = []
    ops = []

    while i < len(string):
        token = string[i]
        if isNumber(token):
            number, i = readNumber(string,i)
            fin.append(number)
            continue
        if isOperator(token):
            top = ops[-1] if ops else None
            if top is not None and precedence[top] >= precedence[token]:
                fin.append(ops.pop())
            ops.append(token)
            i += 1
            continue
        if token == '(':
            ops.append(token)
            i += 1
            continue
        if token == ')':
            while True:
                operator = ops.pop()
                if operator == '(':
                    break
                fin.append(operator)
                if not ops:
                    break
            i += 1
            continue
        i += 1
    while ops:
        fin.append(ops.pop())
    return fin

def calculate_rpn(rpn: list):
    """
    Calculates the result of an expression in reverse polish notation
    """
    stack = []
    for token in rpn:
        if isOperator(token):
            a = stack.pop()
            b = stack.pop()
            if token == '+':
                stack.append(b + a)
            elif token == '-':
                stack.append(b - a)
            elif token == '*':
                stack.append(b * a)
            elif token == '/':
                stack.append(b / a)
            elif token == '%':
                stack.append(b % a)
            elif token == '^':
                stack.append(b ** a)
        else:
            stack.append(token)
    return stack[0]
        

print ("90165: ", calculate_rpn(toRpn("27+38+81+48*33*53+91*53+82*14+96")))
print ("616222: ", calculate_rpn(toRpn("22*26*53+66*8+7*76*25*44+78+100")))
print (calculate_rpn(toRpn("(22+4)*4")))
print (calculate_rpn(toRpn("3^2+2")))