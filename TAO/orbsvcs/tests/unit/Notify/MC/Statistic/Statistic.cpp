// $Id$

#include "ace/Log_Msg.h"
#include "ace/Monitor_Base.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

void
error (const char* msg)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (msg);
#endif /* ACE_NLOGGING */
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      Monitor_Base counter ("counter", Monitor_Control_Types::MC_COUNTER);
      counter.receive (0.0);
      counter.receive (0.0);
      counter.receive (0.0);

      if (!ACE::is_equal (counter.last_sample(), 3.0))
        {
          error ("Counter Monitor_Control::receive() failed");
        }

      Monitor_Base number ("number", Monitor_Control_Types::MC_NUMBER);
      size_t size = 8;
      number.receive (size);
      size = 10;
      number.receive (size);
      size = 42;
      number.receive (size);

      if (!ACE::is_equal (number.average (), 20.0))
        {
          error ("Number Monitor_Base::average() failed");
        }

      if (!ACE::is_equal (number.minimum_sample (), 8.0))
        {
          error ("Number Monitor_Base::minimum_sample() failed");
        }

      if (!ACE::is_equal (number.maximum_sample (), 42.0))
        {
          error ("Number Monitor_Base::maximum_sample() failed");
        }

      if (!ACE::is_equal (number.sum_of_squares (), 1928.0))
        {
          error ("Number Monitor_Base::sum_of_squares() failed");
        }

      Monitor_Base stime ("time", Monitor_Control_Types::MC_TIME);
      stime.receive (1183466309.01234);
      stime.receive (1183466377.9922);
      stime.receive (1083466309.88374);

      if (!ACE::is_equal (stime.average (), 1150132998.96276))
        {
          error ("Time Monitor_Base::average() failed");
        }

      if (!ACE::is_equal (stime.minimum_sample (), 1083466309.88374))
        {
          error ("Time Monitor_Base::minimum_sample() failed");
        }

      if (!ACE::is_equal (stime.maximum_sample (), 1183466377.9922))
        {
          error ("Time Monitor_Base::maximum_sample() failed");
        }

      Monitor_Base interval ("interval", Monitor_Control_Types::MC_INTERVAL);
      interval.receive (.8);
      interval.receive (.1);
      interval.receive (.42);

      // This multiplication and casting is necessary since the machine
      // representation of the floating point values in the receive()
      // calls are not exactly what is represented by the text of the code.
      if (static_cast<int> (interval.average () * 100) != 44)
        {
          error ("Interval Monitor_Base::average() failed");
        }

      if (!ACE::is_equal (interval.minimum_sample (), 0.1))
        {
          error ("Interval Monitor_Base::minimum_sample() failed");
        }

      if (!ACE::is_equal (interval.maximum_sample (), 0.8))
        {
          error ("Interval Monitor_Base::maximum_sample() failed");
        }

      if (static_cast<int> (interval.sum_of_squares () * 10000) != 8264)
        {
          error ("Interval Monitor_Base::sum_of_squares() failed");
        }

      Monitor_Base list ("list", Monitor_Control_Types::MC_LIST);
      Monitor_Control_Types::NameList l;
      l.push_back ("one");
      l.push_back ("two");
      l.push_back ("three");
      list.receive (l);

      if (list.count () != 3)
        {
          error ("List Monitor_Base::receive() failed");
        }

      /// Test for memory leaks (with valgrind).
      list.clear ();
      list.receive (l);
    }
  catch (...)
    {
      error ("Caught an unexpected exception type");
    }

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

