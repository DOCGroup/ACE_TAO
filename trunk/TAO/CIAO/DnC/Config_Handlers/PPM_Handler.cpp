// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "PSPR_Handler.h"
#include "PPM_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    PPM_Handler::PPM_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    PPM_Handler::PPM_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PPM_Handler::~PPM_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the plan property mapping and populate it
    void PPM_Handler::process_PlanPropertyMapping 
    (::Deployment::PlanPropertyMapping &ppm)
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
              this->process_name (text->getNodeValue(), ppm);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              // Fetch the text node which contains the "source"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_source (text->getNodeValue(), ppm);
            }
          else if (node_name == XStr (ACE_TEXT ("externalName")))
            {
              // Fetch the text node which contains the "externalName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_externalName (text->getNodeValue(), ppm);
            }
          else if (node_name == XStr (ACE_TEXT ("delegatesTo")))
            {
              // increase the length of the sequence
              CORBA::ULong i (ppm.delegatesTo.length ());
              ppm.delegatesTo.length (i + 1);

              // fetch the PlanSubcomponentPropertyReference handler
              PSPR_Handler pspr_handler (this->iter_, false);
              
              // delegate the populating process
              pspr_handler.process_PlanSubcomponentPropertyReference (ppm.delegatesTo[i]);
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
    void PPM_Handler::process_name (const XMLCh* name, 
                                    ::Deployment::PlanPropertyMapping &ppm)
    {
      if (name)
        {
          ppm.name = XMLString::transcode (name);
        }
    }

    /// handle external name attribute
    void PPM_Handler::process_externalName (const XMLCh* name, 
                                            ::Deployment::PlanPropertyMapping &ppm)
    {
      if (name)
        {
          ppm.externalName = XMLString::transcode (name);
        }
    }

    /// handle source attribute
    void PPM_Handler::process_source (const XMLCh* source, 
                                      ::Deployment::PlanPropertyMapping &ppm)
    {
      if (source)
        {
          // first increment the length of the sequence
          CORBA::ULong i (ppm.source.length ());
          ppm.source.length (i+1);
          // insert the new source
          ppm.source[i] = XMLString::transcode (source);
        }
    }

  }
}
