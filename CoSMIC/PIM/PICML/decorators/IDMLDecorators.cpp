// Decorator.cpp : Implementation of CDecorator
#include "stdafx.h"
#include "IDMLDecorators.h"
#include "CommonSmart.h"
#include "Resource.h"
#include "DecoratorUtil.h"
#include "MaskedBitmap.h"
#include <algorithm>

CDecoratorUtil d_util;

#define NAME_MARGINY 20

#define ICON_SIZEX 64
#define ICON_SIZEY 64

static const int MAX_PORT_LENGTH = 5;
static const int WIDTH_MODELSIDE = 100;
static const int GAP_LABEL = 2;
static const int GAP_PORT = 3;
static const int GAP_XMODELPORT = 4;
static const int GAP_YMODELPORT = 8;
static const int GAP_XBORDER = 5;
static const int GAP_YBORDER = 2;
static const int GAP_PORTLABEL = 20;
static const int WIDTH_MODEL = 113;
static const int HEIGHT_MODEL = 71;

//########################################################
//
//	CLASS : DecoratorBase
//
//########################################################

DecoratorBase::DecoratorBase() 
  : m_mgaFco( 0 ),
    m_metaFco( 0 ),
    m_lBorderWidth( 0 ),
    m_bActive( true ),
    m_color( GME_BLACK_COLOR ),
    m_nameColor( GME_BLACK_COLOR )
{
}

DecoratorBase::~DecoratorBase()
{
}

void 
DecoratorBase::initialize( IMgaFCO *obj, CComPtr<IMgaMetaFCO>& metaFco )
{
	m_mgaFco = obj;		// obj == NULL, if we are in the PartBrowser
  m_metaFco = metaFco;
	
  CComBSTR bstr;
  COMTHROW( m_metaFco->get_DisplayedName( &bstr ) );
	if ( bstr.Length() == 0 ) {
		bstr.Empty();
		COMTHROW( m_metaFco->get_Name( &bstr ) );
	}
	m_metaName = bstr;

  LoadBitmap();
	
	if ( m_mgaFco ) {
		CComBSTR bstr;
		COMTHROW( m_mgaFco->get_Name( &bstr ) );
		m_name = bstr;
		COMTHROW( m_mgaFco->get_ObjType( &m_eType ) );
	}
	else {
		CComBSTR bstr;
		COMTHROW( m_metaFco->get_DisplayedName( &bstr ) );
		if ( bstr.Length() == 0 ) {
			bstr.Empty();
			COMTHROW( m_metaFco->get_Name( &bstr ) );
		  COMTHROW( m_metaFco->get_ObjType( &m_eType ) );
		}
		m_name = bstr;
	}
}

void
DecoratorBase::destroy()
{
  m_metaFco = NULL;
  m_mgaFco = NULL;
}

CComPtr<IMgaFCO>
DecoratorBase::getFCO() const
{
	return m_mgaFco;
}

objtype_enum
DecoratorBase::getType() const
{
	return m_eType;
}

CRect
DecoratorBase::getBoxLocation( bool bWithBorder ) const
{
	if ( bWithBorder )
		return m_rect;
	CRect cRect = m_rect;
	cRect.DeflateRect( getBorderWidth( false ), getBorderWidth( false ) );
	return cRect;
}

long
DecoratorBase::getBorderWidth( bool bActive ) const
{
	long lBorderWidth = m_lBorderWidth;
	return lBorderWidth;
}

CSize
DecoratorBase::getPreferredSize() const
{
  return CSize ( ICON_SIZEX, ICON_SIZEY );
}

void
DecoratorBase::setLocation( const CRect& cRect )
{
  m_rect = cRect;
}

CRect
DecoratorBase::getLocation() const
{
  return m_rect;
}

void
DecoratorBase::setActive( bool bActive )
{
	m_bActive = bActive;
}

vector<PortDecorator*>
DecoratorBase::getPorts() const
{
	return vector<PortDecorator*> ();
}

