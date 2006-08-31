#include "BGML/TypeEmitter.h"

std::string ReturnTypeEmitter::generate_type_defn (std::string &kind_name,
												   std::string& scope_name)
{
	if (kind_name == "Boolean")
		return "::CORBA::Boolean";
	else if (kind_name == "Byte")
		return "::CORBA::Octet";
	else if (kind_name == "String")
		return "char *";
	else if (kind_name == "LongInteger")
		return "::CORBA::Long";
	else if (kind_name == "RealNumber")
		return "::CORBA::Double";
	else if (kind_name == "ShortInteger")
		return "::CORBA::Short";
	else if (kind_name == "GenericValue")
		return "::CORBA::Any *";
	else if (kind_name == "GenericObject")
		return "::CORBA::Object_ptr";
	else if (kind_name == "GenericValueObject")
		return "::CORBA::ValueBase *";
	else if (kind_name == "TypeEncoding")
		return "::CORBA::TypeCode_ptr";
	else if (kind_name == "TypeKind")
		return "::CORBA::TCKind";

	//// Named Types /////////////////
	else if (kind_name == "Enum")
		return scope_name;

	//@@@ Need to check if these are fixed/un-bounded
	else if (kind_name == "SwitchedAggregate" ||
			     kind_name == "Aggregate")
				 return (scope_name + " *");

	return "";
}


std::string InTypeEmitter::generate_type_defn (std::string &kind_name,
											   std::string& scope_name)
{
	if (kind_name == "Boolean")
		return "::CORBA::Boolean";
	else if (kind_name == "Byte")
		return "::CORBA::Octet";
	else if (kind_name == "String")
		return "const char *";
	else if (kind_name == "LongInteger")
		return "::CORBA::Long";
	else if (kind_name == "RealNumber")
		return "::CORBA::Double";
	else if (kind_name == "ShortInteger")
		return "::CORBA::Short";
	else if (kind_name == "GenericValue")
		return "const ::CORBA::Any &";
	else if (kind_name == "GenericObject")
		return "::CORBA::Object_ptr";
	else if (kind_name == "GenericValueObject" ||
		     kind_name == "ValueObject")
		return "::CORBA::ValueBase *";

	//// Named Types /////////////////
	else if (kind_name == "Enum")
		return scope_name;
	else if (kind_name == "SwitchedAggregate" ||
			     kind_name == "Aggregate")
				 return ("const " + scope_name + " &");

	return "";
}



std::string InoutTypeEmitter::generate_type_defn (std::string& kind_name,
												  std::string& scope_name)
{
	if (kind_name == "Boolean")
		return "::CORBA::Boolean &";
	else if (kind_name == "Byte")
		return "::CORBA::Octet &";
	else if (kind_name == "String")
		return "char *&";
	else if (kind_name == "LongInteger")
		return "::CORBA::Long &";
	else if (kind_name == "RealNumber")
		return "::CORBA::Double &";
	else if (kind_name == "ShortInteger")
		return "::CORBA::Short &";
	else if (kind_name == "GenericValue")
		return "::CORBA::Any &";
	else if (kind_name == "GenericObject")
		return "::CORBA::Object_ptr &";
	else if (kind_name == "GenericValueObject")
		return "::CORBA::ValueBase &";
	//// Named Types /////////////////
	else if (kind_name == "Enum")
		return (scope_name + " &");
	else if (kind_name == "SwitchedAggregate" ||
			     kind_name == "Aggregate")
				 return (scope_name + " &");

	return "";

}

std::string OutTypeEmitter::generate_type_defn (std::string& kind_name,
												std::string& scope_name)
{
	if (kind_name == "Boolean")
		return "::CORBA::Boolean_out";
	else if (kind_name == "Byte")
		return "::CORBA::Octet_out";
	else if (kind_name == "String")
		return "char *";
	else if (kind_name == "LongInteger")
		return "::CORBA::Long_out";
	else if (kind_name == "RealNumber")
		return "::CORBA::Double_out";
	else if (kind_name == "ShortInteger")
		return "::CORBA::Short_out";
	else if (kind_name == "GenericValue")
		return "::CORBA::Any_out";
	else if (kind_name == "GenericObject")
		return "::CORBA::Object_out";
	else if (kind_name == "GenericValueObject")
		return "::CORBA::ValueBase_out";
	//// Named Types /////////////////
	else if (kind_name == "Enum")
		return (scope_name + "_out");
	else if (kind_name == "SwitchedAggregate" ||
			     kind_name == "Aggregate")
				 return (scope_name + "_out");

	return "";
}
