/****************************************************************************
** MapView meta object code from reading C++ file 'MapView.h'
**
** Created: Thu Mar 27 10:41:52 2003
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_MapView
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "MapView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *MapView::className() const
{
    return "MapView";
}

QMetaObject *MapView::metaObj = 0;

void MapView::initMetaObject()
{
    if ( metaObj )
        return;
    if ( strcmp(QCanvasView::className(), "QCanvasView") != 0 )
        badSuperclassWarning("MapView","QCanvasView");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString MapView::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("MapView",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* MapView::staticMetaObject()
{
    if ( metaObj )
        return metaObj;
    (void) QCanvasView::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
        "MapView", "QCanvasView",
        0, 0,
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
