#include "stdafx.h"
#include "WSMLBonX.h"


namespace BON
{

IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::Operation );
IMPLEMENT_BONEXTENSION( WSML::NotificationOperation, "WSML::NotificationOperation" );
IMPLEMENT_BONEXTENSION( WSML::OneWayOperation, "WSML::OneWayOperation" );
IMPLEMENT_BONEXTENSION( WSML::RequestResponseOperation, "WSML::RequestResponseOperation" );
IMPLEMENT_BONEXTENSION( WSML::SolicitResponseOperation, "WSML::SolicitResponseOperation" );
IMPLEMENT_BONEXTENSION( WSML::SchemaReference, "WSML::SchemaReference" );
IMPLEMENT_BONEXTENSION( WSML::SchemaImport, "WSML::SchemaImport" );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::ExtensibilityElement );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::ExtensibleBinding );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::ExtensibleMessage );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::ExtensibleOperation );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::ExtensiblePort );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::SOAPHeaders );
IMPLEMENT_BONEXTENSION( WSML::SOAPAddress, "WSML::SOAPAddress" );
IMPLEMENT_BONEXTENSION( WSML::SOAPBinding, "WSML::SOAPBinding" );
IMPLEMENT_BONEXTENSION( WSML::SOAPBody, "WSML::SOAPBody" );
IMPLEMENT_BONEXTENSION( WSML::SOAPFault, "WSML::SOAPFault" );
IMPLEMENT_BONEXTENSION( WSML::SOAPOperation, "WSML::SOAPOperation" );
IMPLEMENT_BONEXTENSION( WSML::SOAPHeader, "WSML::SOAPHeader" );
IMPLEMENT_BONEXTENSION( WSML::SOAPHeaderFault, "WSML::SOAPHeaderFault" );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::Referrable );
IMPLEMENT_BONEXTENSION( WSML::Element, "WSML::Element" );
IMPLEMENT_BONEXTENSION( WSML::AtomicType, "WSML::AtomicType" );
IMPLEMENT_BONEXTENSION( WSML::Binding, "WSML::Binding" );
IMPLEMENT_BONEXTENSION( WSML::BindingOperation, "WSML::BindingOperation" );
IMPLEMENT_BONEXTENSION( WSML::Definitions, "WSML::Definitions" );
IMPLEMENT_BONEXTENSION( WSML::Documentation, "WSML::Documentation" );
IMPLEMENT_BONEXTENSION( WSML::Fault, "WSML::Fault" );
IMPLEMENT_BONEXTENSION( WSML::Import, "WSML::Import" );
IMPLEMENT_BONEXTENSION( WSML::Input, "WSML::Input" );
IMPLEMENT_BONEXTENSION( WSML::Message, "WSML::Message" );
IMPLEMENT_BONEXTENSION( WSML::Messages, "WSML::Messages" );
IMPLEMENT_BONEXTENSION( WSML::Output, "WSML::Output" );
IMPLEMENT_BONEXTENSION( WSML::Port, "WSML::Port" );
IMPLEMENT_BONEXTENSION( WSML::PortType, "WSML::PortType" );
IMPLEMENT_BONEXTENSION( WSML::Schema, "WSML::Schema" );
IMPLEMENT_BONEXTENSION( WSML::SchemaBuiltins, "WSML::SchemaBuiltins" );
IMPLEMENT_BONEXTENSION( WSML::Service, "WSML::Service" );
IMPLEMENT_BONEXTENSION( WSML::Types, "WSML::Types" );
IMPLEMENT_ABSTRACT_BONEXTENSION( WSML::IORef );
IMPLEMENT_BONEXTENSION( WSML::InputRef, "WSML::InputRef" );
IMPLEMENT_BONEXTENSION( WSML::OutputRef, "WSML::OutputRef" );
IMPLEMENT_BONEXTENSION( WSML::Attribute, "WSML::Attribute" );
IMPLEMENT_BONEXTENSION( WSML::BindingPortType, "WSML::BindingPortType" );
IMPLEMENT_BONEXTENSION( WSML::BindsOperation, "WSML::BindsOperation" );
IMPLEMENT_BONEXTENSION( WSML::BodyBinding, "WSML::BodyBinding" );
IMPLEMENT_BONEXTENSION( WSML::BodyParts, "WSML::BodyParts" );
IMPLEMENT_BONEXTENSION( WSML::FaultMessage, "WSML::FaultMessage" );
IMPLEMENT_BONEXTENSION( WSML::HeaderBinding, "WSML::HeaderBinding" );
IMPLEMENT_BONEXTENSION( WSML::HeaderMessage, "WSML::HeaderMessage" );
IMPLEMENT_BONEXTENSION( WSML::HeaderPart, "WSML::HeaderPart" );
IMPLEMENT_BONEXTENSION( WSML::InputMessage, "WSML::InputMessage" );
IMPLEMENT_BONEXTENSION( WSML::OutputMessage, "WSML::OutputMessage" );
IMPLEMENT_BONEXTENSION( WSML::PortBinding, "WSML::PortBinding" );
IMPLEMENT_BONEXTENSION( WSML::SOAPFaultExtension, "WSML::SOAPFaultExtension" );
IMPLEMENT_BONEXTENSION( WSML::BindingRef, "WSML::BindingRef" );
IMPLEMENT_BONEXTENSION( WSML::FaultRef, "WSML::FaultRef" );
IMPLEMENT_BONEXTENSION( WSML::MessageRef, "WSML::MessageRef" );
IMPLEMENT_BONEXTENSION( WSML::OperationRef, "WSML::OperationRef" );
IMPLEMENT_BONEXTENSION( WSML::Part, "WSML::Part" );
IMPLEMENT_BONEXTENSION( WSML::PartRef, "WSML::PartRef" );


} // end namespace BON

