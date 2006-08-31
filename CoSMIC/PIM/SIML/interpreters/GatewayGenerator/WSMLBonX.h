#ifndef WSMLBONX_H
#define WSMLBONX_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

//
// this dummy template parameter is needed by VC6 to deduct the return type
// VC7 users can modify this macro to a value like "int _dummy = 0"
// TDP standing for TEMPLATE DUMMY PAR
//#define TDP int _dummy = 0			// recommended for VC7 users
#define TDP const T & _dummyComp		// recommended for VC6 users

///BUP
// add your include files/class definitions here
///EUP

namespace WSML
{

DECLARE_ABSTRACT_BONEXTENSION( BON::Model, OperationImpl, Operation );
DECLARE_BONEXTENSION( Operation, NotificationOperationImpl, NotificationOperation );
DECLARE_BONEXTENSION( Operation, OneWayOperationImpl, OneWayOperation );
DECLARE_BONEXTENSION( Operation, RequestResponseOperationImpl, RequestResponseOperation );
DECLARE_BONEXTENSION( Operation, SolicitResponseOperationImpl, SolicitResponseOperation );
DECLARE_BONEXTENSION( BON::Model, SchemaReferenceImpl, SchemaReference );
DECLARE_BONEXTENSION( SchemaReference, SchemaImportImpl, SchemaImport );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ExtensibilityElementImpl, ExtensibilityElement );
DECLARE_ABSTRACT_BONEXTENSION( ExtensibilityElement, ExtensibleBindingImpl, ExtensibleBinding );
DECLARE_ABSTRACT_BONEXTENSION( ExtensibilityElement, ExtensibleMessageImpl, ExtensibleMessage );
DECLARE_ABSTRACT_BONEXTENSION( ExtensibilityElement, ExtensibleOperationImpl, ExtensibleOperation );
DECLARE_ABSTRACT_BONEXTENSION( ExtensibilityElement, ExtensiblePortImpl, ExtensiblePort );
DECLARE_ABSTRACT_BONEXTENSION( ExtensibleMessage, SOAPHeadersImpl, SOAPHeaders );
DECLARE_BONEXTENSION2( BON::Atom, ExtensiblePort, SOAPAddressImpl, SOAPAddress );
DECLARE_BONEXTENSION2( BON::Atom, ExtensibleBinding, SOAPBindingImpl, SOAPBinding );
DECLARE_BONEXTENSION2( BON::Atom, ExtensibleMessage, SOAPBodyImpl, SOAPBody );
DECLARE_BONEXTENSION2( BON::Atom, ExtensibleMessage, SOAPFaultImpl, SOAPFault );
DECLARE_BONEXTENSION2( BON::Atom, ExtensibleOperation, SOAPOperationImpl, SOAPOperation );
DECLARE_BONEXTENSION2( BON::Model, SOAPHeaders, SOAPHeaderImpl, SOAPHeader );
DECLARE_BONEXTENSION2( BON::Atom, SOAPHeaders, SOAPHeaderFaultImpl, SOAPHeaderFault );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ReferrableImpl, Referrable );
DECLARE_BONEXTENSION2( BON::Model, Referrable, ElementImpl, Element );
DECLARE_BONEXTENSION2( BON::Atom, Referrable, AtomicTypeImpl, AtomicType );
DECLARE_BONEXTENSION( BON::Model, BindingImpl, Binding );
DECLARE_BONEXTENSION( BON::Model, BindingOperationImpl, BindingOperation );
DECLARE_BONEXTENSION( BON::Model, DefinitionsImpl, Definitions );
DECLARE_BONEXTENSION( BON::Model, DocumentationImpl, Documentation );
DECLARE_BONEXTENSION( BON::Model, FaultImpl, Fault );
DECLARE_BONEXTENSION( BON::Model, ImportImpl, Import );
DECLARE_BONEXTENSION( BON::Model, InputImpl, Input );
DECLARE_BONEXTENSION( BON::Model, MessageImpl, Message );
DECLARE_BONEXTENSION( BON::Model, MessagesImpl, Messages );
DECLARE_BONEXTENSION( BON::Model, OutputImpl, Output );
DECLARE_BONEXTENSION( BON::Model, PortImpl, Port );
DECLARE_BONEXTENSION( BON::Model, PortTypeImpl, PortType );
DECLARE_BONEXTENSION( BON::Model, SchemaImpl, Schema );
DECLARE_BONEXTENSION( BON::Model, SchemaBuiltinsImpl, SchemaBuiltins );
DECLARE_BONEXTENSION( BON::Model, ServiceImpl, Service );
DECLARE_BONEXTENSION( BON::Model, TypesImpl, Types );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, IORefImpl, IORef );
DECLARE_BONEXTENSION2( BON::Reference, IORef, InputRefImpl, InputRef );
DECLARE_BONEXTENSION2( BON::Reference, IORef, OutputRefImpl, OutputRef );
DECLARE_BONEXTENSION( BON::Atom, AttributeImpl, Attribute );
DECLARE_BONEXTENSION( BON::Connection, BindingPortTypeImpl, BindingPortType );
DECLARE_BONEXTENSION( BON::Connection, BindsOperationImpl, BindsOperation );
DECLARE_BONEXTENSION( BON::Connection, BodyBindingImpl, BodyBinding );
DECLARE_BONEXTENSION( BON::Connection, BodyPartsImpl, BodyParts );
DECLARE_BONEXTENSION( BON::Connection, FaultMessageImpl, FaultMessage );
DECLARE_BONEXTENSION( BON::Connection, HeaderBindingImpl, HeaderBinding );
DECLARE_BONEXTENSION( BON::Connection, HeaderMessageImpl, HeaderMessage );
DECLARE_BONEXTENSION( BON::Connection, HeaderPartImpl, HeaderPart );
DECLARE_BONEXTENSION( BON::Connection, InputMessageImpl, InputMessage );
DECLARE_BONEXTENSION( BON::Connection, OutputMessageImpl, OutputMessage );
DECLARE_BONEXTENSION( BON::Connection, PortBindingImpl, PortBinding );
DECLARE_BONEXTENSION( BON::Connection, SOAPFaultExtensionImpl, SOAPFaultExtension );
DECLARE_BONEXTENSION( BON::Reference, BindingRefImpl, BindingRef );
DECLARE_BONEXTENSION( BON::Reference, FaultRefImpl, FaultRef );
DECLARE_BONEXTENSION( BON::Reference, MessageRefImpl, MessageRef );
DECLARE_BONEXTENSION( BON::Reference, OperationRefImpl, OperationRef );
DECLARE_BONEXTENSION( BON::Reference, PartImpl, Part );
DECLARE_BONEXTENSION( BON::Reference, PartRefImpl, PartRef );


