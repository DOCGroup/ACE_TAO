/****************************************************************************
** NodeItem meta object code from reading C++ file 'NodeItem.h'
**
** Created: Thu Mar 27 10:37:28 2003
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_NodeItem
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "NodeItem.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *NodeItem::className() const
{
    return "NodeItem";
}

QMetaObject *NodeItem::metaObj = 0;

void NodeItem::initMetaObject()
{
    if ( metaObj )
        return;
    if ( strcmp(QObject::className(), "QObject") != 0 )
        badSuperclassWarning("NodeItem","QObject");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString NodeItem::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("NodeItem",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* NodeItem::staticMetaObject()
{
    if ( metaObj )
        return metaObj;
    (void) QObject::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(NodeItem::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND NodeItem::timerDone;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "timerDone()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
        "NodeItem", "QObject",
        slot_tbl, 1,
        0, 0,
#ifndef QT_NO_PROPERTIES
        0, 0,
        0, 0,
#endif // QT_NO_PROPERTIES
        0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
