// $Id$

#ifndef DP_HANDLER_C
#define DP_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "MDD_Handler.h"
#include "IDD_Handler.h"
#include "PCD_Handler.h"
#include "PPM_Handler.h"
#include "ID_Handler.h"
#include "ADD_Handler.h"
#include "DP_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    DP_Handler::DP_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    DP_Handler::DP_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    DP_Handler::~DP_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the deployment plan and populate it
    void DP_Handler
    ::process_DeploymentPlan (::Deployment::DeploymentPlan &dp)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "label"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), dp);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), dp);
            }
          else if (node_name == XStr (ACE_TEXT ("realizes")))
            {
              // fetch the component interface description handler
              CompIntrDesc_Handler handler (iter_, false);

              // delegate the populating process
              handler.process_ComponentInterfaceDescription (dp.realizes);

            }
          else if (node_name == XStr (ACE_TEXT ("implementation")))
            {
              // fetch the monolithic deployment description handler
              MDD_Handler mdd_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (dp.implementation.length ());
              dp.implementation.length (i + 1);

              // delegate the populating process
              mdd_handler.process_MonolithicDeploymentDescription (dp.implementation[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              // fetch the instance deployment description handler
              IDD_Handler idd_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (dp.instance.length ());
              dp.instance.length (i + 1);

              // delegate the populating process
              idd_handler.process_InstanceDeploymentDescription (dp.instance[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              // fetch the plan connection description handler
              PCD_Handler pcd_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (dp.connection.length ());
              dp.connection.length (i + 1);

              // delegate the populating process
              pcd_handler.process_PlanConnectionDescription (dp.connection[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("externalProperty")))
            {
              // fetch the plan property mapping handler
              PPM_Handler ppm_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (dp.externalProperty.length ());
              dp.externalProperty.length (i + 1);

              // delegate the populating process
              ppm_handler.process_PlanPropertyMapping (dp.externalProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("dependsOn")))
            {
              // increase the length of the sequence
              CORBA::ULong i (dp.dependsOn.length ());
              dp.dependsOn.length (i + 1);

              // delegate the populating process
              ID_Handler::process_ImplementationDependency
              (this->iter_, dp.dependsOn[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("artifact")))
            {
              // fetch the artifact deployment description handler
              ADD_Handler add_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (dp.artifact.length ());
              dp.artifact.length (i + 1);

              // delegate the populating process
              add_handler.process_ArtifactDeploymentDescription (dp.artifact[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (dp.infoProperty.length ());
              dp.infoProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_, dp.infoProperty[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle label attribute
    void DP_Handler::process_label (const XMLCh* label,
                                    ::Deployment::DeploymentPlan &dp)
    {
      if (label)
        {
          dp.label = XMLString::transcode (label);
        }
    }

    /// handle UUID attribute
    void DP_Handler::process_UUID (const XMLCh* UUID,
                                   ::Deployment::DeploymentPlan &dp)
    {
      if (UUID)
        {
          dp.UUID = XMLString::transcode (UUID);
        }
    }

  }
}

#endif /* DP_HANDLER_C */
