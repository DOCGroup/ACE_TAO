#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file1 {ACE_TEXT ("test1.ior")};
  ACE_TCHAR const *ior_output_file2 {ACE_TEXT ("test2.ior")};
  CORBA::Short poa_priority {-1};
  CORBA::Short object_priority {-1};
};

class Test_i : public POA_Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  void test_method (CORBA::Short priority) override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var const orb_;
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Short priority)
{
  CORBA::Object_var const object = this->orb_->resolve_initial_references ("RTCurrent");
  RTCORBA::Current_var const current = RTCORBA::Current::_narrow (object.in ());

  if (CORBA::is_nil (object.in ()))
    throw CORBA::INTERNAL ();

  CORBA::Short const servant_thread_priority = current->the_priority ();
  if (servant_thread_priority != priority)
    ACE_DEBUG ((LM_DEBUG,
                "ERROR: servant thread priority is not equal to method argument.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Server_Declared priority: %d  Servant thread priority: %d\n",
              priority,
              servant_thread_priority));
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:o:a:b:"));
  int option {};
  int result {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'p':
        options.ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'o':
        options.ior_output_file2 = get_opts.opt_arg ();
        break;

      case 'a':
        result = ::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                           "%hd",
                           &options.poa_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR, "Unable to process <-a> option"), -1);
        break;

      case 'b':
        result = ::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                           "%hd",
                           &options.object_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR, "Unable to process <-b> option"), -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <iorfile1> "
                           "-o <iorfile2> "
                           "-a <poa_priority> "
                           "-b <object_priority> "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.poa_priority < 0 || options.object_priority < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Valid poa and object priorities must be specified.\n"
                       "See README file for more info\n"),
                      -1);

  return 0;
}

int
check_for_nil (CORBA::Object_ptr const object, char const *message)
{
  if (CORBA::is_nil (object))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%C> is nil\n",
                       message),
                      -1);

  return 0;
}

int
create_object (RTPortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               CORBA::Short priority,
               ACE_TCHAR const *filename)
{
  PortableServer::ObjectId_var id;
  if (priority > -1)
    id = poa->activate_object_with_priority (server_impl, priority);
  else
    id = poa->activate_object (server_impl);

  CORBA::Object_var const server = poa->id_to_reference (id.in ());
  CORBA::String_var const ior = orb->object_to_string (server.in ());
  ACE_DEBUG ((LM_DEBUG, "<%C>\n\n", ior.in ()));

  if (filename != nullptr)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
        ACE_OS::fopen (filename, ACE_TEXT ("w")), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           filename),
                          -1);

      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
    }

  return 0;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Server_Options const &options);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var const orb_;
  Server_Options const options_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Server_Options const &options)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb)),
    options_ (options)
{
}

int
Task::svc ()
{
  try
    {
      CORBA::Object_var object = this->orb_->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var const rt_orb = RTCORBA::RTORB::_narrow (object.in ());
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      object = this->orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (2);
      poa_policy_list[0] = rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                                                 this->options_.poa_priority);
      poa_policy_list[1] = root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("Child_POA", poa_manager.in (), poa_policy_list);
      RTPortableServer::POA_var const rt_poa = RTPortableServer::POA::_narrow (child_poa.in ());
      if (check_for_nil (rt_poa.in (), "RTPOA") == -1)
        return -1;

      Test_i server_impl (this->orb_.in ());
      ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
      int result = create_object (rt_poa.in (),
                                 this->orb_.in (),
                                 &server_impl,
                                 -1,
                                 this->options_.ior_output_file1);
      if (result == -1)
        return -1;

      ACE_DEBUG ((LM_DEBUG, "\nActivated object two as "));
      result = create_object (rt_poa.in (),
                              this->orb_.in (),
                              &server_impl,
                              this->options_.object_priority,
                              this->options_.ior_output_file2);
      if (result == -1)
        return -1;

      poa_manager->activate ();
      this->orb_->run ();
      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Unexpected exception caught in Server_Declared test server:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      if (!check_supported_priorities (orb.in ()))
        return 2;

      ACE_Thread_Manager thread_manager;
      Task task (thread_manager, orb.in (), options);
      long const flags = THR_NEW_LWP |
                         THR_JOINABLE |
                         orb->orb_core ()->orb_params ()->thread_creation_flags ();

      int result = task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot create thread with scheduling policy %s\n"
                               "because the user does not have the appropriate privileges, "
                               "terminating program....\n"
                               "Check svc.conf options and/or run as root\n",
                               sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                              2);
          else
            ACE_ASSERT (0);
        }

      result = thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