PortDecorator*
DecoratorBase::getPort( CComPtr<IMgaFCO> ) const
{
  return NULL;
}

//########################################################
//
//	CLASS : MemberDecorator
//
//########################################################

MemberDecorator::MemberDecorator()
  : DecoratorBase()
{
}

void
MemberDecorator::LoadBitmap()
{
  if (m_metaName == PICML_MEMBER_NAME 
      || m_metaName == PICML_ATTRIBUTEMEMBER_NAME
      || m_metaName == PICML_DATATYPE_NAME) {
	  CComPtr<IMgaFCO> mgaFco = m_mgaFco;
    if ( mgaFco ) {
		  CComPtr<IMgaReference> ref;
		  COMTHROW( mgaFco.QueryInterface( &ref ) );
		  mgaFco = NULL;
		  COMTHROW( ref->get_Referred( &mgaFco ) );
    }
		if (mgaFco) {
      CComPtr<IMgaMetaFCO> metaFco;
      mgaFco->get_Meta( &metaFco );
		  CComBSTR bstr;
		  COMTHROW( metaFco->get_Name( &bstr ) );
      if ( bstr == PICML_STRING_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_STRING );
      }
      else if ( bstr == PICML_LONGINTEGER_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_LONG );       
      }
      else if ( bstr == PICML_BOOLEAN_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_BOOLEAN );       
      }
      else if ( bstr == PICML_REALNUMBER_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_REALNUMBER );       
      }
      else if ( bstr == PICML_COLLECTION_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_COLLECTIONREF );       
      }
      else if ( bstr == PICML_ENUM_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_ENUMREF );       
      }
      else if ( bstr == PICML_AGGREGATE_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_AGGREGATEREF );       
      }
      else if ( bstr == PICML_ALIAS_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_ALIASREF );       
      }
      else if ( bstr == PICML_SHORTINTEGER_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_SHORTINTEGER );       
      }
      else if ( bstr == PICML_OBJECT_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_OBJECTREF );       
      }
      else if ( bstr == PICML_VALUEOBJECT_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_VALUEREF );       
      }
      else if ( bstr == PICML_COMPONENT_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_COMPONENTREF );       
      }
      else if ( bstr == PICML_EVENT_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_EVENTREF );       
      }
      else if ( bstr == PICML_COMPONENTFACTORY_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_COMPONENTFACTORYREF );       
      }
      else if ( bstr == PICML_SWITCHEDAGGREGATE_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_SWITCHEDAGGREGATEREF );       
      }
      else if ( bstr == PICML_GENERICVALUE_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_GENERICVALUE );       
      }
      else if ( bstr == PICML_GENERICOBJECT_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_GENERICOBJECT );       
      }
      else if ( bstr == PICML_GENERICVALUEOBJECT_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_GENERICVALUEOBJECT );       
      }
      else if ( bstr == PICML_BYTE_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_BYTE );       
      }
      else if ( bstr == PICML_TYPEENCODING_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_TYPEENCODING );       
      }
      else if ( bstr == PICML_TYPEKIND_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_TYPEKIND );       
      }
      else if ( bstr == PICML_BOXED_NAME ) {
        m_bitmap.ReadFromResource( IDB_BITMAP_BOXEDREF );       
      }
		  mgaFco = NULL;
    }
    else {
      if (m_metaName == PICML_MEMBER_NAME 
         || m_metaName == PICML_ATTRIBUTEMEMBER_NAME)
         m_bitmap.ReadFromResource(IDB_BITMAP_MEMBER);
      else
        m_bitmap.ReadFromResource(IDB_BITMAP_DATATYPE);
    }
  }
}

