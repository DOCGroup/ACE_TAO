// -*- C++ -*-
// $Id$

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
  Test_i(CORBA::ORB_ptr orb, Client_Task* t) : orb_(CORBA::ORB::_duplicate(orb)), task_ (t)
  {
  }

  virtual ~Test_i()
  {
  }

  char * get_string()
  {
    return CORBA::string_dup("hello");
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown()
  {
    task_->terminate_loop ();
    orb_ = CORBA::ORB::_nil ();
  }
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var orb_;
  Client_Task* task_;
};

const ACE_TCHAR *ior_a_file = ACE_TEXT("iorA.ior");
const ACE_TCHAR *ior_b_file = ACE_TEXT("iorB.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
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
createThreadpool(CORBA::ORB_ptr orb, RTCORBA::RTORB_ptr rtorb, CORBA::ULong nthreads)
{
  CORBA::Object_var obj = orb->resolve_initial_references("RTCurrent");
  RTCORBA::Current_var rtcurrent = RTCORBA::Current::_narrow(obj.in());

  CORBA::ULong const stacksize = 0; // default
  CORBA::ULong const minThreads = 1;
  CORBA::ULong const dynamicThreads = nthreads - minThreads;

  TAO_Protocols_Hooks *tph =
    orb->orb_core ()->get_protocols_hooks ();

  RTCORBA::Priority dfltThreadPrio;

  if (!(tph->get_thread_implicit_CORBA_priority (dfltThreadPrio) == 0))
  {
    throw CORBA::DATA_CONVERSION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
  }

  CORBA::Boolean const doBuffering = false;
  CORBA::ULong maxBufRequests = 0;
  CORBA::ULong maxReqBufSize = 0;

  RTCORBA::ThreadpoolId poolId;
  try
  {
    poolId = rtorb->create_threadpool(stacksize,
                                      minThreads,
                                      dynamicThreads,
                                      dfltThreadPrio,
                                      doBuffering,
                                      maxBufRequests,
                                      maxReqBufSize);
  }
  catch (const CORBA::SystemException &x)
  {
    x._tao_print_exception ("creating thread pool");
    throw;
  }
  return poolId;
}

void destroyThreadpool(RTCORBA::RTORB_ptr rtorb, RTCORBA::ThreadpoolId id)
{
  rtorb->destroy_threadpool(id);
}

RTCORBA::RTORB_ptr getRTORB(CORBA::ORB_ptr orb, const char *id)
{
  CORBA::Object_var obj = orb->resolve_initial_references("RTORB");
  RTCORBA::RTORB_ptr rtorb = RTCORBA::RTORB::_narrow(obj.in());
  if ( CORBA::is_nil(rtorb))
    {
      ACE_ERROR ((LM_ERROR,
                  "Failed getting RTORB for orb <%C>\n",
                  id));
    }
  return rtorb;
}

PortableServer::POA_ptr getRootPoa(CORBA::ORB_ptr orb, const char *id)
{
  CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
  PortableServer::POA_ptr poa = PortableServer::POA::_narrow(obj.in());
  if ( CORBA::is_nil(poa) )
    {
      ACE_ERROR ((LM_ERROR,
                  "Failed getting RootPOA for orb <%C>\n",
                  id));
    }
  return poa;
}

char*
addServant(
  CORBA::ORB_ptr orb,
  RTCORBA::RTORB_ptr rtorb,
  PortableServer::POA_ptr poa,
  PortableServer::ServantBase *servant,
  RTCORBA::ThreadpoolId& id,
  short nthreads)
{
  id = createThreadpool(orb, rtorb, nthreads);
  ACE_DEBUG ((LM_DEBUG,
              "Created threadpool with id <%d>\n",
              id));

  TAO::Utils::PolicyList_Destroyer policies (8);
  policies.length(8);
  CORBA::ULong i = 0;

  policies[i++] = poa->create_id_assignment_policy(PortableServer::SYSTEM_ID);
  policies[i++] = poa->create_id_uniqueness_policy(PortableServer::UNIQUE_ID);
  policies[i++] = poa->create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);
  policies[i++] = poa->create_lifespan_policy(PortableServer::TRANSIENT);
  policies[i++] = poa->create_request_processing_policy(PortableServer::USE_ACTIVE_OBJECT_MAP_ONLY);
  policies[i++] = poa->create_servant_retention_policy(PortableServer::RETAIN);
  policies[i++] = poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL);
  policies[i++] = rtorb->create_threadpool_policy(id);

  PortableServer::ObjectId_var dummy = poa->activate_object(servant);
  CORBA::Object_var ref = poa->servant_to_reference(servant);
  CORBA::String_var str = orb->object_to_string(ref.in ());

  return str._retn();
}

