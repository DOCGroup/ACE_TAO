// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CAD_Handler.h"
#include "MID_Handler.h"
#include "ID_Handler.h"
#include "Cap_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "CompImplDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    CompImplDesc_Handler::CompImplDesc_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    CompImplDesc_Handler::CompImplDesc_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CompImplDesc_Handler::~CompImplDesc_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the component implementation description and populate it
    void CompImplDesc_Handler::process_ComponentImplementationDescription
    (::Deployment::ComponentImplementationDescription &cid)
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
              this->process_label (text->getNodeValue(), cid);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), cid);
            }
          else if (node_name == XStr (ACE_TEXT ("assemblyImpl")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cid.assemblyImpl.length ());
              cid.assemblyImpl.length (i + 1);
              
              // fetch the handler
              CAD_Handler cad_handler (iter_, false);

              // delegate the populating process
              cad_handler.process_ComponentAssemblyDescription (cid.assemblyImpl[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("monolithicImpl")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cid.monolithicImpl.length ());
              cid.monolithicImpl.length (i + 1);
              
              // fetch the handler
              MID_Handler mid_handler (iter_, false);

              // delegate the populating process
              mid_handler.process_MonolithicImplementationDescription (cid.monolithicImpl[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("dependsOn")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cid.dependsOn.length ());
              cid.dependsOn.length (i + 1);
              
              // delegate the populating process
              ID_Handler::process_ImplementationDependency
                (this->iter_, cid.dependsOn[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cid.configProperty.length ());
              cid.configProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  cid.configProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cid.infoProperty.length ());
              cid.infoProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  cid.infoProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("capability")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cid.capability.length ());
              cid.capability.length (i + 1);
              // delegate the populating process
              CAP_Handler cap_handler (this->iter_, false);
              cap_handler.process_Capability (this->iter_,
                                              cid.capability[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("implements")))
            {
              // fetch the handler
              CCD_Handler compIntrDesc_handler (iter_, false);

              // delegate the populating process
              compIntrDesc_handler.process_ComponentInterfaceDescription (cid.implements);
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
    void CompImplDesc_Handler::process_label (const XMLCh* label,
                                              ::Deployment::ComponentImplementationDescription &cid)
    {
      if (label)
        {
          cid.label = XMLString::transcode (label);
        }
    }

    /// handle UUID attribute
    void CompImplDesc_Handler::process_UUID (const XMLCh* UUID,
                                             ::Deployment::ComponentImplementationDescription &cid)
    {
      if (UUID)
        {
          cid.UUID = XMLString::transcode (UUID);
        }
    }

  }
}
