#include "renderer.h"

Renderer::Renderer(QObject *parent, Cycle *cycle): QThread(parent)
{
    alpha = pi/3;

    picture = QPicture();

    pointer = cycle;

    channel = cycle->channel;
    pore =  cycle->pore;

    thickness = cycle->thickness;

    this->run();
}


// This step will update the contents of 'picture'
void Renderer::run()
{
        picture = QPicture();

        QPainter painter;
        painter.begin(& picture);
        painter.scale(scale,scale);
        painter.setRenderHint(QPainter::Antialiasing);


        // This makes a polygon of zero depth that covers the back side
        QPolygonF backSection;
        backSection << transform(channel.last() + aVector3D(0.0, thickness, -chHeight - thickness));
        backSection << transform(channel.first() + aVector3D(0.0, -thickness, - chHeight - thickness));
        backSection << transform(channel.first() + aVector3D(0.0, -thickness, 0.0));
        for(quint16 i=0; i<channel.length(); ++i)
            backSection << transform(channel[i]);
        backSection << transform(channel.last() + aVector3D(0.0, thickness, 0.0));
        backSection << backSection.first();

        // This makes a polygon of zero depth that covers the front side
        QPolygonF frontSection;
        frontSection << transform(channel.last() + aVector3D(chWidth, thickness, - chHeight - thickness));
        frontSection << transform(channel.first() + aVector3D(chWidth, -thickness, - chHeight - thickness));
        frontSection << transform(channel.first() + aVector3D(chWidth, -thickness, 0.0));
        for(quint16 i=0; i<channel.length(); ++i)
            frontSection << transform(channel[i] + aVector3D(chWidth, 0.0, 0.0));
        frontSection << transform(channel.last() + aVector3D(chWidth, thickness, 0.0));
        frontSection << frontSection.first();

        // This makes a polygon of zero depth that is the back of the pore
        QPolygonF backPore;
        for(quint16 i=0; i<pore.length(); ++i)
            backPore << transform(pore[i]);
        backPore << backPore.first();

        // This makes a polygon of zero depth that is the front of the pore
        QPolygonF frontPore;
        frontPore << transform(pore[0] + aVector3D(2*bigPoreRadius, 0.0, 0.0));
        frontPore << transform(pore[1] + aVector3D(2*lilPoreRadius, 0.0, 0.0));
        frontPore << transform(pore[2] + aVector3D(2*bigPoreRadius, 0.0, 0.0));
        frontPore << transform(pore[3] + aVector3D(2*bigPoreRadius, 0.0, 0.0));
        frontPore << transform(pore[4] + aVector3D(2*lilPoreRadius, 0.0, 0.0));
        frontPore << transform(pore[5] + aVector3D(2*bigPoreRadius, 0.0, 0.0));
        frontPore << frontPore.first();



        // Draw the back section stuff first
        this->renderBack(painter, backSection, frontSection);


        // Draw the DNA chain stuff behind the middle
        this->renderDNA(painter, true);


        // Draw the middle
        this->renderMiddle(painter, backSection, frontSection, backPore, frontPore);


        // Draw the DNA chain stuff in front of the middle
        this->renderDNA(painter, false);


        // Draw the front last
        this->renderFront(painter, backSection, frontSection);

        painter.end();

        emit pictureChanged(picture);
}



// This is for converting the 3d vectors into coordinates for the psuedo 3d effect
QPointF Renderer::transform(const aVector3D &v)
{
    return QPointF(v.y() - 0.5*v.x()*cos(alpha) + chWidth*cos(alpha), 0.5*v.x()*sin(alpha) - v.z() + chHeight);
}



void Renderer::renderDNA(QPainter & painter, bool behind)
{
    // Make sure to render the beads closest to the camera first
    int modulo = beadNumber;
    int j = 1;
    int k = 0;

    if (pointer->dnachain[pointer->dnachain.size() - 1].pos.x() < pointer->dnachain[0].pos.x())
    {
        modulo = 1;
        j = 0;
        k = 1;
    }


    // Draw the beads themselves
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(0,0,0,127)));

    for(quint16 i=0; i<pointer->dnachain.size(); ++i)
    {
        unsigned int l = (beadNumber + (i * j)) - (modulo + (i * k));

        if (behind == true  && pointer->dnachain[l].pos.y() < (memEnd + bead_d))
            painter.drawEllipse(transform(aVector3D(pointer->dnachain[l].pos.x(),pointer->dnachain[l].pos.y(),0)), 2.0, 1.0);
        else if (behind == false  && pointer->dnachain[l].pos.y() > (memEnd + bead_d))
            painter.drawEllipse(transform(aVector3D(pointer->dnachain[l].pos.x(),pointer->dnachain[l].pos.y(),0)), 2.0, 1.0);
    }


    // Draw the bond lines
    painter.setPen(QPen(QBrush(Qt::darkMagenta), 1));

    for(quint16 i=0; i<pointer->dnachain.size()-1; ++i)
    {
        unsigned int l = (beadNumber + (i * j)) - (modulo + (i * k));

        if (behind == true  && pointer->dnachain[l].pos.y() < (memEnd + bead_d))
        {
            if (j == 1)
                painter.drawLine(transform(pointer->dnachain[l].pos),transform(pointer->dnachain[l+1].pos));
            else if (j == 0)
                painter.drawLine(transform(pointer->dnachain[l].pos),transform(pointer->dnachain[l-1].pos));
        }
        else if (behind == false  && pointer->dnachain[l].pos.y() > (memEnd + bead_d))
        {
            if (j == 1)
                painter.drawLine(transform(pointer->dnachain[l].pos),transform(pointer->dnachain[l+1].pos));
            else if (j == 0)
                painter.drawLine(transform(pointer->dnachain[l].pos),transform(pointer->dnachain[l-1].pos));
        }
    }

    // Draw the outline of the beads
    painter.setPen(QPen(QBrush(Qt::magenta), 1));
    painter.setBrush(QBrush(Qt::white));

    for(quint16 i=0; i<pointer->dnachain.size(); ++i)
    {
        unsigned int l = (beadNumber + (i * j)) - (modulo + (i * k));

        if (behind == true  && pointer->dnachain[l].pos.y() < (memEnd + bead_d))
            painter.drawEllipse(transform(pointer->dnachain[l].pos), 2.0, 2.0);
        else if (behind == false  && pointer->dnachain[l].pos.y() > (memEnd + bead_d))
            painter.drawEllipse(transform(pointer->dnachain[l].pos), 2.0, 2.0);
    }
}



