// $Id$

#include "Demo_i.h"
#include "BaseValueFactory.h"
#include "NestedValueFactory.h"

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

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
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-o <iorfile>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        {
          return -1;
        }

      CORBA::ValueFactoryBase_var vf = new Demo_BaseValueFactory();
      CORBA::String_var repId = Demo::_tc_BaseValue->id();
      orb->register_value_factory(repId.in(), vf.in());

      vf = new Demo_NestedValueFactory();
      repId = Demo::_tc_NestedValue->id();
      orb->register_value_factory(repId.in(), vf.in());

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      PortableServer::Servant_var<ATC_Test_i> servant = new ATC_Test_i ();
      PortableServer::ObjectId_var oid = poa->activate_object (servant.in ());
      obj = poa->id_to_reference (oid.in ());
      CORBA::String_var ior = orb->object_to_string (obj.in ());

      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%P Cannot write IOR file: %s\n"),
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
      ACE_DEBUG  ( (LM_DEBUG,
                    ACE_TEXT  ("%P IOR written to file %C\n"),
                    ior_output_file));

      orb->run ();
      orb->destroy ();
      return 0;
    }
  catch  (CORBA::Exception& ex)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%P Union test server caught %s.\n"), ex._name()),
                        -1);
    }
  return 1;
}
