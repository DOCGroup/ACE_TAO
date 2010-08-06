// $Id$
/**
* @file Common.h
* @author Will Otte <wotte@dre.vanderbilt.edu>
*
* Some common definitions for all config_handlers.
*/
#ifndef CONFIG_HANDLERS_COMMON_H
#define CONFIG_HANDLERS_COMMON_H
#include <string>

namespace CIAO
{
namespace Config_Handlers
{
/**
* @class Config_Error
* @brief Exception to communicate a config_handlers error
*/
class Config_Error
{
public:
Config_Error (const std::string &name,
const std::string &error) :
name_ (name),
error_ (error)
{};

/// Some identifying information about the element that
/// caused the error
std::string name_;

/// A human readable error message
std::string error_;

};
}
}
#endif /* CONFIG_HANDLERS_COMMON_H */


