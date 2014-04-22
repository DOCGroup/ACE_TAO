// $Id$

#include "ClientInitializer.h"
#include "ClientInterceptor.h"
#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_unistd.h"
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
  // Find the Naming Service
  CORBA::Object_var naming_obj =
    info->resolve_initial_references("NameService");
  CosNaming::NamingContext_var root =
    CosNaming::NamingContext::_narrow(naming_obj.in());
  if( CORBA::is_nil(root.in())) {
    std::cerr << "Nil Naming Context reference" << std::endl;
    ACE_ASSERT(false);
  }

  // Resolve the Messenger object
  CosNaming::Name name;
  name.length( 1 );
  name[0].id = CORBA::string_dup( "Messenger" );
  CORBA::Object_var obj = CORBA::Object::_nil();
  while ( CORBA::is_nil( obj.in() ) ) {
    try {
      obj = root->resolve( name );
    } catch (const CosNaming::NamingContext::NotFound&) {
      // Sleep for a second and try again
      ACE_OS::sleep(1);
    }
   }

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
