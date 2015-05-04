
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

#include "ace/Truncate.h"

int
test_assignments ()
{
  int errors {};
  ACE_Time_Value tv { std::chrono::nanoseconds {100} };
  if (tv.sec () != 0 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::nanoseconds (100) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::nanoseconds {10005} };
  if (tv.sec () != 0 || tv.usec () != 10)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::nanoseconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=10> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::microseconds {1} };
  if (tv.sec () != 0 || tv.usec () != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::microseconds (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=1> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::microseconds {10005} };
  if (tv.sec () != 0 || tv.usec () != 10005)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::microseconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=10005> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  std::chrono::milliseconds ms_test { tv.msec () };
  if (ms_test.count () != 10)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after get_chrono_msec. ")
                ACE_TEXT ("Expected <10> - got <%q>\n"),
                ms_test.count ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::milliseconds {1} };
  if (tv.sec () != 0 || tv.usec () != 1000)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::milliseconds (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=0,usec=1000> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::milliseconds {10005} };
  if (tv.sec () != 10 || tv.usec () != 5000)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::milliseconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=10,usec=5000> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::seconds {1} };
  if (tv.sec () != 1 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::seconds (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=1,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::seconds {10005} };
  if (tv.sec () != 10005 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::seconds (10005) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=10005,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::hours {1} };
  if (tv.sec () != 3600 || tv.usec () != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("std::chrono::hours (1) to an ACE_Time_Value. ")
                ACE_TEXT ("<sec=3600,usec=0> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  tv = ACE_Time_Value { std::chrono::hours {10005} };
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
  std::chrono::duration<double, std::ratio<(24*3600)>> half_day {0.5};
  std::chrono::microseconds const usec {
    2 * (
    std::chrono::duration_cast<std::chrono::microseconds> (
      half_day +
      std::chrono::hours {3} + std::chrono::minutes {24} +
      std::chrono::seconds {54} + std::chrono::milliseconds {238} +
      std::chrono::microseconds {754} + std::chrono::nanoseconds {342}))
  };


  tv = ACE_Time_Value {usec};

  //                       half a day  3 hours   24 minutes 54 seconds
  time_t expected_sec = { ((12*3600) + (3*3600) + (24*60) + 54 ) * 2 };
  //                              238 milli    usec  342 nano
  suseconds_t expected_usec = { ((238*1000) + 754 + 0) * 2 };

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

  tv.set (std::chrono::milliseconds {1120});
  if (tv.sec () != 1 || tv.usec () != 120 * std::kilo::num)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after converting ")
                ACE_TEXT ("a std::chrono::milliseconds of 1120 to an ACE_Time_Value ")
                ACE_TEXT ("Expected <sec=1,usec=120000> - got <sec=%d,usec=%d>\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }

  return errors;
}

int
test_streamers ()
{
  int errors {};

  // Three days, 13 hours, 54 seconds, 25 milliseconds and 132 microseconds
  constexpr int nr_hours { (3*24) + 13 };

  std::chrono::hours day_test_h {nr_hours};
  std::chrono::seconds day_test_s {54};
  std::chrono::milliseconds day_test_ms {25};
  std::chrono::microseconds day_test_us {132};

  std::chrono::seconds day_test_ts { day_test_h+day_test_s };
  std::chrono::microseconds day_test_tus { day_test_ms+day_test_us };
  ACE_Time_Value const test_day {
    ACE_Time_Value { day_test_ts.count (), ACE_Utils::truncate_cast<suseconds_t>(day_test_tus.count ()) }};

  constexpr int expected_min  {nr_hours * 60};
  constexpr int64_t expected_sec  { expected_min * 60 + 54 };
  constexpr int64_t expected_msec { (expected_sec * std::kilo::num) + 25 };
  constexpr int64_t expected_usec { (expected_msec * std::kilo::num) + 132 };
  constexpr int64_t expected_nsec { (expected_usec * std::kilo::num) };

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



  ACE_Time_Value const test_sec {12, 132};
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

  ACE_Time_Value const test_sec2 { ACE_Time_Value {12, 6 * std::mega::num} };
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
  if (ms.count () != 12 * std::kilo::num)
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
  if (ms2.count () != 18 * std::kilo::num)
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
  if (us.count () != (12 * std::mega::num) + 132)
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
  if (us2.count () != 18 * std::mega::num)
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
test_ace_time_value_operators ()
{
  int errors {};

  std::chrono::seconds const sec {2};
  std::chrono::microseconds const usec {3000};

  std::chrono::milliseconds const msec {
    std::chrono::duration_cast<std::chrono::milliseconds>(sec) +
    std::chrono::duration_cast<std::chrono::milliseconds>(usec) };


  ACE_Time_Value tv;
  tv = msec;
  tv += std::chrono::milliseconds {300};
  if (tv.sec () != 2 || tv.usec () != 303 * std::kilo::num)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding a duration ")
                ACE_TEXT ("of 300 ms. Expected <sec=2,usec=3300> - got <sec=%d,")
                ACE_TEXT ("usec=%d>.\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }
  tv -= std::chrono::microseconds {400};
  if (tv.sec () != 2 || tv.usec () != 302600)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting a duration ")
                ACE_TEXT ("of 400 us. Expected <sec=2,usec=3300> - got <sec=%d,")
                ACE_TEXT ("usec=%d>.\n"),
                tv.sec (), tv.usec ()));
    ++errors;
  }
  return errors;
}

int
test_chrono_operators ()
{
  int errors {};

  std::chrono::hours hr {1};
  ACE_Time_Value const tv_hr {3645, 0};
  hr += tv_hr;
  if (hr.count () != 2)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::hours of 1. ")
                ACE_TEXT ("Expected <2> - got <%d>.\n"),
                tv_hr.sec (), tv_hr.usec (), hr.count ()));
    ++errors;
  }

  hr -= tv_hr;
  if (hr.count () != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::hours of 2. ")
                ACE_TEXT ("Expected <1> - got <%d>.\n"),
                tv_hr.sec (), tv_hr.usec (), hr.count ()));
    ++errors;
  }


  std::chrono::minutes mn {1};
  ACE_Time_Value const tv_min {75, 0};
  mn += tv_min;
  if (mn.count () != 2)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::minutes of 1. ")
                ACE_TEXT ("Expected <2> - got <%d>.\n"),
                tv_min.sec (), tv_min.usec (), mn.count ()));
    ++errors;
  }

  mn -= tv_min;
  if (mn.count () != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::minutes of 2. ")
                ACE_TEXT ("Expected <1> - got <%d>.\n"),
                tv_min.sec (), tv_min.usec (), mn.count ()));
    ++errors;
  }

  std::chrono::seconds sec {1};
  ACE_Time_Value const tv_sec {1, ACE_Utils::truncate_cast<suseconds_t>(std::mega::num)};
  sec += tv_sec;
  if (sec.count () != 3)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::seconds of 1. ")
                ACE_TEXT ("Expected <3> - got <%d>.\n"),
                tv_sec.sec (), tv_sec.usec (), sec.count ()));
    ++errors;
  }

  sec -= tv_sec;
  if (sec.count () != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::seconds of 3. ")
                ACE_TEXT ("Expected <1> - got <%d>.\n"),
                tv_sec.sec (), tv_sec.usec (), sec.count ()));
    ++errors;
  }

  std::chrono::milliseconds msec {400};
  ACE_Time_Value const tv_msec {1, 3000};
  msec += tv_msec;
  if (msec.count () != 1403)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::milliseconds of 400. ")
                ACE_TEXT ("Expected <1403> - got <%d>.\n"),
                tv_msec.sec (), tv_msec.usec (), msec.count ()));
    ++errors;
  }

  msec -= tv_msec;
  if (msec.count () != 400)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::milliseconds of 1403. ")
                ACE_TEXT ("Expected <400> - got <%d>.\n"),
                tv_msec.sec (), tv_msec.usec (), msec.count ()));
    ++errors;
  }

  std::chrono::microseconds usec {400};
  ACE_Time_Value const tv_usec {0, 3000};
  usec += tv_usec;
  if (usec.count () != 3400)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::microseconds of 400. ")
                ACE_TEXT ("Expected <3400> - got <%d>.\n"),
                tv_usec.sec (), tv_usec.usec (), usec.count ()));
    ++errors;
  }

  usec -= tv_usec;
  if (usec.count () != 400)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::microseconds of 3400. ")
                ACE_TEXT ("Expected <400> - got <%d>.\n"),
                tv_usec.sec (), tv_usec.usec (), usec.count ()));
    ++errors;
  }

  std::chrono::nanoseconds nsec {4000};
  nsec += tv_usec;
  if (nsec.count () != 3004 * std::kilo::num)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after adding an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::nanoseconds of 4000. ")
                ACE_TEXT ("Expected <3004000> - got <%d>.\n"),
                tv_usec.sec (), tv_usec.usec (), nsec.count ()));
    ++errors;
  }

  nsec -= tv_usec;
  if (nsec.count () != 4000)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) unexpected value after substracting an ACE_Time_Value ")
                ACE_TEXT ("of <sec=%d,usec=%d> to a std::chrono::nanoseconds of 3004000. ")
                ACE_TEXT ("Expected <4000> - got <%d>.\n"),
                tv_usec.sec (), tv_usec.usec (), nsec.count ()));
    ++errors;
  }

  return errors;
}

int
test_chrono ()
{
  int errors = test_assignments ();
  errors += test_streamers ();
  errors += test_ace_time_value_operators ();
  errors += test_chrono_operators ();
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
