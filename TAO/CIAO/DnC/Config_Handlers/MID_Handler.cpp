// $Id$

#ifndef MID_HANDLER_C
#define MID_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "NIA_Handler.h"
#include "MID_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    MID_Handler::MID_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    MID_Handler::MID_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    MID_Handler::~MID_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    void MID_Handler::process_MonolithicImplementationDescription (::Deployment::MonolithicImplementationDescription &mid)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr name (node->getNodeName());
          if (name == XStr (ACE_TEXT ("execParameter")))
            {
              // increase length of sequence
              CORBA::ULong i (mid.execParameter.length ());
              mid.execParameter.length (i + 1);

              // delegate to property handler
              Property_Handler::process_Property (this->iter_, 
                                                  mid.execParameter[i]);
            }
          else if (name == XStr (ACE_TEXT ("deployRequirement")))
            {

              // increase length of sequence
              CORBA::ULong i (mid.deployRequirement.length ());
              mid.deployRequirement.length (i + 1);

              // delegate to requirement handler
              Requirement_Handler::process_Requirement (this->iter_,
                                                        mid.deployRequirement[i]);
            }
          else if (name == XStr (ACE_TEXT ("primaryArtifact")))
            {

              // increase length
              CORBA::ULong i (mid.primaryArtifact.length ());
              mid.primaryArtifact.length (i + 1);

              // delegate to NIA handler
              NIA_Handler nia_handler (iter_, false); // NamedImplementationArtifact
              nia_handler.process_NamedImplementationArtifact (mid.primaryArtifact[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }
  }
}


#endif /* MID_HANDLER_C */
