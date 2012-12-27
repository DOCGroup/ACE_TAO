//
// $Id$
//

#include "ace/Task.h"
#include "ace/Atomic_Op_T.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "HelloC.h"

ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
CORBA::ULong payload_length = 1000;
CORBA::ULong client_threads = 5;
CORBA::ULong id_offset = 0;
CORBA::ULong payload_calls = 100;
CORBA::ULong sleep_millis = 100;
bool do_shutdown = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:p:t:f:c:s:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'p':
        payload_length = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case 't':
        client_threads = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case 'f':
        id_offset = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case 'c':
        payload_calls = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case 's':
        sleep_millis = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case 'x':
        do_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k <ior> ")
                           ACE_TEXT ("-p <payload_length> ")
                           ACE_TEXT ("-t <client_threads> ")
                           ACE_TEXT ("-f <id_offset> ")
                           ACE_TEXT ("-c <payload_calls> ")
                           ACE_TEXT ("-s <sleep_millis> ")
                           ACE_TEXT ("-x")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("args to be used: -k '%s' -p %d -t %d -f %d -c %d -s %d%s\n"),
              ior,
              payload_length,
              client_threads,
              id_offset,
              payload_calls,
              sleep_millis,
              do_shutdown ? " -x" : ""));

  // Indicates sucessful parsing of the command line
  return 0;
}

class ClientThread : public ACE_Task_Base
{
public:
  ClientThread (Test::UIPMC_Object_ptr obj, CORBA::ULong payload,
                CORBA::ULong offset, CORBA::ULong calls,
                CORBA::ULong sleep)
    : obj_ (Test::UIPMC_Object::_duplicate (obj))
    , payload_ (payload)
    , calls_ (calls)
    , id_ (offset)
    , sleep_ (sleep)
  {
  }

  virtual int svc (void)
  {
    try
      {
        CORBA::ULong i = this->id_++;

        Test::Octets seq (this->payload_);
        seq.length (this->payload_);

        CORBA::Octet *buff = seq.get_buffer ();
        ACE_OS::memset (buff, Test::ClientIDs[i], this->payload_);

        for (CORBA::ULong j = 0; j < this->calls_; ++j)
          {
            this->obj_->process (seq);
          }
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught in ClientThread:");
        return -1;
      }

    return 0;
  }

private:
  Test::UIPMC_Object_var obj_;

  CORBA::ULong payload_;
  CORBA::ULong calls_;

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::ULong> id_;

  CORBA::ULong sleep_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: wrong arguments\n")),
                          -1);

      if (id_offset + client_threads >= ACE_OS::strlen (Test::ClientIDs))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: too many clients\n")),
                          -1);

      CORBA::Object_var obj = orb->string_to_object (ior);

      // Create Hello reference.
      Test::Hello_var hello = Test::Hello::_narrow (obj.in ());

      if (CORBA::is_nil (hello.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: nil Hello object\n")),
                          -1);

      if (do_shutdown)
        hello->shutdown ();
      else
        {
          Test::UIPMC_Object_var uipmc_obj = hello->get_object ();

          {
            // start clients
            ClientThread cln_thr (uipmc_obj.in (), payload_length,
                                  id_offset, payload_calls, sleep_millis);
            cln_thr.activate (THR_NEW_LWP | THR_JOINABLE, client_threads);
            cln_thr.wait ();
          }

          // Give a chance to flush all OS buffers for client.
          while (orb->work_pending ())
            orb->perform_work ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client main ():");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nClient finished successfully\n")));
  return 0;
}
