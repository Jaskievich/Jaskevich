/****************************************************************************
** Meta object code from reading C++ file 'chartview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chartview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chartview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChartView_t {
    QByteArrayData data[12];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChartView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChartView_t qt_meta_stringdata_ChartView = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ChartView"
QT_MOC_LITERAL(1, 10, 9), // "sendPoint"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "point"
QT_MOC_LITERAL(4, 27, 7), // "quality"
QT_MOC_LITERAL(5, 35, 5), // "index"
QT_MOC_LITERAL(6, 41, 15), // "sendResetButton"
QT_MOC_LITERAL(7, 57, 7), // "tooltip"
QT_MOC_LITERAL(8, 65, 5), // "state"
QT_MOC_LITERAL(9, 71, 16), // "moveHorScrollBar"
QT_MOC_LITERAL(10, 88, 5), // "value"
QT_MOC_LITERAL(11, 94, 16) // "moveVerScrollBar"

    },
    "ChartView\0sendPoint\0\0point\0quality\0"
    "index\0sendResetButton\0tooltip\0state\0"
    "moveHorScrollBar\0value\0moveVerScrollBar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChartView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,
       6,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   47,    2, 0x0a /* Public */,
       9,    1,   52,    2, 0x0a /* Public */,
      11,    1,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF, QMetaType::UInt, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QPointF, QMetaType::Bool,    3,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void ChartView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChartView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendPoint((*reinterpret_cast< const QPointF(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->sendResetButton(); break;
        case 2: _t->tooltip((*reinterpret_cast< const QPointF(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->moveHorScrollBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->moveVerScrollBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChartView::*)(const QPointF & , unsigned int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChartView::sendPoint)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChartView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChartView::sendResetButton)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChartView::staticMetaObject = { {
    &QChartView::staticMetaObject,
    qt_meta_stringdata_ChartView.data,
    qt_meta_data_ChartView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChartView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChartView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChartView.stringdata0))
        return static_cast<void*>(this);
    return QChartView::qt_metacast(_clname);
}

int ChartView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QChartView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ChartView::sendPoint(const QPointF & _t1, unsigned int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChartView::sendResetButton()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
