// $Id$

#include "ClientInitializer.h"
#include "ClientInterceptor.h"
#include "MessengerC.h"
#include <iostream>

ClientInitializer::ClientInitializer (void)
 : slot_ (0),
   current_ (PortableInterceptor::Current::_nil())
{
}

void
ClientInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ClientInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // resolve Messenger object
  CORBA::Object_var obj = info->resolve_initial_references( "Messenger" );
  Messenger_var messenger = Messenger::_narrow( obj.in() );
  if( CORBA::is_nil( messenger.in() ) ) {
    std::cerr << "Not a Messenger reference" << std::endl;
    ACE_ASSERT(false);
  }

  // allocate slot
  slot_ = info->allocate_slot_id();

  // get PICurrent
  CORBA::Object_var current_obj = info->resolve_initial_references("PICurrent");

  current_ =
    PortableInterceptor::Current::_narrow(current_obj.in());

  // Create and register the request interceptors.
  PortableInterceptor::ClientRequestInterceptor_var ci =
      new ClientInterceptor(messenger, current_.in(), slot_);
  info->add_client_request_interceptor (ci.in());
}

void
ClientInitializer::set_slot_data (void)
{
  // Set the recursion flag
  CORBA::Any flag;
  CORBA::Boolean x = 0;
  flag <<= CORBA::Any::from_boolean(x);
  current_->set_slot(slot_, flag);

  // Now that we're done with the PICurrent, we will release
  // our reference to it.
  current_ = PortableInterceptor::Current::_nil();
}
