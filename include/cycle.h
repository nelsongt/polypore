#ifndef CYCLE_H
#define CYCLE_H

#include <QFile>
#include <QPicture>
#include <QDir>

#include "avector3d.h"
#include "mtwist.h"
#include "particle.h"
#include "field.h"
#include "solver.h"
#include "forces.h"



class Cycle : public QThread
{
    Q_OBJECT

    friend class Renderer;
    friend class Window;
    friend class Console;
    friend class Field;

    QList<aVector3D> channel;
    QList<aVector3D> pore;
    Field *field;
    QList<Particle> dnachain;
    aVector3D CoM;
    aVector3D poreCenter1;
    aVector3D poreCenter2;

    QList<aVector3D> dataSet;
    QList<aVector3D> gyradSet;

    aVector3D mid;

    qreal thickness;

    bool stopped;
    quint32 translocating;
    quint32 transCount;
    quint32 translocating2;
    quint32 transCount2;
    quint32 stepNumber;
    quint32 runNumber;
    quint16 threadNum;
    quint32 bugCounter;

    QFile runFile;
    QTextStream save;
    QFile gyRad;
    QTextStream gy;

    MTRand mtrand1;


protected:

    void run(void);
    void stepForward_langevin();
    void stepForward_brownian();
    void detectCollision(unsigned int i);
    void initialConditions();
    void saveToMemory();
    void saveToFile();
    void checkTranslocate();

public:

    Cycle(QObject *parent = 0, Field *inField = new Field(), quint16 threadCount = 1);
    ~Cycle();

public slots:

    void startSimulation();
    void stopSimulation();
    void resetSimulation();

signals:

    void cycleStart();
    void stepDone();
    void resetDone();
    void inputError(QString error);
    void dataChanged(const QPicture & picture);
};

#endif // CYCLE_H
