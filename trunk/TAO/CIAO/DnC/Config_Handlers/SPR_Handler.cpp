// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "SID_Handler.h"
#include "SPR_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    SPR_Handler::SPR_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    SPR_Handler::SPR_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}

    SPR_Handler::~SPR_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the subcomponent property reference and populate it
    void SPR_Handler::process_SubcomponentPropertyReference
    (::Deployment::SubcomponentPropertyReference &spr)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("propertyName")))
            {
              // handle name
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_propertyName (text->getNodeValue(), spr);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              // fetch the subcomponent instantiation description handler
              SID_Handler sid_handler (this->iter_, false);

              // delegate the populating process
              sid_handler.process_SubcomponentInstantiationDescription (spr.instance);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle property name attribute
    void SPR_Handler::process_propertyName (const XMLCh* name,
                                            ::Deployment::SubcomponentPropertyReference &spr)
    {
      if (name)
        {
          spr.propertyName = XMLString::transcode (name);
        }
    }

  }
}
