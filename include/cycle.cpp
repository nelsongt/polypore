#include "cycle.h"


Cycle::Cycle(QObject *parent, Field *inField, quint16 thread) : QThread(parent)
{
        // seed the random number generator
        mtrand1.seed( QTime(0,0,0).msecsTo(QTime::currentTime()) * static_cast<unsigned int>(getpid()) );

        // channel and pore define the geometry of the system
        channel << aVector3D(0, 0, chHeight) << aVector3D(0,0,0) << aVector3D(0,memStart,0)
                << aVector3D(0,memStart,chHeight) << aVector3D(0,memEnd,chHeight)
                << aVector3D(0,memEnd,0) << aVector3D(0, chLength, 0) << aVector3D(0,chLength,chHeight);

        pore    << aVector3D(bigPoreDepth,memEnd,bigPoreHeightLo) << aVector3D(lilPoreDepth,poreCenterY,lilPoreHeightLo)
                << aVector3D(bigPoreDepth,memStart,bigPoreHeightLo) << aVector3D(bigPoreDepth,memStart,bigPoreHeightHi)
                << aVector3D(lilPoreDepth,poreCenterY,lilPoreHeightHi) << aVector3D(bigPoreDepth,memEnd,bigPoreHeightHi);

        // these two points are important for the collision detection
        poreCenter1 = aVector3D(poreCenterX,50,poreCenterZ);
        poreCenter2 = aVector3D(poreCenterX,600,poreCenterZ);
        thickness = 15;
        mid = aVector3D(poreCenterX,poreCenterY,poreCenterZ);

        field = inField;
        stepNumber = 0;
        translocating = 0;
        transCount = 0;
        translocating2 = 0;
        transCount2 = 0;
        runNumber = 0;
        threadNum = thread;

        // Setup the initial conditions - replace with own method
        for( unsigned int i=0; i < beadNumber; i++ )
        {
            Particle particle;

            if ( i == 0 ) {
                    particle.ptype = 0;  }
            else if ( i == (beadNumber - 1) ) {
                    particle.ptype = 2;  }
            else {
                    particle.ptype = 1;  }

            if ( (i%1) == 0 )
                particle.charge = -1.0 * q_elem;
            else
                particle.charge = 0;

            particle.pos = aVector3D();
            particle.temp_pos = aVector3D();
            particle.vel = aVector3D();
            particle.temp_vel = aVector3D();
            particle.force = aVector3D();
            particle.temp_force = aVector3D();

            //particle.pos = mid + aVector3D(5.55*i,pow(-1, (i + 1)),pow(-1, (i + 1)));
            particle.pos = mid + aVector3D(0,(-15.5 * beadNumber * 4.55) + (4.55 * i),0);
            dnachain << particle;
        }

        this->initialConditions();

        CoM = aVector3D();

        if (!(QDir("./output").exists()))
            QDir().mkdir("./output");

        QFile outAbout("./output/info");
        outAbout.open( QIODevice::WriteOnly );
        QTextStream info;
        info.setDevice(&outAbout);

        info << "Bead Diameter:\t" << bead_d << endl << "Bead-Wall:\t" << bead_w << endl << "V_Bias:\t" << v_slope << endl << "BigPore:\t" << bigPoreRadius << endl;
        info << "Drag:\t" << drag << endl << "Debye Length:\t" << debye << endl << "Bond Length:\t" << bond_l << endl << "Bond Energy:\t" << kbond << endl;
        info << "LJ Well Bead-Bead:\t" << lj_bead << endl << "LJ-Well Bead-Wall:\t" << lj_wall << endl << "Time Step (s):\t" << timeStep << endl << "Membrane Size:\t" << memThick << endl;

        emit stepDone();
}


