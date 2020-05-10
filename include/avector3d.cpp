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

#include "avector3d.h"
#include "qvector2d.h"
#include "qvector4d.h"
#include <QtCore/qmath.h>
#include <QtCore/qvariant.h>
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_VECTOR3D

/*!
    \class aVector3D
    \brief The aVector3D class represents a vector or vertex in 3D space.
    \since 4.6
    \ingroup painting-3D

    Vectors are one of the main building blocks of 3D representation and
    drawing.  They consist of three coordinates, traditionally called
    x, y, and z.

    The aVector3D class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.

    \sa QVector2D, QVector4D, QQuaternion
*/

/*!
    \fn aVector3D::aVector3D()

    Constructs a null vector, i.e. with coordinates (0, 0, 0).
*/

/*!
    \fn aVector3D::aVector3D(qreal xpos, qreal ypos, qreal zpos)

    Constructs a vector with coordinates (\a xpos, \a ypos, \a zpos).
*/

/*!
    \fn aVector3D::aVector3D(const QPoint& point)

    Constructs a vector with x and y coordinates from a 2D \a point, and a
    z coordinate of 0.
*/

/*!
    \fn aVector3D::aVector3D(const QPointF& point)

    Constructs a vector with x and y coordinates from a 2D \a point, and a
    z coordinate of 0.
*/

#ifndef QT_NO_VECTOR2D

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to zero.

    \sa toVector2D()
*/
/*aVector3D::aVector3D(const QVector2D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = 0.0f;
}*/

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to \a zpos.

    \sa toVector2D()
*/
/*aVector3D::aVector3D(const QVector2D& vector, qreal zpos)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = zpos;
}*/

#endif

#ifndef QT_NO_VECTOR4D

/*!
    Constructs a 3D vector from the specified 4D \a vector.  The w
    coordinate is dropped.

    \sa toVector4D()
*/
/*aVector3D::aVector3D(const QVector4D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = vector.zp;
}*/

#endif

/*!
    \fn bool aVector3D::isNull() const

    Returns true if the x, y, and z coordinates are set to 0.0,
    otherwise returns false.
*/

/*!
    \fn qreal aVector3D::x() const

    Returns the x coordinate of this point.

    \sa setX(), y(), z()
*/

/*!
    \fn qreal aVector3D::y() const

    Returns the y coordinate of this point.

    \sa setY(), x(), z()
*/

/*!
    \fn qreal aVector3D::z() const

    Returns the z coordinate of this point.

    \sa setZ(), x(), y()
*/

/*!
    \fn void aVector3D::setX(qreal x)

    Sets the x coordinate of this point to the given \a x coordinate.

    \sa x(), setY(), setZ()
*/

/*!
    \fn void aVector3D::setY(qreal y)

    Sets the y coordinate of this point to the given \a y coordinate.

    \sa y(), setX(), setZ()
*/

/*!
    \fn void aVector3D::setZ(qreal z)

    Sets the z coordinate of this point to the given \a z coordinate.

    \sa z(), setX(), setY()
*/

/*!
    Returns the normalized unit vector form of this vector.

    If this vector is null, then a null vector is returned.  If the length
    of the vector is very close to 1, then the vector will be returned as-is.
    Otherwise the normalized form of the vector of length 1 will be returned.

    \sa length(), normalize()
*/
aVector3D aVector3D::normalized() const
{
    // Need some extra precision if the length is very small - haha disregard this
    double len = xp * xp +
                 yp * yp +
                 zp * zp;
    if (qFuzzyIsNull(len - 1.0))
        return *this;
    else if (!qFuzzyIsNull(len))
        return *this / qSqrt(len);
    else
        return aVector3D();
}

/*!
    Normalizes the currect vector in place.  Nothing happens if this
    vector is a null vector or the length of the vector is very close to 1.

    \sa length(), normalized()
*/
void aVector3D::normalize()
{
    // Need some extra precision if the length is very small  - haha disregard this
    double len = xp * xp +
                 yp * yp +
                 zp * zp;
    if (qFuzzyIsNull(len - 1.0) || qFuzzyIsNull(len))
        return;

    len = qSqrt(len);

    xp /= len;
    yp /= len;
    zp /= len;
}

/*!
    \fn aVector3D &aVector3D::operator+=(const aVector3D &vector)

    Adds the given \a vector to this vector and returns a reference to
    this vector.

    \sa operator-=()
*/

/*!
    \fn aVector3D &aVector3D::operator-=(const aVector3D &vector)

    Subtracts the given \a vector from this vector and returns a reference to
    this vector.

    \sa operator+=()
*/

