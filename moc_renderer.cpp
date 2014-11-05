/****************************************************************************
** Meta object code from reading C++ file 'renderer.h'
**
** Created: Mon Aug 15 16:46:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/renderer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Renderer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   10,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Renderer[] = {
    "Renderer\0\0picture\0pictureChanged(QPicture)\0"
};

const QMetaObject Renderer::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Renderer,
      qt_meta_data_Renderer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Renderer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Renderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Renderer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Renderer))
        return static_cast<void*>(const_cast< Renderer*>(this));
    return QThread::qt_metacast(_clname);
}

int Renderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pictureChanged((*reinterpret_cast< const QPicture(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Renderer::pictureChanged(const QPicture & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
