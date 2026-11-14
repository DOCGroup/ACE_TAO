#include "server_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include <cstdio>
#include <cstring>
#include <memory>

static bool quiet {};
// The test is quiet...

static ACE_TCHAR const *ior_file = ACE_TEXT ("test.ior");
// File of file to which the ior is written

static int number_of_threads {1};
// Number of server threads.

class Server_Task : public ACE_Task_Base
{
public:
  explicit Server_Task (CORBA::ORB_ptr orb);
  int svc () override;

  Server_Task (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task &&) = delete;

private:
  CORBA::ORB_var orb_;
};

Server_Task::Server_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc ()
{
  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const &)
    {
      return -1;
    }
  return 0;
}

static int
write_ior_to_file (char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_file, "w"), &std::fclose);

  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_file),
                      -1);

  int const result = std::fprintf (output_file.get (), "%s", ior);
  if (result < 0 ||
      static_cast<size_t> (result) != std::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "fprintf failed while writing %C to %s\n",
                       ior,
                       ior_file),
                      -1);

  return 0;
}

static int
parse_args (int argc,
            ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("q:f:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'q':
        quiet = true;
        break;

      case 't':
        number_of_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'f':
        ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-t number of threads]"
                           " [-f ior file]"
                           " [-q (quite)]"
                           "\n",
                           argv[0]),
                          -1);
      }

  if (number_of_threads <= 0)
    ACE_ERROR_RETURN ((LM_ERROR, "The number of server threads must be positive.\n"), -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int const parse_result = parse_args (argc, argv);
      if (parse_result != 0)
        return parse_result;

      CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      server_i server_servant (quiet,
                               orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&server_servant);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      server_var server_object = server::_narrow (object_act.in ());

      CORBA::String_var ior = orb->object_to_string (server_object.in ());

      int result = write_ior_to_file (ior.in ());
      if (result != 0)
        return result;

      Server_Task server_tasks (orb.in ());

      result = server_tasks.activate (THR_BOUND,
                                      number_of_threads);

      if (result != 0)
        return result;

      result = ACE_Thread_Manager::instance ()->wait ();

      if (result != 0)
        return result;

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server::main");
      return -1;
    }

  return 0;
}
