// -*- C++ -*-

//=============================================================================
/**
 *
 */
//=============================================================================

#include "cuts/BDC/Benchmark_Data_CollectorC.h"
#include "ace/OS_NS_unistd.h"

int main (int argc, char * argv [])
{
  if (argc == 1)
  {
    ACE_ERROR ((LM_ERROR,
                "missing parameter - expecting location of BDC ior\n"));
    return 0;
  }

  try
  {
    CORBA::ORB_var the_ORB = CORBA::ORB_init (argc, argv);
    CORBA::Object_var obj = the_ORB->string_to_object (argv[1]);

    if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - failed to locate IOR file\n"));
      return 0;
    }

    ::CUTS::Benchmark_Data_Collector_var the_BDC =
      ::CUTS::Benchmark_Data_Collector::_narrow (obj.in ());

    if (CORBA::is_nil (the_BDC.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - IOR was not of Benchmark_Data_Collector type\n"));
      return 0;
    }

    // Right now we are going into an infinite loop. Eventually we will
    // add more control to this admin program.

    ACE_DEBUG ((LM_DEBUG, "[%M] -%T - activating control agent\n"));

    while (1)
    {
      ACE_DEBUG ((LM_INFO, "[%M] -%T - collecting performance metrics\n"));

      try
      {
        CUTS::BDC_Control_Handle_var controls = the_BDC->provide_controls ();
        controls->collect_performance_data ();

        ACE_DEBUG ((LM_DEBUG, "[%M] -%T - done collecting performance metrics\n"));
      }
      catch (CORBA::Exception & ex)
      {
        ACE_ERROR ((LM_ERROR, "[%M] -%T - %s\n", ex._info ().c_str ()));
      }
      catch (...)
      {
        ACE_ERROR ((LM_ERROR, "[%M] -%T - caught unknown exception\n"));
      }

      // Sleep for 10 seconds.
      ACE_OS::sleep (10);
    }
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR,
                "[%M] -%T - caught unknown exception\n"));
  }

  return 0;
}