/*!
    \fn aVector3D &aVector3D::operator*=(qreal factor)

    Multiplies this vector's coordinates by the given \a factor, and
    returns a reference to this vector.

    \sa operator/=()
*/

/*!
    \fn aVector3D &aVector3D::operator*=(const aVector3D& vector)
    \overload

    Multiplies the components of this vector by the corresponding
    components in \a vector.

    Note: this is not the same as the crossProduct() of this
    vector and \a vector.

    \sa crossProduct()
*/

/*!
    \fn aVector3D &aVector3D::operator/=(qreal divisor)

    Divides this vector's coordinates by the given \a divisor, and
    returns a reference to this vector.

    \sa operator*=()
*/

/*!
    Returns the dot product of \a v1 and \a v2.
*/
qreal aVector3D::dotProduct(const aVector3D& v1, const aVector3D& v2)
{
    return v1.xp * v2.xp + v1.yp * v2.yp + v1.zp * v2.zp;
}

/*!
    Returns the cross-product of vectors \a v1 and \a v2, which corresponds
    to the normal vector of a plane defined by \a v1 and \a v2.

    \sa normal()
*/
aVector3D aVector3D::crossProduct(const aVector3D& v1, const aVector3D& v2)
{
    return aVector3D(v1.yp * v2.zp - v1.zp * v2.yp,
                    v1.zp * v2.xp - v1.xp * v2.zp,
                    v1.xp * v2.yp - v1.yp * v2.xp, 1);
}

/*!
    Returns the normal vector of a plane defined by vectors \a v1 and \a v2,
    normalized to be a unit vector.

    Use crossProduct() to compute the cross-product of \a v1 and \a v2 if you
    do not need the result to be normalized to a unit vector.

    \sa crossProduct(), distanceToPlane()
*/
aVector3D aVector3D::normal(const aVector3D& v1, const aVector3D& v2)
{
    return crossProduct(v1, v2).normalized();
}

/*!
    \overload

    Returns the normal vector of a plane defined by vectors
    \a v2 - \a v1 and \a v3 - \a v1, normalized to be a unit vector.

    Use crossProduct() to compute the cross-product of \a v2 - \a v1 and
    \a v3 - \a v1 if you do not need the result to be normalized to a
    unit vector.

    \sa crossProduct(), distanceToPlane()
*/
aVector3D aVector3D::normal
        (const aVector3D& v1, const aVector3D& v2, const aVector3D& v3)
{
    return crossProduct((v2 - v1), (v3 - v1)).normalized();
}

/*!
    Returns the distance from this vertex to a plane defined by
    the vertex \a plane and a \a normal unit vector.  The \a normal
    parameter is assumed to have been normalized to a unit vector.

    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.

    \sa normal(), distanceToLine()
*/
qreal aVector3D::distanceToPlane
        (const aVector3D& plane, const aVector3D& normal) const
{
    return dotProduct(*this - plane, normal);
}

/*!
    \overload

    Returns the distance from this vertex a plane defined by
    the vertices \a plane1, \a plane2 and \a plane3.

    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.

    The two vectors that define the plane are \a plane2 - \a plane1
    and \a plane3 - \a plane1.

    \sa normal(), distanceToLine()
*/
qreal aVector3D::distanceToPlane
    (const aVector3D& plane1, const aVector3D& plane2, const aVector3D& plane3) const
{
    aVector3D n = normal(plane2 - plane1, plane3 - plane1);
    return dotProduct(*this - plane1, n);
}

/*!
    Returns the distance that this vertex is from a line defined
    by \a point and the unit vector \a direction.

    If \a direction is a null vector, then it does not define a line.
    In that case, the distance from \a point to this vertex is returned.

    \sa distanceToPlane()
*/
qreal aVector3D::distanceToLine
        (const aVector3D& point, const aVector3D& direction) const
{
    if (direction.isNull())
        return (*this - point).length();
    aVector3D p = point + dotProduct(*this - point, direction) * direction;
    return (*this - p).length();
}

/*!
    \fn bool operator==(const aVector3D &v1, const aVector3D &v2)
    \relates aVector3D

    Returns true if \a v1 is equal to \a v2; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/

/*!
    \fn bool operator!=(const aVector3D &v1, const aVector3D &v2)
    \relates aVector3D

    Returns true if \a v1 is not equal to \a v2; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/

/*!
    \fn const aVector3D operator+(const aVector3D &v1, const aVector3D &v2)
    \relates aVector3D

    Returns a aVector3D object that is the sum of the given vectors, \a v1
    and \a v2; each component is added separately.

    \sa aVector3D::operator+=()
*/

