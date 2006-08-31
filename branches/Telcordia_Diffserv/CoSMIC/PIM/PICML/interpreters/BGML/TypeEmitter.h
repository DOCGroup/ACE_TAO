#ifndef TYPE_EMITTER_H
#define TYPE_EMITTER_H

#include <string>
#include "BGML/BGML_Export.h"

class BGML_Export ReturnTypeEmitter
{
public:
	static std::string generate_type_defn (std::string& kindName,
		std::string& scope_name);
};


class BGML_Export InTypeEmitter
{
public:
	static std::string generate_type_defn (std::string& kindName,
		std::string& scope_name);
};


class BGML_Export InoutTypeEmitter
{
public:
	static std::string generate_type_defn (std::string& kindName,
		std::string& scope_name);
};

class BGML_Export OutTypeEmitter
{

public:
	static std::string generate_type_defn (std::string& kindName,
		std::string& scope_name);
};


#endif // TYPE_EMITTER
