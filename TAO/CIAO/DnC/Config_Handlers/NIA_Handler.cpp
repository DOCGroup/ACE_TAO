//==================================================================
/**
 *  @file  NIA_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IAD_Handler.h"
#include "NIA_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    NIA_Handler::NIA_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    NIA_Handler::NIA_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    NIA_Handler::~NIA_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void NIA_Handler::process_NamedImplementationArtifact
      (::Deployment::NamedImplementationArtifact &nia)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the "name"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), nia);
	    }
	  else if (node_name == XStr (ACE_TEXT ("referencedArtifact")))
            {
              // fetch the component package reference handler
              IAD_Handler iad_handler (iter_, false);

              // delegate the populating process
              iad_handler.process_ImplementationArtifactDescription (nia.referencedArtifact);
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
    void NIA_Handler::process_name
      (const XMLCh* name, ::Deployment::NamedImplementationArtifact &nia)
    {
      if (name)
        {
          nia.name = XMLString::transcode (name);
        }
    }

  }  // namespace Config_Handler
}  // namespace CIAO
