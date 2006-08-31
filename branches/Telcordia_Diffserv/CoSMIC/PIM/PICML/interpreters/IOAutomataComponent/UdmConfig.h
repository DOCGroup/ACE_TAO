// Component configuration file automatically generated as UdmConfig.h
// by UDM Interpreter Wizard on Monday, May 13, 2002 13:45:42


#ifndef UDM_CONFIG_H
#define UDM_CONFIG_H


#define _USE_DOM

#include "PICML/PICML.h"
#include <string> // STL

#define META_NAMESPACE PICML

//  Uncomment this to specify the meta-xml dynamically at interpreter run time.
// #define _DYNAMIC_META

//  Allows using DOM backend for the dynamic meta
// #define _DYNAMIC_META_DOM


//#define _ACCESS_MEMORY



struct _config
{
	// Global settings
	string metaPath;

};
extern _config config;

#endif /* UDM_CONFIG_H */
