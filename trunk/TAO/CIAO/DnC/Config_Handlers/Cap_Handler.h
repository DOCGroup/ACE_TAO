//==================================================================
/**
 *  @file  Cap_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef CAP_HANDLER_H
#define CAP_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "tao/Exception.h"
#include "XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLUri;
using xercesc::XMLURL;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMInputSource;
using xercesc::DOMText;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;
using xercesc::DOMNamedNodeMap;

namespace CIAO
{
  namespace Config_Handler
  {
    /**
     * @class CAP_Handler
     *
     * @brief Handler class for <Capability> type
     *
     * This class defines handler methods to parse Capability
     * types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export CAP_Handler
    {
    public:

     /// constructor
      CAP_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      CAP_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~CAP_Handler(void);
      
     void process_Capability (DOMNodeIterator * iter,
			      Deployment::Capability &ret_struct);
      // process elements of type Capability definitions in
      // the descriptor files

     /// Process the attributes for the property
      void process_attributes_for_satisfier_property (DOMNamedNodeMap* nm,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int length,
        Deployment::SatisfierProperty& pro);

      /// create a document
      DOMDocument* create_document (const char *url);

    private:
      typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
      typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
      typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;

      DOMDocumentTraversal* traverse_;

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;
      int index_;

      REF_MAP id_map_;
      IDREF_MAP idref_map_;

    };
  }
}

#include /**/ "ace/post.h"

#endif /* ID_HANDLER_H */
