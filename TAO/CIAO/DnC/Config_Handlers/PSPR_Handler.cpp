// $Id$

#ifndef PSPR_HANDLER_C
#define PSPR_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IDD_Handler.h"
#include "PSPR_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    PSPR_Handler::PSPR_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    PSPR_Handler::PSPR_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PSPR_Handler::~PSPR_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the plan subcomponent property reference and populate it
    void PSPR_Handler::process_PlanSubcomponentPropertyReference 
    (::Deployment::PlanSubcomponentPropertyReference &pspr)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("propertyName")))
            {
              // Fetch the text node which contains the "propertyName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_propertyName (text->getNodeValue(), pspr);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              // fetch the InstanceDeploymentDescription handler
              IDD_Handler idd_handler (this->iter_, false);
              
              // delegate the populating process
              idd_handler.process_InstanceDeploymentDescription (pspr.instance);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle  attribute
    void PSPR_Handler::process_propertyName (const XMLCh* name, 
                                    ::Deployment::PlanSubcomponentPropertyReference &pspr)
    {
      if (name)
        {
          pspr.propertyName = XMLString::transcode (name);
        }
    }

  }
}

#endif /* PSPR_HANDLER_C */

