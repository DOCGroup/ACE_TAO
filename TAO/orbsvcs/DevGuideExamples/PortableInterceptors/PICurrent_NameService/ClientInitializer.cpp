#include "ClientInitializer.h"
#include "ClientInterceptor.h"
#include "MessengerC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>

void
ClientInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ClientInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Find the Naming Service
  CORBA::Object_var const naming_object = info->resolve_initial_references ("NameService");
  CosNaming::NamingContext_var const root =
    CosNaming::NamingContext::_narrow (naming_object.in ());
  if (CORBA::is_nil (root.in ()))
    {
      std::cerr << "Nil Naming Context reference" << std::endl;
      ACE_ASSERT (false);
    }

  CosNaming::Name name {};
  name.length (1);
  name[0].id = CORBA::string_dup ("Messenger");
  CORBA::Object_var object {};
  while (CORBA::is_nil (object.in ()))
    {
      try
        {
          object = root->resolve (name);
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          ACE_OS::sleep (1);
        }
    }

  Messenger_var const messenger = Messenger::_narrow (object.in ());
  if (CORBA::is_nil (messenger.in ()))
    {
      std::cerr << "Not a Messenger reference" << std::endl;
      ACE_ASSERT (false);
    }

  slot_ = info->allocate_slot_id ();

  CORBA::Object_var const current_object = info->resolve_initial_references ("PICurrent");
  current_ = PortableInterceptor::Current::_narrow (current_object.in ());

  PortableInterceptor::ClientRequestInterceptor_var const ci =
    new ClientInterceptor (messenger, current_.in (), slot_);
  info->add_client_request_interceptor (ci.in ());
}

void
ClientInitializer::set_slot_data ()
{
  CORBA::Any flag {};
  CORBA::Boolean const initial_value = false;
  flag <<= CORBA::Any::from_boolean (initial_value);
  current_->set_slot (slot_, flag);

  current_ = PortableInterceptor::Current::_nil();
}
