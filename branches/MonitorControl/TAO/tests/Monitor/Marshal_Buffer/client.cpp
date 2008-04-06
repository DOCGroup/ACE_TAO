// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"
#include "ace/streams.h"

#include "tao/Monitor/Monitor.h"

#include "testC.h"

const char *ior_input_file = "file://test.ior";
const char *monitor_output_file = "monitor.ior";

/// Runs the ORB in a separate thread so we can listen
/// for calls on our MC interface without blocking.
class ORB_Runner : public ACE_Task_Base
{
private:
  CORBA::ORB_ptr orb_;

public:
  ORB_Runner (CORBA::ORB_ptr orb)
    : orb_ (orb)
  {}

  int svc (void)
  {
    this->orb_->run ();
    return 0;
  }
};

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      /// Here we take on aspects of a server role, exposing the
      /// Monitor interface of our ORB so the monitoring client
      /// can access it remotely. No need to activate the POA
      /// manager etc here - it is done in the Object_Loader code
      /// in the Monitor lib.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("Monitor");
      Monitor::MC_var monitor = Monitor::MC::_narrow (obj.in ());

      CORBA::String_var ior = orb->object_to_string (monitor.in ());

	    FILE *output_file = ACE_OS::fopen (monitor_output_file, "w");

	    if (output_file == 0)
        {
	        ACE_ERROR_RETURN ((LM_ERROR,
			                        "Can't open output file for writing IOR: %s",
			                        monitor_output_file),
			                      1);
        }

	    ACE_OS::fprintf (output_file,
                        "%s",
                        ior.in ());
	    ACE_OS::fclose (output_file);

	    /// Call orb->run() in a separate thread so the MC interface can
	    /// get remote calls.
	    ORB_Runner orb_runner (orb.in ());
	    orb_runner.activate ();

      obj = orb->string_to_object (ior_input_file);

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "string_to_object failed.\n"),
                            1);
        }

      test_var target = test::_narrow (obj.in ());

      /// Let the monitor client query a time or two.
      ACE_OS::sleep (2);

      /// Create 1k, 2k and 3k strings to marshal, to force resizing of
      /// the CDR stream buffer.
      for (int i = 1; i < 4; ++i)
        {
          char *tmp = new char[1024 * i + 1];
          ACE_OS::memset (tmp, 'a', 1024 * i);
          tmp[1024 * i] = '\0';
          CORBA::String_var result = target->test_op (tmp);
          cout << "reply to client = " << ACE_OS::strlen (result.in ())
               << " bytes" << endl;
          delete [] tmp;

          ACE_OS::sleep (2);
        }

      /// Make sure the monitor client is done before we destroy the ORB
      /// and make the MC objref invalid.
      ACE_OS::sleep (15);

      orb->destroy ();
    }
  catch (CORBA::Exception &ex)
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client: exception caught - ");
      return 1;
    }

  return 0;
}
