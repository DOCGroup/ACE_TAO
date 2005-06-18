/****************************************************************************
** RootPanel meta object code from reading C++ file 'RootPanel.h'
**
** Created: Tue May 10 15:11:29 2005
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "RootPanel.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RootPanel::className() const
{
    return "RootPanel";
}

QMetaObject *RootPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RootPanel( "RootPanel", &RootPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RootPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RootPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RootPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RootPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RootPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"RootPanel", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RootPanel.setMetaObject( metaObj );
    return metaObj;
}

void* RootPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RootPanel" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool RootPanel::qt_invoke( int _id, QUObject* _o )
{
    return QMainWindow::qt_invoke(_id,_o);
}

bool RootPanel::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RootPanel::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool RootPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
