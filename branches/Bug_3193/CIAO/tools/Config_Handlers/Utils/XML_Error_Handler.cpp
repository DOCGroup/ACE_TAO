// $Id$

#include "XML_Error_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/Auto_Ptr.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include "XercesString.h"

using xercesc::XMLString;

namespace CIAO
{
  namespace Config_Handlers
  {
    XML_Error_Handler::XML_Error_Handler (void)
      : errors_ (false)
    {
    }

    XML_Error_Handler::~XML_Error_Handler()
    {
    }


    bool
    XML_Error_Handler::handleError (const DOMError& domError)
    {
      this->errors_ = true;

      if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t), Warning "));
      else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t), Error "));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t), Fatal Error "));
      char *msg =
        XMLString::transcode (domError.getMessage ());

      ACE_Auto_Basic_Array_Ptr<char> cleanup_msg (msg);

      char *file =
        XMLString::transcode (domError.getLocation ()->getURI ());

      ACE_Auto_Basic_Array_Ptr<char> cleanup_file (file);

      ACE_DEBUG ((LM_DEBUG,
                  "%s at line %d and column %d in file %s\n",
                  msg,
                  domError.getLocation ()->getLineNumber (),
                  domError.getLocation ()->getColumnNumber (),
                  file));

      return true;
    }

    void
    XML_Error_Handler::resetErrors (void)
    {
      this->errors_ = false;
    }

    bool
    XML_Error_Handler::getErrors (void) const
    {
      return this->errors_;
    }
  }
}
