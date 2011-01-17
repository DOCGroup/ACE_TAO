// $Id$

#include "amh_ami_pch.h"
#include "inner_cb.h"
#include <iostream>
// Implementation skeleton constructor
Inner_callback_i::Inner_callback_i (PortableServer::POA_ptr p,
                                    AMH_MiddleResponseHandler_ptr rh)
  : poa_(PortableServer::POA::_duplicate (p)),
    response_handler_ (AMH_MiddleResponseHandler::_duplicate (rh))
{
}

// Implementation skeleton destructor
Inner_callback_i::~Inner_callback_i (void)
{
  std::cout << "Inner_callback_i destroying" << std::endl;

}

void
Inner_callback_i::answer (const char * ami_return_val)
{
  std::cout << "Inner_callback_i::answer called, return_val = "
       << ami_return_val << std::endl;
  this->response_handler_->get_the_answer (ami_return_val);

  std::cout << "inner_callback_i deactivating self" << std::endl;
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id(this);
  this->poa_->deactivate_object (oid.in());
}

void
Inner_callback_i::answer_excep (Messaging::ExceptionHolder* excep_holder)
{
  // Here, we need to extract the exception from this holder, and package
  // it in another so the AMH response handler may forward it on.
  try
    {
      excep_holder->raise_exception();
    }
  catch(const CORBA::Exception& ex)
    {
      CORBA::Exception *local_ex = ex._tao_duplicate();
      AMH_MiddleExceptionHolder amh_excep_holder (local_ex);
      this->response_handler_->get_the_answer_excep (&amh_excep_holder);
    }
  catch(...)
    {
      std::cout
        << "inner_callback_i::answer_excep got an unknown exception"
        << std::endl;

      CORBA::Exception *unknown_ex = new CORBA::UNKNOWN;
      AMH_MiddleExceptionHolder amh_excep_holder (unknown_ex);
      this->response_handler_->get_the_answer_excep (&amh_excep_holder);
    }

  std::cout << "inner_callback_i deactivating self" << std::endl;
  PortableServer::ObjectId_var oid = this->poa_->servant_to_id(this);
  this->poa_->deactivate_object (oid.in());
}
