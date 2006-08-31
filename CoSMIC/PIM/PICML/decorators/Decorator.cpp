// Decorator.cpp : Implementation of CDecorator
#include "stdafx.h"
#include "Decorator.h"
#include "IDMLDecorators.h"


#define VERIFY_INIT   { if (!m_isInitialized) return E_DECORATOR_UNINITIALIZED; }
#define VERIFY_LOCSET { if (!m_isLocSet) return E_DECORATOR_LOCISNOTSET; }

/////////////////////////////////////////////////////////////////////////////
// CDecorator
STDMETHODIMP CDecorator::Initialize( IMgaProject *project,
                                     IMgaMetaPart *metaPart,
                                     IMgaFCO *obj )
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  CComPtr<IMgaMetaFCO> metaFco;
	if ( !GetMetaFCO( metaPart, metaFco ) ) {
		return E_DECORATOR_INIT_WITH_NULL;
	}
	else {
		m_isInitialized = true;
	}	
	
	if ( project && metaPart ) {
		objtype_enum eType;

		if ( obj )
			COMTHROW( obj->get_ObjType( &eType ) );
		else {
			CComPtr<IMgaMetaRole> spRole;
			COMTHROW( metaPart->get_Role( &spRole ) );

			CComPtr<IMgaMetaFCO> spMetaFCO;
			COMTHROW( spRole->get_Kind( &spMetaFCO ) );

			COMTHROW( spMetaFCO->get_ObjType( &eType ) );
		}
		switch ( eType ) {
			case OBJTYPE_MODEL :
				m_pDecorator = new ComponentDecorator( metaPart );
				break;
			case OBJTYPE_REFERENCE :
        {
          CComBSTR bstr;
          metaFco->get_Name ( &bstr );

          // ComponentDecorator draws ComponentRefs too.
          if ( bstr == PICML_COMPONENTREF_NAME 
               || bstr == PICML_COMPONENTASMREF_NAME)
				    m_pDecorator = new ComponentDecorator( metaPart );
          else if ( bstr == PICML_INHERITS_NAME )
            m_pDecorator = new InheritsDecorator;
          else
				    m_pDecorator = new MemberDecorator;
				  break;
        }
		}

		if ( m_pDecorator ) {
			m_pDecorator->initialize( obj, metaFco );
		}

	}
	return S_OK;
}

STDMETHODIMP CDecorator::Destroy()
{
	//
	// TODO: At least free all references to MGA objects
	//
	VERIFY_INIT;
	m_isInitialized = false;
	m_isLocSet = false;
  m_pDecorator->destroy();
	return S_OK;
}

STDMETHODIMP CDecorator::GetMnemonic( BSTR *mnemonic )
{	
	//
	// TODO: Return the logical name of the decorator (currently not used by GME)
	//
	*mnemonic = CComBSTR(DECORATOR_NAME).Detach();
	return S_OK;
}

STDMETHODIMP CDecorator::GetFeatures( feature_code *features )
{	
	//
	// TODO: Return supported features (combine multiple features with bitwise-OR)
	// Available feature codes are found in MgaDecorator.idl
	// (curently not used by GME)
	*features = 0;
	return S_OK;
}

STDMETHODIMP CDecorator::SetParam( BSTR name, VARIANT value )
{
	//
	// TODO:  Parse and set all supported parameters, otherwise return error
	// (currently all values are BSTR type)
	//
	VERIFY_INIT;
	return E_DECORATOR_UNKNOWN_PARAMETER;
}

STDMETHODIMP CDecorator::GetParam( BSTR name, VARIANT* value )
{
	//
	// TODO: Return values of supported and previously set parameters, otherwise return error
	// (currently GME does not use this method)
	//
	VERIFY_INIT;
	return E_DECORATOR_UNKNOWN_PARAMETER;
}

STDMETHODIMP CDecorator::SetActive( VARIANT_BOOL isActive )
{
	VERIFY_INIT;

	m_pDecorator->setActive( isActive == VARIANT_TRUE );

	return S_OK;
}

