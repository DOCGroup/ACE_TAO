//$Id$
#include "XML_Helper.h"
#include "ace/Log_Msg.h"
#include "xercesc/util/XMLUniDefs.hpp"
#include "xercesc/dom/DOM.hpp"
#include "XML_Error_Handler.h"
#include "XML_Schema_Resolver.h"
#include "xercesc/framework/LocalFileFormatTarget.hpp"
#include "XercesString.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    using xercesc::XMLException;
    using xercesc::XMLString;
    using xercesc::DOMImplementation;
    using xercesc::DOMImplementationRegistry;
    using xercesc::DOMBuilder;
    using xercesc::DOMImplementationLS;
    using xercesc::XMLUni;
    using xercesc::DOMDocument;
    using xercesc::DOMException;
    using xercesc::DOMDocumentType;

    template <typename Resolver, typename Error>
    XML_Helper_T<Resolver, Error>::XML_Helper_T (void)
      : initialized_ (false),
        impl_ (0)
    {
      this->init_parser ();
    }

    template <typename Resolver, typename Error>
    XML_Helper_T<Resolver, Error>::~XML_Helper_T (void)
    {
      try
        {
          this->terminate_parser ();
        }
      catch (...)
        {
          // We don't care about exceptions here.
        }
    }

    template <typename Resolver, typename Error>
    bool
    XML_Helper_T<Resolver, Error>::is_initialized (void) const
    {
      return this->initialized_ == true;
    }

    template <typename Resolver, typename Error>
    void
    XML_Helper_T<Resolver, Error>::init_parser (void)
    {
      if (this->initialized_)
        return;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Initializing the Xerces runtime \n"));
      // Initialize the Xerces run-time
      try
        {
          xercesc::XMLPlatformUtils::Initialize();
        }
      catch (const XMLException&)
        {
          throw;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Some other exception,"
                      " returning\n"));

          return;
        }

      // Instantiate the DOM parser.
      static const XMLCh gLS[] = { xercesc::chLatin_L,
                                   xercesc::chLatin_S,
                                   xercesc::chNull };

      // Get an implementation of the Load-Store (LS) interface
      // and cache it for later use
      impl_ =
        DOMImplementationRegistry::getDOMImplementation(gLS);

      this->initialized_ = true;
      return;
    }

    template <typename Resolver, typename Error>
    XERCES_CPP_NAMESPACE::DOMDocument *
    XML_Helper_T<Resolver, Error>::create_dom (const ACE_TCHAR *root,
                                               const ACE_TCHAR *ns,
                                               DOMDocumentType *doctype)
    {
      if (!this->initialized_)
        return 0;

      if (root == 0 || ns == 0)
        return 0;

      return this->impl_->createDocument (XStr (ns),
                                          XStr (root),
                                          doctype);
    }

    template <typename Resolver, typename Error>
    XERCES_CPP_NAMESPACE::DOMDocumentType *
    XML_Helper_T<Resolver, Error>::create_doctype (const ACE_TCHAR *qn,
                                                   const ACE_TCHAR *pid,
                                                   const ACE_TCHAR *sid)
    {
      if (!this->initialized_)
        return 0;

      return this->impl_->createDocumentType (XStr (qn),
                                              XStr (pid),
                                              XStr (sid));
    }

    template <typename Resolver, typename Error>
    XERCES_CPP_NAMESPACE::DOMDocument *
    XML_Helper_T<Resolver, Error>::create_dom (const ACE_TCHAR *url)
    {
      if (!this->initialized_)
        return 0;

      if (url == 0)
        return impl_->createDocument(
          XStr (ACE_TEXT ("http://www.omg.org/DeploymentPlan")),
          XStr (ACE_TEXT ("deploymentPlan")),
          0);

      try
        {
          // Create a DOMBuilder
          DOMBuilder* parser =
            impl_->createDOMBuilder (DOMImplementationLS::MODE_SYNCHRONOUS,
                                     0);

          // Discard comment nodes in the document
          parser->setFeature (XMLUni::fgDOMComments, false);

          // Disable datatype normalization. The XML 1.0 attribute value
          // normalization always occurs though.
          parser->setFeature (XMLUni::fgDOMDatatypeNormalization, true);

          // Do not create EntityReference nodes in the DOM tree. No
          // EntityReference nodes will be created, only the nodes
          // corresponding to their fully expanded sustitution text will be
          // created.
          parser->setFeature (XMLUni::fgDOMEntities, false);

          // Perform Namespace processing.
          parser->setFeature (XMLUni::fgDOMNamespaces, true);

          // Perform Validation
          parser->setFeature (XMLUni::fgDOMValidation, true);

          // Do not include ignorable whitespace in the DOM tree.
          parser->setFeature (XMLUni::fgDOMWhitespaceInElementContent, false);

          // Enable the parser's schema support.
          parser->setFeature (XMLUni::fgXercesSchema, true);

          // Enable full schema constraint checking, including checking which
          // may be time-consuming or memory intensive. Currently, particle
          // unique attribution constraint checking and particle derivation
          // restriction checking are controlled by this option.
          parser->setFeature (XMLUni::fgXercesSchemaFullChecking, true);

          // The parser will treat validation error as fatal and will exit.
          parser->setFeature (XMLUni::fgXercesValidationErrorAsFatal, true);

          parser->setErrorHandler (&this->e_handler_);

          parser->setEntityResolver (&this->resolver_);

          std::auto_ptr<DOMDocument> doc (
            parser->parseURI (ACE_TEXT_ALWAYS_CHAR (url)));

          if (this->e_handler_.getErrors ())
            throw 0;

          return doc.release ();
        }
      catch (const DOMException& e)
        {
          return 0;
        }
      catch (const XMLException& e)
        {
          throw 0;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Caught an unknown exception \n"));
          throw 0;
        }

      return 0;
    }

    template <typename Resolver, typename Error>
    void
    XML_Helper_T<Resolver, Error>::terminate_parser (void)
    {
      try
        {
          xercesc::XMLPlatformUtils::Terminate();
        }
      catch (const XMLException&)
        {
          throw;
        }

      this->initialized_ = false;
      return;
    }

    template <typename Resolver, typename Error>
    bool
    XML_Helper_T<Resolver, Error>::write_DOM (
      XERCES_CPP_NAMESPACE::DOMDocument *doc,
      const ACE_TCHAR *file) const
    {
      if (!this->initialized_)
        return false;

      try
        {
          bool retn;
          std::auto_ptr<XERCES_CPP_NAMESPACE::DOMWriter> writer (impl_->createDOMWriter());

          if (writer->canSetFeature (XMLUni::fgDOMWRTFormatPrettyPrint,
                                     true))
            {
              writer->setFeature (XMLUni::fgDOMWRTFormatPrettyPrint, true);
            }

          std::auto_ptr <xercesc::XMLFormatTarget> ft (
            new xercesc::LocalFileFormatTarget(ACE_TEXT_ALWAYS_CHAR (file)));
          retn = writer->writeNode(ft.get (), *doc);
          return retn;
        }
      catch (const xercesc::XMLException &e)
        {
          char* message =
            XMLString::transcode (e.getMessage());
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);

          char* name =
            XMLString::transcode (e.getType());
          ACE_Auto_Basic_Array_Ptr<char> cleanup_name (name);

          ACE_ERROR ((LM_ERROR,
                      "Caught exception while serializing DOM to file.\n"
                      "Name: %C\n"
                      "Message: %C\n"
                      "SrcFile: %C\n"
                      "SrcLine: %d\n",
                      name,
                      message,
                      e.getSrcFile (),
                      e.getSrcLine ()));
          return false;
        }
    }
  }
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<CIAO::Config_Handlers::XML_Helper, ACE_Null_Mutex> *
ACE_Singleton<CIAO::Config_Handlers::XML_Helper, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
