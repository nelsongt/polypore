/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef AVECTOR3D_H
#define AVECTOR3D_H

#include <QtCore/qpoint.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

class QMatrix4x4;
class QVector2D;
class QVector4D;

#ifndef QT_NO_VECTOR3D

class Q_GUI_EXPORT aVector3D
{
public:
    aVector3D();
    aVector3D(qreal xpos, qreal ypos, qreal zpos);
    explicit aVector3D(const QPoint& point);
    explicit aVector3D(const QPointF& point);
#ifndef QT_NO_VECTOR2D
    aVector3D(const QVector2D& vector);
    aVector3D(const QVector2D& vector, qreal zpos);
#endif
#ifndef QT_NO_VECTOR4D
    explicit aVector3D(const QVector4D& vector);
#endif

    bool isNull() const;

    qreal x() const;
    qreal y() const;
    qreal z() const;

    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);

    qreal length() const;
    qreal lengthSquared() const;

    aVector3D normalized() const;
    void normalize();

    aVector3D &operator+=(const aVector3D &vector);
    aVector3D &operator-=(const aVector3D &vector);
    aVector3D &operator*=(qreal factor);
    aVector3D &operator*=(const aVector3D& vector);
    aVector3D &operator/=(qreal divisor);

    static qreal dotProduct(const aVector3D& v1, const aVector3D& v2);
    static aVector3D crossProduct(const aVector3D& v1, const aVector3D& v2);
    static aVector3D normal(const aVector3D& v1, const aVector3D& v2);
    static aVector3D normal
        (const aVector3D& v1, const aVector3D& v2, const aVector3D& v3);

    qreal distanceToPlane(const aVector3D& plane, const aVector3D& normal) const;
    qreal distanceToPlane(const aVector3D& plane1, const aVector3D& plane2, const aVector3D& plane3) const;
    qreal distanceToLine(const aVector3D& point, const aVector3D& direction) const;

    friend inline bool operator==(const aVector3D &v1, const aVector3D &v2);
    friend inline bool operator!=(const aVector3D &v1, const aVector3D &v2);
    friend inline const aVector3D operator+(const aVector3D &v1, const aVector3D &v2);
    friend inline const aVector3D operator-(const aVector3D &v1, const aVector3D &v2);
    friend inline const aVector3D operator*(qreal factor, const aVector3D &vector);
    friend inline const aVector3D operator*(const aVector3D &vector, qreal factor);
    friend const aVector3D operator*(const aVector3D &v1, const aVector3D& v2);
    friend inline const aVector3D operator-(const aVector3D &vector);
    friend inline const aVector3D operator/(const aVector3D &vector, qreal divisor);

    friend inline bool qFuzzyCompare(const aVector3D& v1, const aVector3D& v2);

#ifndef QT_NO_VECTOR2D
    QVector2D toVector2D() const;
#endif
#ifndef QT_NO_VECTOR4D
    QVector4D toVector4D() const;
#endif

    QPoint toPoint() const;
    QPointF toPointF() const;

    operator QVariant() const;

private:
    qreal xp, yp, zp;

    aVector3D(qreal xpos, qreal ypos, qreal zpos, int dummy);

    friend class QVector2D;
    friend class QVector4D;
#ifndef QT_NO_MATRIX4X4
    friend aVector3D operator*(const aVector3D& vector, const QMatrix4x4& matrix);
    friend aVector3D operator*(const QMatrix4x4& matrix, const aVector3D& vector);
#endif
};

Q_DECLARE_TYPEINFO(aVector3D, Q_MOVABLE_TYPE);

inline aVector3D::aVector3D() : xp(0.0), yp(0.0), zp(0.0) {}

inline aVector3D::aVector3D(qreal xpos, qreal ypos, qreal zpos) : xp(xpos), yp(ypos), zp(zpos) {}

inline aVector3D::aVector3D(qreal xpos, qreal ypos, qreal zpos, int dummy) : xp(xpos), yp(ypos), zp(zpos) {}

