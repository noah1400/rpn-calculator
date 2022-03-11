#include <string.h>
#include <stdlib.h>

typedef struct elem
{
    struct elem* next;
    char *element;
} ELEMENT;

int is_operator(char element)
{
    if (element == '+' || element == '-' || element == '*' || element == '/' || element == '%')
        return 1;
    else
        return 0;
}

int is_number(char element)
{
    if (element >= '0' && element <= '9' || element == '.')
        return 1;
    else
        return 0;
}

char *read_number(char *str, int i)
{
    int count = 0;
    while(is_number(str[count+1]))
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

char * push(ELEMENT *anchor, char * element)
{
    ELEMENT *new = (ELEMENT *)malloc(sizeof(ELEMENT)+strlen(element)+1);
    new->next = NULL;
    strcpy(new->element, element);
    
    if (anchor == NULL)
    {
        anchor = new;
        return new;
    }
    else
    {
        ELEMENT *last = top(anchor);
        last->next = new;
    }
    
    return new;
}


char * pop(ELEMENT *anchor)
{
    if (anchor == NULL)
    {
        return NULL;
    }
    else
    {
        ELEMENT *last = anchor;
        ELEMENT *prev = anchor;
        while(last->next != NULL){
            prev = last;
            last = last->next;
        }
        char *element = last->element;
        prev->next = NULL;
        if (anchor == last)
        {
            anchor = NULL;
        }
        free(last);
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

    while ( i < l ){
        char token = str[i];
        if ( is_number(token) )
        {
            char *number = read_number(str, i);
            fin = push(fin, number);
            i += strlen(number);
            continue;
        }
        if ( is_operator(token) )
        {
            ELEMENT *top_op = top(ops);
            if (top_op != NULL && get_precedence(token) <= get_precedence(top_op->element[0]))
            {
                char *element = pop(ops);
                fin = push(fin, element);
            }
            ops = push(ops, &token);
            i++;
            continue;
        }
        if ( token == '(' )
        {
            ops = push(ops, &token);
            i++;
            continue;
        }
        if ( token == ')' )
        {
            do
            {
                ELEMENT *operator = pop(ops);
                if (operator->element[0] == '(')
                {
                    break;
                }
                fin = push(fin, operator);
            } while (ops != NULL);
            i++;
            continue;
        }
        i++;
        while (ops != NULL)
        {
            char *element = pop(ops);
            fin = push(fin, element);
        }
    }
    return fin;
}

char *get_value_of_expression(char *b, char *a, char operator)
{
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
    
    switch (type_of_b)
    {
    case 0:
        switch (type_of_a)
        {
        case 0:
            switch (operator)
            {
                case '+':
                    int result = atoi(b) + atoi(a);
                    char *s = (char*)malloc(sizeof(char)*11);
                    char * ret = (char*)malloc(sizeof(char)*(strlen(itoa(result,s,10))+1));
                    strcpy(ret, itoa(result,s,10));
                    free(s);
                    return ret;
                case '-':
                    int result = atoi(b) - atoi(a);
                    s = (char*)malloc(sizeof(char)*11);
                    ret = (char*)malloc(sizeof(char)*(strlen(itoa(result,s,10))+1));
                    strcpy(ret, itoa(result,s,10));
                    free(s);
                    return ret;
                case '*':
                    int result = atoi(b) * atoi(a);
                    s = (char*)malloc(sizeof(char)*11);
                    ret = (char*)malloc(sizeof(char)*(strlen(itoa(result,s,10))+1));
                    strcpy(ret, itoa(result,s,10));
                    free(s);
                    return ret;
                case '/':
                    result = atoi(b) / atoi(a);
                    s = (char*)malloc(sizeof(char)*11);
                    ret = (char*)malloc(sizeof(char)*(strlen(itoa(result,s,10))+1));
                    strcpy(ret, itoa(result,s,10));
                    free(s);
                    return ret;
            }
            break;
        case 1:
            break;
        default:
            break;
        }
        break;
    case 1:
        break;
    default:
        break;
    }
}