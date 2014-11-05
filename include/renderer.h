#ifndef RENDERER_H
#define RENDERER_H

#include <QPicture>
#include <QPolygonF>
#include <QPainter>
#include <QVector3D>

#include "avector3d.h"
#include "cycle.h"
#include "particle.h"



class Renderer : public QThread
{
    Q_OBJECT

    qreal alpha;

    Cycle * pointer;

    QList<aVector3D> channel;
    QList<aVector3D> pore;

    qreal height;
    qreal length;
    qreal thickness;

protected:

    void run();
    QPointF transform(const aVector3D &v);

    void renderDNA(QPainter & painter, bool behind);
    void renderBack(QPainter & painter, QPolygonF & backSection, QPolygonF & frontSection);
    void renderMiddle(QPainter & painter, QPolygonF & backSection, QPolygonF & frontSection, QPolygonF & backPore, QPolygonF & frontPore);
    void renderFront(QPainter & painter, QPolygonF & backSection, QPolygonF & frontSection);

public:

    Renderer(QObject *parent = 0, Cycle *cycle = 0);
    ~Renderer();
    QPicture picture;

public slots:

signals:

    void pictureChanged(const QPicture & picture);
};

#endif // RENDERER_H
