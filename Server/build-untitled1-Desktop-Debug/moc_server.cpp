/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QTFile/server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_chatServer_t {
    QByteArrayData data[13];
    char stringdata[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_chatServer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_chatServer_t qt_meta_stringdata_chatServer = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 21),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 21),
QT_MOC_LITERAL(4, 56, 21),
QT_MOC_LITERAL(5, 78, 12),
QT_MOC_LITERAL(6, 91, 14),
QT_MOC_LITERAL(7, 106, 14),
QT_MOC_LITERAL(8, 121, 14),
QT_MOC_LITERAL(9, 136, 19),
QT_MOC_LITERAL(10, 156, 19),
QT_MOC_LITERAL(11, 176, 19),
QT_MOC_LITERAL(12, 196, 18)
    },
    "chatServer\0update_value_C_client\0\0"
    "update_value_D_client\0update_value_E_client\0"
    "disconnected\0update_value_C\0update_value_D\0"
    "update_value_E\0send_value_A_client\0"
    "send_value_B_client\0send_value_E_client\0"
    "incomingConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_chatServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    0,   78,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void chatServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        chatServer *_t = static_cast<chatServer *>(_o);
        switch (_id) {
        case 0: _t->update_value_C_client(); break;
        case 1: _t->update_value_D_client(); break;
        case 2: _t->update_value_E_client(); break;
        case 3: _t->disconnected(); break;
        case 4: _t->update_value_C(); break;
        case 5: _t->update_value_D(); break;
        case 6: _t->update_value_E(); break;
        case 7: _t->send_value_A_client(); break;
        case 8: _t->send_value_B_client(); break;
        case 9: _t->send_value_E_client(); break;
        case 10: _t->incomingConnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (chatServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&chatServer::update_value_C_client)) {
                *result = 0;
            }
        }
        {
            typedef void (chatServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&chatServer::update_value_D_client)) {
                *result = 1;
            }
        }
        {
            typedef void (chatServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&chatServer::update_value_E_client)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject chatServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_chatServer.data,
      qt_meta_data_chatServer,  qt_static_metacall, 0, 0}
};


const QMetaObject *chatServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *chatServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_chatServer.stringdata))
        return static_cast<void*>(const_cast< chatServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int chatServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void chatServer::update_value_C_client()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void chatServer::update_value_D_client()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void chatServer::update_value_E_client()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
