//
// $Id$
//
#include "Client_Task.h"

ACE_RCSID(Thread_Per_Connection_Latency, Client_Task, "$Id$")

Client_Task::Client_Task (const char *data_type,
                          const int size,
						  Test::Roundtrip_ptr roundtrip,
                          int niterations)
  : data_type_ (CORBA::string_dup (data_type))
  , size_ (size)
  , roundtrip_ (Test::Roundtrip::_duplicate (roundtrip))
  , niterations_ (niterations)
{
}

int
Client_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->validate_connection (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (this->data_type_, "octet") == 0 )
        {
          this->test_octet_seq (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      if (ACE_OS::strcmp (this->data_type_, "long") == 0 )
        {
          this->test_long_seq (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      if (ACE_OS::strcmp (this->data_type_, "short") == 0 )
        {
          this->test_short_seq (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      if (ACE_OS::strcmp (this->data_type_, "char") == 0 )
        {
          this->test_char_seq (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      if (ACE_OS::strcmp (this->data_type_, "longlong") == 0 )
        {
          this->test_longlong_seq (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      if (ACE_OS::strcmp (this->data_type_, "double") == 0 )
        {
          this->test_double_seq (ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      return 0;
    }
  ACE_ENDTRY;
  return 0;
}

void
Client_Task::accumulate_and_dump (ACE_Basic_Stats &totals,
                                  const char *msg,
                                  ACE_UINT32 gsf)
{
  totals.accumulate (this->latency_);
  this->latency_.dump_results (msg, gsf);
}

void
Client_Task::validate_connection (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ULongLong dummy = 0;
  Test::octet_load oc;

  for (int i = 0; i != 100; ++i)
    {
      ACE_TRY
        {
          (void) this->roundtrip_->test_octet_method (oc, dummy ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY {} ACE_ENDTRY;
    }
}

void
Client_Task::test_octet_seq (ACE_ENV_ARG_DECL)
{
  Test::octet_load ol (this->size_);
  ol.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong start = ACE_OS::gethrtime ();
 
      (void) this->roundtrip_->test_octet_method (ol, start ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_long_seq (ACE_ENV_ARG_DECL)
{
  Test::long_load ll (this->size_);
  ll.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong start = ACE_OS::gethrtime ();
 
      (void) this->roundtrip_->test_long_method (ll, start ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}


void
Client_Task::test_short_seq (ACE_ENV_ARG_DECL)
{
  Test::short_load sl (this->size_);
  sl.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong start = ACE_OS::gethrtime ();
 
      (void) this->roundtrip_->test_short_method (sl, start ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_char_seq (ACE_ENV_ARG_DECL)
{
  Test::char_load cl (this->size_);
  cl.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong start = ACE_OS::gethrtime ();
 
      (void) this->roundtrip_->test_char_method (cl, start ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_longlong_seq (ACE_ENV_ARG_DECL)
{
  Test::longlong_load ll (this->size_);
  ll.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong start = ACE_OS::gethrtime ();
 
      (void) this->roundtrip_->test_longlong_method (ll, start ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_double_seq (ACE_ENV_ARG_DECL)
{
  Test::double_load dl (this->size_);
  dl.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong start = ACE_OS::gethrtime ();
 
      (void) this->roundtrip_->test_double_method (dl, start ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}
