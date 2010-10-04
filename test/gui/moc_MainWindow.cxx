/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.hpp'
**
** Created: Mon Oct 4 19:20:07 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_oooark__MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      52,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_oooark__MainWindow[] = {
    "oooark::MainWindow\0\0on_actionLoad_Model_activated()\0"
    "on_actionLoad_Map_activated()\0"
};

const QMetaObject oooark::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_oooark__MainWindow,
      qt_meta_data_oooark__MainWindow, 0 }
};

const QMetaObject *oooark::MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *oooark::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_oooark__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int oooark::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_actionLoad_Model_activated(); break;
        case 1: on_actionLoad_Map_activated(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