void
removeServant(
  RTCORBA::RTORB_ptr rtorb,
  PortableServer::POA_ptr poa,
  PortableServer::ServantBase *servant,
  RTCORBA::ThreadpoolId id)
{
  PortableServer::ObjectId_var oid = poa->servant_to_id(servant);
  poa->deactivate_object(oid.in());
  rtorb->destroy_threadpool(id);
  ACE_DEBUG ((LM_DEBUG,
              "Destroyed threadpool with id <%d>\n",
              id));
}

void shutdownORB(CORBA::ORB_ptr orb, const char * orbid)
{
  orb->shutdown(true);
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is shutdown\n",
              orbid));

  orb->destroy();
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is destoyed\n",
              orbid));
}

int write_iorfile (const ACE_TCHAR* ior_output_file, const char* ior)
{
  FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                        "Cannot open output file for writing IOR: %s\n",
                        ior_output_file),
                        1);
  ACE_OS::fprintf (output_file, "%s", ior);
  ACE_OS::fclose (output_file);

  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    const char *orbidA = "orbidA";
    const char *orbidB = "orbidB";

    PortableServer::ServantBase *implA = 0;
    PortableServer::ServantBase *implB = 0;
    RTCORBA::ThreadpoolId tpidA = 0;
    RTCORBA::ThreadpoolId tpidB = 0;

    // server - write our iors to two files
    CORBA::ORB_var orbA = CORBA::ORB_init(argc, argv, orbidA);
    CORBA::ORB_var orbB = CORBA::ORB_init(argc, argv, orbidB);
    RTCORBA::RTORB_var rtorbA = getRTORB(orbA.in (), orbidA);
    RTCORBA::RTORB_var rtorbB = getRTORB(orbB.in (), orbidB);

    PortableServer::POA_var rootPoaA = getRootPoa(orbA.in (), orbidA);
    PortableServer::POA_var rootPoaB = getRootPoa(orbB.in (), orbidB);

    PortableServer::POAManager_var managerA = rootPoaA->the_POAManager();
    managerA->activate();
    PortableServer::POAManager_var managerB = rootPoaB->the_POAManager();
    managerB->activate();

    Client_Task client_taskA (orbA.in ());
    Client_Task client_taskB (orbB.in ());

    implA = new Test_i(orbA.in (), &client_taskA);
    PortableServer::ServantBase_var safeA (implA);
    implB = new Test_i(orbB.in (), &client_taskB);
    PortableServer::ServantBase_var safeB (implB);

    CORBA::String_var iorA = addServant(orbA.in (), rtorbA.in (), rootPoaA.in (), implA, tpidA, 3);
    CORBA::String_var iorB = addServant(orbB.in (), rtorbB.in (), rootPoaB.in (), implB, tpidB, 3);

    if (parse_args (argc, argv) != 0)
      return 1;

    if (write_iorfile(ior_a_file, iorA.in ()) == 1)
      return 1;

    if (write_iorfile(ior_b_file, iorB.in ()) == 1)
      return 1;

    // colocated calls work fine
    CORBA::Object_var objA = orbA->string_to_object(iorA.in ());
    Test::Hello_var helloA(Test::Hello::_narrow(objA.in ()));
    CORBA::String_var resA = helloA->get_string();

    CORBA::Object_var objB = orbB->string_to_object(iorB.in ());
    Test::Hello_var helloB(Test::Hello::_narrow(objB.in ()));
    CORBA::String_var resB = helloB->get_string();

    ACE_DEBUG ((LM_DEBUG, "server got resA: %C and resB: %C\n", resA.in (), resB.in ()));

    if (client_taskA.activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
      }
    if (client_taskB.activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Error activating client task\n"));
      }
    client_taskA.thr_mgr ()->wait ();
    client_taskB.thr_mgr ()->wait ();

    if (implA != 0)
      {
        removeServant(rtorbA.in (), rootPoaA.in (), implA, tpidA);
      }

    shutdownORB(orbA.in (), orbidA);

    if (implB != 0)
      {
        removeServant(rtorbB.in (), rootPoaB.in (), implB, tpidB);
      }

    shutdownORB(orbB.in (), orbidB);

    objB = CORBA::Object::_nil ();
    helloB = Test::Hello::_nil ();
    orbB = CORBA::ORB::_nil ();
    rtorbB = RTCORBA::RTORB::_nil ();
    rootPoaB = PortableServer::POA::_nil ();
    objA = CORBA::Object::_nil ();
    helloA = Test::Hello::_nil ();
    orbA = CORBA::ORB::_nil ();
    rtorbA = RTCORBA::RTORB::_nil ();
    rootPoaA = PortableServer::POA::_nil ();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Caught unexpected exception:");
    ACE_ERROR ((LM_ERROR,  "Bug_2953_Regression failed.\n"));
    return 1;
  }

  ACE_DEBUG ((LM_DEBUG, "Bug_2953_Regression server ended successfully.\n"));

  return 0;
}
