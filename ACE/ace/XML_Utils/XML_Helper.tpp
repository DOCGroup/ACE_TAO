#include "XML_Helper.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Category.h"
#include "xercesc/util/XMLUniDefs.hpp"
#include "xercesc/dom/DOM.hpp"
#include "XML_Error_Handler.h"
#include "XML_Schema_Resolver.h"
#include "xercesc/framework/LocalFileFormatTarget.hpp"
#include "xercesc/dom/DOM.hpp"
#include "XercesString.h"
#include "xercesc/dom/DOMLSSerializer.hpp"
#include <memory>
#include <stdexcept>

namespace XML
{
  using xercesc::XMLException;
  using xercesc::XMLString;
  using xercesc::DOMImplementation;
  using xercesc::DOMImplementationRegistry;
//    using xercesc::DOMBuilder;
  using xercesc::DOMImplementationLS;
  using xercesc::XMLUni;
  using xercesc::DOMDocument;
  using xercesc::DOMException;
  using xercesc::DOMDocumentType;
  using xercesc::XercesDOMParser;

  // TODO this is stub implementation
  template <typename Resolver, typename Error>
  XML_Helper<Resolver, Error>::XML_Helper (Resolver *resolver, Error *eh)
    : resolver_ (resolver),
      e_handler_ (eh)
  {
    this->init_parser ();
  }

  template <typename Resolver, typename Error>
  XML_Helper<Resolver, Error>::~XML_Helper ()
  {
    this->terminate_parser ();
  }

  template <typename Resolver, typename Error>
  bool
  XML_Helper<Resolver, Error>::is_initialized () const
  {
    return this->initialized_;
  }

  template <typename Resolver, typename Error>
  void
  XML_Helper<Resolver, Error>::init_parser ()
  {
    if (this->initialized_)
      return;

    bool runtime_started (false);
    try
      {
        if (!resolver_)
          {
            resolver_ = new Resolver ();
            release_resolver_ = true;
          }

        if (!e_handler_)
          {
            e_handler_ = new Error ();
            release_e_handler_ = true;
          }

        xercesc::XMLPlatformUtils::Initialize();
        runtime_started = true;

        // Instantiate the DOM parser implementation.
        static const XMLCh gLS[] = { xercesc::chLatin_L,
                                     xercesc::chLatin_S,
                                     xercesc::chNull };

        impl_ = DOMImplementationRegistry::getDOMImplementation(gLS);
        if (impl_ == nullptr)
          throw std::runtime_error ("Xerces does not provide DOM Load/Store");

        this->initialized_ = true;
      }
    catch (const XMLException& e)
      {
        char* message = XMLString::transcode (e.getMessage());
        std::unique_ptr<char[]> cleanup_message (message);
        impl_ = nullptr;
        if (runtime_started)
          xercesc::XMLPlatformUtils::Terminate ();
        if (release_resolver_) { delete resolver_; resolver_ = nullptr; release_resolver_ = false; }
        if (release_e_handler_) { delete e_handler_; e_handler_ = nullptr; release_e_handler_ = false; }
        throw;
      }
    catch (...)
      {
        impl_ = nullptr;
        if (runtime_started)
          xercesc::XMLPlatformUtils::Terminate ();
        if (release_resolver_) { delete resolver_; resolver_ = nullptr; release_resolver_ = false; }
        if (release_e_handler_) { delete e_handler_; e_handler_ = nullptr; release_e_handler_ = false; }
        throw;
      }
  }

  template <typename Resolver, typename Error>
  XERCES_CPP_NAMESPACE::DOMDocument *
  XML_Helper<Resolver, Error>::create_dom (const ACE_TCHAR *root,
                                           const ACE_TCHAR *ns,
                                           DOMDocumentType *doctype) const
  {
    if (!root || !ns)
      return nullptr;

    if (!initialized_ || impl_ == nullptr)
      throw std::logic_error ("XML_Helper is not initialized");

    return this->impl_->createDocument (XStr (ns),
                                        XStr (root),
                                        doctype);
  }

