// $Id$

#ifndef ACD_HANDLER_C
#define ACD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Requirement_Handler.h"
#include "CEPE_Handler.h"
#include "ERE_Handler.h"
#include "SPE_Handler.h"
#include "ACD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    ACD_Handler::ACD_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    ACD_Handler::ACD_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    ACD_Handler::~ACD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    void ACD_Handler::process_AssemblyConnectionDescription
    (::Deployment::AssemblyConnectionDescription &acd)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr name (node->getNodeName());
          if (name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the value
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), acd);
            }
          else if (name == XStr (ACE_TEXT ("deployRequirement")))
            {
              // increase sequence length
              CORBA::ULong i (acd.deployRequirement.length ());
              acd.deployRequirement.length (i + 1);

              // delegate to requirement handler
              Requirement_Handler::process_Requirement (this->iter_, acd.deployRequirement[i]);
            }
          else if (name == XStr (ACE_TEXT ("externalEndpoint")))
            {
              CORBA::ULong i (acd.externalEndpoint.length ());
              acd.externalEndpoint.length (i + 1);

              // delegate to CEPE handler
              CEPE_Handler::process_ComponentExternalPortEndpoint (this->iter_, acd.externalEndpoint[i]);
            }
          else if (name == XStr (ACE_TEXT ("internalEndpoint")))
            {
              CORBA::ULong i (acd.internalEndpoint.length ());
              acd.internalEndpoint.length (i + 1);

              // fetch SPE handler and delegate to it
              SPE_Handler spe_handler (iter_, false); // SubcomponentPortEndpoint
              spe_handler.process_SubcomponentPortEndpoint (acd.internalEndpoint[i]);
            }
          else if (name == XStr (ACE_TEXT ("externalReference")))
            {
              CORBA::ULong i (acd.externalReference.length ());
              acd.externalReference.length (i + 1);

              // delegate to ERE handler
              ERE_Handler::process_ExternalReferenceEndpoint
              (this->iter_, acd.externalReference[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    void ACD_Handler::process_name (const XMLCh* name, ::Deployment::AssemblyConnectionDescription &acd)
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          acd.name = value.in();
        }
    }
  }
}

#endif /* ACD_HANDLER_C */