STDMETHODIMP CDecorator::GetPreferredSize( long* sizex, long* sizey ) 
{
	VERIFY_INIT;

	CSize cSize = m_pDecorator->getPreferredSize();
	*sizex = cSize.cx;
	*sizey = cSize.cy;

	return S_OK;
}


STDMETHODIMP CDecorator::SetLocation( long sx,
                                      long sy,
                                      long ex,
                                      long ey )
{
	VERIFY_INIT;
	m_pDecorator->setLocation( CRect( sx, sy, ex, ey ) );
	m_isLocSet = true;

	return S_OK;
}

STDMETHODIMP CDecorator::GetLocation( long *sx,
                                      long *sy,
                                      long *ex,
                                      long *ey )
{
	VERIFY_INIT;
	VERIFY_LOCSET;
	CRect cRect = m_pDecorator->getLocation();
	*sx = cRect.left;
	*sy = cRect.top;
	*ex = cRect.right;
	*ey = cRect.bottom;

	return S_OK;
}

STDMETHODIMP CDecorator::GetLabelLocation( long *sx,
                                           long *sy,
                                           long *ex,
                                           long *ey )
{
	//
	// TODO: Return the location of the text box of your label if you support labels.
	// (currently GME does not call this)
	//
	VERIFY_INIT;
	VERIFY_LOCSET;
	return S_OK;
}

STDMETHODIMP CDecorator::GetPortLocation( IMgaFCO *pFCO,
                                          long *sx,
                                          long *sy,
                                          long *ex,
                                          long *ey )
{
	VERIFY_INIT;
	VERIFY_LOCSET;

	PortDecorator* pPort = m_pDecorator->getPort( pFCO );

	if ( pPort ) {
		CRect cRect = pPort->getBoxLocation();
		*sx = cRect.left;
		*sy = cRect.top;
		*ex = cRect.right;
		*ey = cRect.bottom;
		return S_OK;
	}

	return E_DECORATOR_PORTNOTFOUND;
}

STDMETHODIMP CDecorator::GetPorts( IMgaFCOs **portFCOs )
{
	VERIFY_INIT;
	CComPtr<IMgaFCOs> spFCOs;
	COMTHROW( spFCOs.CoCreateInstance( OLESTR( "Mga.MgaFCOs" ) ) );

	vector<PortDecorator*>vecPorts = m_pDecorator->getPorts();

	for ( unsigned int i = 0 ; i < vecPorts.size() ; i++ )
		COMTHROW( spFCOs->Append( vecPorts[ i ]->getFCO() ) );

	*portFCOs = spFCOs.Detach();

	return S_OK;
}


STDMETHODIMP CDecorator::Draw( HDC hdc )
{
	VERIFY_INIT;
	VERIFY_LOCSET;

	CDC dc;
	dc.Attach( hdc );
  m_pDecorator->draw( &dc );
	dc.Detach();
	return S_OK;
}

STDMETHODIMP CDecorator::SaveState()
{
	//
	// TODO: The only method where we are in read-write transaction.
  // Store all permanent information
	// (currently GME does not support this) 
	//
	VERIFY_INIT;
	return S_OK;
}


//////////// Decorator private functions
CDecorator::CDecorator() 
  : m_isInitialized( false),
    m_isLocSet( false ),
    m_pDecorator( 0 )
{
}

CDecorator::~CDecorator()
{
  delete m_pDecorator;
}

bool
CDecorator::GetMetaFCO( const CComPtr<IMgaMetaPart> &metaPart,
                        CComPtr<IMgaMetaFCO> &metaFco )
{
	if ( !metaPart ) {
		return false;
	}

	metaFco = NULL;
	CComPtr<IMgaMetaRole> metaRole;
	try {
		COMTHROW( metaPart->get_Role( &metaRole ) );
		COMTHROW( metaRole->get_Kind( &metaFco ) );
	}
	catch ( hresult_exception & ) {
		metaFco = NULL;
	}
	return ( metaFco != NULL );
}