  template <typename Resolver, typename Error>
  XERCES_CPP_NAMESPACE::DOMDocumentType *
  XML_Helper<Resolver, Error>::create_doctype (const ACE_TCHAR *qn,
                                               const ACE_TCHAR *pid,
                                               const ACE_TCHAR *sid) const
  {
    if (!initialized_ || impl_ == nullptr)
      throw std::logic_error ("XML_Helper is not initialized");
    return this->impl_->createDocumentType (XStr (qn),
                                            XStr (pid),
                                            XStr (sid));
  }
  template <typename Resolver, typename Error>
  XERCES_CPP_NAMESPACE::DOMDocument *
  XML_Helper<Resolver, Error>::create_dom (const ACE_TCHAR *url) const
  {

    if (url == nullptr)
      return nullptr;

    if (!initialized_ || impl_ == nullptr)
      throw std::logic_error ("XML_Helper is not initialized");

    try
      {
        if (!this->parser_)
          this->parser_.reset ((new xercesc::XercesDOMParser ()));

        // Perform Namespace processing.
        this->parser_->setDoNamespaces (true);

        // Discard comment nodes in the document
        this->parser_->setCreateCommentNodes (false);

        // Disable datatype normalization. The XML 1.0 attribute value
        // normalization always occurs though.
        // this->parser_->setFeature (XMLUni::fgDOMDatatypeNormalization, true);

        // Do not create EntityReference nodes in the DOM tree. No
        // EntityReference nodes will be created, only the nodes
        // corresponding to their fully expanded sustitution text will be
        // created.
        this->parser_->setCreateEntityReferenceNodes (false);

        // Perform Validation
        this->parser_->setValidationScheme (xercesc::AbstractDOMParser::Val_Always);

        // Do not include ignorable whitespace in the DOM tree.
        this->parser_->setIncludeIgnorableWhitespace (false);

        // Enable the parser's schema support.
        this->parser_->setDoSchema (true);

        // Enable full schema constraint checking, including checking which
        // may be time-consuming or memory intensive. Currently, particle
        // unique attribution constraint checking and particle derivation
        // restriction checking are controlled by this option.
        this->parser_->setValidationSchemaFullChecking (true);

        // The parser will treat validation error as fatal and will exit.
        this->parser_->setValidationConstraintFatal (true);

        this->parser_->setErrorHandler (e_handler_);

        this->parser_->setEntityResolver (resolver_);

        this->parser_->parse (ACE_TEXT_ALWAYS_CHAR (url));

        if (e_handler_ && e_handler_->getErrors ())
          return nullptr;

        return this->parser_->getDocument ();
      }
    catch (const DOMException& e)
      {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        if (DOMImplementation::loadDOMExceptionMsg (e.code,
                                                    errText,
                                                    maxChars))
          {
            char* message = XMLString::transcode (errText);
            std::unique_ptr<char[]> cleanup_message (message);
          }
        return nullptr;

      }
    catch (const XMLException& e)
      {
        char* message = XMLString::transcode (e.getMessage());
        std::unique_ptr<char[]> cleanup_message (message);
        throw 0;
      }
    catch (...)
      {
        ACELIB_DEBUG ((LM_DEBUG, "(%P|%t) Caught an unknown exception\n"));
        throw;
      }

    return nullptr;
  }

  template <typename Resolver, typename Error>
  void
  XML_Helper<Resolver, Error>::terminate_parser ()
  {
    if (!this->initialized_)
      return;

    try
      {
        if (release_resolver_)
          {
            delete resolver_;
            resolver_ = nullptr;
          }

        if (release_e_handler_)
          {
            delete e_handler_;
            e_handler_ = nullptr;
          }

        this->parser_.reset (nullptr);
        this->impl_ = nullptr;
        xercesc::XMLPlatformUtils::Terminate();
      }
    catch (const XMLException& e)
      {
        char* message = XMLString::transcode (e.getMessage());
        std::unique_ptr<char[]> cleanup_message (message);
        throw;
      }

    this->initialized_ = false;
    return;
  }

  template <typename Resolver, typename Error>
  Resolver &
  XML_Helper<Resolver, Error>::get_resolver ()
  {
    if (!this->resolver_)
      throw std::logic_error ("XML resolver is not available");

    return *this->resolver_;
  }

  template <typename Resolver, typename Error>
  Error &
  XML_Helper<Resolver, Error>::get_error_handler ()
  {
    if (!this->e_handler_)
      throw std::logic_error ("XML error handler is not available");

    return *this->e_handler_;
  }

  template <typename Resolver, typename Error>
  bool
  XML_Helper<Resolver, Error>::write_DOM (XERCES_CPP_NAMESPACE::DOMDocument *doc,
                                          const ACE_TCHAR *file) const
  {
    if (!initialized_ || impl_ == nullptr)
      throw std::logic_error ("XML_Helper is not initialized");
    if (doc == nullptr || file == nullptr)
      return false;

    struct SerializerReleaser
    {
      void operator() (XERCES_CPP_NAMESPACE::DOMLSSerializer* p) const
      { if (p != nullptr) p->release (); }
    };
    struct OutputReleaser
    {
      void operator() (XERCES_CPP_NAMESPACE::DOMLSOutput* p) const
      { if (p != nullptr) p->release (); }
    };

    try
      {
        std::unique_ptr<XERCES_CPP_NAMESPACE::DOMLSSerializer, SerializerReleaser>
          serializer (impl_->createLSSerializer ());
        if (!serializer) return false;
        XERCES_CPP_NAMESPACE::DOMConfiguration *ser_config (serializer->getDomConfig ());
        std::unique_ptr<XERCES_CPP_NAMESPACE::DOMLSOutput, OutputReleaser>
          output (impl_->createLSOutput ());
        if (!output) return false;

        if (ser_config->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
          ser_config->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

        std::unique_ptr<XMLFormatTarget> format_target (
          new XERCES_CPP_NAMESPACE::LocalFileFormatTarget (ACE_TEXT_ALWAYS_CHAR (file)));

        output->setByteStream (format_target.get ());

        bool const result (serializer->write (doc, output.get ()));
        output.reset ();
        return result;
      }
    catch (const xercesc::XMLException &e)
      {
        char* message = XMLString::transcode (e.getMessage());
        std::unique_ptr<char[]> cleanup_message (message);

        char* name = XMLString::transcode (e.getType());
        std::unique_ptr<char[]> cleanup_name (name);

        ACELIB_ERROR ((LM_ERROR, "Caught exception while serializing DOM to file.\n"
                    "Name: %C\n"
                    "Message: %C\n"
                    "SrcFile: %C\n"
                    "SrcLine: %C\n",
                    name,
                    message,
                    e.getSrcFile (),
                    e.getSrcLine ()));
        return false;
      }
  }
}
