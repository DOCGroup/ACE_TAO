
//=============================================================================
/**
 *  @file    CC_naming_service.cpp
 *
 *    This class implements the naming services necessary to test the
 *    concurrency service
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================

#include "CC_naming_service.h"
#include "ace/Log_Msg.h"

CC_naming_service::CC_naming_service (CORBA::ORB_ptr orb)
{
  this->Init (orb);
}

void
CC_naming_service::Init (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);

  int const success = this->init_naming_service ();
  if (success < 0)
    throw CORBA::INTERNAL ();

  CC_naming_service::instance_ = this;
}

CC_naming_service::~CC_naming_service ()
{
  if (CC_naming_service::instance_ == this)
    CC_naming_service::instance_ = nullptr;
}

CC_naming_service *
CC_naming_service::Instance ()
{
  if (CC_naming_service::instance_ == nullptr)
    {
      static CC_naming_service default_instance;
      CC_naming_service::instance_ = &default_instance;
    }
  return CC_naming_service::instance_;
}

CORBA::Object_var
CC_naming_service::get_obj_from_name (std::string const &context,
                                      std::string const &name)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("C: %C, N: %C\n"),
              context.c_str (),
              name.c_str ()));
  CORBA::Object_var obj;

  try
    {
      if (context.empty ())
        {
          CosNaming::Name ns_name (1);
          ns_name.length (1);
          ns_name[0].id = CORBA::string_dup (name.c_str ());
          obj = my_name_client_->resolve (ns_name);
        }
      else
        {
          CosNaming::Name ns_name (2);
          ns_name.length (2);
          ns_name[0].id = CORBA::string_dup (context.c_str ());
          ns_name[1].id = CORBA::string_dup (name.c_str ());
          obj = my_name_client_->resolve (ns_name);
        }
      if (CORBA::is_nil (obj.in ()))
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OBJ was nil (aieee)\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_Client::get_obj_from_name (...)");
      return obj;
    }

  return obj;
}

void
CC_naming_service::bind_name (std::string const &name,
                              CORBA::Object_ptr obj)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("CC_Client::bind_name\n")));

  try
    {
      CosNaming::Name ns_name (1);
      ns_name.length (1);
      ns_name[0].id = CORBA::string_dup (name.c_str ());
      my_name_client_->bind (ns_name,
                             obj);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_Client::bind_name (...)");
    }
}

CosConcurrencyControl::LockSetFactory_var
CC_naming_service::get_lock_set_factory () const
{
  return this->factory_;
}

int
CC_naming_service::init_naming_service ()
{
  try
    {
      // Initialize the naming services
      if (this->my_name_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize "
                                    "the TAO_Naming_Client.\n")),
                          -1);

      CORBA::Object_var const factory_obj =
        this->get_obj_from_name ("CosConcurrency", "LockSetFactory");

      this->factory_ =
        CosConcurrencyControl::LockSetFactory::_narrow
        (factory_obj.in ());

      if (CORBA::is_nil (this->factory_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" could not resolve lock set factory in Naming service\n")),
                          -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CC_Client::init_naming_service");
      return -1;
    }

  return 0;
}

CC_naming_service *CC_naming_service::instance_ {};