inline aVector3D::aVector3D(const QPoint& point) : xp(point.x()), yp(point.y()), zp(0.0) {}

inline aVector3D::aVector3D(const QPointF& point) : xp(point.x()), yp(point.y()), zp(0.0) {}

inline bool aVector3D::isNull() const
{
    return qIsNull(xp) && qIsNull(yp) && qIsNull(zp);
}

inline qreal aVector3D::x() const { return xp; }
inline qreal aVector3D::y() const { return yp; }
inline qreal aVector3D::z() const { return zp; }

inline void aVector3D::setX(qreal aX) { xp = aX; }
inline void aVector3D::setY(qreal aY) { yp = aY; }
inline void aVector3D::setZ(qreal aZ) { zp = aZ; }

inline aVector3D &aVector3D::operator+=(const aVector3D &vector)
{
    xp += vector.xp;
    yp += vector.yp;
    zp += vector.zp;
    return *this;
}

inline aVector3D &aVector3D::operator-=(const aVector3D &vector)
{
    xp -= vector.xp;
    yp -= vector.yp;
    zp -= vector.zp;
    return *this;
}

inline aVector3D &aVector3D::operator*=(qreal factor)
{
    xp *= factor;
    yp *= factor;
    zp *= factor;
    return *this;
}

inline aVector3D &aVector3D::operator*=(const aVector3D& vector)
{
    xp *= vector.xp;
    yp *= vector.yp;
    zp *= vector.zp;
    return *this;
}

inline aVector3D &aVector3D::operator/=(qreal divisor)
{
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    return *this;
}

inline bool operator==(const aVector3D &v1, const aVector3D &v2)
{
    return v1.xp == v2.xp && v1.yp == v2.yp && v1.zp == v2.zp;
}

inline bool operator!=(const aVector3D &v1, const aVector3D &v2)
{
    return v1.xp != v2.xp || v1.yp != v2.yp || v1.zp != v2.zp;
}

inline const aVector3D operator+(const aVector3D &v1, const aVector3D &v2)
{
    return aVector3D(v1.xp + v2.xp, v1.yp + v2.yp, v1.zp + v2.zp, 1);
}

inline const aVector3D operator-(const aVector3D &v1, const aVector3D &v2)
{
    return aVector3D(v1.xp - v2.xp, v1.yp - v2.yp, v1.zp - v2.zp, 1);
}

inline const aVector3D operator*(qreal factor, const aVector3D &vector)
{
    return aVector3D(vector.xp * factor, vector.yp * factor, vector.zp * factor, 1);
}

inline const aVector3D operator*(const aVector3D &vector, qreal factor)
{
    return aVector3D(vector.xp * factor, vector.yp * factor, vector.zp * factor, 1);
}

inline const aVector3D operator*(const aVector3D &v1, const aVector3D& v2)
{
    return aVector3D(v1.xp * v2.xp, v1.yp * v2.yp, v1.zp * v2.zp, 1);
}

inline const aVector3D operator-(const aVector3D &vector)
{
    return aVector3D(-vector.xp, -vector.yp, -vector.zp, 1);
}

inline const aVector3D operator/(const aVector3D &vector, qreal divisor)
{
    return aVector3D(vector.xp / divisor, vector.yp / divisor, vector.zp / divisor, 1);
}

inline bool qFuzzyCompare(const aVector3D& v1, const aVector3D& v2)
{
    return qFuzzyCompare(v1.xp, v2.xp) &&
           qFuzzyCompare(v1.yp, v2.yp) &&
           qFuzzyCompare(v1.zp, v2.zp);
}

inline QPoint aVector3D::toPoint() const
{
    return QPoint(qRound(xp), qRound(yp));
}

inline QPointF aVector3D::toPointF() const
{
    return QPointF(qreal(xp), qreal(yp));
}

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug dbg, const aVector3D &vector);
#endif

#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const aVector3D &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, aVector3D &);
#endif

#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif



//#endif