void Cycle::run(void)
{
    QTextStream out2(stdout);

    out2.setRealNumberPrecision(12);

    out2 << "starting..." << endl;
    //qreal totalF2 = 0;
    //qreal totalF3 = 0;

    while ((dnachain[beadNumber-1].pos.y() < transCut || dnachain[0].pos.y() < transCut || dnachain[beadNumber / 2].pos.y() < transCut) && stopped == false)
    {
        //  This will complete one time step, can switch between langevin and brownian modes here
        this->stepForward_brownian();


        if (stepNumber % 500 == 0)
        {
            /*qreal totalF1 = 0;
            qreal totalF4 = 0;
            qreal totalF5 = 0;
            qreal totalF6 = 0;
            for  (quint16 i = 0; i < beadNumber; i++ )
            {
                totalF1 += eBiasForce(dnachain[i].pos, dnachain[i].charge).y();
                totalF2 = ranForce(mtrand1).y();
                totalF3 += dnachain[i].force.y();
                for ( unsigned int j = 1; j < beadNumber; j++ )
                {
                    totalF4 += LJForce(dnachain[i].pos, dnachain[(i + j) % beadNumber].pos, lj_bead, bead_d).y();
                }

                for ( unsigned int j = i + 1; j < beadNumber; j++ )
                {
                    aVector3D central_forces = force_step_all(dnachain, i, j);  // we don't want to calculate this twice since the forces are symmetric
                    totalF6 += central_forces.y();
                    totalF6 -= central_forces.y();
                }

                // conditional statement decides if and how the bond force(s) should be added
                if ( dnachain[i].ptype == 0 ) {
                    totalF5 += force_step_chain(dnachain, i, i+1).y();
                }
                else if ( dnachain[i].ptype == 1 ) {
                    totalF5 += force_step_chain(dnachain, i, i-1).y();
                    totalF5 += force_step_chain(dnachain, i, i+1).y();
                }
                else if ( dnachain[i].ptype == 2 ) {
                    totalF5 += force_step_chain(dnachain, i, i-1).y();
                }
            }
            out2 << totalF1 << "   " << totalF2 << "   " << totalF3 << "   " << totalF4 << "   " << totalF6 << "   " << totalF5 << "   " << CoM.x() << "   " << CoM.y() << "   " << CoM.z() << endl;
            out2 << "Current step: " << stepNumber << endl;*/
            emit stepDone();

            this->checkTranslocate();
        }

        if (stepNumber % 10000 == 0) {
            //out2 << "CoM.x: " << CoM.x() << endl << "CoM.z: " << CoM.z() << endl;
            this->saveToMemory(); }
            //out2 << "Current step: " << stepNumber << "    \t" << CoM.y() << endl; }


        if (stepNumber % 500000 == 0)
            this->saveToFile();

        transCount += translocating;
        transCount2 += translocating2;
        stepNumber += 1;

        //out2 << field->fieldNearPoint(aVector3D(poreCenterX,poreCenterY,poreCenterZ)).y() << "    \t" << dnachain[0].pos.y() << endl;
    }

    this->saveToFile();
    out2 << "finished writing to file" << endl;
}



void Cycle::stepForward_langevin()
{
    // reset the temp copies of data for each time step so that timesteps aren't mixed up
    for ( unsigned int p = 0; p < beadNumber; p++ )
    {
        dnachain[p].temp_pos = dnachain[p].pos;
        dnachain[p].temp_vel = dnachain[p].vel;
        dnachain[p].temp_force = dnachain[p].force;
        dnachain[p].force = aVector3D();
        CoM = aVector3D();
    }

    // this loop will calculate the movement of each particle one at a time for this timestep
    for  ( unsigned int i = 0; i < beadNumber; i++ )
    {
        // Find the new position, velocity and force vectors
        dnachain[i].pos = posit_step_langevin(dnachain, i);
    }

    for  ( unsigned int i = 0; i < beadNumber; i++ )
        dnachain[i].vel = vel_predict(dnachain, i);

    for  ( unsigned int i = 0; i < beadNumber; i++ )
    {
        // the force vector is dependent upon all other particles in the system
        for ( unsigned int j = 1; j < beadNumber; j++ )
        {
            dnachain[i].force += force_step_all(dnachain, i, j);
        }

        // conditional statement decides if and how the bond force(s) should be added
        if ( dnachain[i].ptype == 0 ) {
            dnachain[i].force += force_step_chain(dnachain, i, i+1);
        }
        else if ( dnachain[i].ptype == 1 ) {
            dnachain[i].force += force_step_chain(dnachain, i, i-1);
            dnachain[i].force += force_step_chain(dnachain, i, i+1);
            dnachain[i].force += force_step_bend(dnachain, i);
        }
        else if ( dnachain[i].ptype == 2 ) {
            dnachain[i].force += force_step_chain(dnachain, i, i-1);
        }

        //dnachain[i].force += force_step_sngl(dnachain, i, field->fieldNearPoint(dnachain[i].pos), stepNumber);
        dnachain[i].force += force_step_sngl(dnachain, i, aVector3D(), mtrand1);
        this->detectCollision(i);

        CoM += dnachain[i].pos;

        //dnachain[beadNumber - 1].force = aVector3D();
    }
    for  ( unsigned int i = 0; i < beadNumber; i++ )
        dnachain[i].vel = speed_step(dnachain, i);

    CoM = CoM / beadNumber;
}



