// $Id$

#ifndef HANDLER_TEST_H
#define HANDLER_TEST_H

#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "tao/Exception.h"
#include "Config_Handlers/XercesString.h"
#include "Config_Handlers/Config_Handler_export.h"
#include "Config_Handlers/DnC_Dump.h"
#include "Config_Handlers/Config_Error_Handler.h"

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/dom/DOM.hpp>

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
}


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Handler_Test.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Handler_Test.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif // HANDLER_TEST_H
