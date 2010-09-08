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

namespace DAnCE
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
      Config_Error (const std::basic_string<ACE_TCHAR> &name,
                    const std::basic_string<ACE_TCHAR> &error) :
        name_ (name),
        error_ (error)
      {};
      
      void add_name (const std::basic_string<ACE_TCHAR> &name)
      {
        name_ = name + ACE_TEXT (":") + name_;
      }

      /// Some identifying information about the element that
      /// caused the error
      std::basic_string<ACE_TCHAR> name_;

      /// A human readable error message
      std::basic_string<ACE_TCHAR> error_;

    };
  }
}
#endif /* CONFIG_HANDLERS_COMMON_H */