void Cycle::stepForward_brownian()
{
    // reset the temp copies of data for each time step so that timesteps aren't mixed up
    for ( unsigned int p = 0; p < beadNumber; p++ )
    {
        dnachain[p].temp_pos = dnachain[p].pos;
        dnachain[p].temp_force = dnachain[p].force;
        dnachain[p].force = aVector3D();
        CoM = aVector3D();
    }

    // this loop will calculate the movement of each particle one at a time for this timestep
    for  ( unsigned int i = 0; i < beadNumber; i++ )
    {
        // Find the new position, velocity and force vectors
        dnachain[i].pos = posit_step_brownian(dnachain, i);
    }

    for  ( unsigned int i = 0; i < (beadNumber - 1); i++ )
    {
        // the force vector is dependent upon all other particles in the system
        for ( unsigned int j = i + 1; j < beadNumber; j++ )
        {
            aVector3D central_forces = force_step_all(dnachain, i, j);  // we don't want to calculate this twice since the forces are symmetric
            dnachain[i].force += central_forces;
            dnachain[j].force -= central_forces;
        }
    }

    for  ( unsigned int i = 0; i < beadNumber; i++ )
    {
        // conditional statement decides if and how the bond force(s) should be added
        if ( dnachain[i].ptype == 0 ) {
            dnachain[i].force += force_step_chain(dnachain, i, i+1);
        }
        else if ( dnachain[i].ptype == 1 ) {
            dnachain[i].force += force_step_chain(dnachain, i, i-1);
            dnachain[i].force += force_step_chain(dnachain, i, i+1);
            dnachain[i].force += force_step_bend(dnachain, i);
        }
        else if ( dnachain[i].ptype == 2 ) {
            dnachain[i].force += force_step_chain(dnachain, i, i-1);
        }

        //dnachain[i].force += force_step_sngl(dnachain, i, field->fieldNearPoint(dnachain[i].pos), mtrand1);
        dnachain[i].force += force_step_sngl(dnachain, i, aVector3D(), mtrand1);
        this->detectCollision(i);

        CoM += dnachain[i].pos;

        //dnachain[beadNumber - 1].force = aVector3D();
    }
    CoM = CoM / beadNumber;

}




