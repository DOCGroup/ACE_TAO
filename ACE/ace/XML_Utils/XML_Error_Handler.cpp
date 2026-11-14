#include "XML_Error_Handler.h"
#include "XercesString.h"
#include "ace/ACE.h"
#include <iostream>
#include <xercesc/sax/SAXParseException.hpp>

namespace XML
{
  void XML_Error_Handler::warning (xercesc::SAXParseException const &toCatch)
  {
    if (ACE::debug ())
    {
      XStr const file (toCatch.getSystemId ());
      XStr const msg (toCatch.getMessage ());

      std::cerr << "Warning: " << file << ':' << toCatch.getLineNumber () << ':' << toCatch.getColumnNumber () << " - "
                << msg << std::endl;
    }
  }

  void XML_Error_Handler::error (xercesc::SAXParseException const &toCatch)
  {
    if (ACE::debug ())
    {
      XStr const file (toCatch.getSystemId ());
      XStr const msg (toCatch.getMessage ());

      std::cerr << "Error: " << file << ':' << toCatch.getLineNumber () << ':' << toCatch.getColumnNumber () << " - "
                << msg << std::endl;
    }
    this->errors_ = true;
  }

  void XML_Error_Handler::fatalError (xercesc::SAXParseException const &toCatch)
  {
    if (ACE::debug ())
    {
      XStr const file (toCatch.getSystemId ());
      XStr const msg (toCatch.getMessage ());

      std::cerr << "Fatal Error: " << file << ':' << toCatch.getLineNumber () << ':' << toCatch.getColumnNumber ()
                << " - " << msg << std::endl;
    }
    this->errors_ = true;
  }

  void XML_Error_Handler::resetErrors ()
  {
    this->errors_ = false;
  }

  bool XML_Error_Handler::getErrors () const
  {
    return this->errors_;
  }
}
