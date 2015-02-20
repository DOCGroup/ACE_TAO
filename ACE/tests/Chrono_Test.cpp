
//=============================================================================
/**
 *  @file    Chrono_Test.cpp
 *
 *  This is a test of the usage of 'std::chrono' throughout ACE
 *  The following items are tested:
 *  - ACE_OS::sleep
 *  - ACE_Time_Value
 *
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Time_Value.h"

#if defined (ACE_HAS_CPP11)

int
test_assignments ()
{
  int errors = 0;
  ACE_Time_Value tv (std::chrono::nanoseconds (100));
  if (tv.sec () != 0 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::nanoseconds (100) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::nanoseconds (10005));
  if (tv.sec () != 0 || tv.usec () != 10)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::nanoseconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=10> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::microseconds (1));
  if (tv.sec () != 0 || tv.usec () != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::microseconds (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=1> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::microseconds (10005));
  if (tv.sec () != 0 || tv.usec () != 10005)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::microseconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=10005> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::milliseconds (1));
  if (tv.sec () != 0 || tv.usec () != 1000)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::milliseconds (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=1000> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::milliseconds (10005));
  if (tv.sec () != 10 || tv.usec () != 5000)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::milliseconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=10,usec=5000> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::seconds (1));
  if (tv.sec () != 1 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::seconds (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=1,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::seconds (10005));
  if (tv.sec () != 10005 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::seconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=10005,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::hours (1));
  if (tv.sec () != 3600 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::hours (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=3600,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value (std::chrono::hours (10005));
  if (tv.sec () != 3600*10005 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::hours (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=%d,usec=0> - got <sec=%d,usec=%d>\n"),
                3600*10005, tv.sec (), tv.usec ()));
    ++errors;
  }

  // Two times half a day, 3 hours, 24 minutes, 54 seconds, 238 milliseconds,
  // 754 microseconds and 342 nanoseconds.
  std::chrono::duration<double, std::ratio<(24*3600)>> half_day (0.5);
  std::chrono::microseconds const usec {
    2 * (
    std::chrono::duration_cast<std::chrono::microseconds> (
      half_day +
      std::chrono::hours (3) + std::chrono::minutes (24) +
      std::chrono::seconds (54) + std::chrono::milliseconds (238) +
      std::chrono::microseconds (754) + std::chrono::nanoseconds (342)))
  };


  tv = ACE_Time_Value (usec);

  //                     half a day  3 hours   24 minutes 54 seconds
  time_t expected_sec = ((12*3600) + (3*3600) + (24*60) + 54 )*2;
  //                           238 milli    usec  342 nano
  suseconds_t expected_usec = ((238*1000) + 754 + 0) *2;

  if (tv.sec () != expected_sec || tv.usec () != expected_usec)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("two times half a day, 3 hours, 24 minutes, 54 seconds, ")
                ACE_TEXT ("238 milliseconds, 754 microseconds and 342 nanoseconds ")
                ACE_TEXT ("to an ACE_Time_Value. Expected <sec=%d,usec=%d> - ")
                ACE_TEXT ("got <sec=%d,usec=%d>\n"),
                expected_sec,  expected_usec, tv.sec (), tv.usec ()));
    ++errors;
  }
  return errors;
}

int
test_streamers ()
{
  int errors = 0;

  // Three days, 13 hours, 54 seconds, 25 milliseconds and 132 microseconds
  constexpr int nr_hours { (3*24) + 13 };

  std::chrono::hours day_test_h (nr_hours);
  std::chrono::seconds day_test_s (54);
  std::chrono::milliseconds day_test_ms (25);
  std::chrono::microseconds day_test_us (132);

  std::chrono::seconds day_test_ts = day_test_h+day_test_s;
  std::chrono::microseconds day_test_tus = day_test_ms+day_test_us;
  ACE_Time_Value const test_day (
    ACE_Time_Value (day_test_ts.count (), day_test_tus.count ()));

  constexpr int expected_min  { nr_hours * 60 };
  constexpr int64_t expected_sec  { expected_min * 60 + 54 };
  constexpr int64_t expected_msec { (expected_sec * 1000) + 25 };
  constexpr int64_t expected_usec { (expected_msec * 1000) + 132 };
  constexpr int64_t expected_nsec { (expected_usec * 1000) };

  std::chrono::hours h;
  h << test_day;
  if (h.count () != nr_hours)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::hours. Expected <%d> - got <%q>.\n"),
                test_day.sec (), test_day.usec (), nr_hours, h.count ()));
    ++errors;
  }

  std::chrono::minutes m;
  m << test_day;
  if (m.count () != expected_min)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::minutes. Expected <%d> - got <%q>.\n"),
                test_day.sec (), test_day.usec (), expected_min, m.count ()));
    ++errors;
  }

  std::chrono::seconds s;
  s << test_day;
  if (s.count () != expected_sec)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::seconds. Expected <%q> - got <%q>.\n"),
                test_day.sec (), test_day.usec (), expected_sec, s.count ()));
    ++errors;
  }

  std::chrono::milliseconds ms;
  ms << test_day;
  if (ms.count () != expected_msec)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::milliseconds. Expected <%q> - got <%q>.\n"),
                test_day.sec (), test_day.usec (), expected_msec, ms.count ()));
    ++errors;
  }

  std::chrono::microseconds us;
  us << test_day;
  if (us.count () != expected_usec)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::microseconds. Expected <%q> - got <%q>.\n"),
                test_day.sec (), test_day.usec (), expected_usec, us.count ()));
    ++errors;
  }

  std::chrono::nanoseconds ns;
  ns << test_day;
  if (ns.count () != expected_nsec)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::nanoseconds. Expected <%q> - got <%q>.\n"),
                test_day.sec (), test_day.usec (), expected_nsec, ns.count ()));
    ++errors;
  }



  ACE_Time_Value const test_sec (12, 132);
  // Seconds
  s << test_sec;
  if (s.count () != 12)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::seconds. Expected <%d> - got <%q>.\n"),
                test_sec.sec (), test_sec.usec (), test_sec.sec (), s.count ()));
    ++errors;
  }

  ACE_Time_Value const test_sec2 (ACE_Time_Value (12, 6000000));
  std::chrono::seconds s2;
  s2 << test_sec2;
  if (s2.count () != 18)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::seconds. Expected <%d> - got <%q>.\n"),
                test_sec2.sec (), test_sec2.usec (), test_sec2.sec (), s2.count ()));
    ++errors;
  }

  // Milliseconds
  ms << test_sec;
  if (ms.count () != 12*std::kilo::num)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::milliseconds. Expected <%d> - got <%q>.\n"),
                test_sec.sec (), test_sec.usec (), 12000, ms.count ()));
    ++errors;
  }

  std::chrono::milliseconds ms2;
  ms2 << test_sec2;
  if (ms2.count () != 18*std::kilo::num)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::milliseconds. Expected <%d> - got <%q>.\n"),
                test_sec2.sec (), test_sec2.usec (), 18000, ms2.count ()));
    ++errors;
  }

  // Microseconds
  us << test_sec;
  if (us.count () != (12*std::mega::num) + 132)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::microseconds. Expected <%d> - got <%q>.\n"),
                test_sec.sec (), test_sec.usec (), test_sec.sec (), us.count ()));
    ++errors;
  }

  std::chrono::microseconds us2;
  us2 << test_sec2;
  if (us2.count () != 18*std::mega::num)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after streaming an ")
                ACE_TEXT ("ACE_Time_Value <sec=%d,usec=%d> into ")
                ACE_TEXT ("std::chrono::microseconds. Expected <%d> - got <%q>.\n"),
                test_sec2.sec (), test_sec2.usec (), test_sec2.sec (), us2.count ()));
    ++errors;
  }

  return errors;
}

int
test_ace_time_value ()
{
  int errors = test_assignments ();
  errors += test_streamers ();
  return errors;
}

int
test_sleep ()
{
  return 0;
}

int
test_chrono ()
{
  int errors = test_ace_time_value ();
  errors += test_sleep ();
  return errors;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Chrono_Test"));

  int errors = test_chrono ();

  ACE_END_TEST;
  return errors;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Chrono_Test"));

  ACE_ERROR ((LM_INFO, ACE_TEXT ("std::chrono is not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_CPP11 */