// This function prevents the particles from breaching the geometry
void Cycle::detectCollision(unsigned int i)
{
    // keep the beads in the system - replace with periodic boundaries
    if(dnachain[i].pos.z() > chHeight) dnachain[i].pos.setZ(chHeight);
    if(dnachain[i].pos.z() < 0) dnachain[i].pos.setZ(0);
    if(dnachain[i].pos.x() > chWidth) dnachain[i].pos.setX(chWidth);
    if(dnachain[i].pos.x() < 0) dnachain[i].pos.setX(0);

    /** The following makes an x-section of the channel for the ith bead,
      then finds the vertices of that section, between which the bead lies.
    */

    //aVector3D lu1 = channel[0]; //left upper edge point
    //aVector3D ld1 = channel[1]; //left downer edge point
    aVector3D lu2 = channel[4]; //left upper edge point
    aVector3D ld2 = channel[5]; //left downer edge point
    aVector3D ru1 = channel[3]; //right upper edge point
    aVector3D rd1 = channel[2]; //right downer edge point
    //aVector3D ru2 = channel[7]; //left upper edge point
    //aVector3D rd2 = channel[6]; //left downer edge point
    aVector3D c1 = poreCenter1; //center of the pore left
    aVector3D c2 = poreCenter2; //center of the pore right


    //make control points at the same vertical plane
    //lu1.setX(dnachain[i].pos.x());
    //ld1.setX(dnachain[i].pos.x());
    lu2.setX(dnachain[i].pos.x());
    ld2.setX(dnachain[i].pos.x());
    ru1.setX(dnachain[i].pos.x());
    rd1.setX(dnachain[i].pos.x());
    //ru2.setX(dnachain[i].pos.x());
    //rd2.setX(dnachain[i].pos.x());

    //aVector3D ldir1 = (lu1-ld1);
    //ldir1.normalize();
    aVector3D ldir2 = (lu2-ld2);
    ldir2.normalize();
    aVector3D rdir1 = (ru1-rd1);
    rdir1.normalize();
    //aVector3D rdir2 = (ru2-rd2);
    //rdir2.normalize();
    aVector3D cdir1 = (c2 - c1);
    cdir1.normalize();


    // Testing the Beads

    // if they aren't close the walls then leave them alone
    if (dnachain[i].pos.y() < (memStart - (bead_w * 3)) || dnachain[i].pos.y() > (memEnd + (bead_w * 3)))
        return;

    // if they are within the poreRadius then find the closest point on the wall and apply repulsive force
    else if (dnachain[i].pos.distanceToLine(c1, cdir1) < bigPoreRadius)
    {
        aVector3D close_dot = aVector3D();

        qreal hypotenuse = dnachain[i].pos.distanceToLine(c1, cdir1);

        if (hypotenuse == 0) return;

        else if (dnachain[i].pos.y() <= memStart)
        //if (dnachain[i].pos.y() <= memStart && hypotenuse >= ((dnachain[i].pos.y() - (memStart - (bigPoreRadius * radSlope))) * (1/radSlope)))
        {
            close_dot.setY(memStart);
            close_dot.setX(((dnachain[i].pos.x() - poreCenterX) * bigPoreRadius / hypotenuse) + poreCenterX);
            close_dot.setZ(((dnachain[i].pos.z() - poreCenterZ) * bigPoreRadius / hypotenuse) + poreCenterZ);
        }

        else if (dnachain[i].pos.y() >= memEnd)
        //else if (dnachain[i].pos.y() >= memEnd && hypotenuse >= ((dnachain[i].pos.y() - (memStart - (bigPoreRadius * radSlope))) * (1/radSlope)))
        {
            close_dot.setY(memEnd);
            close_dot.setX(((dnachain[i].pos.x() - poreCenterX) * bigPoreRadius / hypotenuse) + poreCenterX);
            close_dot.setZ(((dnachain[i].pos.z() - poreCenterZ) * bigPoreRadius / hypotenuse) + poreCenterZ);
        }

        else if (dnachain[i].pos.y() > memStart && dnachain[i].pos.y() <= poreCenterY)
        {
            qreal instantRad = bigPoreRadius + (radSlope * (dnachain[i].pos.y() - memStart));
            close_dot.setY(dnachain[i].pos.y());
            close_dot.setX(((dnachain[i].pos.x() - poreCenterX) * instantRad / hypotenuse) + poreCenterX);
            close_dot.setZ(((dnachain[i].pos.z() - poreCenterZ) * instantRad / hypotenuse) + poreCenterZ);
        }

        else if (dnachain[i].pos.y() > poreCenterY && dnachain[i].pos.y() < memEnd)
        {
            qreal instantRad = lilPoreRadius - (radSlope * (dnachain[i].pos.y() - poreCenterY));
            close_dot.setY(dnachain[i].pos.y());
            close_dot.setX(((dnachain[i].pos.x() - poreCenterX) * instantRad / hypotenuse) + poreCenterX);
            close_dot.setZ(((dnachain[i].pos.z() - poreCenterZ) * instantRad / hypotenuse) + poreCenterZ);
        }

        aVector3D force = LJForce(dnachain[i].pos, close_dot, lj_wall, bead_w);

        dnachain[i].force += force;
    }

    // if they aren't in the poreRadius but are close to walls then repel them
    else if (dnachain[i].pos.distanceToLine(rd1, rdir1) < (bead_w * 3))
    {
        aVector3D close_dot = aVector3D();

        close_dot.setX(dnachain[i].pos.x());
        close_dot.setY(dnachain[i].pos.y() + dnachain[i].pos.distanceToLine(rd1, rdir1));
        close_dot.setZ(dnachain[i].pos.z());

        aVector3D force = LJForce(dnachain[i].pos, close_dot, lj_wall, bead_w);

        dnachain[i].force += force;
    }

    else if (dnachain[i].pos.distanceToLine(ld2, ldir2) < (bead_w * 3))
    {
        aVector3D close_dot = aVector3D();

        close_dot.setX(dnachain[i].pos.x());
        close_dot.setY(dnachain[i].pos.y() - dnachain[i].pos.distanceToLine(ld2, ldir2));
        close_dot.setZ(dnachain[i].pos.z());

        aVector3D force = LJForce(dnachain[i].pos, close_dot, lj_wall, bead_w);

        dnachain[i].force += force;
    }

    // these walls don't seem to need repulsion as they are never used - replace?
    /*else if((dnachain[i].pos.distanceToLine(ld1, ldir1) < 1)||
           (dnachain[i].pos.distanceToLine(rd2, rdir2) < 1))
    {
            dnachain[i].pos.setY(dnachain[i].temp_pos.y());
            dnachain[i].force.setY(0);
    }*/
}



