// $Id$

#ifndef MID_HANDLER_C
#define MID_HANDLER_C

#include "MID_Handler.h"
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "MID_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace Config_Handler
{
  MID_Handler::MID_Handler (DOMDocument* doc, unsigned long filter)
    : traverse_ (doc),
      root_ (doc->getDocumentElement()),
      filter_ (filter),
      iter_ (traverse_->createNodeIterator (this->root_,
                                            this->filter_,
                                            0,
                                            true)),
      release_ (true)
  {}

  MID_Handler::MID_Handler (DOMNodeIterator* iter, bool release)
    : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
  {}


  MID_Handler::~MID_Handler()
  {
    if (this->release_)
      this->iter_->release();
  }

  void MID_Handler::process_MonolithicImplementationDescription ()
  {
    // This is bogus and should be replaced later.
    ACE_DECLARE_NEW_CORBA_ENV;

    for (DOMNode* node = this->iter_->nextNode();
         node != 0;
         node = this->iter_->nextNode())
      {
        XStr name (node->getNodeName());
        if (name == XStr (ACE_TEXT ("MonolithicImplementationDescription")))
          {
            ACE_NEW_THROW_EX (this->mid_,
                              ::Deployment::MonolithicImplementationDescription,
                              CORBA::NO_MEMORY());
          }
        else if (name == XStr (ACE_TEXT ("execParameter")))
          {
            Property_Handler handler (iter_, false); // Property
            handler.process_Property ();

            CORBA::ULong i (mid_->execParameter->length ());
            mid_->execParameter->length (i + 1);

            (*mid_->execParameter)[i] = handler.Property (); // Upper case, or lower case???
          }
       else if (name == XStr (ACE_TEXT ("deployRequirement")))
          {
            Requirement_Handler handler (iter_, false); // Requirement
            handler.process_Requirement ();

            CORBA::ULong i (mid_->deployRequirement->length ());
            mid_->deployRequirement->length (i + 1);

            (*mid_->deployRequirement)[i] = handler.Requirement (); // Upper case, or lower case???
          }
        else if (name == XStr (ACE_TEXT ("primaryArtifact")))
          {
            NIA _Handler handler (iter_, false); // NamedImplementationArtifact
            handler.process_NamedImplementationArtifact ();

            CORBA::ULong i (mid_->primaryArtifact->length ());
            mid_->primaryArtifact->length (i + 1);

            (*mid_->primaryArtifact)[i] = handler.nia ();
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

#endif /* MID_HANDLER_C */
