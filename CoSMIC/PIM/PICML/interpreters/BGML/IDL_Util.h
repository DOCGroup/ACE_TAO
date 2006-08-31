#ifndef IDL_UTIL_H
#define IDL_UTIL_H

#include "PICML/PICML.h"
#include "BGML/BGML_Export.h"

class BGML_Export IDL_Util
{
public:
	static std::string component_name (PICML::OperationRef& op);
	static void return_type_signature (PICML::MemberType& mem_type,
									   std::string& operation_name,
									   std::string& package,
									   int type);
	static std::string operation_name (PICML::TwowayOperation& op);
	static std::vector<std::string> argument_list (PICML::TwowayOperation& op);
	static std::string dependant_idls (PICML::TwowayOperation& op);

protected:
	static std::string scope_name (PICML::TwowayOperation &operation_name);

};

#endif /* IDL_UTIL_H */
