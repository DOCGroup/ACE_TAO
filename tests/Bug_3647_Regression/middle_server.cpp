// -*- C++ -*-
// $Id$

#include "Middle_Impl.h"

#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/Utils/RIR_Narrow.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

bool verbose = false;
const ACE_TCHAR *ior_output_file = ACE_TEXT ("middle.ior");
const ACE_TCHAR *ior = ACE_TEXT ("file://backend.ior");
Messaging::SyncScope scope = Messaging::SYNC_NONE;
long timeout = 2;

void
usage(ACE_TCHAR const *cmd,
      ACE_TCHAR const *msg)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("Usage:  %s ")
              ACE_TEXT("-v ")
              ACE_TEXT("-k <ior> ")
              ACE_TEXT("-o <iorfile> ")
              ACE_TEXT("-s <NONE|TRANSPORT|SERVER|TARGET|DELAYED> ")
              ACE_TEXT("-t timeout ")
              ACE_TEXT("\n")
              ACE_TEXT("        %s\n"),
              cmd, msg));
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("vo:k:s:t:"));
  int c;
  ACE_TCHAR const *sname = ACE_TEXT("NONE");
  ACE_TCHAR const *stimeout = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v':
        verbose = true;
        break;

      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 's':
        sname = get_opts.opt_arg ();
        break;

      case 't':
        stimeout = get_opts.opt_arg();
        break;

      case '?':
      default:
        usage(argv[0], ACE_TEXT("unknown argument"));
        return -1;
      }

  if (ACE_OS::strcmp(sname, ACE_TEXT("NONE")) == 0) {
    scope = Messaging::SYNC_NONE;
  } else if (ACE_OS::strcmp(sname, ACE_TEXT("TRANSPORT")) == 0) {
    scope = Messaging::SYNC_WITH_TRANSPORT;
  } else if (ACE_OS::strcmp(sname, ACE_TEXT("SERVER")) == 0) {
    scope = Messaging::SYNC_WITH_SERVER;
  } else if (ACE_OS::strcmp(sname, ACE_TEXT("TARGET")) == 0) {
    scope = Messaging::SYNC_WITH_TARGET;
  } else if (ACE_OS::strcmp(sname, ACE_TEXT("DELAYED")) == 0) {
    scope = TAO::SYNC_DELAYED_BUFFERING;
  } else {
    usage(argv[0], ACE_TEXT("Invalid scope value"));
    return -1;
  }

  if (stimeout != 0)
    {
      ACE_TCHAR *end;
      long tmp = ACE_OS::strtol(stimeout, &end, 10);
      if (end == 0 || *end != '\0')
        {
          usage(argv[0], ACE_TEXT("Invalid timeout value"));
          return -1;
        }
      timeout = tmp;
    }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                "backend_server(%P|%t) - panic: nil RootPOA\n"),
            1);
      }

      PortableServer::POAManager_var poa_manager =
          root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      // one second in TimeT units
      TimeBase::TimeT const second = 10 * TimeBase::TimeT(1000000);

      CORBA::Any timeout_as_any;
      timeout_as_any <<= TimeBase::TimeT(timeout * second);

      CORBA::Any scope_as_any;
      scope_as_any <<= scope;

      TAO::Utils::PolicyList_Destroyer plist(1);
      plist.length(2);
      plist[0] =
          orb->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
              timeout_as_any);
      plist[1] =
          orb->create_policy(Messaging::SYNC_SCOPE_POLICY_TYPE,
              scope_as_any);

      CORBA::PolicyCurrent_var policy_current =
          TAO::Utils::RIR_Narrow<CORBA::PolicyCurrent>::narrow(
               orb.in (),
               "PolicyCurrent");

      policy_current->set_policy_overrides(
          plist, CORBA::ADD_OVERRIDE);

      Bug_3647_Regression::Backend_var backend =
          Bug_3647_Regression::Backend::_narrow(tmp.in ());

      if (CORBA::is_nil (backend.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                  "middle_server(%P|%t) - nil backend reference <%s>\n",
                  ior),
              1);
        }

      using namespace Bug_3647_Regression;
      PortableServer::ServantBase_var impl(
          new Middle_Impl(backend.in(), orb.in(), verbose,
                          timeout));

      PortableServer::ObjectId_var id =
          root_poa->activate_object (impl.in());

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());


      Bug_3647_Regression::Middle_var middle =
          Bug_3647_Regression::Middle::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (middle.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                "middle_server(%P|%t) - Cannot open output file "
                "for writing IOR: %s\n",
                ior_output_file),
            1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG,
              "middle_server(%P|%t) - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
              "middle_server"));
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