// This function will set the initial conditions from a file of pre-thermalized datapoints
void Cycle::initialConditions()
{
    // read in the position data
    QFile posFile("input/initial." + QString::number(beadNumber) + "beads.dat");

    if ( posFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QTextStream stream( &posFile );
        QList<QString> temp;
        QString temp1 = "0";
        QString line;

        for( unsigned int n=1; n <= (beadNumber * 3); n++ )
        {
            temp << temp1;
        }

        QTextStream out2(stdout);

        qint16 l = qrand()%500;

        out2 << l << endl;

        // we want to cycle through the data file a random number of times
        for (qint16 k = l; k >= 0 ; k--)
        {
            line = stream.readLine(); // line of text excluding '\n'

            if (k == 0)
            {
                //we now have a line of text with a bunch of tabs inside it
                //we want to parse it, tab by tab, and get out the fields we want: n n+1 n+2 ...

                for( unsigned int n=1; n <= (beadNumber * 3); n++ )
                {
                    temp[n-1] = line.section('\t',n,n);
                }
                //line is now loaded

                // now input the initial conditions
                for( unsigned int i=0; i < beadNumber; i++ )
                {
                    dnachain[i].pos.setX(temp[i*3].toDouble());
                    dnachain[i].pos.setY(temp[(i*3)+1].toDouble());
                    dnachain[i].pos.setZ(temp[(i*3)+2].toDouble());
                }
            }
        }
    }
    else if (!posFile.open( QIODevice::ReadOnly | QIODevice::Text ))   emit inputError(posFile.errorString());
}


void Cycle::saveToMemory()
{
    for (quint16 i=0;i<beadNumber;i++)
    {
        dataSet << dnachain[i].pos;
    }

    qreal sum = 0;
    qreal radgy = 0;

    sum = 0;
    for( quint16 k=0; k < beadNumber; k++)
    {
        aVector3D r1rmean = dnachain[k].pos - CoM;
        sum += r1rmean.lengthSquared();
    }

    radgy = sqrt(sum / beadNumber);

    aVector3D Rads = aVector3D();

    Rads.setX(stepNumber*timeStep);
    Rads.setY(CoM.y());
    Rads.setZ(radgy);

    gyradSet << Rads;
}


