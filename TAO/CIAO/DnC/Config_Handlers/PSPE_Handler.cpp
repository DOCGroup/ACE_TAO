// $Id$

#ifndef PSPE_HANDLER_C
#define PSPE_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IDD_Handler.h"
#include "PSPE_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    PSPE_Handler::PSPE_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    PSPE_Handler::PSPE_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PSPE_Handler::~PSPE_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the plan subcomponent port endpoint and populate it
    void PSPE_Handler::process_PlanSubcomponentPortEndpoint 
    (::Deployment::PlanSubcomponentPortEndpoint &pspe)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("portName")))
            {
              // Fetch the text node which contains the "portName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_portName (text->getNodeValue(), pspe);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              // fetch the InstanceDeploymentDescription handler
              IDD_Handler idd_handler (this->iter_, false);
              
              // delegate the populating process
              idd_handler.process_InstanceDeploymentDescription (pspe.instance);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle port name attribute
    void PSPE_Handler::process_portName (const XMLCh* name, 
                                         ::Deployment::PlanSubcomponentPortEndpoint &pspe)
    {
      if (name)
        {
          pspe.portName = XMLString::transcode (name);
        }
    }

    /// handle  provider attribute
    void PSPE_Handler::process_provider (const XMLCh* val, 
                                         ::Deployment::PlanSubcomponentPortEndpoint &pspe)
    {
      if (val)
        {
          // we need to cast it to boolean
          CORBA::String_var temp = XMLString::transcode (val);
          pspe.provider = ACE_static_cast (CORBA::Boolean, 
                                           ACE_OS::strtol (temp.in (), 0, 10));
        }
    }

  }
}

#endif /* PSPE_HANDLER_C */

