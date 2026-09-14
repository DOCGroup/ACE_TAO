#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      const ACE_TCHAR *ready = ACE_TEXT ("ready");
      ACE_Get_Opt options (argc, argv, ACE_TEXT ("o:"));
      if (options () == 'o')
        ready = options.opt_arg ();
      CORBA::Object_var object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var manager = poa->the_POAManager ();
      manager->activate ();
      FILE *file = ACE_OS::fopen (ready, ACE_TEXT ("w"));
      if (!file)
        return 1;
      ACE_OS::fputs ("ready\n", file);
      ACE_OS::fclose (file);
      ACE_Time_Value runtime (40);
      orb->run (runtime);
      poa->destroy (true, true);
      orb->destroy ();
      return 0;
    }
  catch (const CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Transport_Idle_Scanner: %C\n"),
                  ex._name ()));
      return 1;
    }
}