void
MemberDecorator::draw( CDC* pDC )
{
  if (m_bitmap.IsValid ()) {
    m_bitmap.Draw( pDC, m_rect );

    // Skip drawing the name only for attribute members appearing
    // in the editor.
    if (!m_mgaFco || m_metaName != PICML_ATTRIBUTEMEMBER_NAME) {
		  CPoint namePos( m_rect.left + ICON_SIZEX / 2,
                      m_rect.bottom + NAME_MARGINY );
		  d_util.DrawText( pDC, 
                       m_name,
                       namePos,
                       d_util.GetFont(GME_NAME_FONT),
                       m_nameColor,
                       TA_BOTTOM | TA_CENTER);
    }
  }
}

//########################################################
//
//	CLASS : InheritsDecorator
//
//########################################################

InheritsDecorator::InheritsDecorator()
  : DecoratorBase()
{
}

void
InheritsDecorator::LoadBitmap()
{
  m_bitmap.ReadFromResource( IDB_BITMAP_INHERITS );
}

void
InheritsDecorator::draw( CDC* pDC )
{
  if (m_bitmap.IsValid ()) {
    m_bitmap.Draw( pDC, m_rect );

    // Skip drawing the name in the editor.
    if (!m_mgaFco) {
		  CPoint namePos( m_rect.left + ICON_SIZEX / 2,
                      m_rect.bottom + NAME_MARGINY );
		  d_util.DrawText( pDC, 
                       m_name,
                       namePos,
                       d_util.GetFont(GME_NAME_FONT),
                       m_nameColor,
                       TA_BOTTOM | TA_CENTER);
    }
  }
}

//########################################################
//
//	CLASS : PortDecorator
//
//########################################################

PortDecorator::PortDecorator( CComPtr<IMgaFCO> mgaFco,
                              const CPoint& ptInner )
	: DecoratorBase(),
    m_ptInner( ptInner ),
    m_right( false )
{
  m_mgaFco = mgaFco;
}

void 
PortDecorator::initialize()
{
  LoadBitmap();
  CComBSTR bstr;
  m_mgaFco->get_Name( &bstr );
  m_name = bstr;
  m_name = m_name.Left( MAX_PORT_LENGTH );
}

CSize 
PortDecorator::getPreferredSize() const
{
	return CSize( GME_PORT_SIZE, GME_PORT_SIZE );
}

CPoint 
PortDecorator::getInnerPosition() const
{
	return m_ptInner;
}


void
PortDecorator::draw( CDC* pDC )
{
  CRect dst = getLocation();
  m_bitmap.DrawTransparent( pDC,
                            dst,
                            GME_WHITE_COLOR,
                            !m_bActive,
                            GME_GRAYED_OUT_COLOR );

	CPoint namePos( m_right ? dst.left - GAP_LABEL : dst.right + GAP_LABEL,
                  dst.top - GAP_PORT );
	d_util.DrawText( pDC, 
                   m_name,
                   namePos,
                   d_util.GetFont( GME_PORTNAME_FONT ),
                   m_bActive ? m_nameColor : GME_GRAYED_OUT_COLOR,
                   m_right ? TA_RIGHT : TA_LEFT );
}

void
PortDecorator::LoadBitmap()
{
	CComPtr<IMgaFCO> mgaFco = m_mgaFco;
	if ( mgaFco ) {
    CComPtr<IMgaMetaFCO> metaFco;
    mgaFco->get_Meta( &metaFco );
		CComBSTR bstr;
		COMTHROW( metaFco->get_Name( &bstr ) );
    if ( bstr == PICML_INEVENTPORT_NAME ) {
      m_bitmap.ReadFromResource( m_right
                                 ? IDB_BITMAP_INEVENT_RT
                                 : IDB_BITMAP_INEVENT_LF );
    }
    else if ( bstr == PICML_OUTEVENTPORT_NAME ) {
      m_bitmap.ReadFromResource( m_right
                                 ? IDB_BITMAP_OUTEVENT_RT
                                 : IDB_BITMAP_OUTEVENT_LF );
    }
    else if ( bstr == PICML_REQUIREDREQUESTPORT_NAME ) {
      m_bitmap.ReadFromResource( m_right
                                 ? IDB_BITMAP_RECEPTACLE_RT
                                 : IDB_BITMAP_RECEPTACLE_LF );
    }
    else if ( bstr == PICML_PROVIDEDREQUESTPORT_NAME ) {
      m_bitmap.ReadFromResource( IDB_BITMAP_FACET );
    }
    else if ( bstr == PICML_READONLYATTRIBUTE_NAME ) {
      m_bitmap.ReadFromResource( IDB_BITMAP_READONLYATTRIBUTE );
    }
    else if ( bstr == PICML_ATTRIBUTE_NAME ) {
      m_bitmap.ReadFromResource( IDB_BITMAP_ATTRIBUTE );
    }
    else if (bstr == PICML_ATTRIBUTEMAPPING_NAME) {
      m_bitmap.ReadFromResource (IDB_BITMAP_ATTRIBUTEMAPPING);
    }
  }
}

