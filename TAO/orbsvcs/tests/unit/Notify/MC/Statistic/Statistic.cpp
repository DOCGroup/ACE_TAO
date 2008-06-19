// $Id$

#include "orbsvcs/Notify/MonitorControl/Statistic.h"

#include "ace/Log_Msg.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

void
error (const char* msg)
{
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
  try
    {
      TAO_Statistic counter ("counter", TAO_Statistic::TS_COUNTER);
      counter.receive (0.0);
      counter.receive (0.0);
      counter.receive (0.0);
      
      if (counter.last_sample() != 3)
        {
          error ("Counter TAO_Statistic::receive() failed");
        }

      TAO_Statistic number ("number", TAO_Statistic::TS_NUMBER);
      number.receive (static_cast<size_t> (8));
      number.receive (static_cast<size_t> (10));
      number.receive (static_cast<size_t> (42));
      
      if (number.average() != 20)
        {
          error ("Number TAO_Statistic::average() failed");
        }
        
      if (number.minimum_sample() != 8)
        {
          error ("Number TAO_Statistic::minimum_sample() failed");
        }
        
      if (number.maximum_sample() != 42)
        {
          error ("Number TAO_Statistic::maximum_sample() failed");
        }
        
      if (number.sum_of_squares() != 1928)
        {
          error ("Number TAO_Statistic::sum_of_squares() failed");
        }

      TAO_Statistic stime ("time", TAO_Statistic::TS_TIME);
      stime.receive (1183466309.01234);
      stime.receive (1183466377.9922);
      stime.receive (1083466309.88374);
      
      if (stime.average() != 1150132998.96276)
        {
          error ("Time TAO_Statistic::average() failed");
        }
        
      if (stime.minimum_sample() != 1083466309.88374)
        {
          error ("Time TAO_Statistic::minimum_sample() failed");
        }
        
      if (stime.maximum_sample() != 1183466377.9922)
        {
         error ("Time TAO_Statistic::maximum_sample() failed");
        }

      TAO_Statistic interval ("interval", TAO_Statistic::TS_INTERVAL);
      interval.receive (.8);
      interval.receive (.1);
      interval.receive (.42);
      
      // This multiplication and casting is necessary since the machine
      // representation of the floating point values in the receive()
      // calls are not exactly what is represented by the text of the code.
      if (static_cast<int> (interval.average() * 100) != 44)
        {
          error ("Interval TAO_Statistic::average() failed");
        }
        
      if (interval.minimum_sample() != .1)
        {
          error ("Interval TAO_Statistic::minimum_sample() failed");
        }
        
      if (interval.maximum_sample() != .8)
        {
          error ("Interval TAO_Statistic::maximum_sample() failed");
        }
        
      if (static_cast<int> (interval.sum_of_squares() * 10000) != 8264)
        {
         error ("Interval TAO_Statistic::sum_of_squares() failed");
        }

      TAO_Statistic list ("list", TAO_Statistic::TS_LIST);
      TAO_Statistic::List l;
      l.push_back ("one");
      l.push_back ("two");
      l.push_back ("three");
      list.receive (l);
      
      if (counter.count () != 3)
        {
          error ("List TAO_Statistic::receive() failed");
        }

      /// Test for memory leaks (with valgrind).
      list.clear ();
      list.receive (l);
    }
  catch (...)
    {
      error ("Caught an unexpected exception type");
    }

  return 0;
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

