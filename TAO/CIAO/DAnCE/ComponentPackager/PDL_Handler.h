//==================================================================
/**
 *  @file  PDL_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef PDL_HANDLER_H
#define PDL_HANDLER_H
#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include <list>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>

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
  namespace Component_Packager
  {
  /**
   * @class PDL_Handler
   *
   * @brief Handler class for PackageDescriptionList
   *
   * This class defines handler methods to parse the
   * PackageDescriptionList descriptor file
   */
  class PDL_Handler
  {
  public:
    typedef std::list<ACE_TString>  DESC_LIST;
    /// constructor
    PDL_Handler (DOMDocument* pdl_doc,
                unsigned long filter);

    /// Process the pdl and get the descriptors list
    void process_pdl (DESC_LIST &desc_list);

    /// Retrieve the node value
    void get_node_value (DOMNodeIterator *iter,
                         ACE_TString &value);
  protected:
    DOMDocumentTraversal* traverse_;

    DOMDocument* pdl_doc_;

    DOMNode* root_;

    unsigned long filter_;

    DOMNodeIterator* iter_;
  };

  }
}

#include /**/ "ace/post.h"

#endif /* PDL_HANDLER_H */
