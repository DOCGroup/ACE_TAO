// $Id$

#ifndef CONFIG_ERROR_HANDLER_H
#define CONFIG_ERROR_HANDLER_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/dom/DOMErrorHandler.hpp>

using xercesc::DOMErrorHandler;
using xercesc::DOMError;

namespace CIAO
{
  namespace Config_Handler
  {
    class Config_Error_Handler : public DOMErrorHandler
    {
    public:

      Config_Error_Handler ();

      ~Config_Error_Handler ();

      bool handleError (const DOMError& domError);

      void resetErrors ();

      bool getErrors() const;

    private :
      // Disallow copying
      Config_Error_Handler (const Config_Error_Handler&);
      Config_Error_Handler& operator= (const Config_Error_Handler&);

      bool errors_;

    };

  };
}

#include /**/ "ace/post.h"

#endif /* CONFIG_ERROR_HANDLER_H */
