// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Requirement_Handler.h"
#include "ERE_Handler.h"
#include "CEPE_Handler.h"
#include "CRDD_Handler.h"
#include "PSPE_Handler.h"
#include "PCD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    PCD_Handler::PCD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    PCD_Handler::PCD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PCD_Handler::~PCD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the plan connection description and populate it
    void PCD_Handler::process_PlanConnectionDescription
    (::Deployment::PlanConnectionDescription &pcd)
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
              this->process_name (text->getNodeValue(), pcd);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              // Fetch the text node which contains the "source"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_source (text->getNodeValue(), pcd);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pcd.deployRequirement.length ());
              pcd.deployRequirement.length (i + 1);

              // delegate the populating process
              // @@ need to avoid this copy, possible memory leak @@
              // Arvind: Addressed
              Requirement_Handler::process_Requirement (this->iter_,
                                                        pcd.deployRequirement[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("deployedResource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pcd.deployedResource.length ());
              pcd.deployedResource.length (i + 1);

              // fetch the ConnectionResourceDeployment Description
              // handler
              CRDD_Handler crdd_handler (this->iter_, false);

              // delegate the populating process
              crdd_handler.process_ConnectionResourceDeploymentDescription (pcd.deployedResource[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("internalEndpoint")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pcd.internalEndpoint.length ());
              pcd.internalEndpoint.length (i + 1);

              // fetch the PlanSubcomponentPortEndpoint handler
              PSPE_Handler pspe_handler (this->iter_, false);

              // delegate the populating process
              pspe_handler.process_PlanSubcomponentPortEndpoint (pcd.internalEndpoint[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("externalReference")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pcd.externalReference.length ());
              pcd.externalReference.length (i + 1);

              // fetch the ExternalReferenceEndpoint handler and
              // delegate to it
              // @@ need to avoid this copy, possible memory leak @@
              // Arvind: Addressed the same.
              ERE_Handler::process_ExternalReferenceEndpoint (this->iter_,
                                                              pcd.externalReference[i]);

            }
          else if (node_name == XStr (ACE_TEXT ("externalEndpoint")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pcd.externalEndpoint.length ());
              pcd.externalEndpoint.length (i + 1);

              // fetch the ComponentExternalPortEndpoint handler and
              // delegate to it
              // @@ need to avoid this copy, possible memory leak @@
              // Arvind: Addressed
              CEPE_Handler::
                process_ComponentExternalPortEndpoint
                (this->iter_, pcd.externalEndpoint[i]);

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
    void PCD_Handler::process_name (const XMLCh* name,
                                    ::Deployment::PlanConnectionDescription &pcd)
    {
      if (name)
        {
          pcd.name = XMLString::transcode (name);
        }
    }

    /// handle source attribute
    void PCD_Handler::process_source (const XMLCh* source,
                                      ::Deployment::PlanConnectionDescription &pcd)
    {
      if (source)
        {
          // first increment the length of the sequence
          CORBA::ULong i (pcd.source.length ());
          pcd.source.length (i+1);
          // insert the new source
          pcd.source[i] = XMLString::transcode (source);
        }
    }

  }
}
