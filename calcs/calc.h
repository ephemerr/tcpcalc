#ifndef CALCS_H
#define CALCS_H

#include <QtCore>

typedef struct { QString str; char op_id;} Lexem;
typedef QList<Lexem>::iterator Iterator;

QString calc_formula(QString formula);

#endif // CALCS_H
