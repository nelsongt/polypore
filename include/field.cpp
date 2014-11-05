#include "field.h"



// ----Constructors & Destructors---- //
// ---------------------------------- //


Field::Field(QObject *parent) : QThread(parent)
{
    //grid = QList();
    //field = QList();

    xlim = 0;
    ylim = 0;
    zlim = 0;
    xsav = 0;
    ysav = 0;
    zsav = 0;
}


void Field::buildField()
{
    QFile inFile("./input/output.all.datcyl");              // read in the electric field data

    if ( inFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {

        QString tempX, tempY, tempZ, tempP, line;
        aVector3D temp3D;
        float tempFL;
        QTextStream stream( &inFile );

        N = 536520;                                        // number of lines of data file, should change this to find it automatically

        quint32 acc_i = 0;
        quint32 acc_j = 0;
        quint32 acc_k = 0;
        qreal xtemp = 0;
        qreal ytemp = 0;
        qreal ztemp = 0;

        for (unsigned int i = 0; i < N; i++)
        {
            line = stream.readLine();                       // line of text excluding '\n'



            tempX = line.section('\t', 2,2);                // we now have a line of text with a bunch of tabs inside it
            tempY = line.section('\t', 0,0);                // we want to parse it, tab by tab, and get out the fields we want: 2 0 1 3
            tempZ = line.section('\t', 1,1);
            tempP = line.section('\t', 3,3);                // line is now loaded



            temp3D.setX(tempX.toFloat() *10 + poreCenterX);                 //convert strings to floats and save them
            temp3D.setY(tempY.toFloat() *10 + poreCenterY);
            temp3D.setZ(tempZ.toFloat() *10 + poreCenterZ);
            //temp3D.setX(tempX.toFloat() *10 + 30);
            //temp3D.setY(tempY.toFloat() *10);
            //temp3D.setZ(tempZ.toFloat() *10 - 30);
            tempFL = tempP.toFloat() * 1.0e20;

            grid << temp3D;
            field << tempFL;


            if (i == 0)                             {acc_i = 1; xtemp = grid[i].x();}
            else if (grid[i].x() > xtemp)           {acc_i++; xtemp = grid[i].x();}

            if (i == 0)                             {acc_j = 1; ytemp = grid[i].y();}
            else if (grid[i].y() > ytemp)           {acc_j++; ytemp = grid[i].y();}

            if (i == 0)                             {acc_k = 1; ztemp = grid[i].z();}
            else if (grid[i].z() > ztemp)           {acc_k++; ztemp = grid[i].z();}

        }

        xlim = acc_i;
        ylim = acc_j;
        zlim = acc_k;

        xsav = xlim;
        ysav = ylim;
        zsav = zlim;


        //QTextStream out(stdout);

        //for (unsigned int i = 0; i < ylim; i++)
         //   out << this->yAtIndex(i) << "  \t" << this->fieldNearPoint(aVector3D(poreCenterX,this->yAtIndex(i),poreCenterZ)).y() << endl;


        inFile.close();
    }
    else if (!inFile.open( QIODevice::ReadOnly | QIODevice::Text ))
    {
        QTextStream out(stdout);
        out << "unable to load";
        emit inputError(inFile.errorString());}

/*
    QFile fieldOut("./output/field.dat");
    QFile potenOut("./output/poten.dat");

    fieldOut.open( QIODevice::WriteOnly );
    potenOut.open( QIODevice::WriteOnly );

    QTextStream outField;
    QTextStream outPoten;
    outField.setDevice(&fieldOut);
    outPoten.setDevice(&potenOut);

    for (uint i = 0; i <= ylim; i++)
    {
        aVector3D point = aVector3D();
        point.setX(poreCenterX);
        point.setY(yAtIndex(i));
        point.setZ(poreCenterZ);
        outField << yAtIndex(i) << "\t" << fieldNearPoint(point).y() << endl;
        outPoten << yAtIndex(i) << "\t" << potenAtIndex(poreCenterX,i,poreCenterZ) << endl;
    }

    fieldOut.close();
    potenOut.close();*/
}



Field::~Field()
{
    this->terminate();
}
