// $Id$

#ifndef PC_HANDLER_C
#define PC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CPR_Handler.h"
#include "CPD_Handler.h"
#include "PC_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    PC_Handler::PC_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    PC_Handler::PC_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PC_Handler::~PC_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void PC_Handler::process_PackageConfiguration
      (::Deployment::PackageConfiguratio &pc)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

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
              // Fetch the text node which contains the "specializedConfig"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_specializedConfig (text->getNodeValue(), pc);
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
    void SIP_Handler::process_label
      (const XMLCh* name, ::Deployment::PackageConfiguratio &pc)
    {
      if (name)
        {
          pc.label = XMLString::transcode (name);
        }
    }

    /// handle UUID attribute
    void SIP_Handler::process_UUID
      (const XMLCh* name, ::Deployment::PackageConfiguratio &pc)
    {
      if (name)
        {
          pc.UUID = XMLString::transcode (name);
        }
    }

    /// handle specializedConfig attribute
    void MDD_Handler::process_specializedConfig
      (const XMLCh* specializedConfig, ::Deployment::PackageConfiguratio &pc)
    {
      if (source)
        {
          // first increment the length of the sequence
          CORBA::ULong i (pc.specializedConfigRef.length ());
          pc.specializedConfigRef.length (i+1);
          // insert the new specializedConfig
          pc.specializedConfigRef[i] = strtoul (XMLString::transcode (specializedConfig), 0, 10);
        }
    }

  }
}

#endif /* PC_HANDLER_C */
