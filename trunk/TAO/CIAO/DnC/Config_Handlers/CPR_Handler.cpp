// $Id$

#ifndef CPR_HANDLER_C
#define CPR_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "CPR_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    CPR_Handler::CPR_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CPR_Handler::CPR_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CPR_Handler::~CPR_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the connection resource deployment description and populate it
    void CPR_Handler::process_ComponentPackageReference
    (::Deployment::ComponentPackageReference &cpr)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("requiredUUID")))
            {
              // Fetch the node which contains the "requiredUUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_requiredUUID (text->getNodeValue(), cpr);
            }
          else if (node_name == XStr (ACE_TEXT ("requiredName")))
            {
              // Fetch the text node which contains the "requiredName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_requiredName (text->getNodeValue(), cpr);
            }
          else if (node_name == XStr (ACE_TEXT ("requiredType")))
            {
              // Fetch the text node which contains the "requiredType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_requiredType (text->getNodeValue(), cpr);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle requiredUUID attribute
    void CPR_Handler::process_requiredUUID (const XMLCh* UUID,
                                          ::Deployment::ComponentPackageReference &cpr)
    {
      if (UUID)
        {
          cpr.requiredUUID = XMLString::transcode (UUID);
        }
    }

    /// handle required name attribute
    void CPR_Handler::process_requiredName (const XMLCh* name,
                                            ::Deployment::ComponentPackageReference &cpr)
    {
      if (name)
        {
          cpr.requiredName = XMLString::transcode (name);
        }
    }

    /// handle required type attribute
    void CPR_Handler::process_requiredType (const XMLCh* type,
                                            ::Deployment::ComponentPackageReference &cpr)
    {
      if (type)
        {
          cpr.requiredType = XMLString::transcode (type);
        }
    }


  }
}

#endif /* CPR_HANDLER_C */
