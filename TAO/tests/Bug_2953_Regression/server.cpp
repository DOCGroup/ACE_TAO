// -*- C++ -*-
#include <cstdio>
#include <memory>

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/Protocols_Hooks.h"
#include "tao/ORB_Core.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/Get_Opt.h"

#include "TestS.h"
#include "Client_Task.h"

class Test_i : public virtual POA_Test::Hello
{
public:
  Test_i (CORBA::ORB_ptr orb, Client_Task *task)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , task_ (task)
  {
  }
  Test_i (Test_i const &) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i &&) = delete;

  char *get_string () override
  {
    return CORBA::string_dup ("hello");
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override
  {
    task_->terminate_loop ();
    orb_ = CORBA::ORB::_nil ();
  }
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var orb_;
  Client_Task * const task_;
};

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            ACE_TCHAR const *&ior_a_file,
            ACE_TCHAR const *&ior_b_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'a':
        ior_a_file = get_opts.optarg;
        break;

      case 'b':
        ior_b_file = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <iorAfile>"
                           "-b <iorBfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

RTCORBA::ThreadpoolId
createThreadpool (CORBA::ORB_ptr orb, RTCORBA::RTORB_ptr rtorb, CORBA::ULong nthreads)
{
  CORBA::Object_var const obj = orb->resolve_initial_references ("RTCurrent");
  RTCORBA::Current_var const rtcurrent = RTCORBA::Current::_narrow (obj.in ());

  CORBA::ULong const stack_size {};
  CORBA::ULong const min_threads {1};
  CORBA::ULong const dynamic_threads = nthreads - min_threads;

  TAO_Protocols_Hooks * const tph =
    orb->orb_core ()->get_protocols_hooks ();

  RTCORBA::Priority default_thread_priority {};

  if (tph->get_thread_implicit_CORBA_priority (default_thread_priority) != 0)
    throw CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);

  CORBA::Boolean const do_buffering = false;
  CORBA::ULong const max_buffered_requests {};
  CORBA::ULong const max_request_buffer_size {};

  RTCORBA::ThreadpoolId pool_id {};
  try
    {
      pool_id = rtorb->create_threadpool (stack_size,
                                          min_threads,
                                          dynamic_threads,
                                          default_thread_priority,
                                          do_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size);
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("creating thread pool");
      throw;
    }
  return pool_id;
}

RTCORBA::RTORB_var
getRTORB (CORBA::ORB_ptr orb, char const *id)
{
  CORBA::Object_var const obj = orb->resolve_initial_references ("RTORB");
  RTCORBA::RTORB_var rtorb = RTCORBA::RTORB::_narrow (obj.in ());
  if (CORBA::is_nil (rtorb.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Failed getting RTORB for orb <%C>\n",
                  id));
    }
  return rtorb;
}

PortableServer::POA_var
getRootPoa (CORBA::ORB_ptr orb, char const *id)
{
  CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
  if (CORBA::is_nil (poa.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Failed getting RootPOA for orb <%C>\n",
                  id));
    }
  return poa;
}

CORBA::String_var
addServant (
  CORBA::ORB_ptr orb,
  RTCORBA::RTORB_ptr rtorb,
  PortableServer::POA_ptr poa,
  PortableServer::ServantBase *servant,
  RTCORBA::ThreadpoolId &id,
  CORBA::ULong nthreads)
{
  id = createThreadpool (orb, rtorb, nthreads);
  ACE_DEBUG ((LM_DEBUG,
              "Created threadpool with id <%d>\n",
              id));

  TAO::Utils::PolicyList_Destroyer policies (8);
  policies.length (8);
  CORBA::ULong i {};

  policies[i++] = poa->create_id_assignment_policy (PortableServer::SYSTEM_ID);
  policies[i++] = poa->create_id_uniqueness_policy (PortableServer::UNIQUE_ID);
  policies[i++] = poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);
  policies[i++] = poa->create_lifespan_policy (PortableServer::TRANSIENT);
  policies[i++] = poa->create_request_processing_policy (PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  policies[i++] = poa->create_servant_retention_policy (PortableServer::RETAIN);
  policies[i++] = poa->create_thread_policy (PortableServer::ORB_CTRL_MODEL);
  policies[i++] = rtorb->create_threadpool_policy (id);

  PortableServer::ObjectId_var const object_id = poa->activate_object (servant);
  ACE_UNUSED_ARG (object_id);
  CORBA::Object_var const ref = poa->servant_to_reference (servant);
  CORBA::String_var str = orb->object_to_string (ref.in ());

  return str;
}

void
removeServant (
  RTCORBA::RTORB_ptr rtorb,
  PortableServer::POA_ptr poa,
  PortableServer::ServantBase *servant,
  RTCORBA::ThreadpoolId id)
{
  PortableServer::ObjectId_var const oid = poa->servant_to_id (servant);
  poa->deactivate_object (oid.in ());
  rtorb->destroy_threadpool (id);
  ACE_DEBUG ((LM_DEBUG,
              "Destroyed threadpool with id <%d>\n",
              id));
}