//*******************************************************************
//   C  L  A  S  S   OperationImpl
//*******************************************************************
class OperationImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<FaultMessage>          getFaultMessage();
	//********************************************************************************
	// getter for role "FaultMessage" among "FaultMessage"s
	//********************************************************************************
	template <class T>
	std::set<FaultMessage, T>		getFaultMessage(TDP)
	{
		std::set<FaultMessage, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::FaultMessage");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			FaultMessage elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<InputMessage>          getInputMessage();
	//********************************************************************************
	// getter for role "InputMessage" among "InputMessage"s
	//********************************************************************************
	template <class T>
	std::set<InputMessage, T>		getInputMessage(TDP)
	{
		std::set<InputMessage, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::InputMessage");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			InputMessage elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<MessageRef>            getMessageRef();
	//********************************************************************************
	// getter for role "MessageRef" among "MessageRef"s
	//********************************************************************************
	template <class T>
	std::set<MessageRef, T>		getMessageRef(TDP)
	{
		std::set<MessageRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::MessageRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			MessageRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<OutputMessage>         getOutputMessage();
	//********************************************************************************
	// getter for role "OutputMessage" among "OutputMessage"s
	//********************************************************************************
	template <class T>
	std::set<OutputMessage, T>		getOutputMessage(TDP)
	{
		std::set<OutputMessage, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OutputMessage");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			OutputMessage elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NotificationOperationImpl
//*******************************************************************
class NotificationOperationImpl :
	  public OperationImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Output>                getOutput();
	//********************************************************************************
	// getter for role "Output" among "Output"s
	//********************************************************************************
	template <class T>
	std::set<Output, T>		getOutput(TDP)
	{
		std::set<Output, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Output");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Output elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OneWayOperationImpl
//*******************************************************************
class OneWayOperationImpl :
	  public OperationImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Input>                 getInput();
	//********************************************************************************
	// getter for role "Input" among "Input"s
	//********************************************************************************
	template <class T>
	std::set<Input, T>		getInput(TDP)
	{
		std::set<Input, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Input");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Input elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RequestResponseOperationImpl
//*******************************************************************
class RequestResponseOperationImpl :
	  public OperationImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getparameterOrder() ;
	virtual void        setparameterOrder( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Fault>                 getFault();
	//********************************************************************************
	// getter for role "Fault" among "Fault"s
	//********************************************************************************
	template <class T>
	std::set<Fault, T>		getFault(TDP)
	{
		std::set<Fault, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Fault");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Fault elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Input>                 getInput();
	//********************************************************************************
	// getter for role "Input" among "Input"s
	//********************************************************************************
	template <class T>
	std::set<Input, T>		getInput(TDP)
	{
		std::set<Input, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Input");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Input elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Output>                getOutput();
	//********************************************************************************
	// getter for role "Output" among "Output"s
	//********************************************************************************
	template <class T>
	std::set<Output, T>		getOutput(TDP)
	{
		std::set<Output, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Output");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Output elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SolicitResponseOperationImpl
//*******************************************************************
class SolicitResponseOperationImpl :
	  public OperationImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getparameterOrder() ;
	virtual void        setparameterOrder( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Fault>                 getFault();
	//********************************************************************************
	// getter for role "Fault" among "Fault"s
	//********************************************************************************
	template <class T>
	std::set<Fault, T>		getFault(TDP)
	{
		std::set<Fault, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Fault");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Fault elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Input>                 getInput();
	//********************************************************************************
	// getter for role "Input" among "Input"s
	//********************************************************************************
	template <class T>
	std::set<Input, T>		getInput(TDP)
	{
		std::set<Input, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Input");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Input elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Output>                getOutput();
	//********************************************************************************
	// getter for role "Output" among "Output"s
	//********************************************************************************
	template <class T>
	std::set<Output, T>		getOutput(TDP)
	{
		std::set<Output, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Output");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Output elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SchemaReferenceImpl
//*******************************************************************
class SchemaReferenceImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getid() ;
	virtual std::string getschemaLocation() ;
	virtual void        setid( const std::string& val);
	virtual void        setschemaLocation( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Schema>                getSchema();
	//********************************************************************************
	// getter for role "Schema" among "Schema"s
	//********************************************************************************
	template <class T>
	std::set<Schema, T>		getSchema(TDP)
	{
		std::set<Schema, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Schema");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Schema elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SchemaImportImpl
//*******************************************************************
class SchemaImportImpl :
	  public SchemaReferenceImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getnamespace() ;
	virtual void        setnamespace( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExtensibilityElementImpl
//*******************************************************************
class ExtensibilityElementImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isrequired() ;
	virtual void        setrequired( bool val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExtensibleBindingImpl
//*******************************************************************
class ExtensibleBindingImpl :
	  public ExtensibilityElementImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExtensibleMessageImpl
//*******************************************************************
class ExtensibleMessageImpl :
	  public ExtensibilityElementImpl
{
public:
	typedef enum
	{
		literal_use_Type,
		encoded_use_Type
	} use_Type;

	//
	// attribute getters and setters
	virtual std::string getencodingStyle() ;
	virtual std::string getnamespace() ;
	virtual ExtensibleMessageImpl::use_Type getuse();
	virtual void        setencodingStyle( const std::string& val);
	virtual void        setnamespace( const std::string& val);
	virtual void        setuse( ExtensibleMessageImpl::use_Type val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExtensibleOperationImpl
//*******************************************************************
class ExtensibleOperationImpl :
	  public ExtensibilityElementImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExtensiblePortImpl
//*******************************************************************
class ExtensiblePortImpl :
	  public ExtensibilityElementImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPHeadersImpl
//*******************************************************************
class SOAPHeadersImpl :
	  public ExtensibleMessageImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<IORef>            getHeaderBindingSrcs();
	//********************************************************************************
	// returns src IORefs
	//********************************************************************************
	template <class T>
	std::multiset<IORef, T>		getHeaderBindingSrcs(TDP)
	{
		std::multiset<IORef, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::HeaderBinding");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				IORef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<MessageRef>       getHeaderMessageDsts();
	//********************************************************************************
	// returns dst MessageRefs
	//********************************************************************************
	template <class T>
	std::multiset<MessageRef, T>		getHeaderMessageDsts(TDP)
	{
		std::multiset<MessageRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::HeaderMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				MessageRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<PartRef>          getHeaderPartDsts();
	//********************************************************************************
	// returns dst PartRefs
	//********************************************************************************
	template <class T>
	std::multiset<PartRef, T>		getHeaderPartDsts(TDP)
	{
		std::multiset<PartRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::HeaderPart");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				PartRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<HeaderBinding>         getInHeaderBindingLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<HeaderBinding, T>		getInHeaderBindingLinks(TDP)
	{
		std::set<HeaderBinding, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			HeaderBinding c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<HeaderMessage>         getOutHeaderMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<HeaderMessage, T>		getOutHeaderMessageLinks(TDP)
	{
		std::set<HeaderMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			HeaderMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<HeaderPart>            getOutHeaderPartLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<HeaderPart, T>		getOutHeaderPartLinks(TDP)
	{
		std::set<HeaderPart, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			HeaderPart c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPAddressImpl
//*******************************************************************
class SOAPAddressImpl :
	  virtual public BON::AtomImpl
	, public ExtensiblePortImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getlocation() ;
	virtual void        setlocation( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPBindingImpl
//*******************************************************************
class SOAPBindingImpl :
	  virtual public BON::AtomImpl
	, public ExtensibleBindingImpl
{
public:
	typedef enum
	{
		document_style_Type,
		rpc_style_Type
	} style_Type;

	//
	// attribute getters and setters
	virtual SOAPBindingImpl::style_Type     getstyle();
	virtual std::string gettransport() ;
	virtual void        setstyle( SOAPBindingImpl::style_Type val);
	virtual void        settransport( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPBodyImpl
//*******************************************************************
class SOAPBodyImpl :
	  virtual public BON::AtomImpl
	, public ExtensibleMessageImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<IORef>            getBodyBindingSrcs();
	//********************************************************************************
	// returns src IORefs
	//********************************************************************************
	template <class T>
	std::multiset<IORef, T>		getBodyBindingSrcs(TDP)
	{
		std::multiset<IORef, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::BodyBinding");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				IORef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<PartRef>          getBodyPartsDsts();
	//********************************************************************************
	// returns dst PartRefs
	//********************************************************************************
	template <class T>
	std::multiset<PartRef, T>		getBodyPartsDsts(TDP)
	{
		std::multiset<PartRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::BodyParts");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				PartRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BodyBinding>           getInBodyBindingLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BodyBinding, T>		getInBodyBindingLinks(TDP)
	{
		std::set<BodyBinding, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BodyBinding c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<BodyParts>             getOutBodyPartsLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BodyParts, T>		getOutBodyPartsLinks(TDP)
	{
		std::set<BodyParts, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BodyParts c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPFaultImpl
//*******************************************************************
class SOAPFaultImpl :
	  virtual public BON::AtomImpl
	, public ExtensibleMessageImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<SOAPFaultExtension>    getInSOAPFaultExtensionLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<SOAPFaultExtension, T>		getInSOAPFaultExtensionLinks(TDP)
	{
		std::set<SOAPFaultExtension, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			SOAPFaultExtension c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::multiset<FaultRef>         getSOAPFaultExtensionSrcs();
	//********************************************************************************
	// returns src FaultRefs
	//********************************************************************************
	template <class T>
	std::multiset<FaultRef, T>		getSOAPFaultExtensionSrcs(TDP)
	{
		std::multiset<FaultRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::SOAPFaultExtension");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				FaultRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPOperationImpl
//*******************************************************************
class SOAPOperationImpl :
	  virtual public BON::AtomImpl
	, public ExtensibleOperationImpl
{
public:
	typedef enum
	{
		document_style_Type,
		rpc_style_Type
	} style_Type;

	//
	// attribute getters and setters
	virtual std::string getsoapAction() ;
	virtual SOAPOperationImpl::style_Type   getstyle();
	virtual void        setsoapAction( const std::string& val);
	virtual void        setstyle( SOAPOperationImpl::style_Type val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPHeaderImpl
//*******************************************************************
class SOAPHeaderImpl :
	  virtual public BON::ModelImpl
	, public SOAPHeadersImpl
{
public:

	//
	// kind and role getters
	virtual std::set<SOAPHeaderFault>       getSOAPHeaderFault();
	//********************************************************************************
	// getter for role "SOAPHeaderFault" among "SOAPHeaderFault"s
	//********************************************************************************
	template <class T>
	std::set<SOAPHeaderFault, T>		getSOAPHeaderFault(TDP)
	{
		std::set<SOAPHeaderFault, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPHeaderFault");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPHeaderFault elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPHeaderFaultImpl
//*******************************************************************
class SOAPHeaderFaultImpl :
	  virtual public BON::AtomImpl
	, public SOAPHeadersImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReferrableImpl
//*******************************************************************
class ReferrableImpl :
	  virtual public BON::FCOImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ElementImpl
//*******************************************************************
class ElementImpl :
	  virtual public BON::ModelImpl
	, public ReferrableImpl
{
public:
	typedef enum
	{
		local_schemaType_Type,
		element_schemaType_Type,
		attribute_schemaType_Type,
		complexType_schemaType_Type,
		simpleType_schemaType_Type,
		include_schemaType_Type,
		import_schemaType_Type,
		redefine_schemaType_Type,
		any_schemaType_Type,
		anyAttribute_schemaType_Type,
		group_schemaType_Type,
		attributeGroup_schemaType_Type,
		key_schemaType_Type,
		keyref_schemaType_Type,
		unique_schemaType_Type,
		annotation_schemaType_Type,
		documentation_schemaType_Type,
		appinfo_schemaType_Type,
		notation_schemaType_Type
	} schemaType_Type;

	//
	// attribute getters and setters
	virtual long        getOrder() ;
	virtual ElementImpl::schemaType_Type    getschemaType();
	virtual void        setOrder( const long val);
	virtual void        setschemaType( ElementImpl::schemaType_Type val);
	//
	// kind and role getters
	virtual std::set<Attribute>             getAttribute();
	//********************************************************************************
	// getter for role "Attribute" among "Attribute"s
	//********************************************************************************
	template <class T>
	std::set<Attribute, T>		getAttribute(TDP)
	{
		std::set<Attribute, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Attribute");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Attribute elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Element>               getElement();
	//********************************************************************************
	// getter for role "Element" among "Element"s
	//********************************************************************************
	template <class T>
	std::set<Element, T>		getElement(TDP)
	{
		std::set<Element, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Element");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Element elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AtomicTypeImpl
//*******************************************************************
class AtomicTypeImpl :
	  virtual public BON::AtomImpl
	, public ReferrableImpl
{
public:
	typedef enum
	{
		string_typeCode_Type,
		boolean_typeCode_Type,
		float_typeCode_Type,
		double_typeCode_Type,
		decimal_typeCode_Type,
		duration_typeCode_Type,
		dateTime_typeCode_Type,
		time_typeCode_Type,
		date_typeCode_Type,
		gYearMonth_typeCode_Type,
		gYear_typeCode_Type,
		gMonthDay_typeCode_Type,
		gDay_typeCode_Type,
		gMonth_typeCode_Type,
		hexBinary_typeCode_Type,
		base64Binary_typeCode_Type,
		anyURI_typeCode_Type,
		QName_typeCode_Type,
		NOTATION_typeCode_Type,
		normalizedString_typeCode_Type,
		token_typeCode_Type,
		language_typeCode_Type,
		IDREFS_typeCode_Type,
		ENTITIES_typeCode_Type,
		NMTOKEN_typeCode_Type,
		NMTOKENS_typeCode_Type,
		name_typeCode_Type,
		NCName_typeCode_Type,
		ID_typeCode_Type,
		IDREF_typeCode_Type,
		ENTITY_typeCode_Type,
		integer_typeCode_Type,
		nonPositiveInteger_typeCode_Type,
		negativeInteger_typeCode_Type,
		long_typeCode_Type,
		int_typeCode_Type,
		short_typeCode_Type,
		byte_typeCode_Type,
		nonNegativeInteger_typeCode_Type,
		unsignedLong_typeCode_Type,
		unsignedInt_typeCode_Type,
		unsignedShort_typeCode_Type,
		unsignedByte_typeCode_Type,
		positiveInteger_typeCode_Type
	} typeCode_Type;

	//
	// attribute getters and setters
	virtual AtomicTypeImpl::typeCode_Type   gettypeCode();
	virtual void        settypeCode( AtomicTypeImpl::typeCode_Type val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BindingImpl
//*******************************************************************
class BindingImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<PortType>         getBindingPortTypeDsts();
	//********************************************************************************
	// returns dst PortTypes
	//********************************************************************************
	template <class T>
	std::multiset<PortType, T>		getBindingPortTypeDsts(TDP)
	{
		std::multiset<PortType, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::BindingPortType");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				PortType dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BindingPortType>       getOutBindingPortTypeLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BindingPortType, T>		getOutBindingPortTypeLinks(TDP)
	{
		std::set<BindingPortType, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BindingPortType c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// kind and role getters
	virtual std::set<BindingOperation>      getBindingOperation();
	//********************************************************************************
	// getter for role "BindingOperation" among "BindingOperation"s
	//********************************************************************************
	template <class T>
	std::set<BindingOperation, T>		getBindingOperation(TDP)
	{
		std::set<BindingOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindingOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			BindingOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<BindsOperation>        getBindsOperation();
	//********************************************************************************
	// getter for role "BindsOperation" among "BindsOperation"s
	//********************************************************************************
	template <class T>
	std::set<BindsOperation, T>		getBindsOperation(TDP)
	{
		std::set<BindsOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindsOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			BindsOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<OperationRef>          getOperationRef();
	//********************************************************************************
	// getter for role "OperationRef" among "OperationRef"s
	//********************************************************************************
	template <class T>
	std::set<OperationRef, T>		getOperationRef(TDP)
	{
		std::set<OperationRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OperationRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			OperationRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPBinding>           getSOAPBinding();
	//********************************************************************************
	// getter for role "SOAPBinding" among "ExtensibleBinding"s
	//********************************************************************************
	template <class T>
	std::set<SOAPBinding, T>		getSOAPBinding(TDP)
	{
		std::set<SOAPBinding, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPBinding");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPBinding elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BindingOperationImpl
//*******************************************************************
class BindingOperationImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<OperationRef>     getBindsOperationDsts();
	//********************************************************************************
	// returns dst OperationRefs
	//********************************************************************************
	template <class T>
	std::multiset<OperationRef, T>		getBindsOperationDsts(TDP)
	{
		std::multiset<OperationRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::BindsOperation");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				OperationRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BindsOperation>        getOutBindsOperationLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BindsOperation, T>		getOutBindsOperationLinks(TDP)
	{
		std::set<BindsOperation, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BindsOperation c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// kind and role getters
	virtual std::set<BodyBinding>           getBodyBinding();
	//********************************************************************************
	// getter for role "BodyBinding" among "BodyBinding"s
	//********************************************************************************
	template <class T>
	std::set<BodyBinding, T>		getBodyBinding(TDP)
	{
		std::set<BodyBinding, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BodyBinding");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			BodyBinding elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<BodyParts>             getBodyParts();
	//********************************************************************************
	// getter for role "BodyParts" among "BodyParts"s
	//********************************************************************************
	template <class T>
	std::set<BodyParts, T>		getBodyParts(TDP)
	{
		std::set<BodyParts, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BodyParts");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			BodyParts elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<ExtensibleMessage>     getExtensibleMessage();
	//********************************************************************************
	// aggregated getter for role "" among "ExtensibleMessage"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<ExtensibleMessage, T>		getExtensibleMessage(TDP)
	{
		std::set<ExtensibleMessage, T> res;
		const int len = 4;
		std::set<BON::FCO> roles_vec[ len];
		roles_vec[0] = ModelImpl::getChildFCOsAs("WSML::SOAPBody");
		roles_vec[1] = ModelImpl::getChildFCOsAs("WSML::SOAPFault");
		roles_vec[2] = ModelImpl::getChildFCOsAs("WSML::SOAPHeaderFault");
		roles_vec[3] = ModelImpl::getChildFCOsAs("WSML::SOAPHeader");
		for( int k = 0; k < len; ++k)
			for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
			{
				ExtensibleMessage elem(*i);
				ASSERT(elem);
				res.insert(elem);
			}
		return res;
	}

	virtual std::set<FaultRef>              getFaultRef();
	//********************************************************************************
	// getter for role "FaultRef" among "FaultRef"s
	//********************************************************************************
	template <class T>
	std::set<FaultRef, T>		getFaultRef(TDP)
	{
		std::set<FaultRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::FaultRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			FaultRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<HeaderBinding>         getHeaderBinding();
	//********************************************************************************
	// getter for role "HeaderBinding" among "HeaderBinding"s
	//********************************************************************************
	template <class T>
	std::set<HeaderBinding, T>		getHeaderBinding(TDP)
	{
		std::set<HeaderBinding, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::HeaderBinding");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			HeaderBinding elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<HeaderMessage>         getHeaderMessage();
	//********************************************************************************
	// getter for role "HeaderMessage" among "HeaderMessage"s
	//********************************************************************************
	template <class T>
	std::set<HeaderMessage, T>		getHeaderMessage(TDP)
	{
		std::set<HeaderMessage, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::HeaderMessage");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			HeaderMessage elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<HeaderPart>            getHeaderPart();
	//********************************************************************************
	// getter for role "HeaderPart" among "HeaderPart"s
	//********************************************************************************
	template <class T>
	std::set<HeaderPart, T>		getHeaderPart(TDP)
	{
		std::set<HeaderPart, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::HeaderPart");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			HeaderPart elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<IORef>                 getIORef();
	//********************************************************************************
	// aggregated getter for role "" among "IORef"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<IORef, T>		getIORef(TDP)
	{
		std::set<IORef, T> res;
		const int len = 2;
		std::set<BON::FCO> roles_vec[ len];
		roles_vec[0] = ModelImpl::getChildFCOsAs("WSML::InputRef");
		roles_vec[1] = ModelImpl::getChildFCOsAs("WSML::OutputRef");
		for( int k = 0; k < len; ++k)
			for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
			{
				IORef elem(*i);
				ASSERT(elem);
				res.insert(elem);
			}
		return res;
	}

	virtual std::set<InputRef>              getInputRef();
	//********************************************************************************
	// getter for role "InputRef" among "IORef"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<InputRef, T>		getInputRef(TDP)
	{
		std::set<InputRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::InputRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			InputRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<MessageRef>            getMessageRef();
	//********************************************************************************
	// getter for role "MessageRef" among "MessageRef"s
	//********************************************************************************
	template <class T>
	std::set<MessageRef, T>		getMessageRef(TDP)
	{
		std::set<MessageRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::MessageRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			MessageRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<OutputRef>             getOutputRef();
	//********************************************************************************
	// getter for role "OutputRef" among "IORef"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<OutputRef, T>		getOutputRef(TDP)
	{
		std::set<OutputRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OutputRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			OutputRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<PartRef>               getPartRef();
	//********************************************************************************
	// getter for role "PartRef" among "PartRef"s
	//********************************************************************************
	template <class T>
	std::set<PartRef, T>		getPartRef(TDP)
	{
		std::set<PartRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::PartRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			PartRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPBody>              getSOAPBody();
	//********************************************************************************
	// getter for role "SOAPBody" among "ExtensibleMessage"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SOAPBody, T>		getSOAPBody(TDP)
	{
		std::set<SOAPBody, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPBody");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPBody elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPFault>             getSOAPFault();
	//********************************************************************************
	// getter for role "SOAPFault" among "ExtensibleMessage"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SOAPFault, T>		getSOAPFault(TDP)
	{
		std::set<SOAPFault, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPFault");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPFault elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPFaultExtension>    getSOAPFaultExtension();
	//********************************************************************************
	// getter for role "SOAPFaultExtension" among "SOAPFaultExtension"s
	//********************************************************************************
	template <class T>
	std::set<SOAPFaultExtension, T>		getSOAPFaultExtension(TDP)
	{
		std::set<SOAPFaultExtension, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPFaultExtension");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPFaultExtension elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPHeader>            getSOAPHeader();
	//********************************************************************************
	// getter for role "SOAPHeader" among "ExtensibleMessage"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SOAPHeader, T>		getSOAPHeader(TDP)
	{
		std::set<SOAPHeader, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPHeader");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPHeader elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPHeaderFault>       getSOAPHeaderFault();
	//********************************************************************************
	// getter for role "SOAPHeaderFault" among "ExtensibleMessage"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SOAPHeaderFault, T>		getSOAPHeaderFault(TDP)
	{
		std::set<SOAPHeaderFault, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPHeaderFault");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPHeaderFault elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPOperation>         getSOAPOperation();
	//********************************************************************************
	// getter for role "SOAPOperation" among "ExtensibleOperation"s
	//********************************************************************************
	template <class T>
	std::set<SOAPOperation, T>		getSOAPOperation(TDP)
	{
		std::set<SOAPOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DefinitionsImpl
//*******************************************************************
class DefinitionsImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string gettargetNamespace() ;
	virtual void        settargetNamespace( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Attribute>             getAttribute();
	//********************************************************************************
	// getter for role "Attribute" among "Attribute"s
	//********************************************************************************
	template <class T>
	std::set<Attribute, T>		getAttribute(TDP)
	{
		std::set<Attribute, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Attribute");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Attribute elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Binding>               getBinding();
	//********************************************************************************
	// getter for role "Binding" among "Binding"s
	//********************************************************************************
	template <class T>
	std::set<Binding, T>		getBinding(TDP)
	{
		std::set<Binding, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Binding");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Binding elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<BindingPortType>       getBindingPortType();
	//********************************************************************************
	// getter for role "BindingPortType" among "BindingPortType"s
	//********************************************************************************
	template <class T>
	std::set<BindingPortType, T>		getBindingPortType(TDP)
	{
		std::set<BindingPortType, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindingPortType");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			BindingPortType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Import>                getImport();
	//********************************************************************************
	// getter for role "Import" among "Import"s
	//********************************************************************************
	template <class T>
	std::set<Import, T>		getImport(TDP)
	{
		std::set<Import, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Import");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Import elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Messages>              getMessages();
	//********************************************************************************
	// getter for role "Messages" among "Messages"s
	//********************************************************************************
	template <class T>
	std::set<Messages, T>		getMessages(TDP)
	{
		std::set<Messages, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Messages");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Messages elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<PortType>              getPortType();
	//********************************************************************************
	// getter for role "PortType" among "PortType"s
	//********************************************************************************
	template <class T>
	std::set<PortType, T>		getPortType(TDP)
	{
		std::set<PortType, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::PortType");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			PortType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Service>               getService();
	//********************************************************************************
	// getter for role "Service" among "Service"s
	//********************************************************************************
	template <class T>
	std::set<Service, T>		getService(TDP)
	{
		std::set<Service, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Service");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Service elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Types>                 getTypes();
	//********************************************************************************
	// getter for role "Types" among "Types"s
	//********************************************************************************
	template <class T>
	std::set<Types, T>		getTypes(TDP)
	{
		std::set<Types, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Types");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Types elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DocumentationImpl
//*******************************************************************
class DocumentationImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getDocText() ;
	virtual bool        isMixed() ;
	virtual void        setDocText( const std::string& val);
	virtual void        setMixed( bool val);
	//
	// kind and role getters
	virtual std::set<Element>               getElement();
	//********************************************************************************
	// getter for role "Element" among "Element"s
	//********************************************************************************
	template <class T>
	std::set<Element, T>		getElement(TDP)
	{
		std::set<Element, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Element");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Element elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FaultImpl
//*******************************************************************
class FaultImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<MessageRef>       getFaultMessageDsts();
	//********************************************************************************
	// returns dst MessageRefs
	//********************************************************************************
	template <class T>
	std::multiset<MessageRef, T>		getFaultMessageDsts(TDP)
	{
		std::multiset<MessageRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::FaultMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				MessageRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<FaultMessage>          getOutFaultMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<FaultMessage, T>		getOutFaultMessageLinks(TDP)
	{
		std::set<FaultMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			FaultMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ImportImpl
//*******************************************************************
class ImportImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getlocation() ;
	virtual std::string getnamespace() ;
	virtual void        setlocation( const std::string& val);
	virtual void        setnamespace( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Definitions>           getDefinitions();
	//********************************************************************************
	// getter for role "Definitions" among "Definitions"s
	//********************************************************************************
	template <class T>
	std::set<Definitions, T>		getDefinitions(TDP)
	{
		std::set<Definitions, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Definitions");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Definitions elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InputImpl
//*******************************************************************
class InputImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<MessageRef>       getInputMessageDsts();
	//********************************************************************************
	// returns dst MessageRefs
	//********************************************************************************
	template <class T>
	std::multiset<MessageRef, T>		getInputMessageDsts(TDP)
	{
		std::multiset<MessageRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::InputMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				MessageRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<InputMessage>          getOutInputMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<InputMessage, T>		getOutInputMessageLinks(TDP)
	{
		std::set<InputMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			InputMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MessageImpl
//*******************************************************************
class MessageImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Part>                  getPart();
	//********************************************************************************
	// getter for role "Part" among "Part"s
	//********************************************************************************
	template <class T>
	std::set<Part, T>		getPart(TDP)
	{
		std::set<Part, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Part");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Part elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MessagesImpl
//*******************************************************************
class MessagesImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Message>               getMessage();
	//********************************************************************************
	// getter for role "Message" among "Message"s
	//********************************************************************************
	template <class T>
	std::set<Message, T>		getMessage(TDP)
	{
		std::set<Message, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Message");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Message elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutputImpl
//*******************************************************************
class OutputImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<OutputMessage>         getOutOutputMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<OutputMessage, T>		getOutOutputMessageLinks(TDP)
	{
		std::set<OutputMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			OutputMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::multiset<MessageRef>       getOutputMessageDsts();
	//********************************************************************************
	// returns dst MessageRefs
	//********************************************************************************
	template <class T>
	std::multiset<MessageRef, T>		getOutputMessageDsts(TDP)
	{
		std::multiset<MessageRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::OutputMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				MessageRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortImpl
//*******************************************************************
class PortImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<PortBinding>           getOutPortBindingLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<PortBinding, T>		getOutPortBindingLinks(TDP)
	{
		std::set<PortBinding, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			PortBinding c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::multiset<BindingRef>       getPortBindingDsts();
	//********************************************************************************
	// returns dst BindingRefs
	//********************************************************************************
	template <class T>
	std::multiset<BindingRef, T>		getPortBindingDsts(TDP)
	{
		std::multiset<BindingRef, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::PortBinding");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				BindingRef dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SOAPAddress>           getSOAPAddress();
	//********************************************************************************
	// getter for role "SOAPAddress" among "ExtensiblePort"s
	//********************************************************************************
	template <class T>
	std::set<SOAPAddress, T>		getSOAPAddress(TDP)
	{
		std::set<SOAPAddress, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPAddress");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SOAPAddress elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortTypeImpl
//*******************************************************************
class PortTypeImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Binding>          getBindingPortTypeSrcs();
	//********************************************************************************
	// returns src Bindings
	//********************************************************************************
	template <class T>
	std::multiset<Binding, T>		getBindingPortTypeSrcs(TDP)
	{
		std::multiset<Binding, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::BindingPortType");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				Binding dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BindingPortType>       getInBindingPortTypeLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BindingPortType, T>		getInBindingPortTypeLinks(TDP)
	{
		std::set<BindingPortType, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BindingPortType c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<NotificationOperation> getNotificationOperation();
	//********************************************************************************
	// getter for role "NotificationOperation" among "Operation"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<NotificationOperation, T>		getNotificationOperation(TDP)
	{
		std::set<NotificationOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::NotificationOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			NotificationOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<OneWayOperation>       getOneWayOperation();
	//********************************************************************************
	// getter for role "OneWayOperation" among "Operation"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<OneWayOperation, T>		getOneWayOperation(TDP)
	{
		std::set<OneWayOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OneWayOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			OneWayOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Operation>             getOperation();
	//********************************************************************************
	// aggregated getter for role "" among "Operation"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<Operation, T>		getOperation(TDP)
	{
		std::set<Operation, T> res;
		const int len = 4;
		std::set<BON::FCO> roles_vec[ len];
		roles_vec[0] = ModelImpl::getChildFCOsAs("WSML::NotificationOperation");
		roles_vec[1] = ModelImpl::getChildFCOsAs("WSML::OneWayOperation");
		roles_vec[2] = ModelImpl::getChildFCOsAs("WSML::RequestResponseOperation");
		roles_vec[3] = ModelImpl::getChildFCOsAs("WSML::SolicitResponseOperation");
		for( int k = 0; k < len; ++k)
			for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
			{
				Operation elem(*i);
				ASSERT(elem);
				res.insert(elem);
			}
		return res;
	}

	virtual std::set<RequestResponseOperation>        getRequestResponseOperation();
	//********************************************************************************
	// getter for role "RequestResponseOperation" among "Operation"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<RequestResponseOperation, T>		getRequestResponseOperation(TDP)
	{
		std::set<RequestResponseOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::RequestResponseOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			RequestResponseOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SolicitResponseOperation>        getSolicitResponseOperation();
	//********************************************************************************
	// getter for role "SolicitResponseOperation" among "Operation"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SolicitResponseOperation, T>		getSolicitResponseOperation(TDP)
	{
		std::set<SolicitResponseOperation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SolicitResponseOperation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SolicitResponseOperation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SchemaImpl
//*******************************************************************
class SchemaImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string gettargetNamespace() ;
	virtual void        settargetNamespace( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Attribute>             getAttribute();
	//********************************************************************************
	// getter for role "Attribute" among "Attribute"s
	//********************************************************************************
	template <class T>
	std::set<Attribute, T>		getAttribute(TDP)
	{
		std::set<Attribute, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Attribute");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Attribute elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Element>               getElement();
	//********************************************************************************
	// getter for role "Element" among "Element"s
	//********************************************************************************
	template <class T>
	std::set<Element, T>		getElement(TDP)
	{
		std::set<Element, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Element");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Element elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SchemaImport>          getSchemaImport();
	//********************************************************************************
	// getter for role "SchemaImport" among "SchemaReference"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SchemaImport, T>		getSchemaImport(TDP)
	{
		std::set<SchemaImport, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SchemaImport");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SchemaImport elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SchemaReference>       getSchemaReference();
	//********************************************************************************
	// getter for role "SchemaReference" among "SchemaReference"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SchemaReference, T>		getSchemaReference(TDP)
	{
		std::set<SchemaReference, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SchemaReference");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			SchemaReference elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<SchemaReference>       getSchemaReference(int dummy);
	//********************************************************************************
	// aggregated getter for role "" among "SchemaReference"s and its descendants
	//********************************************************************************
	template <class T>
	std::set<SchemaReference, T>		getSchemaReference(int dummy, TDP)
	{
		std::set<SchemaReference, T> res;
		const int len = 2;
		std::set<BON::FCO> roles_vec[ len];
		roles_vec[0] = ModelImpl::getChildFCOsAs("WSML::SchemaImport");
		roles_vec[1] = ModelImpl::getChildFCOsAs("WSML::SchemaReference");
		for( int k = 0; k < len; ++k)
			for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
			{
				SchemaReference elem(*i);
				ASSERT(elem);
				res.insert(elem);
			}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SchemaBuiltinsImpl
//*******************************************************************
class SchemaBuiltinsImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<AtomicType>            getAtomicType();
	//********************************************************************************
	// getter for role "AtomicType" among "AtomicType"s
	//********************************************************************************
	template <class T>
	std::set<AtomicType, T>		getAtomicType(TDP)
	{
		std::set<AtomicType, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::AtomicType");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			AtomicType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ServiceImpl
//*******************************************************************
class ServiceImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<BindingRef>            getBindingRef();
	//********************************************************************************
	// getter for role "BindingRef" among "BindingRef"s
	//********************************************************************************
	template <class T>
	std::set<BindingRef, T>		getBindingRef(TDP)
	{
		std::set<BindingRef, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindingRef");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			BindingRef elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Port>                  getPort();
	//********************************************************************************
	// getter for role "Port" among "Port"s
	//********************************************************************************
	template <class T>
	std::set<Port, T>		getPort(TDP)
	{
		std::set<Port, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Port");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Port elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<PortBinding>           getPortBinding();
	//********************************************************************************
	// getter for role "PortBinding" among "PortBinding"s
	//********************************************************************************
	template <class T>
	std::set<PortBinding, T>		getPortBinding(TDP)
	{
		std::set<PortBinding, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::PortBinding");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			PortBinding elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TypesImpl
//*******************************************************************
class TypesImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Documentation>         getDocumentation();
	//********************************************************************************
	// getter for role "Documentation" among "Documentation"s
	//********************************************************************************
	template <class T>
	std::set<Documentation, T>		getDocumentation(TDP)
	{
		std::set<Documentation, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Documentation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}

	virtual std::set<Schema>                getSchema();
	//********************************************************************************
	// getter for role "Schema" among "Schema"s
	//********************************************************************************
	template <class T>
	std::set<Schema, T>		getSchema(TDP)
	{
		std::set<Schema, T> res;
		std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Schema");
		for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
		{
			Schema elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
		return res;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   IORefImpl
//*******************************************************************
class IORefImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<SOAPBody>         getBodyBindingDsts();
	//********************************************************************************
	// returns dst SOAPBodys
	//********************************************************************************
	template <class T>
	std::multiset<SOAPBody, T>		getBodyBindingDsts(TDP)
	{
		std::multiset<SOAPBody, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::BodyBinding");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				SOAPBody dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<SOAPHeaders>      getHeaderBindingDsts();
	//********************************************************************************
	// returns dst SOAPHeaderss
	//********************************************************************************
	template <class T>
	std::multiset<SOAPHeaders, T>		getHeaderBindingDsts(TDP)
	{
		std::multiset<SOAPHeaders, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::HeaderBinding");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				SOAPHeaders dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BodyBinding>           getOutBodyBindingLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BodyBinding, T>		getOutBodyBindingLinks(TDP)
	{
		std::set<BodyBinding, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BodyBinding c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<HeaderBinding>         getOutHeaderBindingLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<HeaderBinding, T>		getOutHeaderBindingLinks(TDP)
	{
		std::set<HeaderBinding, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			HeaderBinding c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InputRefImpl
//*******************************************************************
class InputRefImpl :
	  virtual public BON::ReferenceImpl
	, public IORefImpl
{
public:

	//
	// ref getters
	virtual Input       getInput();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutputRefImpl
//*******************************************************************
class OutputRefImpl :
	  virtual public BON::ReferenceImpl
	, public IORefImpl
{
public:

	//
	// ref getters
	virtual Output      getOutput();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AttributeImpl
//*******************************************************************
class AttributeImpl :
	  virtual public BON::AtomImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getValue() ;
	virtual void        setValue( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BindingPortTypeImpl
//*******************************************************************
class BindingPortTypeImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual PortType    getDst();
	virtual Binding     getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BindsOperationImpl
//*******************************************************************
class BindsOperationImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual OperationRef                    getDst();
	virtual BindingOperation                getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BodyBindingImpl
//*******************************************************************
class BodyBindingImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual SOAPBody    getDst();
	virtual IORef       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BodyPartsImpl
//*******************************************************************
class BodyPartsImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual PartRef     getDst();
	virtual SOAPBody    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FaultMessageImpl
//*******************************************************************
class FaultMessageImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual MessageRef  getDst();
	virtual Fault       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HeaderBindingImpl
//*******************************************************************
class HeaderBindingImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual SOAPHeaders getDst();
	virtual IORef       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HeaderMessageImpl
//*******************************************************************
class HeaderMessageImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual MessageRef  getDst();
	virtual SOAPHeaders getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HeaderPartImpl
//*******************************************************************
class HeaderPartImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual PartRef     getDst();
	virtual SOAPHeaders getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InputMessageImpl
//*******************************************************************
class InputMessageImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual MessageRef  getDst();
	virtual Input       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutputMessageImpl
//*******************************************************************
class OutputMessageImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual MessageRef  getDst();
	virtual Output      getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortBindingImpl
//*******************************************************************
class PortBindingImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BindingRef  getDst();
	virtual Port        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SOAPFaultExtensionImpl
//*******************************************************************
class SOAPFaultExtensionImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual SOAPFault   getDst();
	virtual FaultRef    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BindingRefImpl
//*******************************************************************
class BindingRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<PortBinding>           getInPortBindingLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<PortBinding, T>		getInPortBindingLinks(TDP)
	{
		std::set<PortBinding, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			PortBinding c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::multiset<Port>             getPortBindingSrcs();
	//********************************************************************************
	// returns src Ports
	//********************************************************************************
	template <class T>
	std::multiset<Port, T>		getPortBindingSrcs(TDP)
	{
		std::multiset<Port, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::PortBinding");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				Port dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	//
	// ref getters
	virtual Binding     getBinding();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FaultRefImpl
//*******************************************************************
class FaultRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<SOAPFaultExtension>    getOutSOAPFaultExtensionLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<SOAPFaultExtension, T>		getOutSOAPFaultExtensionLinks(TDP)
	{
		std::set<SOAPFaultExtension, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			SOAPFaultExtension c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::multiset<SOAPFault>        getSOAPFaultExtensionDsts();
	//********************************************************************************
	// returns dst SOAPFaults
	//********************************************************************************
	template <class T>
	std::multiset<SOAPFault, T>		getSOAPFaultExtensionDsts(TDP)
	{
		std::multiset<SOAPFault, T> res;
		{
			std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("WSML::SOAPFaultExtension");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
			{
				SOAPFault dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	//
	// ref getters
	virtual Fault       getFault();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MessageRefImpl
//*******************************************************************
class MessageRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Fault>            getFaultMessageSrcs();
	//********************************************************************************
	// returns src Faults
	//********************************************************************************
	template <class T>
	std::multiset<Fault, T>		getFaultMessageSrcs(TDP)
	{
		std::multiset<Fault, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::FaultMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				Fault dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<SOAPHeaders>      getHeaderMessageSrcs();
	//********************************************************************************
	// returns src SOAPHeaderss
	//********************************************************************************
	template <class T>
	std::multiset<SOAPHeaders, T>		getHeaderMessageSrcs(TDP)
	{
		std::multiset<SOAPHeaders, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::HeaderMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				SOAPHeaders dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<FaultMessage>          getInFaultMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<FaultMessage, T>		getInFaultMessageLinks(TDP)
	{
		std::set<FaultMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			FaultMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<HeaderMessage>         getInHeaderMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<HeaderMessage, T>		getInHeaderMessageLinks(TDP)
	{
		std::set<HeaderMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			HeaderMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<InputMessage>          getInInputMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<InputMessage, T>		getInInputMessageLinks(TDP)
	{
		std::set<InputMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			InputMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<OutputMessage>         getInOutputMessageLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<OutputMessage, T>		getInOutputMessageLinks(TDP)
	{
		std::set<OutputMessage, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			OutputMessage c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::multiset<Input>            getInputMessageSrcs();
	//********************************************************************************
	// returns src Inputs
	//********************************************************************************
	template <class T>
	std::multiset<Input, T>		getInputMessageSrcs(TDP)
	{
		std::multiset<Input, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::InputMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				Input dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<Output>           getOutputMessageSrcs();
	//********************************************************************************
	// returns src Outputs
	//********************************************************************************
	template <class T>
	std::multiset<Output, T>		getOutputMessageSrcs(TDP)
	{
		std::multiset<Output, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::OutputMessage");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				Output dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	//
	// ref getters
	virtual Message     getMessage();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OperationRefImpl
//*******************************************************************
class OperationRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BindingOperation> getBindsOperationSrcs();
	//********************************************************************************
	// returns src BindingOperations
	//********************************************************************************
	template <class T>
	std::multiset<BindingOperation, T>		getBindsOperationSrcs(TDP)
	{
		std::multiset<BindingOperation, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::BindsOperation");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				BindingOperation dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BindsOperation>        getInBindsOperationLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BindsOperation, T>		getInBindsOperationLinks(TDP)
	{
		std::set<BindsOperation, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BindsOperation c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// ref getters
	virtual Operation   getOperation();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PartImpl
//*******************************************************************
class PartImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Referrable  getReferrable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PartRefImpl
//*******************************************************************
class PartRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<SOAPBody>         getBodyPartsSrcs();
	//********************************************************************************
	// returns src SOAPBodys
	//********************************************************************************
	template <class T>
	std::multiset<SOAPBody, T>		getBodyPartsSrcs(TDP)
	{
		std::multiset<SOAPBody, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::BodyParts");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				SOAPBody dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::multiset<SOAPHeaders>      getHeaderPartSrcs();
	//********************************************************************************
	// returns src SOAPHeaderss
	//********************************************************************************
	template <class T>
	std::multiset<SOAPHeaders, T>		getHeaderPartSrcs(TDP)
	{
		std::multiset<SOAPHeaders, T> res;
		{
			std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("WSML::HeaderPart");
			for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
			{
				SOAPHeaders dst( *cit );
				ASSERT(dst);
				res.insert( dst);
			}
		}
		return res;
	}

	virtual std::set<BodyParts>             getInBodyPartsLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<BodyParts, T>		getInBodyPartsLinks(TDP)
	{
		std::set<BodyParts, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			BodyParts c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	virtual std::set<HeaderPart>            getInHeaderPartLinks();
	//********************************************************************************
	//
	//********************************************************************************
	template <class T>
	std::set<HeaderPart, T>		getInHeaderPartLinks(TDP)
	{
		std::set<HeaderPart, T> result;
		std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
		std::set<BON::Connection>::iterator it = conns.begin();
		for( ; it != conns.end(); ++it)
		{
			HeaderPart c( *it);
			if (c)
				result.insert( c);
		}
		return result;
	}

	//
	// ref getters
	virtual Part        getPart();

	///BUP
	// add your own members here
	///EUP
};



} // namespace WSML

///BUP
// add your additional class definitions here
///EUP

#endif // WSMLBONX_H
