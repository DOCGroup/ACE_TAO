// $Id$

#include "XML_Helper.h"
#include "ace/Log_Msg.h"

#include "xercesc/dom/DOM.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"

#include "ace/XML_Utils/XercesString.h"

namespace Test
{
  Helper::Helper (XML::XML_Error_Handler &eh)
    : impl_ (0)
    , parser_ (0)
    , e_handler_ (eh)
  {
    this->init_parser ();
  }

  Helper::~Helper (void)
  {
    this->terminate_parser ();
  }

  void
  Helper::init_parser (void)
  {
    // Initialize the Xerces run-time
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();

    // Instantiate the DOM parser.
    static const XMLCh gLS[] = { XERCES_CPP_NAMESPACE::chLatin_L,
                                  XERCES_CPP_NAMESPACE::chLatin_S,
                                  XERCES_CPP_NAMESPACE::chNull };

    // Get an implementation of the Load-Store (LS) interface
    // and cache it for later use
    impl_ =
      XERCES_CPP_NAMESPACE::DOMImplementationRegistry::getDOMImplementation(gLS);
  }

  int
  Helper::validate_dom (const ACE_TCHAR *url)
  {
    if (url == 0)
      return 0;

    try
      {
        if (this->parser_ == 0)
          this->parser_ = new XERCES_CPP_NAMESPACE::XercesDOMParser ();

        // Discard comment nodes in the document
        this->parser_->setCreateCommentNodes (false);

        // Do not create EntityReference nodes in the DOM tree. No
        // EntityReference nodes will be created, only the nodes
        // corresponding to their fully expanded sustitution text will be
        // created.
        this->parser_->setCreateEntityReferenceNodes (false);

        // Perform Validation
        this->parser_->setValidationScheme (
          XERCES_CPP_NAMESPACE::AbstractDOMParser::Val_Always);

        // Do not include ignorable whitespace in the DOM tree.
        this->parser_->setIncludeIgnorableWhitespace (false);

        // Enable full schema constraint checking, including checking which
        // may be time-consuming or memory intensive. Currently, particle
        // unique attribution constraint checking and particle derivation
        // restriction checking are controlled by this option.
        this->parser_->setValidationSchemaFullChecking (true);

        this->parser_->setErrorHandler (&e_handler_);

        this->parser_->parse (ACE_TEXT_ALWAYS_CHAR (url));

        if (e_handler_.getErrors ())
          return 1;

        return 0;
      }
    catch (...)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Caught an unknown exception\n"));
        throw;
      }

    return 0;
  }

  void
  Helper::terminate_parser (void)
  {
    XERCES_CPP_NAMESPACE::XMLPlatformUtils::Terminate ();
  }
}
