/****************************************************************************
** NodeItem meta object code from reading C++ file 'NodeItem.h'
**
** Created: Tue May 10 15:11:27 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "NodeItem.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *NodeItem::className() const
{
    return "NodeItem";
}

QMetaObject *NodeItem::metaObj = 0;
static QMetaObjectCleanUp cleanUp_NodeItem( "NodeItem", &NodeItem::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString NodeItem::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NodeItem", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString NodeItem::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NodeItem", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* NodeItem::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"timerDone", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "timerDone()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"NodeItem", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_NodeItem.setMetaObject( metaObj );
    return metaObj;
}

void* NodeItem::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "NodeItem" ) )
	return this;
    if ( !qstrcmp( clname, "QCanvasEllipse" ) )
	return (QCanvasEllipse*)this;
    return QObject::qt_cast( clname );
}

bool NodeItem::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: timerDone(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool NodeItem::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool NodeItem::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool NodeItem::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
