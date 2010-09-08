// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Concurrency
//
// = FILENAME
//    CC_naming_service.cpp
//
// = DESCRIPTION
//      This class implements the naming services necessary to test the
//      concurrency service
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "CC_naming_service.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"



CC_naming_service::CC_naming_service (CORBA::ORB_var orb)
  : cc_factory_key_ (0),
    orb_ (0),
    factory_ (0)
{
  this->Init(orb);

  instance_ = this;
}

CC_naming_service::CC_naming_service(void)
  : cc_factory_key_ (0),
    orb_ (0),
    factory_ (0)
{
}

void
CC_naming_service::Init(CORBA::ORB_var orb)
{
  this->orb_ = orb;

  int success = init_naming_service ();
  if (success < 0)
    throw CORBA::INTERNAL ();
}

CC_naming_service::~CC_naming_service (void)
{
  //  if(instance_!=0)
  // delete instance_;
  // @TAO somthing went wrong when these lines were uncommented
}

CC_naming_service *
CC_naming_service::Instance(void)
{
  if(instance_ == 0)
    {
      instance_ = new CC_naming_service();
    }
  return instance_;
}

CORBA::Object_var
CC_naming_service::get_obj_from_name (const char *c_name,
                                      const char *name)
{
  ACE_DEBUG ((LM_DEBUG, "C: %s, N: %s\n", c_name, name));
  CORBA::Object_var obj;

  try
    {
      if (ACE_OS::strlen (c_name) == 0)
        {
          CosNaming::Name ns_name (1);
          ns_name.length (1);
          ns_name[0].id = CORBA::string_dup (name);
          obj = my_name_client_->resolve (ns_name);
        }
      else
        {
          CosNaming::Name ns_name (2);
          ns_name.length (2);
          ns_name[0].id = CORBA::string_dup (c_name);
          ns_name[1].id = CORBA::string_dup (name);
          obj = my_name_client_->resolve (ns_name);
        }
      if (CORBA::is_nil (obj.in ()) )
        ACE_DEBUG((LM_DEBUG,
                   "OBJ was nill (aieee)\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_Client::get_obj_from_name (...)");
      return obj;
    }

  return obj;
}

void
CC_naming_service::bind_name (const char *n,
                              CORBA::Object_ptr obj)
{
  ACE_DEBUG ((LM_DEBUG, "CC_Client::bind_name\n"));

  try
    {
      CosNaming::Name ns_name (1);
      ns_name.length (1);
      ns_name[0].id = CORBA::string_dup (n);
      my_name_client_->bind (ns_name,
                             obj);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_Client::bind_name (...)");
    }
}

CosConcurrencyControl::LockSetFactory_var
CC_naming_service::get_lock_set_factory (void)
{
  return this->factory_;
}

int
CC_naming_service::init_naming_service (void)
{
  try
    {
      // Initialize the naming services
      if (my_name_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      CORBA::Object_var factory_obj = get_obj_from_name ("CosConcurrency",
                                                         "LockSetFactory");

      this->factory_ =
        CosConcurrencyControl::LockSetFactory::_narrow
        (factory_obj.in ());

      if (CORBA::is_nil (this->factory_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve lock set factory in Naming service\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_Client::init_naming_service");
      return -1;
    }

  return 0;
}

CC_naming_service* CC_naming_service::instance_ = 0;
