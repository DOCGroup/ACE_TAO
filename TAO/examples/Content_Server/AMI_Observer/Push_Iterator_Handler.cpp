// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/Log_Msg.h"
#include "ace/Process_Manager.h"
#include "Push_Iterator_Handler.h"
#include "Callback.h"

ACE_RCSID(Content_Server, Push_Iterator_Handler, "$Id$")

Push_Iterator_Handler::Push_Iterator_Handler (void)
  : callback_servant_ (0),
    callback_ (),
    ami_handler_ ()
{
  // Nothing else
}

Push_Iterator_Handler::~Push_Iterator_Handler (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Get the POA used when activating the Reply Handler object.
      PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the object ID associated with this servant.
      PortableServer::ObjectId_var oid =
        poa->servant_to_id (this,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now deactivate the iterator object.
      poa->deactivate_object (oid.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception ")
                           ACE_TEXT ("in ~Push_Iterator_Handler():"));
    }
  ACE_ENDTRY;
}

void
Push_Iterator_Handler::register_callback (
    const Web_Server::Metadata_Type & metadata,
    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->callback_servant_->metadata (metadata);
}

void
Push_Iterator_Handler::run (int * request_count,
                            const char *pathname,
                            Web_Server::Iterator_Factory_ptr factory,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Web_Server::Error_Result))
{
  ACE_NEW_THROW_EX (this->callback_servant_,
                    Callback_i (request_count),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Pass ownership of the Callback servant to the POA.
  PortableServer::ServantBase_var tmp (this->callback_servant_);

  // Activate the Callback.
  this->callback_ = this->callback_servant_->_this (ACE_TRY_ENV);
  ACE_CHECK;
  
  // Activate this Reply Handler.
  this->ami_handler_ = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  // Register the client callback with the server asynchronously.
  factory->sendc_register_callback (this->ami_handler_.in (),
                                    pathname,
                                    this->callback_.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;
}
