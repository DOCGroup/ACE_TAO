// $Id$

#ifndef IDD_HANDLER_C
#define IDD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "MDD_Handler.h"
#include "IRDD_Handler.h"
#include "IDD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    IDD_Handler::IDD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    IDD_Handler::IDD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    IDD_Handler::~IDD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the instance deployment description and populate it
    void IDD_Handler::process_InstanceDeploymentDescription 
    (::Deployment::InstanceDeploymentDescription &idd)
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
              // Fetch the text node which contains the "name"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), idd);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              // Fetch the text node which contains the "node"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_node (text->getNodeValue(), idd);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              // Fetch the text node which contains the "source"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_source (text->getNodeValue(), idd);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (idd.configProperty.length ());
              idd.configProperty.length (i + 1);

              // delegate the populating process
              // @@ need to avoid this copy, possible memory leak @@
              idd.configProperty[i] = 
                *Property_Handler::process_Property (this->iter_);
            }
          else if (node_name == XStr (ACE_TEXT ("implementation")))
            {
              // fetch the MonolithicDeploymentDescription handler
              MDD_Handler mdd_handler (this->iter_, false);
              
              // delegate the populating process
              mdd_handler.process_MonolithicDeploymentDescription (idd.implementation);
            }
          else if (node_name == XStr (ACE_TEXT ("deployedResource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (idd.deployedResource.length ());
              idd.deployedResource.length (i + 1);

              // fetch the InstanceResourceDeployment Description
              // handler
              IRDD_Handler irdd_handler (this->iter_, false);
              
              // delegate the populating process
              irdd_handler.process_InstanceResourceDeploymentDescription (idd.deployedResource[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("deployedSharedResource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (idd.deployedSharedResource.length ());
              idd.deployedSharedResource.length (i + 1);

              // fetch the InstanceResourceDeployment Description
              // handler
              IRDD_Handler irdd_handler (this->iter_, false);
              
              // delegate the populating process
              irdd_handler.process_InstanceResourceDeploymentDescription (idd.deployedSharedResource[i]);
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
    void IDD_Handler::process_name (const XMLCh* name, 
                                    ::Deployment::InstanceDeploymentDescription &idd)
    {
      if (name)
        {
          idd.name = XMLString::transcode (name);
        }
    }

    /// handle node attribute
    void IDD_Handler::process_node (const XMLCh* node, 
                                    ::Deployment::InstanceDeploymentDescription &idd)
    {
      if (node)
        {
          idd.node = XMLString::transcode (node);
        }
    }

    /// handle source attribute
    void IDD_Handler::process_source (const XMLCh* source, 
                                      ::Deployment::InstanceDeploymentDescription &idd)
    {
      if (source)
        {
          // first increment the length of the sequence
          CORBA::ULong i (idd.source.length ());
          idd.source.length (i+1);
          // insert the new source
          idd.source[i] = XMLString::transcode (source);
        }
    }

  }
}

#endif /* IDD_HANDLER_C */

