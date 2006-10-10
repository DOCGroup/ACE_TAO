//$Id$
#include "XML_Helper.h"
#include "ace/Auto_Ptr.h"
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

    XML_Helper::XML_Helper (void)
      : initialized_ (false)
    {
      this->init_parser ();
    }

    XML_Helper::~XML_Helper (void)
    {
      this->terminate_parser ();
    }

    bool
    XML_Helper::is_initialized (void) const
    {
      return this->initialized_ == true;
    }

    void
    XML_Helper::init_parser (void)
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
      catch (const XMLException& e)
        {
          char* message =
            XMLString::transcode (e.getMessage());
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);

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

    XERCES_CPP_NAMESPACE::DOMDocument *
    XML_Helper::create_dom (const ACE_TCHAR *root,
                            const ACE_TCHAR *ns)
    {
      if (root == 0 || ns == 0)
        return 0;

      return this->impl_->createDocument (XStr (ns),
                                          XStr (root),
                                          0);
    }

    XERCES_CPP_NAMESPACE::DOMDocument *
    XML_Helper::create_dom (const ACE_TCHAR *url)
    {

      if (url == 0)
        return impl_->createDocument(
                                     XStr ("http://www.omg.org/DeploymentPlan"),
                                     XStr ("deploymentPlan"),
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

          XML_Error_Handler handler;

          parser->setErrorHandler (&handler);

          CIAO_Schema_Resolver resolver;

          parser->setEntityResolver (&resolver);

          DOMDocument* doc = parser->parseURI (url);

          if (handler.getErrors ())
            throw 0;

          return doc;
        }
      catch (const DOMException& e)
        {
          const unsigned int maxChars = 2047;
          XMLCh errText[maxChars + 1];

          if (DOMImplementation::loadDOMExceptionMsg (e.code,
                                                      errText,
                                                      maxChars))
            {
              char* message =
                XMLString::transcode (errText);
              ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
            }
          return 0;

        }
      catch (const XMLException& e)
        {
          char* message = XMLString::transcode (e.getMessage());
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
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

    void
    XML_Helper::terminate_parser (void)
    {
      try
        {
          xercesc::XMLPlatformUtils::Terminate();
        }
      catch (const XMLException& e)
        {
          char* message =
            XMLString::transcode (e.getMessage());
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
          throw;
        }

      this->initialized_ = false;
      return;
    }

    bool XML_Helper::write_DOM (XERCES_CPP_NAMESPACE::DOMDocument *doc,
                                const ACE_TCHAR *file)
    {
      bool retn;
      XERCES_CPP_NAMESPACE::DOMWriter *writer = impl_->createDOMWriter();

      if (writer->canSetFeature (XMLUni::fgDOMWRTFormatPrettyPrint,
                                 true))
        writer->setFeature (XMLUni::fgDOMWRTFormatPrettyPrint, true);

      xercesc::XMLFormatTarget* ft (new xercesc::LocalFileFormatTarget(file));
      retn = writer->writeNode(ft, *doc);
      delete writer;
      delete ft;
      return retn;
    }
  }
}
#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton< CIAO::Config_Handlers::XML_Helper, ACE_Null_Mutex> *
ACE_Singleton<CIAO::Config_Handlers::XML_Helper, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */


