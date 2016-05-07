/****************************************************************************
** Meta object code from reading C++ file 'MainFrame.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../inc/MainFrame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainFrame[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      29,   10,   10,   10, 0x08,
      44,   10,   10,   10, 0x08,
      57,   10,   10,   10, 0x08,
      68,   10,   10,   10, 0x08,
      79,   10,   10,   10, 0x08,
      86,   10,   10,   10, 0x08,
      97,   10,   10,   10, 0x08,
     110,   10,   10,   10, 0x08,
     120,   10,   10,   10, 0x08,
     134,   10,   10,   10, 0x08,
     152,   10,   10,   10, 0x08,
     164,  160,   10,   10, 0x08,
     184,   10,   10,   10, 0x08,
     191,   10,   10,   10, 0x08,
     199,   10,   10,   10, 0x08,
     207,   10,   10,   10, 0x08,
     215,   10,   10,   10, 0x08,
     223,   10,   10,   10, 0x08,
     231,   10,   10,   10, 0x08,
     243,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainFrame[] = {
    "MainFrame\0\0ViewModeChanged()\0"
    "SaveSequence()\0ImportStep()\0PlayBack()\0"
    "StepBack()\0Stop()\0NextStep()\0PlayFoward()\0"
    "DBGShow()\0ExportPhoto()\0ShowFullProduct()\0"
    "about()\0pos\0contextMenu(QPoint)\0Exit()\0"
    "Test1()\0Test2()\0Test3()\0Test4()\0Test5()\0"
    "PartGraph()\0TestVoxelGeneration()\0"
};

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainFrame *_t = static_cast<MainFrame *>(_o);
        switch (_id) {
        case 0: _t->ViewModeChanged(); break;
        case 1: _t->SaveSequence(); break;
        case 2: _t->ImportStep(); break;
        case 3: _t->PlayBack(); break;
        case 4: _t->StepBack(); break;
        case 5: _t->Stop(); break;
        case 6: _t->NextStep(); break;
        case 7: _t->PlayFoward(); break;
        case 8: _t->DBGShow(); break;
        case 9: _t->ExportPhoto(); break;
        case 10: _t->ShowFullProduct(); break;
        case 11: _t->about(); break;
        case 12: _t->contextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 13: _t->Exit(); break;
        case 14: _t->Test1(); break;
        case 15: _t->Test2(); break;
        case 16: _t->Test3(); break;
        case 17: _t->Test4(); break;
        case 18: _t->Test5(); break;
        case 19: _t->PartGraph(); break;
        case 20: _t->TestVoxelGeneration(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainFrame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainFrame,
      qt_meta_data_MainFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrame))
        return static_cast<void*>(const_cast< MainFrame*>(this));
    if (!strcmp(_clname, "Ui::MainFrame"))
        return static_cast< Ui::MainFrame*>(const_cast< MainFrame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
