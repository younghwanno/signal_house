/****************************************************************************
** Meta object code from reading C++ file 'client_thread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QTFile/client_thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CountWorker_t {
    QByteArrayData data[12];
    char stringdata[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CountWorker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CountWorker_t qt_meta_stringdata_CountWorker = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 11),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 8),
QT_MOC_LITERAL(4, 34, 14),
QT_MOC_LITERAL(5, 49, 14),
QT_MOC_LITERAL(6, 64, 14),
QT_MOC_LITERAL(7, 79, 14),
QT_MOC_LITERAL(8, 94, 14),
QT_MOC_LITERAL(9, 109, 6),
QT_MOC_LITERAL(10, 116, 9),
QT_MOC_LITERAL(11, 126, 12)
    },
    "CountWorker\0updateCount\0\0finished\0"
    "update_value_A\0update_value_B\0"
    "update_value_C\0update_value_D\0"
    "update_value_E\0doWork\0readyRead\0"
    "disconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CountWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    0,   67,    2, 0x06 /* Public */,
       4,    0,   68,    2, 0x06 /* Public */,
       5,    0,   69,    2, 0x06 /* Public */,
       6,    0,   70,    2, 0x06 /* Public */,
       7,    0,   71,    2, 0x06 /* Public */,
       8,    0,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   73,    2, 0x0a /* Public */,
      10,    0,   74,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CountWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CountWorker *_t = static_cast<CountWorker *>(_o);
        switch (_id) {
        case 0: _t->updateCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->finished(); break;
        case 2: _t->update_value_A(); break;
        case 3: _t->update_value_B(); break;
        case 4: _t->update_value_C(); break;
        case 5: _t->update_value_D(); break;
        case 6: _t->update_value_E(); break;
        case 7: _t->doWork(); break;
        case 8: _t->readyRead(); break;
        case 9: _t->disconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CountWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::updateCount)) {
                *result = 0;
            }
        }
        {
            typedef void (CountWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::finished)) {
                *result = 1;
            }
        }
        {
            typedef void (CountWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::update_value_A)) {
                *result = 2;
            }
        }
        {
            typedef void (CountWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::update_value_B)) {
                *result = 3;
            }
        }
        {
            typedef void (CountWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::update_value_C)) {
                *result = 4;
            }
        }
        {
            typedef void (CountWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::update_value_D)) {
                *result = 5;
            }
        }
        {
            typedef void (CountWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CountWorker::update_value_E)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject CountWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CountWorker.data,
      qt_meta_data_CountWorker,  qt_static_metacall, 0, 0}
};


const QMetaObject *CountWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CountWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CountWorker.stringdata))
        return static_cast<void*>(const_cast< CountWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int CountWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CountWorker::updateCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CountWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CountWorker::update_value_A()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CountWorker::update_value_B()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CountWorker::update_value_C()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CountWorker::update_value_D()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CountWorker::update_value_E()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
