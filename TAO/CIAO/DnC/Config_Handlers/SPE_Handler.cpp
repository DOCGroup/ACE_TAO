// $Id$

#ifndef SPE_HANDLER_C
#define SPE_HANDLER_C

#include "SPE_Handler.h"
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "SPE_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace Config_Handler
{
  SPE_Handler::SPE_Handler (DOMDocument* doc, unsigned long filter)
    : traverse_ (doc),
      root_ (doc->getDocumentElement()),
      filter_ (filter),
      iter_ (traverse_->createNodeIterator (this->root_,
                                            this->filter_,
                                            0,
                                            true)),
      release_ (true)
  {}

  SPE_Handler::SPE_Handler (DOMNodeIterator* iter, bool release)
    : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
  {}


  SPE_Handler::~SPE_Handler()
  {
    if (this->release_)
      this->iter_->release();
  }

  void SPE_Handler::process_SubcomponentPortEndpoint ()
  {
    // This is bogus and should be replaced later.
    ACE_DECLARE_NEW_CORBA_ENV;

    for (DOMNode* node = this->iter_->nextNode();
         node != 0;
         node = this->iter_->nextNode())
      {
        XStr name (node->getNodeName());
        if (name == XStr (ACE_TEXT ("SubcomponentPortEndpoint")))
          {
            ACE_NEW_THROW_EX (this->spe_,
                              ::Deployment::SubcomponentPortEndpoint,
                              CORBA::NO_MEMORY());
          }
        else if (name == XStr (ACE_TEXT ("portName")))
          {
            // Fetch the text node which contains the value
            node = this->iter_->nextNode();
            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
            this->process_portName (text->getNodeValue());
          }
        else if (name == XStr (ACE_TEXT ("instance")))
          {
            SID_Handler handler (iter_, false); // SubcomponentInstantiationDescription
            handler.process_SubcomponentInstantiationDescription ();

            CORBA::ULong i (spe_->instance->length ());
            spe_->instance->length (i + 1);

            (*spe_->instance)[i] = handler.sid ();
          }
        else
          {
            // ??? How did we get here ???
            ACE_THROW (CORBA::INTERNAL());
          }
      }
    return;
  }

  void SIP_Handler::process_portName (const XMLCh* name)
  {
    if (name)
      {
        CORBA::String_var value (XMLString::transcode (name));
        this->spe_->portName = value.in();
      }
  }
}

#endf /* SPE_HANDLER_C */
