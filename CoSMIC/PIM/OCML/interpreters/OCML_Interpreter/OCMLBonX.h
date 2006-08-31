#ifndef OCMLBONX_H
#define OCMLBONX_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
// add your include files/class definitions here
#include <xercesc/dom/DOM.hpp>
///EUP

namespace OCML_BON
{

DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, Described_ItemImpl, Described_Item );
DECLARE_BONEXTENSION2( BON::Model, Described_Item, Option_CategoryImpl, Option_Category );
DECLARE_ABSTRACT_BONEXTENSION( Described_Item, OptionImpl, Option );
DECLARE_ABSTRACT_BONEXTENSION( Described_Item, Option_ItemImpl, Option_Item );
DECLARE_BONEXTENSION2( BON::Model, Option, Enum_OptionImpl, Enum_Option );
DECLARE_BONEXTENSION2( BON::Model, Option, Flag_OptionImpl, Flag_Option );
DECLARE_BONEXTENSION2( BON::Atom, Option, Boolean_OptionImpl, Boolean_Option );
DECLARE_BONEXTENSION2( BON::Atom, Option_Item, Enum_ItemImpl, Enum_Item );
DECLARE_BONEXTENSION2( BON::Atom, Option_Item, Flag_ItemImpl, Flag_Item );
DECLARE_BONEXTENSION2( BON::Atom, Option, Integer_OptionImpl, Integer_Option );
DECLARE_BONEXTENSION2( BON::Atom, Option, String_OptionImpl, String_Option );


//*******************************************************************
//   C  L  A  S  S   Described_ItemImpl
//*******************************************************************
class Described_ItemImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getDescription() ;
	virtual std::string getTitle() ;
	virtual void        setDescription( const std::string& val);
	virtual void        setTitle( const std::string& val);

	///BUP
	// add your own members here
        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

protected:
	virtual const char* get_kind_name() const = 0;
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Option_CategoryImpl
//*******************************************************************
class Option_CategoryImpl :
	  virtual public BON::ModelImpl
	, public Described_ItemImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Boolean_Option>        getBoolean_Option();
	virtual std::set<Enum_Option>           getEnum_Option();
	virtual std::set<Flag_Option>           getFlag_Option();
	virtual std::set<Integer_Option>        getInteger_Option();
	virtual std::set<Option>                getOption();
	virtual std::set<Option_Category>       getOption_Category();
	virtual std::set<String_Option>         getString_Option();

	///BUP
	// add your own members here
        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

protected:
	virtual const char* get_kind_name() const { return "OptionCategory"; }
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OptionImpl
//*******************************************************************
class OptionImpl :
	  public Described_ItemImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getParameter_Name() ;
	virtual void        setParameter_Name( const std::string& val);

	///BUP
	// add your own members here

        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Option_ItemImpl
//*******************************************************************
class Option_ItemImpl :
	  public Described_ItemImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isItem_Default_Value() ;
	virtual void        setItem_Default_Value( bool val);

	///BUP
	// add your own members here

        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Enum_OptionImpl
//*******************************************************************
class Enum_OptionImpl :
	  virtual public BON::ModelImpl
	, public OptionImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Enum_Item>             getEnum_Item();

	///BUP
	// add your own members here
        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

protected:
	virtual const char* get_kind_name() const { return "StringOption"; } // "EnumOption"
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Flag_OptionImpl
//*******************************************************************
class Flag_OptionImpl :
	  virtual public BON::ModelImpl
	, public OptionImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Flag_Item>             getFlag_Item();

	///BUP
	// add your own members here
protected:
	virtual const char* get_kind_name() const { return "FlagOption"; }
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Boolean_OptionImpl
//*******************************************************************
class Boolean_OptionImpl :
	  virtual public BON::AtomImpl
	, public OptionImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isBoolean_Default_Value() ;
	virtual void        setBoolean_Default_Value( bool val);

	///BUP
	// add your own members here

        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

protected:
	virtual const char* get_kind_name() const { return "BooleanOption"; }

	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Enum_ItemImpl
//*******************************************************************
class Enum_ItemImpl :
	  virtual public BON::AtomImpl
	, public Option_ItemImpl
{
public:


	///BUP
	// add your own members here
protected:
	virtual const char* get_kind_name() const { return "Item"; }
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Flag_ItemImpl
//*******************************************************************
class Flag_ItemImpl :
	  virtual public BON::AtomImpl
	, public Option_ItemImpl
{
public:


	///BUP
	// add your own members here
protected:
	virtual const char* get_kind_name() const { return "Item"; }
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Integer_OptionImpl
//*******************************************************************
class Integer_OptionImpl :
	  virtual public BON::AtomImpl
	, public OptionImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getInteger_Default_Value() ;
	virtual void        setInteger_Default_Value( const std::string& val);

	///BUP
	// add your own members here

        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

protected:
	virtual const char* get_kind_name() const { return "IntegerOption"; }

	///EUP
};


//*******************************************************************
//   C  L  A  S  S   String_OptionImpl
//*******************************************************************
class String_OptionImpl :
	  virtual public BON::AtomImpl
	, public OptionImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getString_Default_Value() ;
	virtual void        setString_Default_Value( const std::string& val);

	///BUP
	// add your own members here

        virtual xercesc::DOMElement* xml_export(xercesc::DOMDocument* doc);

protected:
	virtual const char* get_kind_name() const { return "StringOption"; }

	///EUP
};



}; // namespace OCML_BON

///BUP
// add your additional class definitions here
///EUP

#endif // OCMLBONX_H
