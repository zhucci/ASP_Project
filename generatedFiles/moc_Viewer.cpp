/****************************************************************************
** Meta object code from reading C++ file 'Viewer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../inc/Viewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Viewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Viewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    8,    7,    7, 0x05,
      55,    8,    7,    7, 0x05,
      97,    8,    7,    7, 0x05,
     142,    8,    7,    7, 0x05,
     194,  190,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     220,    7,    7,    7, 0x09,
     240,    7,    7,    7, 0x09,
     260,    7,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Viewer[] = {
    "Viewer\0\0,,\0mousePressed(Qt::KeyboardModifiers,int,int)\0"
    "mouseMoved(Qt::KeyboardModifiers,int,int)\0"
    "mouseReleased(Qt::KeyboardModifiers,int,int)\0"
    "mouseDoubleClick(Qt::KeyboardModifiers,int,int)\0"
    "pos\0contextMenuSignal(QPoint)\0"
    "ChangeShadingMode()\0CloseLocalContext()\0"
    "DisplayAll()\0"
};

void Viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Viewer *_t = static_cast<Viewer *>(_o);
        switch (_id) {
        case 0: _t->mousePressed((*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->mouseMoved((*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->mouseReleased((*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->mouseDoubleClick((*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->contextMenuSignal((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->ChangeShadingMode(); break;
        case 6: _t->CloseLocalContext(); break;
        case 7: _t->DisplayAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Viewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Viewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Viewer,
      qt_meta_data_Viewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Viewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Viewer))
        return static_cast<void*>(const_cast< Viewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int Viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Viewer::mousePressed(Qt::KeyboardModifiers _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Viewer::mouseMoved(Qt::KeyboardModifiers _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Viewer::mouseReleased(Qt::KeyboardModifiers _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Viewer::mouseDoubleClick(Qt::KeyboardModifiers _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Viewer::contextMenuSignal(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