void
PortDecorator::setToRight()
{
  m_right = true;
}

struct PortLess
{
	bool operator()( PortDecorator* pPortA, PortDecorator* pPortB )
	{
		return pPortA->getInnerPosition().y < pPortB->getInnerPosition().y;
	}
};

//########################################################
//
//	CLASS : ComponentDecorator
//
//########################################################

ComponentDecorator::ComponentDecorator( CComPtr<IMgaMetaPart>	metaPart )
  : DecoratorBase(),
    m_metaPart( metaPart ),
    m_iMaxPortTextLength( MAX_PORT_LENGTH ),
    m_bTypeNameEnabled( false ),
    m_iTypeInfo( 0 )
{
}

ComponentDecorator::~ComponentDecorator()
{
  unsigned int i;
	for ( i = 0 ; i < m_vecLeftPorts.size() ; i++ )
		delete m_vecLeftPorts[ i ];
	for ( i = 0 ; i < m_vecRightPorts.size() ; i++ )
		delete m_vecRightPorts[ i ];
}

void 
ComponentDecorator::initialize(IMgaFCO *obj, CComPtr<IMgaMetaFCO>& metaFco)
{
  DecoratorBase::initialize(obj, metaFco);
  if (!m_mgaFco) return;
  
	CComPtr<IMgaMetaAspect>	spParentAspect;
	COMTHROW( m_metaPart->get_ParentAspect( &spParentAspect ) );

	CComPtr<IMgaMetaFCO> spMetaFCO;
  m_mgaFco->get_Meta( &spMetaFCO );
  CComQIPtr<IMgaMetaModel> spMetaModel;

  if ( m_metaName == PICML_COMPONENT_NAME
       || m_metaName == PICML_COMPONENTASSEMBLY_NAME )
	  spMetaModel = spMetaFCO;
  else if (m_metaName == PICML_COMPONENTREF_NAME 
    || m_metaName == PICML_COMPONENTASMREF_NAME)
    {
      CComQIPtr<IMgaReference> spRef = m_mgaFco;
      CComPtr<IMgaFCO> spFCO = NULL;
      spRef->get_Referred( &spFCO );

      // We could be drawing an unassigned ComponentRef.
      if ( !spFCO ) return;

      CComPtr<IMgaMetaFCO> tmp = NULL;
      spFCO->get_Meta( &tmp );
      spMetaModel = tmp;
    }

	CComBSTR bstrAspect;
	COMTHROW( m_metaPart->get_KindAspect( &bstrAspect ) );
	if ( bstrAspect.Length() == 0 ) {
		bstrAspect.Empty();
		COMTHROW( spParentAspect->get_Name( &bstrAspect ) );
	}
	
	HRESULT hr = spMetaModel->get_AspectByName( bstrAspect, &m_spAspect );
	
	if ( hr == E_NOTFOUND) {
		try {
			// JEFF: There is at present only one aspect in PICML,
      // but this is still the easiest way
			m_spAspect = NULL;
			CComPtr<IMgaMetaAspects> spAspects;
			COMTHROW( spMetaModel->get_Aspects( &spAspects ) );
			ASSERT( spAspects );
			long nAspects = 0;
			COMTHROW( spAspects->get_Count( &nAspects ) );
			if ( nAspects > 0 ) {
				COMTHROW( spAspects->get_Item( 1, &m_spAspect ) );
			}
		}
		catch ( hresult_exception& ) {
		}
	}

  loadPorts();

	CComBSTR bstrPath = PREF_TYPESHOWN;
	CComBSTR bstrValue;
	COMTHROW( m_mgaFco->get_RegistryValue( bstrPath, &bstrValue ) );
  m_bTypeNameEnabled =
    (bstrValue == "t" || bstrValue == "true" || bstrValue == "1" );

	VARIANT_BOOL bInstance = VARIANT_FALSE;
	COMTHROW( m_mgaFco->get_IsInstance( &bInstance ) );
	if ( bInstance == VARIANT_TRUE ) {
		m_iTypeInfo = 3;
		if ( m_bTypeNameEnabled ) {
			CComPtr<IMgaFCO> spType;
			COMTHROW( m_mgaFco->get_DerivedFrom( &spType ) );
			CComBSTR bstrName;
			COMTHROW( spType->get_Name( &bstrName ) );
			m_strTypeName = bstrName;
		}
	}
	else {
		CComPtr<IMgaFCO> spType;
		COMTHROW( m_mgaFco->get_DerivedFrom( &spType ) );
		if ( spType )
			m_iTypeInfo = 2;
		else {
			CComPtr<IMgaFCOs> spFCOs;
			COMTHROW( m_mgaFco->get_DerivedObjects( &spFCOs ) );
			long lCount = 0;
			COMTHROW( spFCOs->get_Count( &lCount ) );
			m_iTypeInfo = ( lCount == 0 ) ? 0 : 1;
		}
	}
}

