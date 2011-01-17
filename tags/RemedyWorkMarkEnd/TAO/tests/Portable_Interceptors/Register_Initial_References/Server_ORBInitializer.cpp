// -*- C++ -*-
// $Id$

#include "Server_ORBInitializer.h"
#include "test_i.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Constants.h"

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr orbinitinfo)
{
  POA_TestModule::test* test = 0;
  ACE_NEW (test,
           test_i);
  PortableServer::ServantBase_var safe (test);

  CORBA::Object_var object = test->_this ();

  orbinitinfo->register_initial_reference ("MyService",
                                           object.in ());

  bool invalid_name = false;
  try
    {
      // Registering with an empty string should give an exception
      orbinitinfo->register_initial_reference ("",
                                               object.in ());
    }
  catch (const PortableInterceptor::ORBInitInfo::InvalidName&)
    {
      invalid_name = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!invalid_name)
    ACE_ERROR ((LM_ERROR, "ERROR: Registering with an empty string doesn't "
                          "throw an exception\n"));

  bool duplicate_name = false;
  try
    {
      // Registering with an duplicate string should give an exception
      orbinitinfo->register_initial_reference ("MyService",
                                               object.in ());
    }
  catch (const PortableInterceptor::ORBInitInfo::InvalidName&)
    {
      duplicate_name = true;
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!duplicate_name)
    ACE_ERROR ((LM_ERROR, "ERROR: Registering with a duplicate with ORBInitInfo "
                          "doesn't throw the expected exception\n"));

  bool invalid_object = false;
  try
    {
      // Registering with a nil object
      orbinitinfo->register_initial_reference ("NilServer",
                                               CORBA::Object::_nil());
    }
  catch (const CORBA::BAD_PARAM& ex)
    {
      if (ex.minor () == (CORBA::OMGVMCID | 27))
        {
          invalid_object = true;
        }
    }
  catch (const CORBA::Exception&)
    {
    }

  if (!invalid_object)
    ACE_ERROR ((LM_ERROR, "ERROR: Registering with a nil object to ORBInitInfo "
                          "doesn't throw bad param with OMG minor code 27\n"));
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

