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
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      29,   10,   10,   10, 0x08,
      48,   10,   10,   10, 0x08,
      63,   10,   10,   10, 0x08,
      76,   10,   10,   10, 0x08,
      87,   10,   10,   10, 0x08,
      98,   10,   10,   10, 0x08,
     105,   10,   10,   10, 0x08,
     116,   10,   10,   10, 0x08,
     129,   10,   10,   10, 0x08,
     139,   10,   10,   10, 0x08,
     153,   10,   10,   10, 0x08,
     171,   10,   10,   10, 0x08,
     190,   10,   10,   10, 0x08,
     213,   10,   10,   10, 0x08,
     235,   10,   10,   10, 0x08,
     251,   10,   10,   10, 0x08,
     270,   10,   10,   10, 0x08,
     282,  278,   10,   10, 0x08,
     302,   10,   10,   10, 0x08,
     309,   10,   10,   10, 0x08,
     334,   10,   10,   10, 0x08,
     352,   10,   10,   10, 0x08,
     371,   10,   10,   10, 0x08,
     392,   10,   10,   10, 0x08,
     406,   10,   10,   10, 0x08,
     418,   10,   10,   10, 0x08,
     440,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainFrame[] = {
    "MainFrame\0\0ViewModeChanged()\0"
    "ShowAssemblyInfo()\0SaveSequence()\0"
    "ImportStep()\0PlayBack()\0StepBack()\0"
    "Stop()\0NextStep()\0PlayFoward()\0DBGShow()\0"
    "ExportPhoto()\0ShowFullProduct()\0"
    "HideSelectedPart()\0HideDisaasembledPart()\0"
    "DisplayJustSelected()\0SetShadedMode()\0"
    "SetWireFrameMode()\0about()\0pos\0"
    "contextMenu(QPoint)\0Exit()\0"
    "TestContactSpotVerbose()\0TestPointOnPart()\0"
    "TestSpartialDesc()\0TestIsoFaceForPart()\0"
    "TestDBGTime()\0PartGraph()\0"
    "TestVoxelGeneration()\0TestBodyDescriptor()\0"
};

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainFrame *_t = static_cast<MainFrame *>(_o);
        switch (_id) {
        case 0: _t->ViewModeChanged(); break;
        case 1: _t->ShowAssemblyInfo(); break;
        case 2: _t->SaveSequence(); break;
        case 3: _t->ImportStep(); break;
        case 4: _t->PlayBack(); break;
        case 5: _t->StepBack(); break;
        case 6: _t->Stop(); break;
        case 7: _t->NextStep(); break;
        case 8: _t->PlayFoward(); break;
        case 9: _t->DBGShow(); break;
        case 10: _t->ExportPhoto(); break;
        case 11: _t->ShowFullProduct(); break;
        case 12: _t->HideSelectedPart(); break;
        case 13: _t->HideDisaasembledPart(); break;
        case 14: _t->DisplayJustSelected(); break;
        case 15: _t->SetShadedMode(); break;
        case 16: _t->SetWireFrameMode(); break;
        case 17: _t->about(); break;
        case 18: _t->contextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 19: _t->Exit(); break;
        case 20: _t->TestContactSpotVerbose(); break;
        case 21: _t->TestPointOnPart(); break;
        case 22: _t->TestSpartialDesc(); break;
        case 23: _t->TestIsoFaceForPart(); break;
        case 24: _t->TestDBGTime(); break;
        case 25: _t->PartGraph(); break;
        case 26: _t->TestVoxelGeneration(); break;
        case 27: _t->TestBodyDescriptor(); break;
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
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
