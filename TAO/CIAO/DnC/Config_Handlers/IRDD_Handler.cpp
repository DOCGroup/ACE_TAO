// $Id$

#ifndef IRDD_HANDLER_C
#define IRDD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Any_Handler.h"
#include "IRDD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    IRDD_Handler::IRDD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    IRDD_Handler::IRDD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    IRDD_Handler::~IRDD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the instance resource deployment description and populate it
    void IRDD_Handler::process_InstanceResourceDeploymentDescription
    (::Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("resourceUsage")))
            {
              // Fetch the enum node which contains the "resource usage"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resourceUsage (text->getNodeValue(), irdd);
            }
          else if (node_name == XStr (ACE_TEXT ("requirementName")))
            {
              // Fetch the text node which contains the "requirementName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_requirementName (text->getNodeValue(), irdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceName")))
            {
              // Fetch the text node which contains the "resourceName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resourceName (text->getNodeValue(), irdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
            {
              // delegate the populating process
              // @@should avoid this copy - Andy @@
              // Arvind: Resolved
              Any_Handler::process_Any (this->iter_, irdd.resourceValue);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle resource usage attribute
    void IRDD_Handler::process_resourceUsage (const XMLCh* val,
                                              ::Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      if (val)
        {
          // we need to convert the string to an enum
          CORBA::String_var temp = XMLString::transcode (val);
          CORBA::Short sval = ACE_static_cast (CORBA::Short,
                                              ACE_OS::strtol (temp.in
                                              (), 0, 10));
          // now cast it to enum
          irdd.resourceUsage =
            ACE_static_cast (::Deployment::ResourceUsageKind,
                             sval);
        }
    }

    /// handle requirement name attribute
    void IRDD_Handler::process_requirementName (const XMLCh* name,
                                               ::Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      if (name)
        {
          irdd.requirementName = XMLString::transcode (name);
        }
    }

    /// handle resource name attribute
    void IRDD_Handler::process_resourceName (const XMLCh* name,
                                            ::Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      if (name)
        {
          irdd.resourceName = XMLString::transcode (name);
        }
    }


  }
}

#endif /* IRDD_HANDLER_C */
