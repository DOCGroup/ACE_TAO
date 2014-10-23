// $Id$

// server.cpp
// This version uses the Implementation Repository.

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

PortableServer::POA_ptr
createPOA (PortableServer::POA_ptr root_poa, const ACE_TCHAR* poa_name)
{
  PortableServer::LifespanPolicy_var life =
    root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy(PortableServer::USER_ID);

  CORBA::PolicyList pols;
  pols.length(2);
  pols[0] = PortableServer::LifespanPolicy::_duplicate(life.in());
  pols[1] = PortableServer::IdAssignmentPolicy::_duplicate(assign.in());

  PortableServer::POAManager_var mgr = root_poa->the_POAManager();
  PortableServer::POA_var poa =
    root_poa->create_POA(ACE_TEXT_ALWAYS_CHAR (poa_name), mgr.in(), pols);

  life->destroy();
  assign->destroy();

  return poa._retn();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      const ACE_TCHAR *ior_file_name = ACE_TEXT ("test.ior");
      const ACE_TCHAR *poa_name = ACE_TEXT ("manual_test");
      ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:p:"));
      int c;

      while ((c = get_opts ()) != -1)
        {
          switch (c)
            {
            case 'o':
              ior_file_name = get_opts.opt_arg ();
              break;
            case 'p':
              poa_name = get_opts.opt_arg ();
              break;
            default:
              return -1;
            }
        }

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
      PortableServer::POA_var test_poa = createPOA (root_poa.in(), poa_name );

    Test_i *impl = new Test_i ();
    PortableServer::Servant_var<Test_i> test_servant = impl;

    PortableServer::ObjectId_var object_id =
      PortableServer::string_to_ObjectId ("test_object");

    test_poa->activate_object_with_id (object_id.in(), test_servant.in());

    obj = test_poa->id_to_reference (object_id.in ());
    CORBA::String_var test_ior = orb->object_to_string (obj.in ());
    FILE *f = ACE_OS::fopen (ior_file_name, ACE_TEXT ("w"));
    ACE_OS::fprintf (f, "%s", test_ior.in ());
    ACE_OS::fclose (f);

    mgr->activate();

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Started Server <%s>\n"),
                poa_name));

    ACE_Time_Value tv (1,0);
    orb->run (tv);
    root_poa->destroy(1,1);
    orb->destroy();
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) Server <%s> orb destroyed\n"),
                poa_name));
  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception (ACE_TEXT ("Server main()"));
    return 1;
  }

  return 0;
}
