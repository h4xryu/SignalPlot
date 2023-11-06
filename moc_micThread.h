/****************************************************************************
** Meta object code from reading C++ file 'micThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "micThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'micThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MicThread_t {
    QByteArrayData data[21];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MicThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MicThread_t qt_meta_stringdata_MicThread = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MicThread"
QT_MOC_LITERAL(1, 10, 7), // "send_in"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 15), // "QVector<double>"
QT_MOC_LITERAL(4, 35, 2), // "in"
QT_MOC_LITERAL(5, 38, 6), // "send_f"
QT_MOC_LITERAL(6, 45, 1), // "f"
QT_MOC_LITERAL(7, 47, 11), // "resultReady"
QT_MOC_LITERAL(8, 59, 1), // "s"
QT_MOC_LITERAL(9, 61, 8), // "fin_send"
QT_MOC_LITERAL(10, 70, 4), // "send"
QT_MOC_LITERAL(11, 75, 7), // "results"
QT_MOC_LITERAL(12, 83, 14), // "connect_serial"
QT_MOC_LITERAL(13, 98, 13), // "getDeviceInfo"
QT_MOC_LITERAL(14, 112, 11), // "const char*"
QT_MOC_LITERAL(15, 124, 3), // "dev"
QT_MOC_LITERAL(16, 128, 8), // "chk_trig"
QT_MOC_LITERAL(17, 137, 1), // "a"
QT_MOC_LITERAL(18, 139, 7), // "wait_on"
QT_MOC_LITERAL(19, 147, 8), // "wait_off"
QT_MOC_LITERAL(20, 156, 12) // "chk_sendFlag"

    },
    "MicThread\0send_in\0\0QVector<double>\0"
    "in\0send_f\0f\0resultReady\0s\0fin_send\0"
    "send\0results\0connect_serial\0getDeviceInfo\0"
    "const char*\0dev\0chk_trig\0a\0wait_on\0"
    "wait_off\0chk_sendFlag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MicThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       7,    1,   75,    2, 0x06 /* Public */,
       9,    0,   78,    2, 0x06 /* Public */,
      10,    1,   79,    2, 0x06 /* Public */,
      12,    0,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,   83,    2, 0x0a /* Public */,
      16,    1,   86,    2, 0x0a /* Public */,
      18,    0,   89,    2, 0x0a /* Public */,
      19,    0,   90,    2, 0x0a /* Public */,
      20,    0,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,   11,
    QMetaType::Int,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,

       0        // eod
};

void MicThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MicThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->send_in((*reinterpret_cast< QVector<double>(*)>(_a[1]))); break;
        case 1: _t->send_f((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->resultReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->fin_send(); break;
        case 4: _t->send((*reinterpret_cast< QVector<double>(*)>(_a[1]))); break;
        case 5: { int _r = _t->connect_serial();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->getDeviceInfo((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 7: _t->chk_trig((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->wait_on(); break;
        case 9: _t->wait_off(); break;
        case 10: { bool _r = _t->chk_sendFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MicThread::*)(QVector<double> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MicThread::send_in)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MicThread::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MicThread::send_f)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MicThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MicThread::resultReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MicThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MicThread::fin_send)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MicThread::*)(QVector<double> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MicThread::send)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = int (MicThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MicThread::connect_serial)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MicThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_MicThread.data,
    qt_meta_data_MicThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MicThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MicThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MicThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MicThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MicThread::send_in(QVector<double> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MicThread::send_f(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MicThread::resultReady(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MicThread::fin_send()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MicThread::send(QVector<double> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
int MicThread::connect_serial()
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
