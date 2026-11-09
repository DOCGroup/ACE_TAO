#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

#include <memory>

#include "../check_supported_priorities.cpp"

class test_i :
  public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  ~test_i () override = default;

  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

  PortableServer::POA_ptr _default_POA () override;

private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::method ()
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::method\n"));
}

void
test_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::shutdown\n"));

  this->orb_->shutdown (false);
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

struct Server_Options
{
  ACE_TCHAR const *persistent_ior {ACE_TEXT ("persistent_ior")};
  ACE_TCHAR const *transient_ior {ACE_TEXT ("transient_ior")};
  ACE_TCHAR const *tp_persistent_ior {ACE_TEXT ("tp_persistent_ior")};
  bool debug {};
  CORBA::ULong stacksize {};
  CORBA::ULong static_threads {2};
  CORBA::ULong dynamic_threads {};
  RTCORBA::Priority default_thread_priority {};
  bool allow_request_buffering {};
  CORBA::ULong max_buffered_requests {};
  CORBA::ULong max_request_buffer_size {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:p:t:d:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.tp_persistent_ior = get_opts.opt_arg ();
        break;

      case 'p':
        options.persistent_ior = get_opts.opt_arg ();
        break;

      case 't':
        options.transient_ior = get_opts.opt_arg ();
        break;

      case 'd':
        options.debug = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <tp persistent ior file> "
                           "-p <persistent ior file> "
                           "-t <transient ior file> "
                           "-d <debug> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

static void
write_iors_to_file (CORBA::Object_ptr object,
                    CORBA::ORB_ptr orb,
                    ACE_TCHAR const *filename,
                    bool debug)
{
  std::unique_ptr<FILE, int (*) (FILE *)> file (
    ACE_OS::fopen (filename, "w"), ACE_OS::fclose);
  ACE_ASSERT (file != nullptr);

  CORBA::String_var ior =
    orb->object_to_string (object);

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "%s: %C\n",
                filename,
                ior.in ()));

  int const result = ACE_OS::fprintf (file.get (),
                                      "%s",
                                      ior.in ());

  ACE_ASSERT (result >= 0
              && static_cast<size_t> (result) == ACE_OS::strlen (ior.in ()));
  ACE_UNUSED_ARG (result);
}

static void
create_poa_and_servant_with_persistent_policy (CORBA::ORB_ptr orb,
                                               PortableServer::POA_ptr root_poa,
                                               PortableServer::POAManager_ptr poa_manager,
                                               Server_Options const &options)
{
  CORBA::PolicyList policies;

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  CORBA::Policy_var lifespan_policy =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    lifespan_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("persistent_child",
                          poa_manager,
                          policies);

  test_i *servant {};
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this ();

  write_iors_to_file (test.in (),
                      orb,
                      options.persistent_ior,
                      options.debug);
}

static void
create_poa_and_servant_with_tp_policy (CORBA::ORB_ptr orb,
                                       RTCORBA::RTORB_ptr rt_orb,
                                       PortableServer::POA_ptr root_poa,
                                       PortableServer::POAManager_ptr poa_manager,
                                       Server_Options const &options)
{
  RTCORBA::ThreadpoolId threadpool_id = rt_orb->create_threadpool (
    options.stacksize,
    options.static_threads,
    options.dynamic_threads,
    options.default_thread_priority,
    options.allow_request_buffering,
    options.max_buffered_requests,
    options.max_request_buffer_size);

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id);

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("tp_child",
                          poa_manager,
                          policies);

  test_i *servant {};
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this ();

  write_iors_to_file (test.in (),
                      orb,
                      options.transient_ior,
                      options.debug);
}

static void
create_poa_with_tp_and_persistent_policy (CORBA::ORB_ptr orb,
                                          RTCORBA::RTORB_ptr rt_orb,
                                          PortableServer::POA_ptr root_poa,
                                          PortableServer::POAManager_ptr poa_manager,
                                          Server_Options const &options)
{
  RTCORBA::ThreadpoolId threadpool_id = rt_orb->create_threadpool (
    options.stacksize,
    options.static_threads,
    options.dynamic_threads,
    options.default_thread_priority,
    options.allow_request_buffering,
    options.max_buffered_requests,
    options.max_request_buffer_size);

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id);

  CORBA::Policy_var implicit_activation_policy =
    root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

  CORBA::Policy_var lifespan_policy =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

  CORBA::PolicyList policies;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    implicit_activation_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    lifespan_policy;

  PortableServer::POA_var poa =
    root_poa->create_POA ("tp_persistent_child",
                          poa_manager,
                          policies);

  test_i *servant {};
  ACE_NEW_THROW_EX (servant,
                    test_i (orb,
                            poa.in ()),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var safe_servant (servant);

  test_var test =
    servant->_this ();

  write_iors_to_file (test.in (),
                      orb,
                      options.tp_persistent_ior,
                      options.debug);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Server_Options options;
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        orb->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      options.default_thread_priority =
        get_implicit_thread_CORBA_priority (orb.in ());

      object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      create_poa_and_servant_with_tp_policy (orb.in (),
                                             rt_orb.in (),
                                             root_poa.in (),
                                             poa_manager.in (),
                                             options);

      create_poa_and_servant_with_persistent_policy (orb.in (),
                                                     root_poa.in (),
                                                     poa_manager.in (),
                                                     options);

      create_poa_with_tp_and_persistent_policy (orb.in (),
                                                rt_orb.in (),
                                                root_poa.in (),
                                                poa_manager.in (),
                                                options);

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::INTERNAL& ex)
    {
      if ((ex.minor() & 0x1F) == 9) {
        ACE_DEBUG ((LM_DEBUG, "You must be superuser to run "
                              "this test on this platform.\n"));
        return 0;
      }
      else {
        ex._tao_print_exception ("Exception caught");
        return -1;
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
