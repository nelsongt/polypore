/****************************************************************************
** Meta object code from reading C++ file 'console.h'
**
** Created: Mon Aug 15 16:45:59 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/console.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'console.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Console[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      23,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,    8,    8,    8, 0x0a,
      58,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Console[] = {
    "Console\0\0consoleDone()\0finished()\0"
    "dataErrorField(QString)\0checkFinish()\0"
};

const QMetaObject Console::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Console,
      qt_meta_data_Console, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Console::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Console::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Console::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Console))
        return static_cast<void*>(const_cast< Console*>(this));
    return QObject::qt_metacast(_clname);
}

int Console::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: consoleDone(); break;
        case 1: finished(); break;
        case 2: dataErrorField((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: checkFinish(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Console::consoleDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Console::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
