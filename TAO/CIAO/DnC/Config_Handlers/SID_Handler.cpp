// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Requirement_Handler.h"
#include "Property_Handler.h"
#include "CPR_Handler.h"
#include "CompPkgDesc_Handler.h"
#include "SID_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    SID_Handler::SID_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    SID_Handler::SID_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    SID_Handler::~SID_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the subcomponent instantiation description and populate it
    void SID_Handler::process_SubcomponentInstantiationDescription
    (::Deployment::SubcomponentInstantiationDescription &sid)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // handle name
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), sid);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (sid.configProperty.length ());
              sid.configProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  sid.configProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("selectRequirement")))
            {
              // increase the length of the sequence
              CORBA::ULong i (sid.selectRequirement.length ());
              sid.selectRequirement.length (i + 1);

              // delegate the populating process
              Requirement_Handler::process_Requirement (this->iter_,
                                                        sid.selectRequirement[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("package")))
            {
              // increase the length of the sequence
              CORBA::ULong i (sid.package.length ());
              sid.package.length (i + 1);

              // fetch the Compone package description handler
              CompPkgDesc_Handler cpd_handler (this->iter_, false);

              // delegate the populating process
              cpd_handler.process_ComponentPackageDescription (sid.package[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("reference")))
            {
              // increase the length of the sequence
              CORBA::ULong i (sid.reference.length ());
              sid.reference.length (i + 1);

              // fetch the Component package reference handler
              CPR_Handler cpr_handler (this->iter_, false);

              // delegate the populating process
              cpr_handler.process_ComponentPackageReference (sid.reference[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle name attribute
    void SID_Handler::process_name (const XMLCh* name,
                                    ::Deployment::SubcomponentInstantiationDescription &sid)
    {
      if (name)
        {
          sid.name = XMLString::transcode (name);
        }
    }

  }
}
