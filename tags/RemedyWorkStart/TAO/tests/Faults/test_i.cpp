// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_unistd.h"

void
Callback_i::shutdown (CORBA::Boolean is_clean)
{
  if (is_clean == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Performing catastrophic shutdown\n"));

// OpenVMS seems to hang and not abort and dump core when abort() is called
// here. This needs further investigation.
#if defined (DEC_CXX) || defined (ACE_OPENVMS)
      ACE_OS::_exit();
#else
      ACE_OS::abort();
#endif
      return;
    }
  this->orb_->shutdown (0);

  ACE_DEBUG ((LM_DEBUG, "Shutdown: Performed clean shutdown\n"));
}

// ****************************************************************

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback,
                              CORBA::Boolean is_clean,
                              Callback_ptr callback)
{
  if (do_callback)
    {
      ACE_DEBUG ((LM_DEBUG, "Callback to shutdown client (%d)\n",
                  is_clean));
      callback->shutdown (is_clean);
    }
  ACE_Time_Value tv (0, 20000);
  ACE_OS::sleep (tv);
  return 0;
}

void
Simple_Server_i::shutdown_now (CORBA::Boolean is_clean)
{
  if (is_clean == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Performing catastrophic shutdown\n"));
#if defined (DEC_CXX) || defined (ACE_OPENVMS)
      ACE_OS::_exit();
#else
      ACE_OS::abort();
#endif
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "shutdown_now:Performing clean shutdown\n"));
  this->orb_->shutdown (0);
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

// ****************************************************************

CORBA::Long
Middle_i::test_method (CORBA::Boolean do_callback,
                       CORBA::Boolean is_clean,
                       Callback_ptr callback)
{
  int i = 0;
  for (; i != 10; ++i)
    {
      this->server_->test_method (0,
                                  0,
                                  callback);
    }

  this->server_->test_method (do_callback,
                              is_clean,
                              callback);

  for (; i != 10; ++i)
    {
      this->server_->test_method (0,
                                  0,
                                  callback);
    }

  return 0;
}

void
Middle_i::shutdown_now (CORBA::Boolean is_clean)
{
  this->server_->shutdown_now (is_clean);
}

void
Middle_i::shutdown (void)
{
  try
    {
      this->server_->shutdown ();
    }
  catch (const CORBA::Exception&)
    {
      // ignore them
    }

  this->orb_->shutdown (0);
}
