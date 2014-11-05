#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>

#include "cycle.h"
#include "field.h"

class Console : public QObject
{
    Q_OBJECT

    quint16 done;

    Field *field;
    QList<Cycle*> cycle;

    friend class Cycle;

public:

    explicit Console(QObject *parent = 0);

public slots:

    void dataErrorField(QString);
    void checkFinish();

signals:

    void consoleDone();
    void finished();
};

#endif // CONSOLE_H
