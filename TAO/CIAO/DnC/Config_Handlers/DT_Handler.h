
//==================================================================
/**
 *  @file  DT_Handler.h
 *
 *  $Id$ 
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef DT_HANDLER_H
#define DT_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include "XercesString.h"

using Config_Handler::XStr;
using xercesc::XMLString;
using xercesc::DOMImplementation;
using xercesc::DOMText;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;

namespace CIAO
{
  namespace Config_Handler
  {
    /**
     * @class DT_Handler
     *
     * @brief Handler class for <type> tag
     *
     * This class defines handler methods to parse the aforementioned type
     * in the descriptor files. The corresponding CORBA IDL mapped type in this
     * a CORBA::Typecode is created and returned. 
     */

    class Config_Handler_Export DT_Handler
    {
    public:
      static void process_DataType (DOMNodeIterator * iter, 
                                    ::CORBA::TypeCode_ptr type);
      // process <type> definitions in the descriptor files

    };
  }
}

#include /**/ "ace/post.h"

#endif /* DT_HANDLER_H */
