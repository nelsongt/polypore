/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created: Mon Aug 15 16:45:57 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Window[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      21,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,    7,    7,    7, 0x0a,
      45,    7,    7,    7, 0x0a,
      57,    7,    7,    7, 0x0a,
      73,    7,    7,    7, 0x0a,
      90,    7,    7,    7, 0x0a,
     100,    7,    7,    7, 0x0a,
     115,    7,    7,    7, 0x0a,
     127,    7,    7,    7, 0x0a,
     138,    7,    7,    7, 0x0a,
     147,    7,    7,    7, 0x0a,
     161,    7,    7,    7, 0x0a,
     174,    7,    7,    7, 0x0a,
     198,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Window[] = {
    "Window\0\0windowDone()\0stepDone()\0"
    "cycleStart()\0cycleDone()\0enableDisplay()\0"
    "disableDisplay()\0saveSvg()\0loadFromFile()\0"
    "startLoad()\0stopLoad()\0rewind()\0"
    "fastForward()\0cancelLoad()\0"
    "dataErrorField(QString)\0"
    "dataErrorInitial(QString)\0"
};

const QMetaObject Window::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Window,
      qt_meta_data_Window, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Window))
        return static_cast<void*>(const_cast< Window*>(this));
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: windowDone(); break;
        case 1: stepDone(); break;
        case 2: cycleStart(); break;
        case 3: cycleDone(); break;
        case 4: enableDisplay(); break;
        case 5: disableDisplay(); break;
        case 6: saveSvg(); break;
        case 7: loadFromFile(); break;
        case 8: startLoad(); break;
        case 9: stopLoad(); break;
        case 10: rewind(); break;
        case 11: fastForward(); break;
        case 12: cancelLoad(); break;
        case 13: dataErrorField((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: dataErrorInitial((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Window::windowDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Window::stepDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
