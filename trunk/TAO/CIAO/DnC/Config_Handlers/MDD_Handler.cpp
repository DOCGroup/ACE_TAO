// $Id$

#ifndef MDD_HANDLER_C
#define MDD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "ADD_Handler.h"
#include "MDD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    MDD_Handler::MDD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    MDD_Handler::MDD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    MDD_Handler::~MDD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the deployment plan and populate it
    void MDD_Handler::process_MonolithicDeploymentDescription 
    (::Deployment::MonolithicDeploymentDescription &mdd)
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
              this->process_name (text->getNodeValue(), mdd);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              // Fetch the text node which contains the "source"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_source (text->getNodeValue(), mdd);
            }
          else if (node_name == XStr (ACE_TEXT ("execParameter")))
            {
              // increase the length of the sequence
              CORBA::ULong i (mdd.execParameter.length ());
              mdd.execParameter.length (i + 1);

              // delegate the populating process
              // @@ need to avoid this copy, possible memory leak @@
              mdd.execParameter[i] = 
                *Property_Handler::process_Property (this->iter_);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              // increase the length of the sequence
              CORBA::ULong i (mdd.deployRequirement.length ());
              mdd.deployRequirement.length (i + 1);

              // delegate the populating process
              // @@ need to avoid this copy, possible memory leak @@
              mdd.deployRequirement[i] = 
                *Requirement_Handler::process_Requirement (this->iter_);
            }
          else if (node_name == XStr (ACE_TEXT ("artifact")))
            {
              // @@Issue 5957@@
              // fetch the artifact deployment description handler
              ADD_Handler add_handler (iter_, false); 

              // increase the length of the sequence
              CORBA::ULong i (mdd.artifact.length ());
              mdd.artifact.length (i + 1);

              // delegate the populating process
              add_handler.process_ArtifactDeploymentDescription (mdd.artifact[i]);
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
    void MDD_Handler::process_name (const XMLCh* name, 
                                    ::Deployment::MonolithicDeploymentDescription &mdd)
    {
      if (name)
        {
          mdd.name = XMLString::transcode (name);
        }
    }

    /// handle source attribute
    void MDD_Handler::process_source (const XMLCh* source, 
                                      ::Deployment::MonolithicDeploymentDescription &mdd)
    {
      if (source)
        {
          // first increment the length of the sequence
          CORBA::ULong i (mdd.source.length ());
          mdd.source.length (i+1);
          // insert the new source
          mdd.source[i] = XMLString::transcode (source);
        }
    }

  }
}

#endif /* MDD_HANDLER_C */

