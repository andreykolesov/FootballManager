/****************************************************************************
** Meta object code from reading C++ file 'Season_Manager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Season_Manager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Season_Manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSeasonManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSSeasonManagerENDCLASS = QtMocHelpers::stringData(
    "SeasonManager",
    "tournamentFinished",
    "",
    "tournamentName",
    "Team*",
    "winner",
    "seasonEnded",
    "seasonStarted"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSeasonManagerENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[14];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[6];
    char stringdata5[7];
    char stringdata6[12];
    char stringdata7[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSeasonManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSeasonManagerENDCLASS_t qt_meta_stringdata_CLASSSeasonManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "SeasonManager"
        QT_MOC_LITERAL(14, 18),  // "tournamentFinished"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 14),  // "tournamentName"
        QT_MOC_LITERAL(49, 5),  // "Team*"
        QT_MOC_LITERAL(55, 6),  // "winner"
        QT_MOC_LITERAL(62, 11),  // "seasonEnded"
        QT_MOC_LITERAL(74, 13)   // "seasonStarted"
    },
    "SeasonManager",
    "tournamentFinished",
    "",
    "tournamentName",
    "Team*",
    "winner",
    "seasonEnded",
    "seasonStarted"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSeasonManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   32,    2, 0x06,    1 /* Public */,
       6,    0,   37,    2, 0x06,    4 /* Public */,
       7,    0,   38,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SeasonManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSSeasonManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSeasonManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSeasonManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SeasonManager, std::true_type>,
        // method 'tournamentFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<Team *, std::false_type>,
        // method 'seasonEnded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'seasonStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SeasonManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SeasonManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tournamentFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Team*>>(_a[2]))); break;
        case 1: _t->seasonEnded(); break;
        case 2: _t->seasonStarted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SeasonManager::*)(const QString & , Team * );
            if (_t _q_method = &SeasonManager::tournamentFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SeasonManager::*)();
            if (_t _q_method = &SeasonManager::seasonEnded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SeasonManager::*)();
            if (_t _q_method = &SeasonManager::seasonStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *SeasonManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SeasonManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSeasonManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SeasonManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SeasonManager::tournamentFinished(const QString & _t1, Team * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SeasonManager::seasonEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SeasonManager::seasonStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
