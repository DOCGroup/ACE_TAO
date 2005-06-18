/****************************************************************************
** DetailView meta object code from reading C++ file 'DetailView.h'
**
** Created: Tue May 10 15:11:23 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "DetailView.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DetailView::className() const
{
    return "DetailView";
}

QMetaObject *DetailView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DetailView( "DetailView", &DetailView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DetailView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DetailView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DetailView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DetailView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DetailView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"movieStatus", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_varptr, "\x08", QUParameter::In }
    };
    static const QUMethod slot_1 = {"movieUpdate", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "unit", &static_QUType_ptr, "NavUnit", QUParameter::In }
    };
    static const QUMethod slot_2 = {"currentNode", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "loc", &static_QUType_ptr, "UnitLocation", QUParameter::In }
    };
    static const QUMethod slot_3 = {"updateLocation", 1, param_slot_3 };
    static const QUMethod slot_4 = {"apply", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "movieStatus(int)", &slot_0, QMetaData::Public },
	{ "movieUpdate(const QRect&)", &slot_1, QMetaData::Public },
	{ "currentNode(NavUnit*)", &slot_2, QMetaData::Public },
	{ "updateLocation(const UnitLocation&)", &slot_3, QMetaData::Public },
	{ "apply()", &slot_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"DetailView", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DetailView.setMetaObject( metaObj );
    return metaObj;
}

void* DetailView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DetailView" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool DetailView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: movieStatus((int)static_QUType_int.get(_o+1)); break;
    case 1: movieUpdate((const QRect&)*((const QRect*)static_QUType_ptr.get(_o+1))); break;
    case 2: currentNode((NavUnit*)static_QUType_ptr.get(_o+1)); break;
    case 3: updateLocation((const UnitLocation&)*((const UnitLocation*)static_QUType_ptr.get(_o+1))); break;
    case 4: apply(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DetailView::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool DetailView::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool DetailView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
