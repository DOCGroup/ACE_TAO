// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CPR_Handler.h"
#include "CompPkgDesc_Handler.h"
#include "PC_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    PC_Handler::PC_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    PC_Handler::PC_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PC_Handler::~PC_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void PC_Handler::process_PackageConfiguration
      (::Deployment::PackageConfiguration &pc)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());

          /*
          if
            (process_string(this->iter_->nextNode(), pc.label));
          else if
            (process_string(this->iter_->nextNode(), pc.uuid));
          */

          if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "label"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), pc);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), pc);
            }
          else if (node_name == XStr (ACE_TEXT ("specializedConfig")))
            {
              // fetch the component package reference handler
              PC_Handler pc_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (pc.specializedConfig.length ());
              pc.specializedConfig.length (i + 1);

              // delegate the populating process
              pc_handler.process_PackageConfiguration (pc.specializedConfig[i]);
              /*
              // Fetch the text node which contains the "specializedConfig"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_specializedConfig (text->getNodeValue(), pc);
              */
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pc.configProperty.length ());
              pc.configProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  pc.configProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("selectRequirement")))
            {
              // increase the length of the sequence
              CORBA::ULong i (pc.selectRequirement.length ());
              pc.selectRequirement.length (i + 1);

              // delegate the populating process
              Requirement_Handler::process_Requirement (this->iter_,
                                                        pc.selectRequirement[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("reference")))
            {
              // fetch the component package reference handler
              CPR_Handler cpr_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (pc.reference.length ());
              pc.reference.length (i + 1);

              // delegate the populating process
              cpr_handler.process_ComponentPackageReference (pc.reference[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("basePackage")))
            {
              // fetch the component package description handler
              CompPkgDesc_Handler comppkgdesc_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (pc.basePackage.length ());
              pc.basePackage.length (i + 1);

              // delegate the populating process
              comppkgdesc_handler.process_ComponentPackageDescription (pc.basePackage[i]);
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
    void PC_Handler::process_label
      (const XMLCh* label, ::Deployment::PackageConfiguration &pc)
    {
      if (label)
        {
          pc.label = XMLString::transcode (label);
        }
    }

    /// handle UUID attribute
    void PC_Handler::process_UUID
      (const XMLCh* UUID, ::Deployment::PackageConfiguration &pc)
    {
      if (UUID)
        {
          pc.UUID = XMLString::transcode (UUID);
        }
    }

  }
}