void Cycle::saveToFile()
{
    for (quint32 i=0; i < gyradSet.size(); i++)
    {
        save << gyradSet[i].x() << "     \t";
        for (quint16 j=0; j < beadNumber; j++)
            save << dataSet[i*beadNumber + j].x() << "     \t" << dataSet[i*beadNumber + j].y() << "     \t" << dataSet[i*beadNumber + j].z() << "     \t";
        save << endl;

        gy << gyradSet[i].x() << "   \t" << gyradSet[i].y() << "   \t" << gyradSet[i].z() << "   \t" << endl;
    }

    gyradSet.clear();
    dataSet.clear();

    //qreal dhcoulpot = dh_coul * q_elem * q_elem / (dnachain[0].pos - dnachain[1].pos).length() * exp(-1 * debye * (dnachain[0].pos - dnachain[1].pos).length());
    //qreal voltpotent = dnachain[1].charge * v_slope * dnachain[1].pos.y();
    //qreal bondpotent = kbond * ((dnachain[0].pos - dnachain[1].pos).length() - bond_l) * ((dnachain[0].pos - dnachain[1].pos).length() - bond_l);
    //qreal ljpotential = lj_bead * (pow(bead_d / (dnachain[0].pos.x() - dnachain[1].pos.x()), 12) - 2 * pow(bead_d / (dnachain[0].pos.x() - dnachain[1].pos.x()), 6));
    //out << stepNumber << "    \t" << (dnachain[0].pos - dnachain[1].pos).length() << endl;
}


void Cycle::checkTranslocate()
{
    for  (quint16 i = 0; i < beadNumber; i++)
    {
        if (CoM.y() > memStart && CoM.y() < memEnd)
        {
            translocating = 1;
            break;
        }

        else if (CoM.y() < poreCenterY)
        {
            translocating = 0;
            transCount = 0;
        }


        else if (translocating == 1)
        {
            QFile trData("./output/translocate." + QString::number(beadNumber) + "beads.run" + QString::number(runNumber) + ".thread" + QString::number(threadNum) + ".dat");
            trData.open( QIODevice::WriteOnly );
            QTextStream tr;
            tr.setDevice(&trData);
            tr << transCount*timeStep << endl;
            trData.close();
            translocating = 0;
        }
    }

    translocating2 = 0;

    for  ( unsigned int i = 0; i < beadNumber; i++ )
    {
        if (dnachain[i].pos.y() > memStart && dnachain[i].pos.y() < memEnd)
        {
            translocating2 = 1;
            break;
        }
    }

    if (CoM.y() < poreCenterY && translocating2 == 0)
    {
        transCount2 = 0;
    }

    else if (translocating2 == 0)
    {
        QFile trData("./output/alternatetrans." + QString::number(beadNumber) + "beads.run" + QString::number(runNumber) + ".dat");
        trData.open( QIODevice::WriteOnly );
        QTextStream tr;
        tr.setDevice(&trData);
        tr << transCount2*timeStep << endl;
        trData.close();
    }


    if (CoM.y() < memStart && stepNumber > 15000000)
    {
	for  ( unsigned int i = 0; i < beadNumber; i++ )
	{
	    dnachain[i].pos.setY(480.0);
	}
    }
}




void Cycle::startSimulation()
{
    emit cycleStart();
    stopped = false;

    runNumber++;

    // initialize the data output
    runFile.setFileName("./output/data." + QString::number(beadNumber) + "beads.run" + QString::number(runNumber) + ".thread" + QString::number(threadNum) + ".dat");
    runFile.open( QIODevice::WriteOnly );
    save.setDevice(&runFile);
    save.setRealNumberPrecision(6);

    gyRad.setFileName("./output/gyrad." + QString::number(beadNumber) + "beads.run" + QString::number(runNumber) + ".thread" + QString::number(threadNum) + ".dat");
    gyRad.open( QIODevice::WriteOnly );
    gy.setDevice(&gyRad);
    gy.setRealNumberPrecision(12);;

    this->quit();
    this->start();
}

void Cycle::stopSimulation()
{
    stopped = true;
}

void Cycle::resetSimulation()
{
    runFile.close();
    gyRad.close();

    stepNumber = 0;
    translocating = 0;
    transCount = 0;
    translocating2 = 0;
    transCount2 = 0;

    for( unsigned int i=0; i < beadNumber; i++ )
    {
        dnachain[i].temp_pos = aVector3D();
        dnachain[i].vel = aVector3D();
        dnachain[i].temp_vel = aVector3D();
        dnachain[i].force = aVector3D();
        dnachain[i].temp_force = aVector3D();

        dnachain[i].pos = mid + aVector3D(0,(-0.5 * beadNumber * 4.55) + (4.55 * i),0);
    }

    this->initialConditions();

    emit resetDone();
}

Cycle::~Cycle()
{
    runFile.close();
    gyRad.close();
    this->terminate();
}