namespace WSML
{
//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> OperationImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FaultMessage" among "FaultMessage"s
//********************************************************************************
std::set<FaultMessage> OperationImpl::getFaultMessage()
{
	std::set<FaultMessage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::FaultMessage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FaultMessage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InputMessage" among "InputMessage"s
//********************************************************************************
std::set<InputMessage> OperationImpl::getInputMessage()
{
	std::set<InputMessage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::InputMessage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InputMessage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MessageRef" among "MessageRef"s
//********************************************************************************
std::set<MessageRef> OperationImpl::getMessageRef()
{
	std::set<MessageRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::MessageRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MessageRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutputMessage" among "OutputMessage"s
//********************************************************************************
std::set<OutputMessage> OperationImpl::getOutputMessage()
{
	std::set<OutputMessage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OutputMessage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutputMessage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Output" among "Output"s
//********************************************************************************
std::set<Output> NotificationOperationImpl::getOutput()
{
	std::set<Output> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Output");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Output elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Input" among "Input"s
//********************************************************************************
std::set<Input> OneWayOperationImpl::getInput()
{
	std::set<Input> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Input");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Input elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string RequestResponseOperationImpl::getparameterOrder()
{
	return FCOImpl::getAttribute("parameterOrder")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void RequestResponseOperationImpl::setparameterOrder( const std::string& val)
{
	FCOImpl::getAttribute("parameterOrder")->setStringValue( val);
}


//********************************************************************************
// getter for role "Fault" among "Fault"s
//********************************************************************************
std::set<Fault> RequestResponseOperationImpl::getFault()
{
	std::set<Fault> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Fault");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Fault elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Input" among "Input"s
//********************************************************************************
std::set<Input> RequestResponseOperationImpl::getInput()
{
	std::set<Input> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Input");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Input elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Output" among "Output"s
//********************************************************************************
std::set<Output> RequestResponseOperationImpl::getOutput()
{
	std::set<Output> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Output");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Output elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string SolicitResponseOperationImpl::getparameterOrder()
{
	return FCOImpl::getAttribute("parameterOrder")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void SolicitResponseOperationImpl::setparameterOrder( const std::string& val)
{
	FCOImpl::getAttribute("parameterOrder")->setStringValue( val);
}


//********************************************************************************
// getter for role "Fault" among "Fault"s
//********************************************************************************
std::set<Fault> SolicitResponseOperationImpl::getFault()
{
	std::set<Fault> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Fault");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Fault elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Input" among "Input"s
//********************************************************************************
std::set<Input> SolicitResponseOperationImpl::getInput()
{
	std::set<Input> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Input");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Input elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Output" among "Output"s
//********************************************************************************
std::set<Output> SolicitResponseOperationImpl::getOutput()
{
	std::set<Output> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Output");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Output elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string SchemaReferenceImpl::getid()
{
	return FCOImpl::getAttribute("id")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string SchemaReferenceImpl::getschemaLocation()
{
	return FCOImpl::getAttribute("schemaLocation")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void SchemaReferenceImpl::setid( const std::string& val)
{
	FCOImpl::getAttribute("id")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void SchemaReferenceImpl::setschemaLocation( const std::string& val)
{
	FCOImpl::getAttribute("schemaLocation")->setStringValue( val);
}


//********************************************************************************
// getter for role "Schema" among "Schema"s
//********************************************************************************
std::set<Schema> SchemaReferenceImpl::getSchema()
{
	std::set<Schema> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Schema");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Schema elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string SchemaImportImpl::getnamespace()
{
	return FCOImpl::getAttribute("namespace")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void SchemaImportImpl::setnamespace( const std::string& val)
{
	FCOImpl::getAttribute("namespace")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
bool ExtensibilityElementImpl::isrequired()
{
	return FCOImpl::getAttribute("required")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void ExtensibilityElementImpl::setrequired( bool val)
{
	FCOImpl::getAttribute("required")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
std::string ExtensibleMessageImpl::getencodingStyle()
{
	return FCOImpl::getAttribute("encodingStyle")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ExtensibleMessageImpl::getnamespace()
{
	return FCOImpl::getAttribute("namespace")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
ExtensibleMessageImpl::use_Type ExtensibleMessageImpl::getuse()
{
	std::string val = FCOImpl::getAttribute("use")->getStringValue();

	if ( val == "literal") return literal_use_Type;
	else if ( val == "encoded") return encoded_use_Type;
	else throw("WSML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
void ExtensibleMessageImpl::setencodingStyle( const std::string& val)
{
	FCOImpl::getAttribute("encodingStyle")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ExtensibleMessageImpl::setnamespace( const std::string& val)
{
	FCOImpl::getAttribute("namespace")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ExtensibleMessageImpl::setuse( ExtensibleMessageImpl::use_Type val)
{
	std::string str_val = "";

	if ( val == literal_use_Type) str_val = "literal";
	else if ( val == encoded_use_Type) str_val = "encoded";
	else throw("WSML::None of the possible items");

	FCOImpl::getAttribute("use")->setStringValue( str_val);
}


//********************************************************************************
// returns src IORefs
//********************************************************************************
std::multiset<IORef> SOAPHeadersImpl::getHeaderBindingSrcs()
{
	std::multiset<IORef> res;
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


//********************************************************************************
// returns dst MessageRefs
//********************************************************************************
std::multiset<MessageRef> SOAPHeadersImpl::getHeaderMessageDsts()
{
	std::multiset<MessageRef> res;
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


//********************************************************************************
// returns dst PartRefs
//********************************************************************************
std::multiset<PartRef> SOAPHeadersImpl::getHeaderPartDsts()
{
	std::multiset<PartRef> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<HeaderBinding> SOAPHeadersImpl::getInHeaderBindingLinks()
{
	std::set<HeaderBinding> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<HeaderMessage> SOAPHeadersImpl::getOutHeaderMessageLinks()
{
	std::set<HeaderMessage> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<HeaderPart> SOAPHeadersImpl::getOutHeaderPartLinks()
{
	std::set<HeaderPart> result;
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


//********************************************************************************
//
//********************************************************************************
std::string SOAPAddressImpl::getlocation()
{
	return FCOImpl::getAttribute("location")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void SOAPAddressImpl::setlocation( const std::string& val)
{
	FCOImpl::getAttribute("location")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
SOAPBindingImpl::style_Type SOAPBindingImpl::getstyle()
{
	std::string val = FCOImpl::getAttribute("style")->getStringValue();

	if ( val == "document") return document_style_Type;
	else if ( val == "rpc") return rpc_style_Type;
	else throw("WSML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
std::string SOAPBindingImpl::gettransport()
{
	return FCOImpl::getAttribute("transport")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void SOAPBindingImpl::setstyle( SOAPBindingImpl::style_Type val)
{
	std::string str_val = "";

	if ( val == document_style_Type) str_val = "document";
	else if ( val == rpc_style_Type) str_val = "rpc";
	else throw("WSML::None of the possible items");

	FCOImpl::getAttribute("style")->setStringValue( str_val);
}


//********************************************************************************
//
//********************************************************************************
void SOAPBindingImpl::settransport( const std::string& val)
{
	FCOImpl::getAttribute("transport")->setStringValue( val);
}


//********************************************************************************
// returns src IORefs
//********************************************************************************
std::multiset<IORef> SOAPBodyImpl::getBodyBindingSrcs()
{
	std::multiset<IORef> res;
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


//********************************************************************************
// returns dst PartRefs
//********************************************************************************
std::multiset<PartRef> SOAPBodyImpl::getBodyPartsDsts()
{
	std::multiset<PartRef> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BodyBinding> SOAPBodyImpl::getInBodyBindingLinks()
{
	std::set<BodyBinding> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<BodyParts> SOAPBodyImpl::getOutBodyPartsLinks()
{
	std::set<BodyParts> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<SOAPFaultExtension> SOAPFaultImpl::getInSOAPFaultExtensionLinks()
{
	std::set<SOAPFaultExtension> result;
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


//********************************************************************************
// returns src FaultRefs
//********************************************************************************
std::multiset<FaultRef> SOAPFaultImpl::getSOAPFaultExtensionSrcs()
{
	std::multiset<FaultRef> res;
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


//********************************************************************************
//
//********************************************************************************
std::string SOAPOperationImpl::getsoapAction()
{
	return FCOImpl::getAttribute("soapAction")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
SOAPOperationImpl::style_Type SOAPOperationImpl::getstyle()
{
	std::string val = FCOImpl::getAttribute("style")->getStringValue();

	if ( val == "document") return document_style_Type;
	else if ( val == "rpc") return rpc_style_Type;
	else throw("WSML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
void SOAPOperationImpl::setsoapAction( const std::string& val)
{
	FCOImpl::getAttribute("soapAction")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void SOAPOperationImpl::setstyle( SOAPOperationImpl::style_Type val)
{
	std::string str_val = "";

	if ( val == document_style_Type) str_val = "document";
	else if ( val == rpc_style_Type) str_val = "rpc";
	else throw("WSML::None of the possible items");

	FCOImpl::getAttribute("style")->setStringValue( str_val);
}


//********************************************************************************
// getter for role "SOAPHeaderFault" among "SOAPHeaderFault"s
//********************************************************************************
std::set<SOAPHeaderFault> SOAPHeaderImpl::getSOAPHeaderFault()
{
	std::set<SOAPHeaderFault> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPHeaderFault");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPHeaderFault elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
long ElementImpl::getOrder()
{
	return FCOImpl::getAttribute("Order")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
ElementImpl::schemaType_Type ElementImpl::getschemaType()
{
	std::string val = FCOImpl::getAttribute("schemaType")->getStringValue();

	if ( val == "local") return local_schemaType_Type;
	else if ( val == "element") return element_schemaType_Type;
	else if ( val == "attribute") return attribute_schemaType_Type;
	else if ( val == "complexType") return complexType_schemaType_Type;
	else if ( val == "simpleType") return simpleType_schemaType_Type;
	else if ( val == "include") return include_schemaType_Type;
	else if ( val == "import") return import_schemaType_Type;
	else if ( val == "redefine") return redefine_schemaType_Type;
	else if ( val == "any") return any_schemaType_Type;
	else if ( val == "anyAttribute") return anyAttribute_schemaType_Type;
	else if ( val == "group") return group_schemaType_Type;
	else if ( val == "attributeGroup") return attributeGroup_schemaType_Type;
	else if ( val == "key") return key_schemaType_Type;
	else if ( val == "keyref") return keyref_schemaType_Type;
	else if ( val == "unique") return unique_schemaType_Type;
	else if ( val == "annotation") return annotation_schemaType_Type;
	else if ( val == "documentation") return documentation_schemaType_Type;
	else if ( val == "appinfo") return appinfo_schemaType_Type;
	else if ( val == "notation") return notation_schemaType_Type;
	else throw("WSML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
void ElementImpl::setOrder( const long val)
{
	FCOImpl::getAttribute("Order")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ElementImpl::setschemaType( ElementImpl::schemaType_Type val)
{
	std::string str_val = "";

	if ( val == local_schemaType_Type) str_val = "local";
	else if ( val == element_schemaType_Type) str_val = "element";
	else if ( val == attribute_schemaType_Type) str_val = "attribute";
	else if ( val == complexType_schemaType_Type) str_val = "complexType";
	else if ( val == simpleType_schemaType_Type) str_val = "simpleType";
	else if ( val == include_schemaType_Type) str_val = "include";
	else if ( val == import_schemaType_Type) str_val = "import";
	else if ( val == redefine_schemaType_Type) str_val = "redefine";
	else if ( val == any_schemaType_Type) str_val = "any";
	else if ( val == anyAttribute_schemaType_Type) str_val = "anyAttribute";
	else if ( val == group_schemaType_Type) str_val = "group";
	else if ( val == attributeGroup_schemaType_Type) str_val = "attributeGroup";
	else if ( val == key_schemaType_Type) str_val = "key";
	else if ( val == keyref_schemaType_Type) str_val = "keyref";
	else if ( val == unique_schemaType_Type) str_val = "unique";
	else if ( val == annotation_schemaType_Type) str_val = "annotation";
	else if ( val == documentation_schemaType_Type) str_val = "documentation";
	else if ( val == appinfo_schemaType_Type) str_val = "appinfo";
	else if ( val == notation_schemaType_Type) str_val = "notation";
	else throw("WSML::None of the possible items");

	FCOImpl::getAttribute("schemaType")->setStringValue( str_val);
}


//********************************************************************************
// getter for role "Attribute" among "Attribute"s
//********************************************************************************
std::set<Attribute> ElementImpl::getAttribute()
{
	std::set<Attribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Attribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Attribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Element" among "Element"s
//********************************************************************************
std::set<Element> ElementImpl::getElement()
{
	std::set<Element> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Element");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Element elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
AtomicTypeImpl::typeCode_Type AtomicTypeImpl::gettypeCode()
{
	std::string val = FCOImpl::getAttribute("typeCode")->getStringValue();

	if ( val == "string") return string_typeCode_Type;
	else if ( val == "boolean") return boolean_typeCode_Type;
	else if ( val == "float") return float_typeCode_Type;
	else if ( val == "double") return double_typeCode_Type;
	else if ( val == "decimal") return decimal_typeCode_Type;
	else if ( val == "duration") return duration_typeCode_Type;
	else if ( val == "dateTime") return dateTime_typeCode_Type;
	else if ( val == "time") return time_typeCode_Type;
	else if ( val == "date") return date_typeCode_Type;
	else if ( val == "gYearMonth") return gYearMonth_typeCode_Type;
	else if ( val == "gYear") return gYear_typeCode_Type;
	else if ( val == "gMonthDay") return gMonthDay_typeCode_Type;
	else if ( val == "gDay") return gDay_typeCode_Type;
	else if ( val == "gMonth") return gMonth_typeCode_Type;
	else if ( val == "hexBinary") return hexBinary_typeCode_Type;
	else if ( val == "base64Binary") return base64Binary_typeCode_Type;
	else if ( val == "anyURI") return anyURI_typeCode_Type;
	else if ( val == "QName") return QName_typeCode_Type;
	else if ( val == "NOTATION") return NOTATION_typeCode_Type;
	else if ( val == "normalizedString") return normalizedString_typeCode_Type;
	else if ( val == "token") return token_typeCode_Type;
	else if ( val == "language") return language_typeCode_Type;
	else if ( val == "IDREFS") return IDREFS_typeCode_Type;
	else if ( val == "ENTITIES") return ENTITIES_typeCode_Type;
	else if ( val == "NMTOKEN") return NMTOKEN_typeCode_Type;
	else if ( val == "NMTOKENS") return NMTOKENS_typeCode_Type;
	else if ( val == "name") return name_typeCode_Type;
	else if ( val == "NCName") return NCName_typeCode_Type;
	else if ( val == "ID") return ID_typeCode_Type;
	else if ( val == "IDREF") return IDREF_typeCode_Type;
	else if ( val == "ENTITY") return ENTITY_typeCode_Type;
	else if ( val == "integer") return integer_typeCode_Type;
	else if ( val == "nonPositiveInteger") return nonPositiveInteger_typeCode_Type;
	else if ( val == "negativeInteger") return negativeInteger_typeCode_Type;
	else if ( val == "long") return long_typeCode_Type;
	else if ( val == "int") return int_typeCode_Type;
	else if ( val == "short") return short_typeCode_Type;
	else if ( val == "byte") return byte_typeCode_Type;
	else if ( val == "nonNegativeInteger") return nonNegativeInteger_typeCode_Type;
	else if ( val == "unsignedLong") return unsignedLong_typeCode_Type;
	else if ( val == "unsignedInt") return unsignedInt_typeCode_Type;
	else if ( val == "unsignedShort") return unsignedShort_typeCode_Type;
	else if ( val == "unsignedByte") return unsignedByte_typeCode_Type;
	else if ( val == "positiveInteger") return positiveInteger_typeCode_Type;
	else throw("WSML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
void AtomicTypeImpl::settypeCode( AtomicTypeImpl::typeCode_Type val)
{
	std::string str_val = "";

	if ( val == string_typeCode_Type) str_val = "string";
	else if ( val == boolean_typeCode_Type) str_val = "boolean";
	else if ( val == float_typeCode_Type) str_val = "float";
	else if ( val == double_typeCode_Type) str_val = "double";
	else if ( val == decimal_typeCode_Type) str_val = "decimal";
	else if ( val == duration_typeCode_Type) str_val = "duration";
	else if ( val == dateTime_typeCode_Type) str_val = "dateTime";
	else if ( val == time_typeCode_Type) str_val = "time";
	else if ( val == date_typeCode_Type) str_val = "date";
	else if ( val == gYearMonth_typeCode_Type) str_val = "gYearMonth";
	else if ( val == gYear_typeCode_Type) str_val = "gYear";
	else if ( val == gMonthDay_typeCode_Type) str_val = "gMonthDay";
	else if ( val == gDay_typeCode_Type) str_val = "gDay";
	else if ( val == gMonth_typeCode_Type) str_val = "gMonth";
	else if ( val == hexBinary_typeCode_Type) str_val = "hexBinary";
	else if ( val == base64Binary_typeCode_Type) str_val = "base64Binary";
	else if ( val == anyURI_typeCode_Type) str_val = "anyURI";
	else if ( val == QName_typeCode_Type) str_val = "QName";
	else if ( val == NOTATION_typeCode_Type) str_val = "NOTATION";
	else if ( val == normalizedString_typeCode_Type) str_val = "normalizedString";
	else if ( val == token_typeCode_Type) str_val = "token";
	else if ( val == language_typeCode_Type) str_val = "language";
	else if ( val == IDREFS_typeCode_Type) str_val = "IDREFS";
	else if ( val == ENTITIES_typeCode_Type) str_val = "ENTITIES";
	else if ( val == NMTOKEN_typeCode_Type) str_val = "NMTOKEN";
	else if ( val == NMTOKENS_typeCode_Type) str_val = "NMTOKENS";
	else if ( val == name_typeCode_Type) str_val = "name";
	else if ( val == NCName_typeCode_Type) str_val = "NCName";
	else if ( val == ID_typeCode_Type) str_val = "ID";
	else if ( val == IDREF_typeCode_Type) str_val = "IDREF";
	else if ( val == ENTITY_typeCode_Type) str_val = "ENTITY";
	else if ( val == integer_typeCode_Type) str_val = "integer";
	else if ( val == nonPositiveInteger_typeCode_Type) str_val = "nonPositiveInteger";
	else if ( val == negativeInteger_typeCode_Type) str_val = "negativeInteger";
	else if ( val == long_typeCode_Type) str_val = "long";
	else if ( val == int_typeCode_Type) str_val = "int";
	else if ( val == short_typeCode_Type) str_val = "short";
	else if ( val == byte_typeCode_Type) str_val = "byte";
	else if ( val == nonNegativeInteger_typeCode_Type) str_val = "nonNegativeInteger";
	else if ( val == unsignedLong_typeCode_Type) str_val = "unsignedLong";
	else if ( val == unsignedInt_typeCode_Type) str_val = "unsignedInt";
	else if ( val == unsignedShort_typeCode_Type) str_val = "unsignedShort";
	else if ( val == unsignedByte_typeCode_Type) str_val = "unsignedByte";
	else if ( val == positiveInteger_typeCode_Type) str_val = "positiveInteger";
	else throw("WSML::None of the possible items");

	FCOImpl::getAttribute("typeCode")->setStringValue( str_val);
}


//********************************************************************************
// returns dst PortTypes
//********************************************************************************
std::multiset<PortType> BindingImpl::getBindingPortTypeDsts()
{
	std::multiset<PortType> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BindingPortType> BindingImpl::getOutBindingPortTypeLinks()
{
	std::set<BindingPortType> result;
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


//********************************************************************************
// getter for role "BindingOperation" among "BindingOperation"s
//********************************************************************************
std::set<BindingOperation> BindingImpl::getBindingOperation()
{
	std::set<BindingOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindingOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BindingOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "BindsOperation" among "BindsOperation"s
//********************************************************************************
std::set<BindsOperation> BindingImpl::getBindsOperation()
{
	std::set<BindsOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindsOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BindsOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> BindingImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OperationRef" among "OperationRef"s
//********************************************************************************
std::set<OperationRef> BindingImpl::getOperationRef()
{
	std::set<OperationRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OperationRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OperationRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPBinding" among "ExtensibleBinding"s
//********************************************************************************
std::set<SOAPBinding> BindingImpl::getSOAPBinding()
{
	std::set<SOAPBinding> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPBinding");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPBinding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst OperationRefs
//********************************************************************************
std::multiset<OperationRef> BindingOperationImpl::getBindsOperationDsts()
{
	std::multiset<OperationRef> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BindsOperation> BindingOperationImpl::getOutBindsOperationLinks()
{
	std::set<BindsOperation> result;
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


//********************************************************************************
// getter for role "BodyBinding" among "BodyBinding"s
//********************************************************************************
std::set<BodyBinding> BindingOperationImpl::getBodyBinding()
{
	std::set<BodyBinding> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BodyBinding");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BodyBinding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "BodyParts" among "BodyParts"s
//********************************************************************************
std::set<BodyParts> BindingOperationImpl::getBodyParts()
{
	std::set<BodyParts> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BodyParts");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BodyParts elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> BindingOperationImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "ExtensibleMessage"s and its descendants
//********************************************************************************
std::set<ExtensibleMessage> BindingOperationImpl::getExtensibleMessage()
{
	std::set<ExtensibleMessage> res;
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


//********************************************************************************
// getter for role "FaultRef" among "FaultRef"s
//********************************************************************************
std::set<FaultRef> BindingOperationImpl::getFaultRef()
{
	std::set<FaultRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::FaultRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FaultRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "HeaderBinding" among "HeaderBinding"s
//********************************************************************************
std::set<HeaderBinding> BindingOperationImpl::getHeaderBinding()
{
	std::set<HeaderBinding> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::HeaderBinding");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		HeaderBinding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "HeaderMessage" among "HeaderMessage"s
//********************************************************************************
std::set<HeaderMessage> BindingOperationImpl::getHeaderMessage()
{
	std::set<HeaderMessage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::HeaderMessage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		HeaderMessage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "HeaderPart" among "HeaderPart"s
//********************************************************************************
std::set<HeaderPart> BindingOperationImpl::getHeaderPart()
{
	std::set<HeaderPart> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::HeaderPart");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		HeaderPart elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "IORef"s and its descendants
//********************************************************************************
std::set<IORef> BindingOperationImpl::getIORef()
{
	std::set<IORef> res;
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


//********************************************************************************
// getter for role "InputRef" among "IORef"s and its descendants
//********************************************************************************
std::set<InputRef> BindingOperationImpl::getInputRef()
{
	std::set<InputRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::InputRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InputRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MessageRef" among "MessageRef"s
//********************************************************************************
std::set<MessageRef> BindingOperationImpl::getMessageRef()
{
	std::set<MessageRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::MessageRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MessageRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutputRef" among "IORef"s and its descendants
//********************************************************************************
std::set<OutputRef> BindingOperationImpl::getOutputRef()
{
	std::set<OutputRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OutputRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutputRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PartRef" among "PartRef"s
//********************************************************************************
std::set<PartRef> BindingOperationImpl::getPartRef()
{
	std::set<PartRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::PartRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PartRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPBody" among "ExtensibleMessage"s and its descendants
//********************************************************************************
std::set<SOAPBody> BindingOperationImpl::getSOAPBody()
{
	std::set<SOAPBody> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPBody");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPBody elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPFault" among "ExtensibleMessage"s and its descendants
//********************************************************************************
std::set<SOAPFault> BindingOperationImpl::getSOAPFault()
{
	std::set<SOAPFault> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPFault");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPFault elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPFaultExtension" among "SOAPFaultExtension"s
//********************************************************************************
std::set<SOAPFaultExtension> BindingOperationImpl::getSOAPFaultExtension()
{
	std::set<SOAPFaultExtension> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPFaultExtension");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPFaultExtension elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPHeader" among "ExtensibleMessage"s and its descendants
//********************************************************************************
std::set<SOAPHeader> BindingOperationImpl::getSOAPHeader()
{
	std::set<SOAPHeader> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPHeader");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPHeader elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPHeaderFault" among "ExtensibleMessage"s and its descendants
//********************************************************************************
std::set<SOAPHeaderFault> BindingOperationImpl::getSOAPHeaderFault()
{
	std::set<SOAPHeaderFault> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPHeaderFault");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPHeaderFault elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPOperation" among "ExtensibleOperation"s
//********************************************************************************
std::set<SOAPOperation> BindingOperationImpl::getSOAPOperation()
{
	std::set<SOAPOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string DefinitionsImpl::gettargetNamespace()
{
	return FCOImpl::getAttribute("targetNamespace")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void DefinitionsImpl::settargetNamespace( const std::string& val)
{
	FCOImpl::getAttribute("targetNamespace")->setStringValue( val);
}


//********************************************************************************
// getter for role "Attribute" among "Attribute"s
//********************************************************************************
std::set<Attribute> DefinitionsImpl::getAttribute()
{
	std::set<Attribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Attribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Attribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Binding" among "Binding"s
//********************************************************************************
std::set<Binding> DefinitionsImpl::getBinding()
{
	std::set<Binding> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Binding");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Binding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "BindingPortType" among "BindingPortType"s
//********************************************************************************
std::set<BindingPortType> DefinitionsImpl::getBindingPortType()
{
	std::set<BindingPortType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindingPortType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BindingPortType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> DefinitionsImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Import" among "Import"s
//********************************************************************************
std::set<Import> DefinitionsImpl::getImport()
{
	std::set<Import> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Import");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Import elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Messages" among "Messages"s
//********************************************************************************
std::set<Messages> DefinitionsImpl::getMessages()
{
	std::set<Messages> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Messages");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Messages elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PortType" among "PortType"s
//********************************************************************************
std::set<PortType> DefinitionsImpl::getPortType()
{
	std::set<PortType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::PortType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PortType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Service" among "Service"s
//********************************************************************************
std::set<Service> DefinitionsImpl::getService()
{
	std::set<Service> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Service");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Service elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Types" among "Types"s
//********************************************************************************
std::set<Types> DefinitionsImpl::getTypes()
{
	std::set<Types> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Types");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Types elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string DocumentationImpl::getDocText()
{
	return FCOImpl::getAttribute("DocText")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
bool DocumentationImpl::isMixed()
{
	return FCOImpl::getAttribute("Mixed")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void DocumentationImpl::setDocText( const std::string& val)
{
	FCOImpl::getAttribute("DocText")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void DocumentationImpl::setMixed( bool val)
{
	FCOImpl::getAttribute("Mixed")->setBooleanValue( val);
}


//********************************************************************************
// getter for role "Element" among "Element"s
//********************************************************************************
std::set<Element> DocumentationImpl::getElement()
{
	std::set<Element> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Element");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Element elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst MessageRefs
//********************************************************************************
std::multiset<MessageRef> FaultImpl::getFaultMessageDsts()
{
	std::multiset<MessageRef> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<FaultMessage> FaultImpl::getOutFaultMessageLinks()
{
	std::set<FaultMessage> result;
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


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> FaultImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string ImportImpl::getlocation()
{
	return FCOImpl::getAttribute("location")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ImportImpl::getnamespace()
{
	return FCOImpl::getAttribute("namespace")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ImportImpl::setlocation( const std::string& val)
{
	FCOImpl::getAttribute("location")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ImportImpl::setnamespace( const std::string& val)
{
	FCOImpl::getAttribute("namespace")->setStringValue( val);
}


//********************************************************************************
// getter for role "Definitions" among "Definitions"s
//********************************************************************************
std::set<Definitions> ImportImpl::getDefinitions()
{
	std::set<Definitions> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Definitions");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Definitions elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> ImportImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst MessageRefs
//********************************************************************************
std::multiset<MessageRef> InputImpl::getInputMessageDsts()
{
	std::multiset<MessageRef> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<InputMessage> InputImpl::getOutInputMessageLinks()
{
	std::set<InputMessage> result;
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


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> InputImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Part" among "Part"s
//********************************************************************************
std::set<Part> MessageImpl::getPart()
{
	std::set<Part> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Part");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Part elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Message" among "Message"s
//********************************************************************************
std::set<Message> MessagesImpl::getMessage()
{
	std::set<Message> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Message");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Message elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<OutputMessage> OutputImpl::getOutOutputMessageLinks()
{
	std::set<OutputMessage> result;
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


//********************************************************************************
// returns dst MessageRefs
//********************************************************************************
std::multiset<MessageRef> OutputImpl::getOutputMessageDsts()
{
	std::multiset<MessageRef> res;
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


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> OutputImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<PortBinding> PortImpl::getOutPortBindingLinks()
{
	std::set<PortBinding> result;
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


//********************************************************************************
// returns dst BindingRefs
//********************************************************************************
std::multiset<BindingRef> PortImpl::getPortBindingDsts()
{
	std::multiset<BindingRef> res;
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


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> PortImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SOAPAddress" among "ExtensiblePort"s
//********************************************************************************
std::set<SOAPAddress> PortImpl::getSOAPAddress()
{
	std::set<SOAPAddress> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SOAPAddress");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SOAPAddress elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns src Bindings
//********************************************************************************
std::multiset<Binding> PortTypeImpl::getBindingPortTypeSrcs()
{
	std::multiset<Binding> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BindingPortType> PortTypeImpl::getInBindingPortTypeLinks()
{
	std::set<BindingPortType> result;
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


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> PortTypeImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "NotificationOperation" among "Operation"s and its descendants
//********************************************************************************
std::set<NotificationOperation> PortTypeImpl::getNotificationOperation()
{
	std::set<NotificationOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::NotificationOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		NotificationOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OneWayOperation" among "Operation"s and its descendants
//********************************************************************************
std::set<OneWayOperation> PortTypeImpl::getOneWayOperation()
{
	std::set<OneWayOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::OneWayOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OneWayOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Operation"s and its descendants
//********************************************************************************
std::set<Operation> PortTypeImpl::getOperation()
{
	std::set<Operation> res;
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


//********************************************************************************
// getter for role "RequestResponseOperation" among "Operation"s and its descendants
//********************************************************************************
std::set<RequestResponseOperation> PortTypeImpl::getRequestResponseOperation()
{
	std::set<RequestResponseOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::RequestResponseOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		RequestResponseOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SolicitResponseOperation" among "Operation"s and its descendants
//********************************************************************************
std::set<SolicitResponseOperation> PortTypeImpl::getSolicitResponseOperation()
{
	std::set<SolicitResponseOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SolicitResponseOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SolicitResponseOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string SchemaImpl::gettargetNamespace()
{
	return FCOImpl::getAttribute("targetNamespace")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void SchemaImpl::settargetNamespace( const std::string& val)
{
	FCOImpl::getAttribute("targetNamespace")->setStringValue( val);
}


//********************************************************************************
// getter for role "Attribute" among "Attribute"s
//********************************************************************************
std::set<Attribute> SchemaImpl::getAttribute()
{
	std::set<Attribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Attribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Attribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Element" among "Element"s
//********************************************************************************
std::set<Element> SchemaImpl::getElement()
{
	std::set<Element> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Element");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Element elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SchemaImport" among "SchemaReference"s and its descendants
//********************************************************************************
std::set<SchemaImport> SchemaImpl::getSchemaImport()
{
	std::set<SchemaImport> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SchemaImport");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SchemaImport elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SchemaReference" among "SchemaReference"s and its descendants
//********************************************************************************
std::set<SchemaReference> SchemaImpl::getSchemaReference()
{
	std::set<SchemaReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::SchemaReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SchemaReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "SchemaReference"s and its descendants
//********************************************************************************
std::set<SchemaReference> SchemaImpl::getSchemaReference(int dummy)
{
	std::set<SchemaReference> res;
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


//********************************************************************************
// getter for role "AtomicType" among "AtomicType"s
//********************************************************************************
std::set<AtomicType> SchemaBuiltinsImpl::getAtomicType()
{
	std::set<AtomicType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::AtomicType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AtomicType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "BindingRef" among "BindingRef"s
//********************************************************************************
std::set<BindingRef> ServiceImpl::getBindingRef()
{
	std::set<BindingRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::BindingRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BindingRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Port" among "Port"s
//********************************************************************************
std::set<Port> ServiceImpl::getPort()
{
	std::set<Port> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Port");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Port elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PortBinding" among "PortBinding"s
//********************************************************************************
std::set<PortBinding> ServiceImpl::getPortBinding()
{
	std::set<PortBinding> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::PortBinding");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PortBinding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Documentation" among "Documentation"s
//********************************************************************************
std::set<Documentation> TypesImpl::getDocumentation()
{
	std::set<Documentation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Documentation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Documentation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Schema" among "Schema"s
//********************************************************************************
std::set<Schema> TypesImpl::getSchema()
{
	std::set<Schema> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("WSML::Schema");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Schema elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst SOAPBodys
//********************************************************************************
std::multiset<SOAPBody> IORefImpl::getBodyBindingDsts()
{
	std::multiset<SOAPBody> res;
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


//********************************************************************************
// returns dst SOAPHeaderss
//********************************************************************************
std::multiset<SOAPHeaders> IORefImpl::getHeaderBindingDsts()
{
	std::multiset<SOAPHeaders> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BodyBinding> IORefImpl::getOutBodyBindingLinks()
{
	std::set<BodyBinding> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<HeaderBinding> IORefImpl::getOutHeaderBindingLinks()
{
	std::set<HeaderBinding> result;
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


//********************************************************************************
//
//********************************************************************************
Input InputRefImpl::getInput()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Input(r);
}


//********************************************************************************
//
//********************************************************************************
Output OutputRefImpl::getOutput()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Output(r);
}


//********************************************************************************
//
//********************************************************************************
std::string AttributeImpl::getValue()
{
	return FCOImpl::getAttribute("Value")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void AttributeImpl::setValue( const std::string& val)
{
	FCOImpl::getAttribute("Value")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PortType
//********************************************************************************
PortType BindingPortTypeImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PortType sp( ce);
	if ( sp)
		return sp;

	PortType empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Binding
//********************************************************************************
Binding BindingPortTypeImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Binding sp( ce);
	if ( sp)
		return sp;

	Binding empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to OperationRef
//********************************************************************************
OperationRef BindsOperationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	OperationRef sp( ce);
	if ( sp)
		return sp;

	OperationRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BindingOperation
//********************************************************************************
BindingOperation BindsOperationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BindingOperation sp( ce);
	if ( sp)
		return sp;

	BindingOperation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to SOAPBody
//********************************************************************************
SOAPBody BodyBindingImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	SOAPBody sp( ce);
	if ( sp)
		return sp;

	SOAPBody empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to IORef
//********************************************************************************
IORef BodyBindingImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	IORef sp( ce);
	if ( sp)
		return sp;

	IORef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PartRef
//********************************************************************************
PartRef BodyPartsImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PartRef sp( ce);
	if ( sp)
		return sp;

	PartRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to SOAPBody
//********************************************************************************
SOAPBody BodyPartsImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	SOAPBody sp( ce);
	if ( sp)
		return sp;

	SOAPBody empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to MessageRef
//********************************************************************************
MessageRef FaultMessageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	MessageRef sp( ce);
	if ( sp)
		return sp;

	MessageRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Fault
//********************************************************************************
Fault FaultMessageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Fault sp( ce);
	if ( sp)
		return sp;

	Fault empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to SOAPHeaders
//********************************************************************************
SOAPHeaders HeaderBindingImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	SOAPHeaders sp( ce);
	if ( sp)
		return sp;

	SOAPHeaders empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to IORef
//********************************************************************************
IORef HeaderBindingImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	IORef sp( ce);
	if ( sp)
		return sp;

	IORef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to MessageRef
//********************************************************************************
MessageRef HeaderMessageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	MessageRef sp( ce);
	if ( sp)
		return sp;

	MessageRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to SOAPHeaders
//********************************************************************************
SOAPHeaders HeaderMessageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	SOAPHeaders sp( ce);
	if ( sp)
		return sp;

	SOAPHeaders empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PartRef
//********************************************************************************
PartRef HeaderPartImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PartRef sp( ce);
	if ( sp)
		return sp;

	PartRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to SOAPHeaders
//********************************************************************************
SOAPHeaders HeaderPartImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	SOAPHeaders sp( ce);
	if ( sp)
		return sp;

	SOAPHeaders empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to MessageRef
//********************************************************************************
MessageRef InputMessageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	MessageRef sp( ce);
	if ( sp)
		return sp;

	MessageRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Input
//********************************************************************************
Input InputMessageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Input sp( ce);
	if ( sp)
		return sp;

	Input empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to MessageRef
//********************************************************************************
MessageRef OutputMessageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	MessageRef sp( ce);
	if ( sp)
		return sp;

	MessageRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Output
//********************************************************************************
Output OutputMessageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Output sp( ce);
	if ( sp)
		return sp;

	Output empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BindingRef
//********************************************************************************
BindingRef PortBindingImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BindingRef sp( ce);
	if ( sp)
		return sp;

	BindingRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Port
//********************************************************************************
Port PortBindingImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Port sp( ce);
	if ( sp)
		return sp;

	Port empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to SOAPFault
//********************************************************************************
SOAPFault SOAPFaultExtensionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	SOAPFault sp( ce);
	if ( sp)
		return sp;

	SOAPFault empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to FaultRef
//********************************************************************************
FaultRef SOAPFaultExtensionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	FaultRef sp( ce);
	if ( sp)
		return sp;

	FaultRef empty;
	return empty;
}


//********************************************************************************
//
//********************************************************************************
std::set<PortBinding> BindingRefImpl::getInPortBindingLinks()
{
	std::set<PortBinding> result;
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


//********************************************************************************
// returns src Ports
//********************************************************************************
std::multiset<Port> BindingRefImpl::getPortBindingSrcs()
{
	std::multiset<Port> res;
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


//********************************************************************************
//
//********************************************************************************
Binding BindingRefImpl::getBinding()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Binding(r);
}


//********************************************************************************
//
//********************************************************************************
std::set<SOAPFaultExtension> FaultRefImpl::getOutSOAPFaultExtensionLinks()
{
	std::set<SOAPFaultExtension> result;
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


//********************************************************************************
// returns dst SOAPFaults
//********************************************************************************
std::multiset<SOAPFault> FaultRefImpl::getSOAPFaultExtensionDsts()
{
	std::multiset<SOAPFault> res;
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


//********************************************************************************
//
//********************************************************************************
Fault FaultRefImpl::getFault()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Fault(r);
}


//********************************************************************************
// returns src Faults
//********************************************************************************
std::multiset<Fault> MessageRefImpl::getFaultMessageSrcs()
{
	std::multiset<Fault> res;
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


//********************************************************************************
// returns src SOAPHeaderss
//********************************************************************************
std::multiset<SOAPHeaders> MessageRefImpl::getHeaderMessageSrcs()
{
	std::multiset<SOAPHeaders> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<FaultMessage> MessageRefImpl::getInFaultMessageLinks()
{
	std::set<FaultMessage> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<HeaderMessage> MessageRefImpl::getInHeaderMessageLinks()
{
	std::set<HeaderMessage> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<InputMessage> MessageRefImpl::getInInputMessageLinks()
{
	std::set<InputMessage> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<OutputMessage> MessageRefImpl::getInOutputMessageLinks()
{
	std::set<OutputMessage> result;
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


//********************************************************************************
// returns src Inputs
//********************************************************************************
std::multiset<Input> MessageRefImpl::getInputMessageSrcs()
{
	std::multiset<Input> res;
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


//********************************************************************************
// returns src Outputs
//********************************************************************************
std::multiset<Output> MessageRefImpl::getOutputMessageSrcs()
{
	std::multiset<Output> res;
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


//********************************************************************************
//
//********************************************************************************
Message MessageRefImpl::getMessage()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Message(r);
}


//********************************************************************************
// returns src BindingOperations
//********************************************************************************
std::multiset<BindingOperation> OperationRefImpl::getBindsOperationSrcs()
{
	std::multiset<BindingOperation> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BindsOperation> OperationRefImpl::getInBindsOperationLinks()
{
	std::set<BindsOperation> result;
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


//********************************************************************************
//
//********************************************************************************
Operation OperationRefImpl::getOperation()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Operation(r);
}


//********************************************************************************
//
//********************************************************************************
Referrable PartImpl::getReferrable()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Referrable(r);
}


//********************************************************************************
// returns src SOAPBodys
//********************************************************************************
std::multiset<SOAPBody> PartRefImpl::getBodyPartsSrcs()
{
	std::multiset<SOAPBody> res;
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


//********************************************************************************
// returns src SOAPHeaderss
//********************************************************************************
std::multiset<SOAPHeaders> PartRefImpl::getHeaderPartSrcs()
{
	std::multiset<SOAPHeaders> res;
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


//********************************************************************************
//
//********************************************************************************
std::set<BodyParts> PartRefImpl::getInBodyPartsLinks()
{
	std::set<BodyParts> result;
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


//********************************************************************************
//
//********************************************************************************
std::set<HeaderPart> PartRefImpl::getInHeaderPartLinks()
{
	std::set<HeaderPart> result;
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


//********************************************************************************
//
//********************************************************************************
Part PartRefImpl::getPart()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Part(r);
}



} // namespace WSML
