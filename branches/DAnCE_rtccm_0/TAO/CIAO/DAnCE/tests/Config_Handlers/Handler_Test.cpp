// $Id$
#ifndef HANDLER_TEST_CPP
#define HANDLER_TEST_CPP

#include "Handler_Test.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/Auto_Ptr.h"
#include "tao/Exception.h"
#include "Config_Handlers/XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include "Config_Handlers/Config_Handler_export.h"
#include "Config_Handlers/DnC_Dump.h"
#include "Config_Handlers/Config_Error_Handler.h"

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMAttr;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMNodeList;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;

namespace Config_Handler_Test 
{
  void
  usage (const ACE_TCHAR* program)
  {
    ACE_ERROR ((LM_ERROR,
		ACE_TEXT ("Usage: %s -u <URI>\n")
		ACE_TEXT (" <URI>: URI identifying the resource\n"),
		program));
  }
  
  template <typename HANDLER, typename DATA>
  int run_test (int argc, ACE_TCHAR *argv[], void (HANDLER::*func) (DATA&))
  {
    ACE_TCHAR* url = 0;
    
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("u:"));
    int c;
    
    while ((c = get_opt ()) != EOF)
      {
	switch (c)
	  {
          case 'u':
            url = get_opt.opt_arg();
            break;
          default:
            usage(argv[0]);
            return -1;
	  }
      }
    
    if (url == 0) {
      usage (argv[0]);
      return -1;
    }
    
    // Initialize the ORB so that CORBA::Any will work
    //
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

    // Initialize the Xerces run-time
    try
    {
      xercesc::XMLPlatformUtils::Initialize();
    }
    
    catch (const XMLException& e)
      {
	char* message = XMLString::transcode (e.getMessage());
	ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
	ACE_DEBUG ((LM_DEBUG, "Error during initialization : %s\n", message));
	return 1;
      }
    
    try
      {
	
	// Instantiate the DOM parser.
	static const XMLCh gLS[] = { xercesc::chLatin_L,
				     xercesc::chLatin_S,
				     xercesc::chNull };
	
	// Get an implementation of the Load-Store (LS) interface
	DOMImplementation* impl
	  = DOMImplementationRegistry::getDOMImplementation(gLS);
	
	auto_ptr<DOMImplementation> cleanup_impl (impl);
	
	// Create a DOMBuilder
	DOMBuilder* parser =
	  ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	auto_ptr<DOMBuilder> cleanup_builder (parser);
	
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
	
	// Enable the parser schema support.
	parser->setFeature (XMLUni::fgXercesSchema, true);
	
	// Enable full schema constraint checking, including checking which
	// may be time-consuming or memory intensive. Currently, particle
	// unique attribution constraint checking and particle derivation
	// restriction checking are controlled by this option.
	parser->setFeature (XMLUni::fgXercesSchemaFullChecking, true);
	
	// The parser will treat validation error as fatal and will exit.
	parser->setFeature (XMLUni::fgXercesValidationErrorAsFatal, true);
	
	CIAO::Config_Handler::Config_Error_Handler handler;
	parser->setErrorHandler(&handler);
	
	DOMDocument* doc = parser->parseURI(url);
	auto_ptr<DOMDocument> cleanup_doc (doc);
	
	if (handler.getErrors())
	  {
	    return 1;
	  }
	HANDLER obj (doc, DOMNodeFilter::SHOW_ELEMENT |
		     DOMNodeFilter::SHOW_TEXT);
	
	DATA data;
	(obj.*func)(data);
	Deployment::DnC_Dump::dump (data);
	//domain_handler.dump(domain);
	
	// parser->release ();
      }
    catch (const DOMException& e)
      {
	const unsigned int maxChars = 2047;
	XMLCh errText[maxChars + 1];
	
	ACE_ERROR ((LM_ERROR, 
		    "\nException occured while parsing %s: \n", url));
	ACE_ERROR ((LM_ERROR, 
		    "DOMException code: %d\n ", e.code));
	if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
	  {
	    char* message = XMLString::transcode (errText);
	    ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
	    ACE_ERROR ((LM_ERROR, "Message is: %s\n", message));
	  }
	return 1;
      }
    catch (const XMLException& e)
      {
	char* message = XMLString::transcode (e.getMessage());
	ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
	ACE_ERROR ((LM_ERROR, "\nException occured: %s\n ", message));
	return 1;
      }
    catch (CORBA::Exception& ex)
      {
	ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
	return 1;
      }
    catch (...)
      {
	ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
	return 1;
      }
    
    xercesc::XMLPlatformUtils::Terminate();
    return 0;
  }
}

#endif /*HANDLER_TEST_CPP*/
