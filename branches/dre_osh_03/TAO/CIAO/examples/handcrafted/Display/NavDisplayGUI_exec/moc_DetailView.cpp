/****************************************************************************
** DetailView meta object code from reading C++ file 'DetailView.h'
**
** Created: Thu Mar 27 11:59:11 2003
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DetailView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DetailView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DetailView::className() const
{
    return "DetailView";
}

QMetaObject *DetailView::metaObj = 0;

void DetailView::initMetaObject()
{
    if ( metaObj )
        return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
        badSuperclassWarning("DetailView","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString DetailView::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DetailView",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* DetailView::staticMetaObject()
{
    if ( metaObj )
        return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(DetailView::*m1_t0)(int);
    typedef void(DetailView::*m1_t1)(const QRect&);
    typedef void(DetailView::*m1_t2)(NavUnit*);
    typedef void(DetailView::*m1_t3)(const UnitLocation&);
    typedef void(DetailView::*m1_t4)();
    m1_t0 v1_0 = Q_AMPERSAND DetailView::movieStatus;
    m1_t1 v1_1 = Q_AMPERSAND DetailView::movieUpdate;
    m1_t2 v1_2 = Q_AMPERSAND DetailView::currentNode;
    m1_t3 v1_3 = Q_AMPERSAND DetailView::updateLocation;
    m1_t4 v1_4 = Q_AMPERSAND DetailView::apply;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(5);
    slot_tbl[0].name = "movieStatus(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "movieUpdate(const QRect&)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "currentNode(NavUnit*)";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "updateLocation(const UnitLocation&)";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "apply()";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
        "DetailView", "QWidget",
        slot_tbl, 5,
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