CSize
ComponentDecorator::getPreferredSize() const
{
	long lWidth =
    ( 8 * m_iMaxPortTextLength
      + GAP_LABEL
      + GME_PORT_SIZE
      + GAP_XMODELPORT
      + GAP_XBORDER )
      * 2
    + GAP_PORTLABEL;

	long lHeight =
    ( GAP_YMODELPORT + GAP_YBORDER ) * 2
    + max( m_vecLeftPorts.size(), m_vecRightPorts.size() )
      * ( GME_PORT_SIZE + GAP_PORT )
    - GAP_PORT;

	return CSize( max( m_bitmap.Width(), lWidth ),
                max( m_bitmap.Height(), lHeight ) );
}

void
ComponentDecorator::setLocation( const CRect& cRect )
{
  m_rect = cRect;
	long lY = ( m_rect.Height()
              - m_vecLeftPorts.size() * ( GME_PORT_SIZE + GAP_PORT )
              + GAP_PORT )
             / 2;

  unsigned int i;

	for ( i = 0 ; i < m_vecLeftPorts.size() ; i++ ) {
		m_vecLeftPorts[ i ]->setLocation( CRect( GAP_XMODELPORT
                                             + GAP_XBORDER, 
                                             lY, 
                                             GAP_XMODELPORT
                                             + GME_PORT_SIZE
                                             + GAP_XBORDER, 
                                             lY + GME_PORT_SIZE ) );
		lY += GME_PORT_SIZE + GAP_PORT;
	}

	lY = ( m_rect.Height()
         - m_vecRightPorts.size() * ( GME_PORT_SIZE + GAP_PORT )
         + GAP_PORT )
        / 2;

	for ( i = 0 ; i < m_vecRightPorts.size() ; i++ ) {
		m_vecRightPorts[ i ]->setLocation( CRect( cRect.Width()
                                              - GAP_XMODELPORT
                                              - GME_PORT_SIZE
                                              - GAP_XBORDER, 
                                              lY,
                                              cRect.Width()
                                              - GAP_XMODELPORT
                                              - GAP_XBORDER, 
                                              lY + GME_PORT_SIZE ) );
		lY += GME_PORT_SIZE + GAP_PORT;
	}
}