/*!
    \fn const aVector3D operator-(const aVector3D &v1, const aVector3D &v2)
    \relates aVector3D

    Returns a aVector3D object that is formed by subtracting \a v2 from \a v1;
    each component is subtracted separately.

    \sa aVector3D::operator-=()
*/

/*!
    \fn const aVector3D operator*(qreal factor, const aVector3D &vector)
    \relates aVector3D

    Returns a copy of the given \a vector,  multiplied by the given \a factor.

    \sa aVector3D::operator*=()
*/

/*!
    \fn const aVector3D operator*(const aVector3D &vector, qreal factor)
    \relates aVector3D

    Returns a copy of the given \a vector,  multiplied by the given \a factor.

    \sa aVector3D::operator*=()
*/

/*!
    \fn const aVector3D operator*(const aVector3D &v1, const aVector3D& v2)
    \relates aVector3D

    Multiplies the components of \a v1 by the corresponding components in \a v2.

    Note: this is not the same as the crossProduct() of \a v1 and \a v2.

    \sa aVector3D::crossProduct()
*/

/*!
    \fn const aVector3D operator-(const aVector3D &vector)
    \relates aVector3D
    \overload

    Returns a aVector3D object that is formed by changing the sign of
    all three components of the given \a vector.

    Equivalent to \c {aVector3D(0,0,0) - vector}.
*/

/*!
    \fn const aVector3D operator/(const aVector3D &vector, qreal divisor)
    \relates aVector3D

    Returns the aVector3D object formed by dividing all three components of
    the given \a vector by the given \a divisor.

    \sa aVector3D::operator/=()
*/

/*!
    \fn bool qFuzzyCompare(const aVector3D& v1, const aVector3D& v2)
    \relates aVector3D

    Returns true if \a v1 and \a v2 are equal, allowing for a small
    fuzziness factor for floating-point comparisons; false otherwise.
*/

#ifndef QT_NO_VECTOR2D

/*!
    Returns the 2D vector form of this 3D vector, dropping the z coordinate.

    \sa toVector4D(), toPoint()
*/
/*QVector2D aVector3D::toVector2D() const
{
    return QVector2D(xp, yp, 1);
}*/

#endif

#ifndef QT_NO_VECTOR4D

/*!
    Returns the 4D form of this 3D vector, with the w coordinate set to zero.

    \sa toVector2D(), toPoint()
*/
/*QVector4D aVector3D::toVector4D() const
{
    return QVector4D(xp, yp, zp, 0.0f, 1);
}*/

#endif

/*!
    \fn QPoint aVector3D::toPoint() const

    Returns the QPoint form of this 3D vector. The z coordinate
    is dropped.

    \sa toPointF(), toVector2D()
*/

/*!
    \fn QPointF aVector3D::toPointF() const

    Returns the QPointF form of this 3D vector. The z coordinate
    is dropped.

    \sa toPoint(), toVector2D()
*/

/*!
    Returns the 3D vector as a QVariant.
*/
aVector3D::operator QVariant() const
{
    return QVariant(QVariant::Vector3D, this);
}

/*!
    Returns the length of the vector from the origin.

    \sa lengthSquared(), normalized()
*/
qreal aVector3D::length() const
{
    return qSqrt(xp * xp + yp * yp + zp * zp);
}

/*!
    Returns the squared length of the vector from the origin.
    This is equivalent to the dot product of the vector with itself.

    \sa length(), dotProduct()
*/
qreal aVector3D::lengthSquared() const
{
    return xp * xp + yp * yp + zp * zp;
}

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<(QDebug dbg, const aVector3D &vector)
{
    dbg.nospace() << "aVector3D("
        << vector.x() << ", " << vector.y() << ", " << vector.z() << ')';
    return dbg.space();
}

#endif

#ifndef QT_NO_DATASTREAM

/*!
    \fn QDataStream &operator<<(QDataStream &stream, const aVector3D &vector)
    \relates aVector3D

    Writes the given \a vector to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

//QDataStream &operator<<(QDataStream &stream, const aVector3D &vector)
//{
//    stream << qreal(vector.x()) << qreal(vector.y())
//           << qreal(vector.z());
//    return stream;
//}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, aVector3D &vector)
    \relates aVector3D

    Reads a 3D vector from the given \a stream into the given \a vector
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

//QDataStream &operator>>(QDataStream &stream, aVector3D &vector)
//{
//    double x, y, z;
//    stream >> x;
//    stream >> y;
//    stream >> z;
//    vector.setX(qreal(x));
//   vector.setY(qreal(y));
//    vector.setZ(qreal(z));
//    return stream;
//}

#endif // QT_NO_DATASTREAM

#endif // QT_NO_VECTOR3D

QT_END_NAMESPACE



