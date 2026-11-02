// -*- C++ -*-
#include "Server_ORBInitializer.h"
#include "test_i.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Constants.h"

Server_ORBInitializer::Server_ORBInitializer ()
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr orbinitinfo)
{
  POA_TestModule::test *test = nullptr;
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
  catch (PortableInterceptor::ORBInitInfo::InvalidName const &)
    {
      invalid_name = true;
    }
  catch (CORBA::Exception const &)
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
  catch (PortableInterceptor::ORBInitInfo::InvalidName const &)
    {
      duplicate_name = true;
    }
  catch (CORBA::Exception const &)
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
  catch (CORBA::BAD_PARAM const &ex)
    {
      if (ex.minor () == (CORBA::OMGVMCID | 27))
        {
          invalid_object = true;
        }
    }
  catch (CORBA::Exception const &)
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

