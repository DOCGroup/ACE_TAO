// IDMLDecorators.h : Declaration of the IDML Decorator classes

#ifndef __IDMLDECORATOR_H_
#define __IDMLDECORATOR_H_

#pragma warning( disable : 4290 )

#include "Resource.h"       // main symbols
#include "DecoratorLib.h"
#include "MaskedBitmap.h"
#include <vector>

using namespace std;

#define PICML_AGGREGATE_NAME           "Aggregate"
#define PICML_ALIAS_NAME               "Alias"
#define PICML_ATTRIBUTE_NAME           "Attribute"
#define PICML_ATTRIBUTEMEMBER_NAME     "AttributeMember"
#define PICML_BOOLEAN_NAME             "Boolean"
#define PICML_BOXED_NAME               "Boxed"
#define PICML_BYTE_NAME                "Byte"
#define PICML_COLLECTION_NAME          "Collection"
#define PICML_COMPONENT_NAME           "Component"
#define PICML_COMPONENTREF_NAME        "ComponentRef"
#define PICML_COMPONENTFACTORY_NAME    "ComponentFactory"
#define PICML_ENUM_NAME                "Enum"
#define PICML_EVENT_NAME               "Event"
#define PICML_GENERICOBJECT_NAME       "GenericObject"
#define PICML_GENERICVALUE_NAME        "GenericValue"
#define PICML_GENERICVALUEOBJECT_NAME  "GenericValueObject"
#define PICML_INEVENTPORT_NAME         "InEventPort"
#define PICML_INHERITS_NAME            "Inherits"
#define PICML_LONGINTEGER_NAME         "LongInteger"
#define PICML_MEMBER_NAME              "Member"
#define PICML_OBJECT_NAME              "Object"
#define PICML_OUTEVENTPORT_NAME        "OutEventPort"
#define PICML_PROVIDEDREQUESTPORT_NAME "ProvidedRequestPort"
#define PICML_READONLYATTRIBUTE_NAME   "ReadonlyAttribute"
#define PICML_REALNUMBER_NAME          "RealNumber"
#define PICML_REQUIREDREQUESTPORT_NAME "RequiredRequestPort"
#define PICML_SHORTINTEGER_NAME        "ShortInteger"
#define PICML_STRING_NAME              "String"
#define PICML_SWITCHEDAGGREGATE_NAME   "SwitchedAggregate"
#define PICML_TYPEENCODING_NAME        "TypeEncoding"
#define PICML_TYPEKIND_NAME            "TypeKind"
#define PICML_VALUEOBJECT_NAME         "ValueObject"
#define PICML_COMPONENTASSEMBLY_NAME   "ComponentAssembly"
#define PICML_COMPONENTASMREF_NAME     "ComponentAssemblyReference"
#define PICML_DATATYPE_NAME            "DataType"
#define PICML_ATTRIBUTEMAPPING_NAME    "AttributeMapping"

static const char* PREF_TYPESHOWN				= "isTypeShown";

class PortDecorator;

//########################################################
//
//	CLASS : DecoratorBase
//
//########################################################

class DecoratorBase
{
public:
  virtual ~DecoratorBase();

  virtual void      initialize( IMgaFCO *obj, CComPtr<IMgaMetaFCO>& metaFco );
  virtual void      destroy();
	CComPtr<IMgaFCO>	getFCO() const;
	objtype_enum			getType() const;
	CRect			        getBoxLocation( bool bWithBorder = false ) const;
	long					    getBorderWidth( bool bActive = true ) const;
	virtual CSize			getPreferredSize() const;
  virtual void      setLocation( const CRect& cRect );
  virtual CRect     getLocation() const;
	virtual void 			setActive( bool bActive );
  virtual void 			draw( CDC* pDC ) = 0;
  virtual void      LoadBitmap() = 0;

	virtual vector<PortDecorator*>  getPorts() const;
	virtual PortDecorator*			    getPort( CComPtr<IMgaFCO> ) const;

protected:
  DecoratorBase();

protected:
  CRect                 m_rect;
	CComPtr<IMgaFCO>	    m_mgaFco;
	CComPtr<IMgaMetaFCO>	m_metaFco;
	CString		            m_name;
  CString               m_metaName;
	objtype_enum					m_eType;
	CMaskedBitmap         m_bitmap;
	long							    m_lBorderWidth;
	bool							    m_bActive;

	COLORREF	            m_color;
	COLORREF	            m_nameColor;
};

//########################################################
//
//	CLASS : MemberDecorator
//
//########################################################

class MemberDecorator : public DecoratorBase
{
public:
  MemberDecorator();

  virtual void draw( CDC* pDC );
  virtual void LoadBitmap();
};

//########################################################
//
//	CLASS : InheritsDecorator
//
//########################################################

class InheritsDecorator : public DecoratorBase
{
public:
  InheritsDecorator();

  virtual void draw( CDC* pDC );
  virtual void LoadBitmap();
};

//########################################################
//
//	CLASS : PortDecorator
//
//########################################################

class PortDecorator	: public DecoratorBase
{
private :
	CPoint m_ptInner;
  bool m_right;

public :
	PortDecorator( CComPtr<IMgaFCO> mgaFco, const CPoint& ptInner );

	virtual void   initialize();
	virtual CSize	 getPreferredSize() const;
		      CPoint getInnerPosition() const;
  virtual void 	 draw( CDC* pDC );
  virtual void   LoadBitmap();
          void   setToRight();
};

//########################################################
//
//	CLASS : ComponentDecorator
//
//########################################################

class ComponentDecorator : public DecoratorBase
{
private:
	CComPtr<IMgaMetaPart>	    m_metaPart;
	vector<PortDecorator*>		m_vecLeftPorts;
	vector<PortDecorator*>		m_vecRightPorts;
	CComPtr<IMgaMetaAspect>	  m_spAspect;
	long							        m_iMaxPortTextLength;
	CString		                m_strTypeName;
  bool							        m_bTypeNameEnabled;
  int                       m_iTypeInfo;
public:
  ComponentDecorator( CComPtr<IMgaMetaPart>	metaPart );
  virtual ~ComponentDecorator();

  virtual void  initialize( IMgaFCO *obj, CComPtr<IMgaMetaFCO>& metaFco );
  virtual CSize getPreferredSize() const;
  virtual void  setLocation( const CRect& cRect );
	virtual void  setActive( bool bActive );
  virtual void  draw( CDC* pDC );
  virtual void  LoadBitmap();

	virtual vector<PortDecorator*>  getPorts() const;
	virtual PortDecorator*			    getPort( CComPtr<IMgaFCO> ) const;

private:
	void loadPorts();
	void orderPorts( vector<PortDecorator*>& );
  void findPorts( vector<PortDecorator*>&, CComPtr<IMgaFCOs>& );
  void checkInherits( vector<PortDecorator*>&, CComPtr<IMgaFCO>& );
};

#endif //__DECORATOR_H_
