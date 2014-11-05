#include "console.h"



Console::Console(QObject *parent) :
    QObject(parent)
{
    done = 0;
    field = new Field(this);

    connect(this, SIGNAL(consoleDone()), field, SLOT(buildField()));

    for (quint16 k = 0; k < THREADS; k++)
    {
        cycle << new Cycle(this,field,k);                            // Create the backend components

        connect(cycle[k], SIGNAL(finished()), cycle[k], SLOT(resetSimulation()));
        //connect(cycle[k], SIGNAL(finished()), this, SLOT(checkFinish()));
        connect(cycle[k], SIGNAL(resetDone()), cycle[k], SLOT(startSimulation()));
        connect(field, SIGNAL(inputError(QString)), this, SLOT(dataErrorField(QString)));
    }

    emit consoleDone();


    for (quint16 k = 0; k < THREADS; k++)
    {
        cycle[k]->startSimulation();
    }
}


void Console::dataErrorField(QString error)
{
    QTextStream out(stdout);
    out << "Field Data Error: " << error << endl;
}

void Console::checkFinish()
{
    done++;

    if (done == THREADS)
    {
        emit finished();
    }
}
