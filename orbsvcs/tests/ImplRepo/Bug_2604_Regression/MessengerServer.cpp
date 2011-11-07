// $Id$
#include "Messenger_i.h"

#include "tao/ImR_Client/ImR_Client.h"

#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

PortableServer::POA_ptr
createPersistentPOA (PortableServer::POA_ptr root_poa, const char *poa_name)
{
  CORBA::PolicyList policies;
  policies.length (2);

  policies[0] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
  policies[1] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID);

  PortableServer::POAManager_var mgr = root_poa->the_POAManager ();

  PortableServer::POA_var poa = root_poa->create_POA (poa_name, mgr.in (),
                                                      policies);
  policies[0]->destroy ();
  policies[1]->destroy ();

  return poa._retn ();
}

void
writeIORFile (const char* ior)
{
  FILE *out = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_output_file), "w");
  if (out)
    {
      ACE_OS::fprintf (out, "%s", ior);
      ACE_OS::fclose (out);
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Server Process started\n")));

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();

      const char* poa_name = "MessengerService";

      PortableServer::POA_var poa = createPersistentPOA (root_poa.in (),
                                                         poa_name);
      Messenger_i servant (orb.in ());

      PortableServer::ObjectId_var object_id =
        PortableServer::string_to_ObjectId ("object");

      poa->activate_object_with_id (object_id.in (), &servant);
      obj = poa->id_to_reference (object_id.in ());
      CORBA::String_var ior = orb->object_to_string (obj.in ());

      writeIORFile (ior.in ());

      mgr->activate ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Messenger server ready.\n")));

      orb->run ();

      // the following 1 second of sleep is needed to help
      // Windows with "server shutdown too quickly so the
      // client cannot get the reply" issue.
      ACE_OS::sleep(1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Messenger server shutting down.\n")));
      root_poa->destroy (1, 1);
      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception &ex)
    {
      ACE_CString str = ex._info ();
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Server main() caught: %C\n"),
                  str.c_str ()));
    }
  return -1;
}
