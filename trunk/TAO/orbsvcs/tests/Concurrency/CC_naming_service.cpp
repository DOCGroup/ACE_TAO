// -*- c++ -*-
// $Id$
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

CC_naming_service::CC_naming_service(CORBA::ORB_var orb)
  : naming_context_ (0),
    cc_factory_key_ (0),
    orb_ (0),
    factory_ (0)
{
  this->orb_ = orb;

  init_naming_service();
}

CC_naming_service::~CC_naming_service(void)
{
}

CORBA::Object_var
CC_naming_service::get_obj_from_name(char *c_name, char *name,
                                     CORBA::Environment &_env)
{
  ACE_DEBUG((LM_DEBUG, "C: %s, N: %s\n", c_name, name));
  CORBA::Object_var obj;

  TAO_TRY
    {
      if(strlen(c_name)==0)
        {
          CosNaming::Name ns_name (1);
          ns_name.length (1);
          ns_name[0].id = CORBA::string_dup (name);
          obj = naming_context_->resolve (ns_name, TAO_TRY_ENV);
        }
      else
        {
          CosNaming::Name ns_name (2);
          ns_name.length (2);
          ns_name[0].id = CORBA::string_dup (c_name);
          ns_name[1].id = CORBA::string_dup (name);
          obj = naming_context_->resolve (ns_name, TAO_TRY_ENV);
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::get_obj_from_name(...)");
      return 0;
    }
  TAO_ENDTRY;

  return obj;
}

void
CC_naming_service::bind_name(char *n,
                             CORBA::Object_ptr obj,
                             CORBA::Environment &_env)
{
  ACE_DEBUG((LM_DEBUG, "CC_Client::bind_name\n"));
  
  TAO_TRY
    {
      CosNaming::Name ns_name (1);
      ns_name.length(1);
      ns_name[0].id = CORBA::string_dup (n);
      naming_context_->bind(ns_name,
                            obj,
                            _env);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::bind_name(...)");
    }
  TAO_ENDTRY;
}

CosConcurrencyControl::LockSetFactory_var
CC_naming_service::get_lock_set_factory(void)
{
  return this->factory_;
}

int 
CC_naming_service::init_naming_service(void)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);
      naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      CORBA::Object_var factory_obj = get_obj_from_name("CosConcurrency",
                                                        "LockSetFactory",
                                                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->factory_ =
        CosConcurrencyControl::LockSetFactory::_narrow
        (factory_obj.in (),TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      if (CORBA::is_nil (this->factory_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve lock set factory in Naming service <%s>\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::init_naming_service");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}