void
ComponentDecorator::setActive( bool bActive )
{
	m_bActive = bActive;
	for ( unsigned int i = 0 ; i < m_vecLeftPorts.size() ; i++ )
		m_vecLeftPorts[ i ]->setActive( bActive );
	for ( unsigned int i = 0 ; i < m_vecRightPorts.size() ; i++ )
		m_vecRightPorts[ i ]->setActive( bActive );
}

void
ComponentDecorator::LoadBitmap()
{
  if ( m_metaName == PICML_COMPONENT_NAME
       || m_metaName == PICML_COMPONENTASSEMBLY_NAME )
    m_bitmap.ReadFromResource( IDB_BITMAP_COMPONENT );
  else if (m_metaName == PICML_COMPONENTASMREF_NAME
    || m_metaName == PICML_COMPONENTREF_NAME)
    m_bitmap.ReadFromResource( IDB_BITMAP_COMPONENTREF );
}

void
ComponentDecorator::draw( CDC* pDC )
{
  // Draw the component icon.
  CRect dst = getLocation();
//	m_bitmap.Draw( pDC, dst );
  m_bitmap.DrawTransparent( pDC,
                            dst,
                            GME_WHITE_COLOR,
                            !m_bActive,
                            GME_GRAYED_OUT_COLOR );

  // Draw the component name.
	CPoint namePos( dst.left + dst.Width () / 2, dst.bottom + NAME_MARGINY );
	d_util.DrawText( pDC, 
                   m_name,
                   namePos,
                   d_util.GetFont( GME_NAME_FONT ),
                   m_bActive ? m_nameColor : GME_GRAYED_OUT_COLOR,
                   TA_BOTTOM | TA_CENTER );

  // If we are an instance, draw the type info.
	if ( m_bTypeNameEnabled && m_iTypeInfo == 3 ) {
	  CPoint typeNamePos( dst.left + dst.Width () / 2,
                        dst.bottom + 2 * NAME_MARGINY );
    CString typeInfoStr = "[  " + m_strTypeName + "  ]";
	  d_util.DrawText( pDC, 
                     typeInfoStr,
                     typeNamePos,
                     d_util.GetFont( GME_PORTNAME_FONT ),
                     m_bActive ? m_nameColor : GME_GRAYED_OUT_COLOR,
                     TA_BOTTOM | TA_CENTER );
	}

  // Draw the component ports, if any.
	CSize cExtentD = pDC->GetViewportExt();
	CSize cExtentL = pDC->GetWindowExt();
	CRect cRect = getLocation();
	CPoint ptOrigin = 
    pDC->OffsetViewportOrg( (long) ( cRect.left
                                     * ( (double) cExtentD.cx
                                         / cExtentL.cx ) ),
                            (long) ( cRect.top
                                     * ( (double) cExtentD.cy
                                         / cExtentL.cy ) ) );
  unsigned int i;

	for ( i = 0 ; i < m_vecLeftPorts.size() ; i++ ) {
		m_vecLeftPorts[ i ]->draw( pDC );
  }

	for ( i = 0 ; i < m_vecRightPorts.size() ; i++ ) {
		m_vecRightPorts[ i ]->draw( pDC );
  }
	pDC->SetViewportOrg( ptOrigin );
}

vector<PortDecorator*>
ComponentDecorator::getPorts() const
{
	vector<PortDecorator*> vecPorts( m_vecLeftPorts );
	for ( unsigned int i = 0 ; i < m_vecRightPorts.size() ; i++ )
			vecPorts.push_back( m_vecRightPorts[ i ] );
	return vecPorts;
}

