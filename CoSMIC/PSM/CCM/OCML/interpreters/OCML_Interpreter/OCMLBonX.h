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

DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, Associated_OptionImpl, Associated_Option );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, Described_ItemImpl, Described_Item );
DECLARE_BONEXTENSION2( BON::Model, Described_Item, Option_CategoryImpl, Option_Category );
DECLARE_ABSTRACT_BONEXTENSION( Described_Item, OptionImpl, Option );
DECLARE_ABSTRACT_BONEXTENSION2( Associated_Option, Described_Item, Option_ItemImpl, Option_Item );
DECLARE_ABSTRACT_BONEXTENSION( Associated_Option, Option_ReferenceImpl, Option_Reference );
DECLARE_BONEXTENSION2( BON::Model, Option, Enum_OptionImpl, Enum_Option );
DECLARE_BONEXTENSION2( BON::Model, Option, Flag_OptionImpl, Flag_Option );
DECLARE_ABSTRACT_BONEXTENSION( Option_Reference, Atom_Option_ReferenceImpl, Atom_Option_Reference );
DECLARE_ABSTRACT_BONEXTENSION( Option_Reference, Model_Option_ReferenceImpl, Model_Option_Reference );
DECLARE_BONEXTENSION2( BON::Atom, Option, Boolean_OptionImpl, Boolean_Option );
DECLARE_BONEXTENSION2( BON::Atom, Option_Item, Enum_ItemImpl, Enum_Item );
DECLARE_BONEXTENSION2( BON::Atom, Option_Item, Flag_ItemImpl, Flag_Item );
DECLARE_BONEXTENSION2( BON::Atom, Option, Integer_OptionImpl, Integer_Option );
DECLARE_BONEXTENSION2( BON::Atom, Option, String_OptionImpl, String_Option );
DECLARE_BONEXTENSION2( BON::Reference, Atom_Option_Reference, Boolean_Option_ReferenceImpl, Boolean_Option_Reference );
DECLARE_BONEXTENSION2( BON::Reference, Model_Option_Reference, Enum_Option_ReferenceImpl, Enum_Option_Reference );
DECLARE_BONEXTENSION2( BON::Reference, Model_Option_Reference, Flag_Option_ReferenceImpl, Flag_Option_Reference );
DECLARE_BONEXTENSION2( BON::Reference, Atom_Option_Reference, Integer_Option_ReferenceImpl, Integer_Option_Reference );
DECLARE_BONEXTENSION2( BON::Reference, Atom_Option_Reference, String_Option_ReferenceImpl, String_Option_Reference );
DECLARE_BONEXTENSION( BON::Model, RuleImpl, Rule );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, AssociationImpl, Association );
DECLARE_ABSTRACT_BONEXTENSION( Association, Equality_AssociationImpl, Equality_Association );
DECLARE_ABSTRACT_BONEXTENSION( Association, Numeric_AssociationImpl, Numeric_Association );
DECLARE_BONEXTENSION2( BON::Connection, Association, Comparison_AssociationImpl, Comparison_Association );
DECLARE_BONEXTENSION2( BON::Connection, Association, Logical_AssociationImpl, Logical_Association );
DECLARE_BONEXTENSION2( BON::Connection, Association, Select_AssociationImpl, Select_Association );
DECLARE_BONEXTENSION2( BON::Connection, Equality_Association, Boolean_Equality_AssociationImpl, Boolean_Equality_Association );
DECLARE_BONEXTENSION2( BON::Connection, Equality_Association, Integer_Equality_AssociationImpl, Integer_Equality_Association );
DECLARE_BONEXTENSION2( BON::Connection, Numeric_Association, Max_AssociationImpl, Max_Association );
DECLARE_BONEXTENSION2( BON::Connection, Numeric_Association, Min_AssociationImpl, Min_Association );
DECLARE_BONEXTENSION2( BON::Connection, Numeric_Association, Range_AssociationImpl, Range_Association );
DECLARE_BONEXTENSION2( BON::Connection, Equality_Association, String_Equality_AssociationImpl, String_Equality_Association );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, Expression_BoxImpl, Expression_Box );
DECLARE_ABSTRACT_BONEXTENSION( Expression_Box, Logical_ExpressionImpl, Logical_Expression );
DECLARE_BONEXTENSION2( BON::Atom, Expression_Box, EqualImpl, Equal );
DECLARE_BONEXTENSION2( BON::Atom, Logical_Expression, ANDImpl, AND );
DECLARE_BONEXTENSION2( BON::Atom, Logical_Expression, IFImpl, IF );
DECLARE_BONEXTENSION2( BON::Atom, Logical_Expression, IFFImpl, IFF );
DECLARE_BONEXTENSION2( BON::Atom, Logical_Expression, NOTImpl, NOT );
DECLARE_BONEXTENSION2( BON::Atom, Logical_Expression, ORImpl, OR );