void Renderer::renderBack(QPainter & painter, QPolygonF & backSection, QPolygonF & frontSection)
{
    painter.setPen(QPen(QBrush(Qt::darkBlue), 2));
    painter.setBrush(QColor(127, 208, 224, 165));
    painter.drawPolygon(backSection);

    painter.setBrush(QBrush(QColor(60, 60, 224, 85), Qt::BDiagPattern));
    painter.drawPolygon(backSection);

    painter.setPen(QPen(QBrush(Qt::darkBlue), 2));
    painter.setBrush(QColor(102, 153, 204, 165));


    // This creates several polygons that connect the the front of back sections one by one
    for(quint16 i=0; i<backSection.count()-8; ++i)
    {
        QPolygonF polygon;
        polygon << backSection[i] << backSection[i+1] << frontSection[i+1] << frontSection[i];
        painter.drawPolygon(polygon);
    }

    QPolygonF polygon;
    polygon << backSection[8] << backSection[9] << frontSection[9] << frontSection[8];
    painter.drawPolygon(polygon);


    // Draw some reference lines
    painter.setPen(QPen(QBrush(Qt::darkBlue), 1));

    painter.drawLine(transform(aVector3D(0,poreCenterY,0)),transform(aVector3D(chWidth,poreCenterY,0)));
    painter.drawLine(transform(aVector3D(poreCenterX,0,0)),transform(aVector3D(poreCenterX,chLength,0)));
    //painter.drawLine(transform(aVector3D(poreCenterX,memEnd,0)),transform(aVector3D(poreCenterX,chLength,0)));
}



void Renderer::renderMiddle(QPainter & painter, QPolygonF & backSection, QPolygonF & frontSection, QPolygonF & backPore, QPolygonF & frontPore)
{
    // Draw the middle membrane pieces
    painter.setPen(QPen(QBrush(Qt::darkBlue), 2));
    painter.setBrush(QColor(102, 153, 204, 165));

    QPolygonF polygon;
    polygon << backSection[7] << backSection[8] << frontSection[8] << frontSection[7];
    painter.drawPolygon(polygon);

    polygon = QPolygonF();
    polygon << backSection[6] << backSection[7] << frontSection[7] << frontSection[6];
    painter.drawPolygon(polygon);
    // Finished with the membrane pieces

    // Draw the pore -- pore code is currently very sloppy
    painter.setPen(QPen(QBrush(Qt::darkBlue), 1));
    painter.setBrush(QColor(102, 153, 204, 0));

    painter.drawLine(backPore[3],backPore[4]);
    painter.drawLine(backPore[4],backPore[5]);
    painter.drawLine(frontPore[0],frontPore[1]);
    painter.drawLine(frontPore[1],frontPore[2]);

    QRectF bigRectangle(187.5, 51.0, 8.0, 29.0);
    //QRectF bigRectangle(187.5, 51.0, 16.0, 87.0);
    //painter.translate(memStart - 84,-29.5);
    painter.translate(memStart - 84,0);
    painter.rotate( 10 );
    painter.drawEllipse(bigRectangle);
    painter.rotate( -10 );
    //painter.translate(84 - memStart,29.5);
    painter.translate(84 - memStart,0);

    QRectF lilRectangle(187.5, 51.0, 8.0, 29.0);
    painter.translate(poreCenterY - 84,0);
    painter.rotate( 10 );
    painter.drawEllipse(lilRectangle);
    painter.rotate( -10 );
    painter.translate(84 - poreCenterY,0);

    //painter.translate(memEnd - 84,-29.5);
    painter.translate(memEnd - 84,0);
    painter.rotate( 10 );
    painter.drawEllipse(bigRectangle);
    painter.rotate( -10 );
    //painter.translate(84 - memEnd,29.5);
    painter.translate(84 - memEnd,0);
    // Finished drawing pore


}



void Renderer::renderFront(QPainter & painter, QPolygonF & backSection, QPolygonF & frontSection)
{
    painter.setPen(QPen(QBrush(Qt::darkBlue), 2));
    painter.setBrush(QColor(102, 153, 204, 165));

    // This creates several polygons that connect the the front of back sections one by one
    for(quint16 i=backSection.count()-3; i<backSection.count()-1; ++i)
    {
        QPolygonF polygon;
        polygon << backSection[i] << backSection[i+1] << frontSection[i+1] << frontSection[i];
        painter.drawPolygon(polygon);
    }

    painter.setPen(QPen(QBrush(Qt::darkBlue), 2));
    painter.setBrush(QColor(127, 208, 224, 165));
    painter.drawPolygon(frontSection);

    painter.setBrush(QBrush(QColor(60, 60, 224, 85), Qt::BDiagPattern));
    painter.drawPolygon(frontSection);
}



Renderer::~Renderer()
{
    this->terminate();
}
