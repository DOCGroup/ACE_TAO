// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "ACD_Handler.h"
#include "APM_Handler.h"
#include "CAD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    CAD_Handler::CAD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CAD_Handler::CAD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CAD_Handler::~CAD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the connection resource deployment description and populate it
    void CAD_Handler::process_ComponentAssemblyDescription
    (::Deployment::ComponentAssemblyDescription &cad)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("instance")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cad.instance.length ());
              cad.instance.length (i + 1);


            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cad.connection.length ());
              cad.connection.length (i + 1);

              // fetch the ACD handler
              ACD_Handler acd_handler (this->iter_, false);

              // delegate the populating process
              acd_handler.process_AssemblyConnectionDescription (cad.connection[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("externalProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (cad.externalProperty.length ());
              cad.externalProperty.length (i + 1);

              // fetch the APM handler
              APM_Handler apm_handler (this->iter_, false);

              // delegate the populating process
              apm_handler.process_AssemblyPropertyMapping (cad.externalProperty[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

  }
}