PortDecorator*
ComponentDecorator::getPort( CComPtr<IMgaFCO> spFCO ) const
{
  unsigned int i;

	for ( i = 0 ; i < m_vecLeftPorts.size() ; i++ )
		if ( m_vecLeftPorts[ i ]->getFCO() == spFCO )
			return m_vecLeftPorts[ i ];
	for ( i = 0 ; i < m_vecRightPorts.size() ; i++ )
		if ( m_vecRightPorts[ i ]->getFCO() == spFCO )
			return m_vecRightPorts[ i ];
	return NULL;
}

void
ComponentDecorator::loadPorts()
{
  // If we are in the Part Browser, we don't want to load ports.
	if ( !m_spAspect ) return;

	vector<PortDecorator*>	vecPorts;
  CComQIPtr<IMgaModel> spModel;

  if ( m_metaName == PICML_COMPONENT_NAME
       || m_metaName == PICML_COMPONENTASSEMBLY_NAME )
	  spModel = m_mgaFco;
  else if (m_metaName == PICML_COMPONENTREF_NAME 
    || m_metaName == PICML_COMPONENTASMREF_NAME)
    {
      CComQIPtr<IMgaReference> spRef = m_mgaFco;
      CComPtr<IMgaFCO> tmp = NULL;
      spRef->get_Referred ( &tmp );
      if ( !tmp ) return;
      spModel = tmp;
    }

	CComPtr<IMgaFCOs> spFCOs;
	COMTHROW( spModel->get_ChildFCOs( &spFCOs ) );

  // Iterate over the child FCOs list and add any ports to
  // the vector.
  this->findPorts( vecPorts, spFCOs );

	orderPorts( vecPorts );
}

void
ComponentDecorator::orderPorts( vector<PortDecorator*>& vecPorts )
{
	long lMin = 100000000;
	long lMax = 0;
  unsigned int i;

	for ( i = 0 ; i < vecPorts.size() ; i++ ) {
		lMin = min( lMin, vecPorts[ i ]->getInnerPosition().x );
		lMax = max( lMax, vecPorts[ i ]->getInnerPosition().x );
	}

	for ( i = 0 ; i < vecPorts.size() ; i++ ) {
		if ( vecPorts[ i ]->getInnerPosition().x <= WIDTH_MODELSIDE 
         || vecPorts[ i ]->getInnerPosition().x < lMax / 2 ) {
			m_vecLeftPorts.push_back( vecPorts[ i ] );
		}
		else {
			m_vecRightPorts.push_back( vecPorts[ i ] );
      vecPorts[ i ]->setToRight();
		}

		vecPorts[ i ]->initialize();
	}

	sort( m_vecLeftPorts.begin(), m_vecLeftPorts.end(), PortLess() );
	sort( m_vecRightPorts.begin(), m_vecRightPorts.end(), PortLess() );
}

void
ComponentDecorator::findPorts( vector<PortDecorator*>& vecPorts,
                               CComPtr<IMgaFCOs>& spFCOs )
{
	MGACOLL_ITERATE( IMgaFCO, spFCOs ) {
		CComPtr<IMgaPart> spPart;
		COMTHROW( MGACOLL_ITER->get_Part( m_spAspect, &spPart ) );
		if ( spPart ) {
			CComPtr<IMgaMetaPart> spMetaPart;
			COMTHROW( spPart->get_Meta( &spMetaPart ) );
			VARIANT_BOOL vbLinked;
			COMTHROW( spMetaPart->get_IsLinked( &vbLinked ) );
			if ( vbLinked ) {
				long lX = 0;
				long lY = 0;
				COMTHROW( spPart->GetGmeAttrs( 0, &lX, &lY ) );
				vecPorts.push_back( new PortDecorator( MGACOLL_ITER,
                                               CPoint( lX, lY ) ) );
			}
			else
				COMTHROW( MGACOLL_ITER->Close() );
		}
		else
			COMTHROW( MGACOLL_ITER->Close() );
	} MGACOLL_ITERATE_END;
}

