#include "XML_Error_Handler.h"
#include "ace/ACE.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include "XercesString.h"
#include <iostream>

namespace XML
{
  void XML_Error_Handler::warning(const xercesc::SAXParseException& toCatch)
  {
    if (ACE::debug ())
      {
        XStr file (toCatch.getSystemId ());
        XStr msg (toCatch.getMessage ());

        std::cerr << "Warning: " << file << ':' << toCatch.getLineNumber ()
                  << ':' << toCatch.getColumnNumber () << " - "
                  << msg << std::endl;
      }
  }

  void XML_Error_Handler::error(const xercesc::SAXParseException& toCatch)
  {
    if (ACE::debug ())
      {
        XStr file (toCatch.getSystemId ());
        XStr msg (toCatch.getMessage ());

        std::cerr << "Error: " << file << ':' << toCatch.getLineNumber ()
                  << ':' << toCatch.getColumnNumber () << " - "
                  << msg << std::endl;
      }
    this->errors_ = true;
  }

  void XML_Error_Handler::fatalError(const xercesc::SAXParseException& toCatch)
  {
    if (ACE::debug ())
      {
        XStr file (toCatch.getSystemId ());
        XStr msg (toCatch.getMessage ());

        std::cerr << "Fatal Error: " << file << ':' << toCatch.getLineNumber ()
                  << ':' << toCatch.getColumnNumber () << " - "
                  << msg << std::endl;
      }
    this->errors_ = true;
  }

  void XML_Error_Handler::resetErrors()
  {
    this->errors_ = false;
  }

  bool
  XML_Error_Handler::getErrors () const
  {
    return this->errors_;
  }
}
