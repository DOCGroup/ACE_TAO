// $Id$

#ifndef ADD_HANDLER_C
#define ADD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "RDD_Handler.h"
#include "ADD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    ADD_Handler::ADD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    ADD_Handler::ADD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    ADD_Handler::~ADD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the deployment plan and populate it
    void ADD_Handler::process_ArtifactDeploymentDescription 
    (::Deployment::ArtifactDeploymentDescription &add)
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
              this->process_name (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("location")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_location (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              // Fetch the text node which contains the "node"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_node (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              // Fetch the text node which contains the "source"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_source (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("execParameter")))
            {
              // increase the length of the sequence
              CORBA::ULong i (add.execParameter.length ());
              add.execParameter.length (i + 1);

              // delegate the populating process
              // @@ need to avoid this copy, possible memory leak @@
              add.execParameter[i] = *Property_Handler::process_Property (this->iter_);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              // increase the length of the sequence
              CORBA::ULong i (add.deployRequirement.length ());
              add.deployRequirement.length (i + 1);

              // delegate the populating process
              // @@ need to avoid this copy, possible memory leak @@
              add.deployRequirement[i] = 
                *Requirement_Handler::process_Requirement (this->iter_);
            }
          else if (node_name == XStr (ACE_TEXT ("deployedResource")))
            {
              // @@Issue 6392@@
              // fetch the resource deployment description handler
              RDD_Handler rdd_handler (iter_, false); 

              // increase the length of the sequence
              CORBA::ULong i (add.deployedResource.length ());
              add.deployedResource.length (i + 1);

              // delegate the populating process
              rdd_handler.process_ResourceDeploymentDescription (add.deployedResource[i]);
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
    void ADD_Handler::process_name (const XMLCh* name, 
                                    ::Deployment::ArtifactDeploymentDescription &add)
    {
      if (name)
        {
          add.name = XMLString::transcode (name);
        }
    }

    /// handle location attribute
    void ADD_Handler::process_location (const XMLCh* location, 
                                        ::Deployment::ArtifactDeploymentDescription &add)
    {
      if (location)
        {
          // first increment the length of the sequence
          CORBA::ULong i (add.location.length ());
          add.location.length (i+1);
          // insert the new location
          add.location[i] = XMLString::transcode (location);
        }
    }

    /// handle node attribute
    void ADD_Handler::process_node (const XMLCh* node, 
                                    ::Deployment::ArtifactDeploymentDescription &add)
    {
      if (node)
        {
          add.node = XMLString::transcode (node);
        }
    }

    /// handle source attribute
    void ADD_Handler::process_source (const XMLCh* source, 
                                      ::Deployment::ArtifactDeploymentDescription &add)
    {
      if (source)
        {
          // first increment the length of the sequence
          CORBA::ULong i (add.source.length ());
          add.source.length (i+1);
          // insert the new source
          add.source[i] = XMLString::transcode (source);
        }
    }

  }
}

#endif /* ADD_HANDLER_C */

