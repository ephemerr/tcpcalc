#include "calc.h"

inline double mult (double a, double b) {
    return a*b;
}
inline double div (double a, double b) {
    return a/b;
}
inline double sum (double a, double b) {
    return a+b;
}
inline double min (double a, double b) {
    return a-b;
}
typedef double (*Arithmetic)(double, double);

enum {
    CALC_OP_NONE,
    CALC_OP_SUM,
    CALC_OP_MIN,
    CALC_OP_MULT,
    CALC_OP_DIV,
    CALC_OP_UNKNOWN
};

struct Operation {
    int priority;
    Arithmetic calc;
};

Operation operations[CALC_OP_UNKNOWN] =
{
    {0,NULL},
    {1, sum},
    {1, min},
    {2, mult},
    {2, div}
};

int op_bind(char op){
    switch (op) {
        case '+': return CALC_OP_SUM;
        case '-': return CALC_OP_MIN;
        case '*': return CALC_OP_MULT;
        case '/': return CALC_OP_DIV;
    }
    return CALC_OP_NONE;
}

/// Returns 0 at operator and >0(size) at operand
int test_char(const QString &str, int i) {
    if (i >= str.length()) return 0;
    char c = str[i].toLatin1();
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
        return 1 + test_char(str,i+1);
    }
    return 0;
}

/// Convert string to reverse polish notation (RPN) list of lexems
QList<Lexem> polish(const QString& str) {
    int len = str.length();
    QList<Lexem> res;
    QStack<Lexem> station;
    for (int i=0; i < len; i++)
    {
        int operand = test_char(str,i);
        char operator_id = operand ? 0 : op_bind(str[i].toLatin1());
        int len = operand ? operand : 1;
        Lexem lexem = {str.mid(i,len), operator_id};
        i += len-1;
        if (operator_id) {
            while (station.size()) {
                Lexem &top = station.top();
                if (operations[operator_id].priority <=
                        operations[top.op_id].priority) {
                    res.append(station.pop());
                } else {
                    break;
                }
            }
            station.push(lexem);
        } else {
            res.append(lexem);
        }
    }
    while (station.size()) {
        res.append(station.pop());
    }
    return res;
}

QString lexem_print(QList<Lexem> to_print) {
    QString res = "";
    foreach (Lexem lexem, to_print) {
        res += lexem.str;
        if (!lexem.op_id) {
            res += ",";
        }
    }
    return res;
}

/// Go through RPN list of lexems from end to begin
double calc_rpn(QList<Lexem> &rpn, int &i)
{
    Lexem &current = rpn[i];
    i--;
    if (current.op_id == CALC_OP_NONE) {
        double res = current.str.toDouble();
        return res;
    } else {
        double a = calc_rpn(rpn,i);
        double b = i < 0 ? 0 : calc_rpn(rpn,i); /// -2-2 fix
        Arithmetic func = operations[current.op_id].calc;
        return func(b,a);
    }
}

QString calc_formula(QString formula) {
    QList<Lexem> pol = polish(formula);
    QString rpn = lexem_print(pol);
    int i = pol.size()-1;
    double res = calc_rpn(pol, i);
    return QString::number(res);
}
