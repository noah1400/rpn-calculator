#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct elem
{
    struct elem* next;
    char *element;
} ELEMENT;


void print_rpn(ELEMENT *rpn)
{
    printf("RPN: \n");
    while (rpn != NULL)
    {
        printf("%s ", rpn->element);
        rpn = rpn->next;
    }
    printf("\n");
}

int is_operator(char element)
{
    if (element == '+' || element == '-' || element == '*' || element == '/' || element == '%')
        return 1;
    else
        return 0;
}

int is_number(char element)
{
    if ((element >= '0' && element <= '9') || element == '.')
        return 1;
    else
        return 0;
}

char *read_number(char *str, int i)
{
    int count = 0;
    while(is_number(str[count+i]))
    {
        count++;
    }
    char *number = (char*)malloc(sizeof(char)*(count+1));
    for (int j = 0; j < count; j++)
    {
        number[j] = str[i+j];
    }
    number[count] = '\0';
    return number;
}

ELEMENT *top(ELEMENT *stack)
{
    ELEMENT *top = stack;
    if ( stack == NULL)
    {
        return NULL;
    }
    while (top->next != NULL)
    {
        top = top->next;
    }
    return top;
}

char * push(ELEMENT **anchor, char * element)
{
    printf("    push %s\n", element);
    printf("        strlen %d\n", strlen(element));
    ELEMENT *new = (ELEMENT *)malloc(sizeof(ELEMENT)+strlen(element)+1);
    new->next = NULL;
    new->element = (char*)malloc(sizeof(char)*(strlen(element)+1));
    strcpy(new->element, element);
    printf("        new->element %s\n", new->element);
    
    if (*anchor == NULL)
    {
        *anchor = new;
        return element;
    }
    else
    {
        ELEMENT *last = top(*anchor);
        last->next = new;
    }
    
    return element;
}


ELEMENT * pop(ELEMENT **anchor)
{
    if (*anchor == NULL)
    {
        return NULL;
    }
    else
    {
        ELEMENT *last = *anchor;
        ELEMENT *prev = *anchor;
        while(last->next != NULL){
            prev = last;
            last = last->next;
        }
        ELEMENT *element = last;
        prev->next = NULL;
        if (*anchor == last)
        {
            *anchor = NULL;
        }
        printf("    pop %s\n", element->element);
        return element;
    }
}

char get_precedence(char element)
{
    if (element == '+' || element == '-')
        return 1;
    else if (element == '*' || element == '/' || element == '%')
        return 2;
    else
        return 0;
}

ELEMENT * to_rpn(char *str)
{
    int i = 0, l = strlen(str);
    ELEMENT *fin = NULL;
    ELEMENT *ops = NULL;

    printf("string length: %d\n", l);

    while ( i < l ){
        printf("i: %d\n", i);
        char token = str[i];
        printf("Processing token: %c\n", token);
        if ( is_number(token) )
        {
            printf("    Found number in token: %c\n", token);
            char *number = read_number(str, i);
            printf("    Number: %s\n", number);
            push(&fin, number);
            i += strlen(number);
            continue;
        }
        if ( is_operator(token) )
        {
            printf("    Found operator in token: %c\n", token);
            ELEMENT *top_op = top(ops);
            if (top_op != NULL && get_precedence(token) <= get_precedence(top_op->element[0]))
            {
                char *element = pop(&ops)->element;
                printf("    Pushing into fin: %s\n", element);
                push(&fin, element);
            }
            char tmp[2] = {token, '\0'};
            printf("    Pushing into ops: %s\n", tmp);
            push(&ops, tmp);
            i++;
            printf("    i: %d\n", i);
            continue;
        }
        if ( token == '(' )
        {
            printf("    Found left parenthesis in token: %c\n", token);
            char tmp[2] = {token, '\0'};
            push(&ops, tmp);
            i++;
            continue;
        }
        if ( token == ')' )
        {
            printf("    Found right parenthesis in token: %c\n", token);
            do
            {
                ELEMENT *operator = pop(&ops);
                if (operator->element[0] == '(')
                {
                    break;
                }
                push(&fin, operator->element);
            } while (ops != NULL);
            i++;
            continue;
        }
        i++;
    }
    while (ops != NULL)
    {
        char *element = pop(&ops)->element;
        push(&fin, element);
    }
    return fin;
}

char *get_value_of_expression(char *b, char *a, char operator)
{
    printf("    get_value_of_expression: a: %s, b: %s\n", a, b);

    char type_of_a = 0;
    char type_of_b = 0;

    if (strstr(a, ".") != NULL)
    {
        type_of_a = 1;
    }
    if (strstr(b, ".") != NULL)
    {
        type_of_b = 1;
    }
    
    char *buffer = (char*)malloc(sizeof(char)*1024);

    double b_value = atof(b);
    double a_value = atof(a);

    printf("    get_value_of_expression: a_value: %f, b_value: %f\n", a_value, b_value);
    printf("    expression: %f%c%f", b_value, operator, a_value);
    switch(operator){
        case '+':
            sprintf(buffer, "%f", b_value + a_value);
            break;
        case '-':
            sprintf(buffer, "%f", b_value - a_value);
            break;
        case '*':
            sprintf(buffer, "%f", b_value * a_value);
            break;
        case '/':
            sprintf(buffer, "%f", b_value / a_value);
            break;
        case '%':
            sprintf(buffer, "%d", (int)b_value % (int)a_value);
            break;
    }
    char *result = (char*)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(result, buffer);
    printf("    get_value_of_expression: result: %s\n", result);
    free(buffer);
    return result;
}

char *calculate_rpn(ELEMENT *rpn){
    printf("calculating rpn\n");
    ELEMENT *stack = NULL;
    while (rpn != NULL){
        char *element = rpn->element;
        printf("    calculate_rpn: element: %s\n", element);
        if (is_operator(element[0]))
        {
            char *a = pop(&stack)->element;
            char *b = pop(&stack)->element;
            char *result = get_value_of_expression(b, a, element[0]);
            push(&stack, result);
            free(a);free(b);
        }
        else
        {
            push(&stack, element);
        }
        rpn = rpn->next;
    }
    return stack->element;
}


int main(int argc, char *argv[])
{
    
    
    char *expression = "4/23-3.5";
    printf("size of ELEMENT: %d\n", sizeof(ELEMENT));
    printf("%s\n", expression);
    ELEMENT *rpn = to_rpn(expression);
    print_rpn(rpn);
    char *result = calculate_rpn(rpn);
    printf("Result: %s\n", result);
    return 0;
}
