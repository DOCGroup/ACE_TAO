// $Id$

#ifndef SPE_HANDLER_C
#define SPE_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "SID_Handler.h"
#include "SPE_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
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

    void SPE_Handler::process_SubcomponentPortEndpoint
    (::Deployment::SubcomponentPortEndpoint &spe)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr name (node->getNodeName());
          if (name == XStr (ACE_TEXT ("portName")))
            {
              // Fetch the text node which contains the value
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_portName (text->getNodeValue(), spe);
            }
          else if (name == XStr (ACE_TEXT ("instance")))
            {
              CORBA::ULong i (spe.instance.length ());
              spe.instance.length (i + 1);

              // fetch SID handler
              SID_Handler sid_handler (iter_, false); //                                                   // SubcomponentInstantiationDescription

              // delegate populating process
              sid_handler.process_SubcomponentInstantiationDescription (spe.instance[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    void SPE_Handler::process_portName (const XMLCh* name,
    ::Deployment::SubcomponentPortEndpoint &spe) 
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          spe.portName = value.in();
        }
    }
  }
}

#endif /* SPE_HANDLER_C */
