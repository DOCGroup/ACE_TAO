// $Id$
/**
 * @file Exceptions.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * Defines exceptions that may be thrown during the XSC<=>IDL translation process.
 */
#include <string>

namespace CIAO
{
  namespace Config_Handlers
  {
    struct Parse_Error
    {
      Parse_Error (const std::string &reason)
        : reason_ (reason)
      {
      }

      std::string reason_;
    };

    struct Plan_Error
    {
      Plan_Error (const std::string &reason)
        : reason_ (reason)
      {
      }

      std::string reason_;
    };

  }
}




