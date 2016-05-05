/****************************************************************************
** Meta object code from reading C++ file 'MainFrame.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../inc/MainFrame.h"
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
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      26,   10,   10,   10, 0x08,
      39,   10,   10,   10, 0x08,
      50,   10,   10,   10, 0x08,
      61,   10,   10,   10, 0x08,
      68,   10,   10,   10, 0x08,
      79,   10,   10,   10, 0x08,
      92,   10,   10,   10, 0x08,
     102,   10,   10,   10, 0x08,
     116,   10,   10,   10, 0x08,
     134,   10,   10,   10, 0x08,
     146,  142,   10,   10, 0x08,
     166,   10,   10,   10, 0x08,
     173,   10,   10,   10, 0x08,
     181,   10,   10,   10, 0x08,
     189,   10,   10,   10, 0x08,
     197,   10,   10,   10, 0x08,
     205,   10,   10,   10, 0x08,
     213,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainFrame[] = {
    "MainFrame\0\0SaveSequence()\0ImportStep()\0"
    "PlayBack()\0StepBack()\0Stop()\0NextStep()\0"
    "PlayFoward()\0DBGShow()\0ExportPhoto()\0"
    "ShowFullProduct()\0about()\0pos\0"
    "contextMenu(QPoint)\0Exit()\0Test1()\0"
    "Test2()\0Test3()\0Test4()\0Test5()\0"
    "TestVoxelGeneration()\0"
};

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainFrame *_t = static_cast<MainFrame *>(_o);
        switch (_id) {
        case 0: _t->SaveSequence(); break;
        case 1: _t->ImportStep(); break;
        case 2: _t->PlayBack(); break;
        case 3: _t->StepBack(); break;
        case 4: _t->Stop(); break;
        case 5: _t->NextStep(); break;
        case 6: _t->PlayFoward(); break;
        case 7: _t->DBGShow(); break;
        case 8: _t->ExportPhoto(); break;
        case 9: _t->ShowFullProduct(); break;
        case 10: _t->about(); break;
        case 11: _t->contextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 12: _t->Exit(); break;
        case 13: _t->Test1(); break;
        case 14: _t->Test2(); break;
        case 15: _t->Test3(); break;
        case 16: _t->Test4(); break;
        case 17: _t->Test5(); break;
        case 18: _t->TestVoxelGeneration(); break;
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