//*******************************************************************
//   C  L  A  S  S   Associated_OptionImpl
//*******************************************************************
class Associated_OptionImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getSelect_AssociationDsts();
	virtual std::multiset<BON::FCO>         getSelect_AssociationEnds();
	virtual std::set<Select_Association>    getSelect_AssociationLinks();
	virtual std::multiset<BON::FCO>         getSelect_AssociationSrcs();

	///BUP
	// add your own members here
	///EUP
};


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
	  public Associated_OptionImpl
	, public Described_ItemImpl
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
//   C  L  A  S  S   Option_ReferenceImpl
//*******************************************************************
class Option_ReferenceImpl :
	  public Associated_OptionImpl
{
public:


	///BUP
	// add your own members here
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
	virtual const char* get_kind_name() const { return "EnumOption"; }
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
//   C  L  A  S  S   Atom_Option_ReferenceImpl
//*******************************************************************
class Atom_Option_ReferenceImpl :
	  public Option_ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getComparison_AssociationDsts();
	virtual std::multiset<BON::FCO>         getComparison_AssociationEnds();
	virtual std::set<Comparison_Association>          getComparison_AssociationLinks();
	virtual std::multiset<BON::FCO>         getComparison_AssociationSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Model_Option_ReferenceImpl
//*******************************************************************
class Model_Option_ReferenceImpl :
	  public Option_ReferenceImpl
{
public:


	///BUP
	// add your own members here
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


//*******************************************************************
//   C  L  A  S  S   Boolean_Option_ReferenceImpl
//*******************************************************************
class Boolean_Option_ReferenceImpl :
	  virtual public BON::ReferenceImpl
	, public Atom_Option_ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getBoolean_Equality_AssociationDsts();
	virtual std::multiset<BON::FCO>         getBoolean_Equality_AssociationEnds();
	virtual std::set<Boolean_Equality_Association>    getBoolean_Equality_AssociationLinks();
	virtual std::multiset<BON::FCO>         getBoolean_Equality_AssociationSrcs();
	//
	// ref getters
	virtual Boolean_Option                  getBoolean_Option();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Enum_Option_ReferenceImpl
//*******************************************************************
class Enum_Option_ReferenceImpl :
	  virtual public BON::ReferenceImpl
	, public Model_Option_ReferenceImpl
{
public:

	//
	// ref getters
	virtual Enum_Option getEnum_Option();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Flag_Option_ReferenceImpl
//*******************************************************************
class Flag_Option_ReferenceImpl :
	  virtual public BON::ReferenceImpl
	, public Model_Option_ReferenceImpl
{
public:

	//
	// ref getters
	virtual Flag_Option getFlag_Option();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Integer_Option_ReferenceImpl
//*******************************************************************
class Integer_Option_ReferenceImpl :
	  virtual public BON::ReferenceImpl
	, public Atom_Option_ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getInteger_Equality_AssociationDsts();
	virtual std::multiset<BON::FCO>         getInteger_Equality_AssociationEnds();
	virtual std::set<Integer_Equality_Association>    getInteger_Equality_AssociationLinks();
	virtual std::multiset<BON::FCO>         getInteger_Equality_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getMax_AssociationDsts();
	virtual std::multiset<BON::FCO>         getMax_AssociationEnds();
	virtual std::set<Max_Association>       getMax_AssociationLinks();
	virtual std::multiset<BON::FCO>         getMax_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getMin_AssociationDsts();
	virtual std::multiset<BON::FCO>         getMin_AssociationEnds();
	virtual std::set<Min_Association>       getMin_AssociationLinks();
	virtual std::multiset<BON::FCO>         getMin_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getRange_AssociationDsts();
	virtual std::multiset<BON::FCO>         getRange_AssociationEnds();
	virtual std::set<Range_Association>     getRange_AssociationLinks();
	virtual std::multiset<BON::FCO>         getRange_AssociationSrcs();
	//
	// ref getters
	virtual Integer_Option                  getInteger_Option();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   String_Option_ReferenceImpl
//*******************************************************************
class String_Option_ReferenceImpl :
	  virtual public BON::ReferenceImpl
	, public Atom_Option_ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getString_Equality_AssociationDsts();
	virtual std::multiset<BON::FCO>         getString_Equality_AssociationEnds();
	virtual std::set<String_Equality_Association>     getString_Equality_AssociationLinks();
	virtual std::multiset<BON::FCO>         getString_Equality_AssociationSrcs();
	//
	// ref getters
	virtual String_Option                   getString_Option();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RuleImpl
//*******************************************************************
class RuleImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<AND>                   getAND();
	virtual std::set<Association>           getAssociation();
	virtual std::set<Boolean_Equality_Association>    getBoolean_Equality_Association();
	virtual std::set<Boolean_Option_Reference>        getBoolean_Option_Reference();
	virtual std::set<Comparison_Association>          getComparison_Association();
	virtual std::set<Enum_Option_Reference> getEnum_Option_Reference();
	virtual std::set<Equal>                 getEqual();
	virtual std::set<Expression_Box>        getExpression_Box();
	virtual std::set<Flag_Option_Reference> getFlag_Option_Reference();
	virtual std::set<IF>                    getIF();
	virtual std::set<IFF>                   getIFF();
	virtual std::set<Integer_Equality_Association>    getInteger_Equality_Association();
	virtual std::set<Integer_Option_Reference>        getInteger_Option_Reference();
	virtual std::set<Logical_Association>   getLogical_Association();
	virtual std::set<Max_Association>       getMax_Association();
	virtual std::set<Min_Association>       getMin_Association();
	virtual std::set<NOT>                   getNOT();
	virtual std::set<OR>                    getOR();
	virtual std::set<Option_Reference>      getOption_Reference();
	virtual std::set<Range_Association>     getRange_Association();
	virtual std::set<Select_Association>    getSelect_Association();
	virtual std::set<String_Equality_Association>     getString_Equality_Association();
	virtual std::set<String_Option_Reference>         getString_Option_Reference();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AssociationImpl
//*******************************************************************
class AssociationImpl :
	  virtual public BON::FCOImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Equality_AssociationImpl
//*******************************************************************
class Equality_AssociationImpl :
	  public AssociationImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Numeric_AssociationImpl
//*******************************************************************
class Numeric_AssociationImpl :
	  public AssociationImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Comparison_AssociationImpl
//*******************************************************************
class Comparison_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public AssociationImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Logical_AssociationImpl
//*******************************************************************
class Logical_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public AssociationImpl
{
public:

	//
	// connectionEnd getters
	virtual Expression_Box                  getDst();
	virtual Expression_Box                  getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Select_AssociationImpl
//*******************************************************************
class Select_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public AssociationImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Boolean_Equality_AssociationImpl
//*******************************************************************
class Boolean_Equality_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public Equality_AssociationImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isBoolean_Value() ;
	virtual void        setBoolean_Value( bool val);
	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Integer_Equality_AssociationImpl
//*******************************************************************
class Integer_Equality_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public Equality_AssociationImpl
{
public:

	//
	// attribute getters and setters
	virtual long        getInteger_Value() ;
	virtual void        setInteger_Value( const long val);
	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Max_AssociationImpl
//*******************************************************************
class Max_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public Numeric_AssociationImpl
{
public:

	//
	// attribute getters and setters
	virtual long        getMaximum() ;
	virtual void        setMaximum( const long val);
	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Min_AssociationImpl
//*******************************************************************
class Min_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public Numeric_AssociationImpl
{
public:

	//
	// attribute getters and setters
	virtual long        getMinimum() ;
	virtual void        setMinimum( const long val);
	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Range_AssociationImpl
//*******************************************************************
class Range_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public Numeric_AssociationImpl
{
public:

	//
	// attribute getters and setters
	virtual long        getMaximum() ;
	virtual long        getMinimum() ;
	virtual void        setMaximum( const long val);
	virtual void        setMinimum( const long val);
	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   String_Equality_AssociationImpl
//*******************************************************************
class String_Equality_AssociationImpl :
	  virtual public BON::ConnectionImpl
	, public Equality_AssociationImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getString_Value() ;
	virtual void        setString_Value( const std::string& val);
	//
	// connectionEnd getters
	virtual BON::FCO    getDst();
	virtual BON::FCO    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Expression_BoxImpl
//*******************************************************************
class Expression_BoxImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getBoolean_Equality_AssociationDsts();
	virtual std::multiset<BON::FCO>         getBoolean_Equality_AssociationEnds();
	virtual std::set<Boolean_Equality_Association>    getBoolean_Equality_AssociationLinks();
	virtual std::multiset<BON::FCO>         getBoolean_Equality_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getInteger_Equality_AssociationDsts();
	virtual std::multiset<BON::FCO>         getInteger_Equality_AssociationEnds();
	virtual std::set<Integer_Equality_Association>    getInteger_Equality_AssociationLinks();
	virtual std::multiset<BON::FCO>         getInteger_Equality_AssociationSrcs();
	virtual std::multiset<Expression_Box>   getLogical_AssociationDsts();
	virtual std::multiset<Expression_Box>   getLogical_AssociationEnds();
	virtual std::set<Logical_Association>   getLogical_AssociationLinks();
	virtual std::multiset<Expression_Box>   getLogical_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getMax_AssociationDsts();
	virtual std::multiset<BON::FCO>         getMax_AssociationEnds();
	virtual std::set<Max_Association>       getMax_AssociationLinks();
	virtual std::multiset<BON::FCO>         getMax_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getMin_AssociationDsts();
	virtual std::multiset<BON::FCO>         getMin_AssociationEnds();
	virtual std::set<Min_Association>       getMin_AssociationLinks();
	virtual std::multiset<BON::FCO>         getMin_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getRange_AssociationDsts();
	virtual std::multiset<BON::FCO>         getRange_AssociationEnds();
	virtual std::set<Range_Association>     getRange_AssociationLinks();
	virtual std::multiset<BON::FCO>         getRange_AssociationSrcs();
	virtual std::multiset<BON::ConnectionEnd>         getSelect_AssociationDsts();
	virtual std::multiset<BON::ConnectionEnd>         getSelect_AssociationEnds();
	virtual std::set<Select_Association>    getSelect_AssociationLinks();
	virtual std::multiset<BON::ConnectionEnd>         getSelect_AssociationSrcs();
	virtual std::multiset<BON::FCO>         getString_Equality_AssociationDsts();
	virtual std::multiset<BON::FCO>         getString_Equality_AssociationEnds();
	virtual std::set<String_Equality_Association>     getString_Equality_AssociationLinks();
	virtual std::multiset<BON::FCO>         getString_Equality_AssociationSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Logical_ExpressionImpl
//*******************************************************************
class Logical_ExpressionImpl :
	  public Expression_BoxImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Expression_Box>   getLogical_AssociationDsts();
	virtual std::multiset<Expression_Box>   getLogical_AssociationEnds();
	virtual std::set<Logical_Association>   getLogical_AssociationLinks();
	virtual std::multiset<Expression_Box>   getLogical_AssociationSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EqualImpl
//*******************************************************************
class EqualImpl :
	  virtual public BON::AtomImpl
	, public Expression_BoxImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::FCO>         getComparison_AssociationDsts();
	virtual std::multiset<BON::FCO>         getComparison_AssociationEnds();
	virtual std::set<Comparison_Association>          getComparison_AssociationLinks();
	virtual std::multiset<BON::FCO>         getComparison_AssociationSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ANDImpl
//*******************************************************************
class ANDImpl :
	  virtual public BON::AtomImpl
	, public Logical_ExpressionImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   IFImpl
//*******************************************************************
class IFImpl :
	  virtual public BON::AtomImpl
	, public Logical_ExpressionImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   IFFImpl
//*******************************************************************
class IFFImpl :
	  virtual public BON::AtomImpl
	, public Logical_ExpressionImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NOTImpl
//*******************************************************************
class NOTImpl :
	  virtual public BON::AtomImpl
	, public Logical_ExpressionImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ORImpl
//*******************************************************************
class ORImpl :
	  virtual public BON::AtomImpl
	, public Logical_ExpressionImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};



}; // namespace OCML_BON

///BUP
// add your additional class definitions here
///EUP

#endif // OCMLBONX_H
