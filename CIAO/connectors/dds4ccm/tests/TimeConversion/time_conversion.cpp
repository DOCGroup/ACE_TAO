// $Id$

#include "dds4ccm/impl/TimeUtilities.h"
#include "ace/OS_NS_sys_time.h"

int
ACE_TMAIN(int , ACE_TCHAR **)
{
  try
    {
      ::DDS::Time_t dds_time;
      ACE_Time_Value ace_time = ACE_OS::gettimeofday ();

      dds_time <<= ace_time;
      if (dds_time.sec != ace_time.sec ())
        {
          ACE_ERROR ((LM_ERROR, "ERROR : TimeConversion - Conversion from "
                                "ACE_Time_Value to DDS::Time_t "
                                "seconds expected <%u> - seconds found <%u>\n",
                                ace_time.sec (), dds_time.sec));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "TimeConversion - Conversion from "
                                "ACE_Time_Value to DDS::Time_t "
                                "regarding seconds succeeded\n"));
        }
      if (dds_time.nanosec / 1000 != static_cast <CORBA::ULong>(ace_time.usec ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR : TimeConversion - Conversion from "
                                "ACE_Time_Value to DDS::Time_t "
                                "micro seconds expected <%u> - micro seconds found <%u>\n",
                                ace_time.usec (), dds_time.nanosec/1000));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "TimeConversion - Conversion from "
                                "ACE_Time_Value to DDS::Time_t "
                                "regarding micro seconds succeeded\n"));
        }

      ::DDS::Time_t dds_now;
      ACE_Time_Value ace_now = ACE_OS::gettimeofday ();
      dds_now <<= ace_now;
      //create real nanoseconds.
      dds_now.nanosec = 1234567890;
      ace_time <<= dds_now;

      if (ace_time.sec () != dds_now.sec)
        {
          ACE_ERROR ((LM_ERROR, "ERROR : TimeConversion - Conversion from "
                                "DDS::Time_t to ACE_Time_Value "
                                "seconds expected <%u> - seconds found <%u>\n",
                                dds_now.sec, ace_time.sec ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "TimeConversion - Conversion from "
                                "DDS::Time_t to ACE_Time_Value "
                                "regarding seconds succeeded\n"));
        }
      // The resolution of an ACE_Time_Value is less than the resolution of a
      // DDS::Time_T. Therefor don't multiply ace_time.usec by 1000 but devide
      // dds_time.nanosec by 1000.
      if (static_cast<CORBA::ULong>(ace_time.usec ()) != dds_now.nanosec/1000)
        {
          ACE_ERROR ((LM_ERROR, "ERROR : TimeConversion - Conversion from "
                                "DDS::Time_t to ACE_Time_Value "
                                "nano seconds expected <%u> - nano seconds found <%u>\n",
                                dds_now.nanosec/1000, ace_time.usec ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "TimeConversion - Conversion from "
                                "DDS::Time_t to ACE_Time_Value "
                                "regarding nano seconds succeeded\n"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
