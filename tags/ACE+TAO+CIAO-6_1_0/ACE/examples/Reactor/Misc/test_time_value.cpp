// $Id$

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"



inline int my_abs (int d) { return d > 0 ? d : -d; }

ostream &
operator<< (ostream &stream, const ACE_Time_Value &tv)
{
  if (tv.usec () < 0 || tv.sec () < 0)
    stream << "-";

  stream << my_abs (int (tv.sec ())) << "."
//       << setw (6) << setfill ('0')
         << my_abs (int (tv.usec ()));
  return stream;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Time_Value tv1;
  ACE_Time_Value tv2 (2);
  ACE_Time_Value tv3 (100);
  ACE_Time_Value tv4 (1, 1000000);
  ACE_Time_Value tv5 (2);
  ACE_Time_Value tv6 (1, -1000000);

  ACE_ASSERT (tv1 == ACE_Time_Value (0));
  ACE_ASSERT (tv2 < tv3);
  ACE_ASSERT (tv2 <= tv2);
  ACE_ASSERT (tv2 >= tv4);
  ACE_ASSERT (tv5 >= tv6);
  ACE_ASSERT (tv2 == ACE_Time_Value (1, 1000000));
  ACE_ASSERT (tv5 == tv4);
  ACE_ASSERT (tv2 == tv4);
  ACE_ASSERT (tv1 != tv2);
  ACE_ASSERT (tv6 == tv1);

# if defined (ACE_NDEBUG)
  ACE_UNUSED_ARG (tv1);
  ACE_UNUSED_ARG (tv2);
  ACE_UNUSED_ARG (tv3);
  ACE_UNUSED_ARG (tv4);
  ACE_UNUSED_ARG (tv5);
  ACE_UNUSED_ARG (tv6);
# endif /* ACE_NDEBUG */

  cout << "0,0 :\t\t"        << ACE_Time_Value (0,0) << endl;
  cout << "-0,0 :\t\t"       << ACE_Time_Value (-0,0) << endl;
  cout << "0,-0 :\t\t"       << ACE_Time_Value (0,-0) << endl;
  cout << "-0,-0 :\t\t"      << ACE_Time_Value (-0,-0) << endl;
  cout << endl;

  cout << "0,1 :\t\t"        << ACE_Time_Value (0,1) << endl;
  cout << "1,0 :\t\t"        << ACE_Time_Value (1,0) << endl;
  cout << "-1,0 :\t\t"       << ACE_Time_Value (-1,0) << endl;
  cout << "-1,-0 :\t\t"      << ACE_Time_Value (-1,-0) << endl;
  cout << endl;

  cout << "1,1 :\t\t"        << ACE_Time_Value (1,1) << endl;
  cout << "-1,1 :\t\t"       << ACE_Time_Value (-1,1) << endl;
  cout << "1,-1 :\t\t"       << ACE_Time_Value (1,-1) << endl;
  cout << "-1,-1 :\t\t"      << ACE_Time_Value (-1,-1) << endl;
  cout << endl;

  cout << "1,-1111111 :\t"   << ACE_Time_Value (1,-1111111) << endl;
  cout << "1,-100000 :\t"    << ACE_Time_Value (1,-100000) << endl;
  cout << "1,-1000000 :\t"   << ACE_Time_Value (1,-1000000) << endl;
  cout << "-1,1000000 :\t"   << ACE_Time_Value (-1,1000000) << endl;
  cout << "5,-1000000 :\t"   << ACE_Time_Value (5,-1000000) << endl;
  cout << "5,-1500000 :\t"   << ACE_Time_Value (5,-1500000) << endl;
  cout << "2,-2500000 :\t"   << ACE_Time_Value (2,-2500000) << endl;
  cout << "2,-4500000 :\t"   << ACE_Time_Value (2,-4500000) << endl;

  return 0;
}
