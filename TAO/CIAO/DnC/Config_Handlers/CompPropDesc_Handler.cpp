// $Id$

#ifndef COMPPROPDESC_HANDLER_C
#define COMPPROPDESC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IAD_Handler.h"
#include "CompPropDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    CompPropDesc_Handler::CompPropDesc_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CompPropDesc_Handler::CompPropDesc_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CompPropDesc_Handler::~CompPropDesc_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void CompPropDesc_Handler::process_ComponentPropertyDescription
      (::Deployment::ComponentPropertyDescription &comppropdesc)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the "label"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), comppropdesc);
	    }
	  else if (node_name == XStr (ACE_TEXT ("type")))
            {
	      // TODO: How to implement this?
	      // DataType ???
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle name attribute
    void CompPropDesc_Handler::process_name
      (const XMLCh* name, ::Deployment::ComponentPropertyDescription &comppropdesc)
    {
      if (name)
        {
          comppropdesc.name = XMLString::transcode (name);
        }
    }

  }
}

#endif /* COMPPROPDESC_HANDLER_C */
