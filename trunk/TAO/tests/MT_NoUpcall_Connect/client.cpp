// $Id$

#include "SharedIntfC.h"
#include "SharedIntfS.h"

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"

Test_Idl::SharedIntf_var intf;

ACE_Time_Value upcall_start (0,0);
ACE_Time_Value upcall_end (0,0);
ACE_Time_Value ping_start (0,0);
ACE_Time_Value ping_end (0,0);

class SharedIntfCB : public POA_Test_Idl::AMI_SharedIntfHandler
{
public:
  void ping (void)
  {
    ACE_DEBUG ((LM_DEBUG, "ping returned\n"));
    ping_end = ping_end.now ();
  }

  void ping_excep (::Messaging::ExceptionHolder * )
  {
    ACE_DEBUG ((LM_DEBUG, "exception caught on ping req\n"));
    ping_end = ping_end.now ();
  }

  void do_upcall (void)
  {
    ACE_DEBUG ((LM_DEBUG, "upcall returned\n"));
    upcall_end = upcall_end.now ();
  }

  void do_upcall_excep (::Messaging::ExceptionHolder * excep )
  {
    try
      {
        excep->raise_exception ();
      }
    catch (CORBA::Exception &)
      {
      }
    ACE_DEBUG ((LM_DEBUG, "exception caught on upcall req\n"));
    upcall_end = upcall_end.now ();
  }

};


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-t threads "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb_;
  int result = 0;

  try
  {
    ACE_DEBUG((LM_INFO,"(%P|%t) START OF CLIENT TEST\n"));

    orb_ = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj = orb_->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (obj.in());

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();
    PortableServer::POA_var poa = root_poa;

    poa_manager->activate ();

    PortableServer::ServantBase_var cb_serv = new SharedIntfCB;
    PortableServer::ObjectId_var oid = root_poa->activate_object (cb_serv.in());
    obj = root_poa->id_to_reference (oid.in());

    Test_Idl::AMI_SharedIntfHandler_var cb =
      Test_Idl::AMI_SharedIntfHandler::_narrow (obj.in());

    obj = orb_->string_to_object ("file://server.ior");
    intf = Test_Idl::SharedIntf::_narrow(obj.in());

    ACE_DEBUG((LM_INFO,"(%P|%t) invoking async upcall.\n"));

    upcall_start = upcall_start.now ();
    intf->sendc_do_upcall (cb.in());
    ACE_DEBUG((LM_INFO,"(%P|%t) invoking ping\n"));

    ping_start = ping_start.now ();
    intf->ping ();
    ACE_DEBUG((LM_INFO,"(%P|%t) sync ping returned\n"));
    ping_end = ping_end.now ();

    result = (upcall_end > ACE_Time_Value::zero && upcall_end <= ping_end) ? 0 : 1;

#if 0
    int retries = 120;
    while (upcall_end == ACE_Time_Value::zero && retries-- > 0)
      {
        ACE_Time_Value delay (1,0);
        orb_->perform_work (&delay);
      }

#endif

    intf->farewell ();

    ACE_DEBUG ((LM_INFO,"(%P|%t) Client Test %C\n",
                (result == 0 ? "succeeded":"failed")));
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Error: Exception caught:");
  }

  return result;
}

