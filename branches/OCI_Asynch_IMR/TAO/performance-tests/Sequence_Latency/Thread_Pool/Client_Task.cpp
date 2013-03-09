//
// $Id$
//
#include "Client_Task.h"
#include "ace/OS_NS_time.h"

Client_Task::Client_Task (const ACE_TCHAR *data_type,
                          int size,
                          Test::Roundtrip_ptr roundtrip,
                          int niterations)
  : data_type_ (data_type)
  , size_ (size)
  , roundtrip_ (Test::Roundtrip::_duplicate (roundtrip))
  , niterations_ (niterations)
{
}

int
Client_Task::svc (void)
{
  try
    {
      this->validate_connection ();

      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("octet")) == 0 )
        {
          this->test_octet_seq ();
        }
      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("long")) == 0 )
        {
          this->test_long_seq ();
        }
      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("short")) == 0 )
        {
          this->test_short_seq ();
        }
      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("char")) == 0 )
        {
          this->test_char_seq ();
        }
      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("longlong")) == 0 )
        {
          this->test_longlong_seq ();
        }
      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("double")) == 0 )
        {
          this->test_double_seq ();
        }
    }
  catch (const CORBA::Exception&)
    {
      return 0;
    }
  return 0;
}

void
Client_Task::accumulate_and_dump (
  ACE_Basic_Stats &totals,
  const ACE_TCHAR *msg,
  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  totals.accumulate (this->latency_);
  this->latency_.dump_results (msg, gsf);
}

void
Client_Task::validate_connection (void)
{
  CORBA::ULongLong dummy = 0;
  Test::octet_load oc;

  for (int i = 0; i != 100; ++i)
    {
      try
        {
          (void) this->roundtrip_->test_octet_method (oc, dummy);
        }
      catch (const CORBA::Exception&){}
    }
}

void
Client_Task::test_octet_seq (void)
{
  Test::octet_load ol (this->size_);
  ol.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) this->roundtrip_->test_octet_method (ol, start);

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_long_seq (void)
{
  Test::long_load ll (this->size_);
  ll.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) this->roundtrip_->test_long_method (ll, start);

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}


void
Client_Task::test_short_seq (void)
{
  Test::short_load sl (this->size_);
  sl.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) this->roundtrip_->test_short_method (sl, start);

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_char_seq (void)
{
  Test::char_load cl (this->size_);
  cl.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) this->roundtrip_->test_char_method (cl, start);

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_longlong_seq (void)
{
  Test::longlong_load ll (this->size_);
  ll.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) this->roundtrip_->test_longlong_method (ll, start);

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}

void
Client_Task::test_double_seq (void)
{
  Test::double_load dl (this->size_);
  dl.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      ACE_hrtime_t start = ACE_OS::gethrtime ();

      (void) this->roundtrip_->test_double_method (dl, start);

      ACE_hrtime_t now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);

    }
}
