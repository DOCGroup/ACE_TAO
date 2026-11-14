#include "Client_Task.h"
#include "ace/OS_NS_time.h"

Client_Task::Client_Task (ACE_TCHAR const *data_type,
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
Client_Task::svc ()
{
  try
    {
      this->validate_connection ();

      if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("octet")) == 0)
        this->test_sequence<Test::octet_load> (&Test::Roundtrip::test_octet_method);
      else if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("long")) == 0)
        this->test_sequence<Test::long_load> (&Test::Roundtrip::test_long_method);
      else if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("short")) == 0)
        this->test_sequence<Test::short_load> (&Test::Roundtrip::test_short_method);
      else if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("char")) == 0)
        this->test_sequence<Test::char_load> (&Test::Roundtrip::test_char_method);
      else if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("longlong")) == 0)
        this->test_sequence<Test::longlong_load> (&Test::Roundtrip::test_longlong_method);
      else if (ACE_OS::strcmp (this->data_type_, ACE_TEXT("double")) == 0)
        this->test_sequence<Test::double_load> (&Test::Roundtrip::test_double_method);
    }
  catch (CORBA::Exception const &)
    {
      return 0;
    }
  return 0;
}

template <typename Sequence, typename Operation>
void
Client_Task::test_sequence (Operation const operation)
{
  Sequence sequence (this->size_);
  sequence.length (this->size_);

  for (int i = 0; i != this->niterations_; ++i)
    {
      CORBA::ULongLong const start = ACE_OS::gethrtime ();
      (void) (this->roundtrip_->*operation) (sequence, start);
      ACE_hrtime_t const now = ACE_OS::gethrtime ();
      this->latency_.sample (now - start);
    }
}

void
Client_Task::accumulate_and_dump (ACE_Basic_Stats &totals,
                                  ACE_TCHAR const *msg,
                                  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  totals.accumulate (this->latency_);
  this->latency_.dump_results (msg, gsf);
}

void
Client_Task::validate_connection ()
{
  CORBA::ULongLong const dummy {};
  Test::octet_load octets;

  for (int i = 0; i != 100; ++i)
    {
      try
        {
          (void) this->roundtrip_->test_octet_method (octets, dummy);
        }
      catch (CORBA::Exception const &)
        {
        }
    }
}
