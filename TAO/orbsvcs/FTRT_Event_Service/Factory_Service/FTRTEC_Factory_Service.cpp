// $Id$

#include "EventChannelFactory_i.h"
#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"

ACE_RCSID (FTRTEC_Factory_Service,
           FTRTEC_Factory_Service.cpp,
           "$Id$")

namespace {
  ACE_CString id, kind, output;
}

int parse_args(int argc, char* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("i:k:o:"));
  int opt;

  int result = 0;
  while ((opt = get_opt ()) != EOF)
  {
    switch (opt)
    {
    case 'i':
      id = get_opt.opt_arg ();
      break;
    case 'k':
      kind = get_opt.opt_arg (); 
      break;
    case 'o':
      output = get_opt.opt_arg ();
      break;
    default:
      result = -1;
      break;
    }
  }

  if (result == -1 || (id.length() == 0 && output.length() == 0))
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_LIB_TEXT("Usage: %s \n")
      ACE_LIB_TEXT("  [-i id]  set the id that is used to register to the naming service\n")
      ACE_LIB_TEXT("  [-k kind] set the kind that is used to register to the naming service\n")
      ACE_LIB_TEXT("  [-o filename] set the output file name for the IOR\n")
      ACE_LIB_TEXT("\n"),
      argv[0]));
    return -1;
  }
  return 0;
}

int main(int argc, ACE_TCHAR* argv[])
{

	ACE_DECLARE_NEW_CORBA_ENV;
	ACE_TRY {
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv 
			                                       ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

	
    if (parse_args(argc, argv) == -1)
      return -1;

		CORBA::Object_var obj = 
			orb->resolve_initial_references("RootPOA"  
			ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

		PortableServer::POA_var poa = 
			PortableServer::POA::_narrow(obj.in()  
			                             ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		
		PortableServer::POAManager_var mgr = poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

		mgr->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;
		
		EventChannelFactory_i servant("factory.cfg", orb.in());

		FT::GenericFactory_var event_channel_factory = 
			servant._this(ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

		// register to the Event Service

    if (id.length()) {
      CORBA::Object_var namng_contex_object =
        orb->resolve_initial_references("NameService"
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow(namng_contex_object.in()
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // register to naming service
      CosNaming::Name name(1);
      name.length(1);
      name[0].id = CORBA::string_dup(id.c_str());
      if (kind.length())
        name[0].kind = CORBA::string_dup(kind.c_str());

      naming_context->bind(name, event_channel_factory.in()
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG((LM_DEBUG, "Register to naming service with %s", id.c_str()));
      if (kind.length())
        ACE_DEBUG((LM_DEBUG, ", %s", kind.c_str()));
      ACE_DEBUG((LM_DEBUG,"\n"));
    }

    if (output.length()) {
      // get the IOR of factory
      CORBA::String_var str = orb->object_to_string(event_channel_factory.in()
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp(output.c_str(), "") != 0)
      {
        FILE *output_file=
          ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(output.c_str()),
          ACE_LIB_TEXT("w"));
        if (output_file == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
          "Cannot open output file for writing IOR: %s",
          output.c_str()),
          1);
        ACE_OS::fprintf (output_file, "%s", str.in ());
        ACE_OS::fclose (output_file);
      } 
    }

    ACE_TRY_CHECK;

    orb->run(ACE_ENV_SINGLE_ARG_PARAMETER);

  }
	ACE_CATCHANY
	{
		ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA Exception occurred.");
	}
	ACE_ENDTRY;

	
	ACE_CHECK_RETURN(1);
 
	return 0;
}

