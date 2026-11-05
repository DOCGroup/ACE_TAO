// -*- C++ -*-
// Ossama Othman <ossama@uci.edu>

#include "ace/Log_Msg.h"
#include "ace/Process_Manager.h"
#include "Push_Iterator_Handler.h"
#include "Callback_i.h"

void
Push_Iterator_Handler::register_callback
  (Web_Server::Metadata_Type const& metadata)
{
  this->callback_servant_->metadata (metadata);

  // This handler is no longer needed, so deactivate it.
  this->deactivate ();
}

void
Push_Iterator_Handler::run (std::atomic<int>& request_count,
                            char const *pathname,
                            Web_Server::Iterator_Factory_ptr factory)
{
  Callback_i *callback_servant {};
  ACE_NEW_THROW_EX (callback_servant,
                    Callback_i (request_count),
                    CORBA::NO_MEMORY ());
  this->callback_servant_ = callback_servant;

  // Pass ownership of the Callback servant to the POA.
  PortableServer::ServantBase_var tmp (callback_servant);

  // Activate the Callback.
  this->callback_ =
    callback_servant->_this ();

  // Activate this Reply Handler.
  this->ami_handler_ =
    this->_this ();

  // Register the client callback with the server asynchronously.
  factory->sendc_register_callback (this->ami_handler_.in (),
                                    pathname,
                                    this->callback_.in ());
}

void
Push_Iterator_Handler::deactivate ()
{
  // Get the POA used when activating the Reply Handler object.
  PortableServer::POA_var poa = this->_default_POA ();

  // Get the object ID associated with this servant.
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);

  // Now deactivate the AMI_CallbackHandler object.
  poa->deactivate_object (oid.in ());
}