void
shutdownORB (CORBA::ORB_ptr orb, char const *orbid)
{
  orb->shutdown (true);
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is shutdown\n",
              orbid));

  orb->destroy ();
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is destroyed\n",
              orbid));
}

int
write_iorfile (ACE_TCHAR const *ior_output_file, char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
  if (output_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                        "Cannot open output file for writing IOR: %s\n",
                        ior_output_file),
                        1);
  ACE_OS::fprintf (output_file.get (), "%s", ior);

  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_a_file = ACE_TEXT ("iorA.ior");
  ACE_TCHAR const *ior_b_file = ACE_TEXT ("iorB.ior");
  try
    {
      char const * const orbid_a = "orbidA";
      char const * const orbid_b = "orbidB";

      RTCORBA::ThreadpoolId tpid_a {};
      RTCORBA::ThreadpoolId tpid_b {};

      // server - write our iors to two files
      CORBA::ORB_var orb_a = CORBA::ORB_init (argc, argv, orbid_a);
      CORBA::ORB_var orb_b = CORBA::ORB_init (argc, argv, orbid_b);
      RTCORBA::RTORB_var rtorb_a = getRTORB (orb_a.in (), orbid_a);
      RTCORBA::RTORB_var rtorb_b = getRTORB (orb_b.in (), orbid_b);

      PortableServer::POA_var root_poa_a = getRootPoa (orb_a.in (), orbid_a);
      PortableServer::POA_var root_poa_b = getRootPoa (orb_b.in (), orbid_b);

      PortableServer::POAManager_var const manager_a = root_poa_a->the_POAManager ();
      manager_a->activate ();
      PortableServer::POAManager_var const manager_b = root_poa_b->the_POAManager ();
      manager_b->activate ();

      Client_Task client_task_a (orb_a.in ());
      Client_Task client_task_b (orb_b.in ());

      // The servant variables own the objects; these pointers are borrowed for POA operations.
      Test_i * const impl_a = new Test_i (orb_a.in (), &client_task_a);
      PortableServer::ServantBase_var const safe_a (impl_a);
      Test_i * const impl_b = new Test_i (orb_b.in (), &client_task_b);
      PortableServer::ServantBase_var const safe_b (impl_b);

      CORBA::String_var const ior_a = addServant (orb_a.in (), rtorb_a.in (), root_poa_a.in (),
                                                  impl_a, tpid_a, 3);
      CORBA::String_var const ior_b = addServant (orb_b.in (), rtorb_b.in (), root_poa_b.in (),
                                                  impl_b, tpid_b, 3);

      if (parse_args (argc, argv, ior_a_file, ior_b_file) != 0)
        return 1;

      if (write_iorfile (ior_a_file, ior_a.in ()) != 0)
        return 1;

      if (write_iorfile (ior_b_file, ior_b.in ()) != 0)
        return 1;

      // colocated calls work fine
      CORBA::Object_var obj_a = orb_a->string_to_object (ior_a.in ());
      Test::Hello_var hello_a (Test::Hello::_narrow (obj_a.in ()));
      CORBA::String_var const result_a = hello_a->get_string ();

      CORBA::Object_var obj_b = orb_b->string_to_object (ior_b.in ());
      Test::Hello_var hello_b (Test::Hello::_narrow (obj_b.in ()));
      CORBA::String_var const result_b = hello_b->get_string ();

      ACE_DEBUG ((LM_DEBUG,
                  "server got resA: %C and resB: %C\n",
                  result_a.in (), result_b.in ()));

      if (client_task_a.activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
        ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
      if (client_task_b.activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
        ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
      client_task_a.thr_mgr ()->wait ();
      client_task_b.thr_mgr ()->wait ();

      removeServant (rtorb_a.in (), root_poa_a.in (), impl_a, tpid_a);
      shutdownORB (orb_a.in (), orbid_a);

      removeServant (rtorb_b.in (), root_poa_b.in (), impl_b, tpid_b);
      shutdownORB (orb_b.in (), orbid_b);

      obj_b = CORBA::Object::_nil ();
      hello_b = Test::Hello::_nil ();
      orb_b = CORBA::ORB::_nil ();
      rtorb_b = RTCORBA::RTORB::_nil ();
      root_poa_b = PortableServer::POA::_nil ();
      obj_a = CORBA::Object::_nil ();
      hello_a = Test::Hello::_nil ();
      orb_a = CORBA::ORB::_nil ();
      rtorb_a = RTCORBA::RTORB::_nil ();
      root_poa_a = PortableServer::POA::_nil ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught unexpected exception:");
      ACE_ERROR ((LM_ERROR, "Bug_2953_Regression failed.\n"));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG, "Bug_2953_Regression server ended successfully.\n"));

  return 0;
}
