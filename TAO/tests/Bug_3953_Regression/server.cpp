#include "server_i.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/Messaging/Messaging.h"

#include <cstring>
#include <memory>
#include <string>

static bool quiet {};
// The test is quiet...

static std::basic_string<ACE_TCHAR> ior_file {ACE_TEXT ("test.ior")};
// File of file to which the ior is written

static int number_of_threads {1};
// Number of server threads.

class Server_Task : public ACE_Task_Base
{
public:
  explicit Server_Task (CORBA::ORB_ptr const orb);
  Server_Task (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task &&) = delete;
  int svc () override;

private:
  CORBA::ORB_var orb_ {};
};

Server_Task::Server_Task (CORBA::ORB_ptr const orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc ()
{
  try
    {
      CORBA::Object_var const object =
        this->orb_->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      TimeBase::TimeT const timeout_period = 100 * 100000;

      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        this->orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                   timeout_as_any);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE);

      policy_list[0]->destroy();

      this->orb_->run ();
    }
  catch (CORBA::Exception const &)
    {
      return -1;
    }
  return 0;
}

static int
write_ior_to_file (char const *const ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (ior_file.c_str (), "w"), ACE_OS::fclose);

  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_file.c_str ()),
                      -1);

  int const result = ACE_OS::fprintf (output_file.get (), "%s", ior);
  if (result < 0 || static_cast<size_t> (result) != std::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       ior,
                       ior_file.c_str ()),
                      -1);

  return 0;
}

static int
parse_args (int argc,
            ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("q:f:t:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  int result {};
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      result = parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var const object {orb->resolve_initial_references ("RootPOA")};

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      server_i server_servant (quiet,
                               orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&server_servant);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      server_var const server_object = server::_narrow (object_act.in ());

      CORBA::String_var const ior = orb->object_to_string (server_object.in ());

      result = write_ior_to_file (ior.in ());
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

      if (!server_servant.exception ())
        ++result;

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server::main");
      return -1;
    }

  return result;
}
