// $Id$

#ifndef ACD_HANDLER_C
#define ACD_HANDLER_C

#include "ACD_Handler.h"
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "ACD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


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

  void ACD_Handler::process_AssemblyConnectionDescription ()
  {
    // This is bogus and should be replaced later.
    ACE_DECLARE_NEW_CORBA_ENV;

    for (DOMNode* node = this->iter_->nextNode();
         node != 0;
         node = this->iter_->nextNode())
      {
        XStr name (node->getNodeName());
        if (name == XStr (ACE_TEXT ("AssemblyConnectionDescription")))
          {
            ACE_NEW_THROW_EX (this->acd_,
                              ::Deployment::AssemblyConnectionDescription,
                              CORBA::NO_MEMORY());
          }
        else if (name == XStr (ACE_TEXT ("name")))
          {
            // Fetch the text node which contains the value
            node = this->iter_->nextNode();
            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
            this->process_name (text->getNodeValue());
          }
        else if (name == XStr (ACE_TEXT ("deployRequirement")))
          {
            Requirement_Handler handler (iter_, false); // Is the constructor right?
            handler.process_Requirement ();

            CORBA::ULong i (acd_->deployRequirement->length ());
            acd_->deployRequirement->length (i + 1);

            (*acd_->deployRequirement)[i] = handler.Requirement ();
          }
        else if (name == XStr (ACE_TEXT ("externalEndpoint")))
          {
            CEPE _Handler handler (iter_, false); // ComponentExternalPortEndpoint
            handler.process_ComponentExternalPortEndpoint ();

            CORBA::ULong i (acd_->externalEndpoint->length ());
            acd_->externalEndpoint->length (i + 1);

            (*acd_->externalEndpoint)[i] = handler.cepe ();
          }
        else if (name == XStr (ACE_TEXT ("internalEndpoint")))
          {
            SPE _Handler handler (iter_, false); // SubcomponentPortEndpoint
            handler.process_SubcomponentPortEndpoint ();

            CORBA::ULong i (acd_->internalEndpoint->length ());
            acd_->internalEndpoint->length (i + 1);

            (*acd_->internalEndpoint)[i] = handler.spe ();
          }
        else if (name == XStr (ACE_TEXT ("externalReference")))
          {
            ERE _Handler handler (iter_, false); // ExternalReferenceEndpoint
            handler.process_ExternalReferenceEndpoint ();

            CORBA::ULong i (acd_->externalReference->length ());
            acd_->externalReference->length (i + 1);

            (*acd_->externalReference)[i] = handler.ere ();
          }
        else
          {
            // ??? How did we get here ???
            ACE_THROW (CORBA::INTERNAL());
          }
      }
    return;
  }

  void SIP_Handler::process_name (const XMLCh* name)
  {
    if (name)
      {
        CORBA::String_var value (XMLString::transcode (name));
        this->acd_->name = value.in();
      }
  }
}

#endif /* ACD_HANDLER_C */
