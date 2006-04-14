// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "test_i.h"

ACE_RCSID (Recursive_ORBInitializer,
           Server_ORBInitializer,
           "$Id$")

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr orbinitinfo
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  POA_TestModule::test* test = 0;
  ACE_NEW (test,
           test_i);

  CORBA::Object_ptr object = test->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  orbinitinfo->register_initial_reference ("MyService",
                                           object
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  bool invalid_name = false;
  ACE_TRY
    {
      // Registering with an empty string should give an exception
      orbinitinfo->register_initial_reference ("",
                                               object
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableInterceptor::ORBInitInfo::InvalidName, ex)
    {
      invalid_name = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!invalid_name)
    ACE_ERROR ((LM_ERROR, "ERROR: Registering with an empty string doesn't "
                          "throw an exception\n"));

  bool duplicate_name = false;
  ACE_TRY_EX (duplicate)
    {
      // Registering with an duplicate string should give an exception
      orbinitinfo->register_initial_reference ("MyService",
                                               object
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (duplicate) ;
    }
  ACE_CATCH (PortableInterceptor::ORBInitInfo::InvalidName, ex)
    {
      duplicate_name = true;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!duplicate_name)
    ACE_ERROR ((LM_ERROR, "ERROR: Registering with a duplicate with ORBInitInfo "
                          "doesn't throw the expected exception\n"));

  bool invalid_object = false;
  ACE_TRY_EX (invalid)
    {
      // Registering with a nil object
      orbinitinfo->register_initial_reference ("NilServer",
                                               CORBA::Object::_nil()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (invalid);
    }
  ACE_CATCH (CORBA::BAD_PARAM , ex)
    {
      if ((ex.minor() & 0xFFFU) == 27)
        {
          invalid_object = true;
        }
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  if (!invalid_object)
    ACE_ERROR ((LM_ERROR, "ERROR: Registering with a nil object to ORBInitInfo "
                          "doesn't throw bad param with minor code 27\n"));
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

