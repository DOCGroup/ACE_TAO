// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Failure, test_i, "$Id$")

void
Callback_i::shutdown (CORBA::Boolean is_clean,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (is_clean == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Performing catastrophic shutdown\n%a"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
      this->orb_->shutdown (0, ACE_TRY_ENV);
    }
}

// ****************************************************************

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback,
                              CORBA::Boolean is_clean,
                              Callback_ptr callback,
                              CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (do_callback)
    {
      ACE_DEBUG ((LM_DEBUG, "Callback to shutdown client (%d)\n",
                  is_clean));
      callback->shutdown (is_clean, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  ACE_Time_Value tv (0, 20000);
  ACE_OS::sleep (tv);
  return 0;
}

void
Simple_Server_i::shutdown_now (CORBA::Boolean is_clean,
                               CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (is_clean == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Performing catastrophic shutdown %a\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
      this->orb_->shutdown (0, ACE_TRY_ENV);
    }
}

void
Simple_Server_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

// ****************************************************************

CORBA::Long
Middle_i::test_method (CORBA::Boolean do_callback,
                       CORBA::Boolean is_clean,
                       Callback_ptr callback,
                       CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  int i = 0;
  for (; i != 10; ++i)
    {
      this->server_->test_method (0,
                                  0,
                                  callback,
                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  this->server_->test_method (do_callback,
                              is_clean,
                              callback,
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  for (; i != 10; ++i)
    {
      this->server_->test_method (0,
                                  0,
                                  callback,
                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return 0;
}

void
Middle_i::shutdown_now (CORBA::Boolean is_clean,
                        CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->server_->shutdown_now (is_clean, ACE_TRY_ENV);
}

void
Middle_i::shutdown (CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->server_->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // ignore them
    }
  ACE_ENDTRY;

  this->orb_->shutdown (0, ACE_TRY_ENV);
}
