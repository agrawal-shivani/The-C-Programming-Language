#include <math.h>
#include <stdio.h>
#include <stdlib.h>   /* for atof() */
#include <string.h>

#define MAXOP       100   /* max size of operand or operator */
#define NUMBER      '0'   /* signal that a number was found */
#define IDENTIFIER  '1'

int getop(char []);
void push(double);
double pop(void);
double top(void);
void clear(void);
void func(char []);
void ungets(char []);

/* reverse Polish calculator */
main(int argc, char* argv[])
{
    int type, i;
    double op1, op2, v;
    char s[MAXOP];
    double variable[26] = { 0 };

    while (--argc > 0) {
        ungets(" ");
        ungets(*++argv);
        switch (getop(s)) {
        case NUMBER:
            push(atof(s));
            break;
        case IDENTIFIER:
            func(s);
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if (op2 != 0.0)
                push((int)pop() % (int)op2);
            else
                printf("error: zero divisor\n");
            break;
        case '?':
            printf("\t%.8g\n", top());
            break;
        case '#':
            op2 = pop();
            push(op2);
            push(op2);
            break;
        case '~':
            op1 = pop();
            op2 = pop();
            push(op1);
            push(op2);
            break;
        case '!':
            clear();
            break;
        case '=':
            push(variable[i] = pop());
            pop();
            break;
        case '\n':
            v = pop();
            printf("\t%.8g\n", v);
            break;
        default:
            if (type >= 'A' && type <= 'Z') {
                push(variable[type - 'A']);
                i = type - 'A';
            } else
                printf("error: unknown command %s\n", s);
            argc = 0;
            break;
        }
    }
    printf("\t%.8g\n", pop());
    return 0;
}

#define MAXVAL  100   /* maximum depth of val stack */

int sp = 0;           /* next free stack position */
double val[MAXVAL];   /* value stack */

/* push:  push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

/* pop:  pop and return top value from stack */
double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

double top(void)
{
    if (sp > 0)
        return val[sp - 1];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

void clear(void)
{
    sp = 0;
}

void func(char s[])
{
    double op2;
    if (strcmp(s, "sin") == 0)
        push(sin(pop()));
    else if (strcmp(s, "exp") == 0)
        push(exp(pop()));
    else if (strcmp(s, "pow") == 0) {
        op2 = pop();
        push(pow(pop(), op2));
    } else
        printf("error: unknown command %s\n", s);
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop:  get next operator or numeric operand */
int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    i = 0;
    if (islower(c)) {
        while (islower(s[++i] = c = getch()))
            ;
        s[i] = '\0';
        if (c != EOF)
            ungetch(c);
        return IDENTIFIER;
    }
    if (!isdigit(c) && c != '.' && c != '-')
        return c;     /* not a number */
    if (c == '-')
        if (!isdigit(s[++i] = c = getch())) {
            ungetch(c);
            return '-';
        }
    if (isdigit(c))   /* collect integer part */
        while (isdigit(s[++i] = c = getch()))
            ;
    if (c == '.')     /* collect fraction part */
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

#define BUFSIZE 100

int buf[BUFSIZE];   /* buffer for ungetch */
int bufp = 0;       /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* push character back on input */
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

void ungets(char s[])
{
    int i = strlen(s) - 1;

    if (bufp + i + 1 >= BUFSIZE)
        printf("ungets: too many characters\n");
    else
        while (i >= 0)
            ungetch(s[i--]);
}
