/****************************************************************************
** Meta object code from reading C++ file 'Application.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../inc/Visual/Application.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Application.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Application[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      19,   12,   12,   12, 0x08,
      30,   12,   12,   12, 0x08,
      38,   12,   12,   12, 0x08,
      47,   12,   12,   12, 0x08,
      56,   12,   12,   12, 0x08,
      70,   12,   12,   12, 0x08,
      89,   12,   12,   12, 0x08,
      96,   12,   12,   12, 0x08,
     103,   12,   12,   12, 0x08,
     116,   12,   12,   12, 0x08,
     130,   12,   12,   12, 0x08,
     144,   12,   12,   12, 0x08,
     160,   12,   12,   12, 0x08,
     177,   12,   12,   12, 0x08,
     194,   12,   12,   12, 0x08,
     212,   12,   12,   12, 0x08,
     230,   12,   12,   12, 0x08,
     246,   12,   12,   12, 0x08,
     263,   12,   12,   12, 0x08,
     280,   12,   12,   12, 0x08,
     295,   12,   12,   12, 0x08,
     311,   12,   12,   12, 0x08,
     324,   12,   12,   12, 0x08,
     333,   12,   12,   12, 0x08,
     342,   12,   12,   12, 0x08,
     351,   12,   12,   12, 0x08,
     365,   12,   12,   12, 0x08,
     387,   12,   12,   12, 0x08,
     410,   12,   12,   12, 0x08,
     430,   12,   12,   12, 0x08,
     445,   12,   12,   12, 0x08,
     464,   12,   12,   12, 0x08,
     483,   12,   12,   12, 0x08,
     503,   12,   12,   12, 0x08,
     522,   12,   12,   12, 0x08,
     541,   12,   12,   12, 0x08,
     560,   12,   12,   12, 0x08,
     579,   12,   12,   12, 0x08,
     598,   12,   12,   12, 0x08,
     617,   12,   12,   12, 0x08,
     633,   12,   12,   12, 0x08,
     656,   12,   12,   12, 0x08,
     671,   12,   12,   12, 0x08,
     693,   12,   12,   12, 0x08,
     713,   12,   12,   12, 0x08,
     728,   12,   12,   12, 0x08,
     741,   12,   12,   12, 0x08,
     761,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Application[] = {
    "Application\0\0box()\0cylinder()\0torus()\0"
    "sphere()\0choose()\0load(QString)\0"
    "load(TopoDS_Shape)\0save()\0open()\0"
    "testBoolDS()\0testColorDS()\0testFloatDS()\0"
    "testOctBoolDS()\0testROctBoolDS()\0"
    "testFuseBoolDS()\0testFuseColorDS()\0"
    "testFuseFloatDS()\0testCutBoolDS()\0"
    "testCutColorDS()\0testCutFloatDS()\0"
    "convert2bool()\0convert2color()\0"
    "convert(int)\0setNbX()\0setNbY()\0setNbZ()\0"
    "setScanSide()\0setVolumicBoolValue()\0"
    "setVolumicColorValue()\0setQuadrangleSize()\0"
    "setPointSize()\0setColorMinValue()\0"
    "setColorMaxValue()\0setUsageOfGLlists()\0"
    "setDisplayedXMin()\0setDisplayedXMax()\0"
    "setDisplayedYMin()\0setDisplayedYMax()\0"
    "setDisplayedZMin()\0setDisplayedZMax()\0"
    "displayPoints()\0displayNearestPoints()\0"
    "displayBoxes()\0displayNearestBoxes()\0"
    "displayColorScale()\0displayWaves()\0"
    "displayCut()\0displayCollisions()\0"
    "about()\0"
};

void Application::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Application *_t = static_cast<Application *>(_o);
        switch (_id) {
        case 0: _t->box(); break;
        case 1: _t->cylinder(); break;
        case 2: _t->torus(); break;
        case 3: _t->sphere(); break;
        case 4: _t->choose(); break;
        case 5: _t->load((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->load((*reinterpret_cast< const TopoDS_Shape(*)>(_a[1]))); break;
        case 7: _t->save(); break;
        case 8: _t->open(); break;
        case 9: _t->testBoolDS(); break;
        case 10: _t->testColorDS(); break;
        case 11: _t->testFloatDS(); break;
        case 12: _t->testOctBoolDS(); break;
        case 13: _t->testROctBoolDS(); break;
        case 14: _t->testFuseBoolDS(); break;
        case 15: _t->testFuseColorDS(); break;
        case 16: _t->testFuseFloatDS(); break;
        case 17: _t->testCutBoolDS(); break;
        case 18: _t->testCutColorDS(); break;
        case 19: _t->testCutFloatDS(); break;
        case 20: _t->convert2bool(); break;
        case 21: _t->convert2color(); break;
        case 22: _t->convert((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 23: _t->setNbX(); break;
        case 24: _t->setNbY(); break;
        case 25: _t->setNbZ(); break;
        case 26: _t->setScanSide(); break;
        case 27: _t->setVolumicBoolValue(); break;
        case 28: _t->setVolumicColorValue(); break;
        case 29: _t->setQuadrangleSize(); break;
        case 30: _t->setPointSize(); break;
        case 31: _t->setColorMinValue(); break;
        case 32: _t->setColorMaxValue(); break;
        case 33: _t->setUsageOfGLlists(); break;
        case 34: _t->setDisplayedXMin(); break;
        case 35: _t->setDisplayedXMax(); break;
        case 36: _t->setDisplayedYMin(); break;
        case 37: _t->setDisplayedYMax(); break;
        case 38: _t->setDisplayedZMin(); break;
        case 39: _t->setDisplayedZMax(); break;
        case 40: _t->displayPoints(); break;
        case 41: _t->displayNearestPoints(); break;
        case 42: _t->displayBoxes(); break;
        case 43: _t->displayNearestBoxes(); break;
        case 44: _t->displayColorScale(); break;
        case 45: _t->displayWaves(); break;
        case 46: _t->displayCut(); break;
        case 47: _t->displayCollisions(); break;
        case 48: _t->about(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Application::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Application::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Application,
      qt_meta_data_Application, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Application::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Application::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Application::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Application))
        return static_cast<void*>(const_cast< Application*>(this));
    if (!strcmp(_clname, "Ui::MainWindow"))
        return static_cast< Ui::MainWindow*>(const_cast< Application*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Application::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
