// $Id$

#include "ace/Log_Msg.h"
#include "ace/Auto_Ptr.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include "Config_Error_Handler.h"
#include "XercesString.h"

using xercesc::XMLString;

namespace CIAO
{
  namespace Config_Handler
  {
    Config_Error_Handler::Config_Error_Handler()
      : errors_ (false)
    {}

    Config_Error_Handler::~Config_Error_Handler()
    {}


    bool Config_Error_Handler::handleError (const DOMError& domError)
    {
      this->errors_ = true;
      if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
        {
          ACE_ERROR ((LM_ERROR, "Warning "));
        }
      else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
        {
          ACE_ERROR ((LM_ERROR, "Error "));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "FatalError "));
        }

      char *msg = XMLString::transcode(domError.getMessage());
      ACE_Auto_Basic_Array_Ptr<char> cleanup_msg (msg);
      ACE_ERROR ((LM_ERROR, "while processing resource '%s', "
                  "Line %d, Position %d : %s\n",
                  domError.getLocation()->getURI(),
                  domError.getLocation()->getLineNumber(),
                  domError.getLocation()->getColumnNumber(),
                  msg));
      return true;
    }

    void Config_Error_Handler::resetErrors()
    {
      this->errors_ = false;
    }

    bool Config_Error_Handler::getErrors() const
    {
      return this->errors_;
    }
  }

}
