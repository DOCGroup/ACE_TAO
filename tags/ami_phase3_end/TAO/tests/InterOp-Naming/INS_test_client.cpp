// $Id$


#include "orbsvcs/CosNamingC.h"
#include "INSC.h"

int
main (int argc, char *argv[])
{

  CORBA::Environment ACE_TRY_ENV;

  ACE_TRY
    {

      // Retrieve a reference to the ORB.
      CORBA::ORB_var orb_ = CORBA::ORB_init (argc,
                                             argv,
                                             0,
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "No Name specified to be resolved\n"));
        }
      else
        {
          CORBA::Object_var objref =
            orb_->resolve_initial_references (argv[1]);

          if (CORBA::is_nil (objref.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to resolve the given Name.\n"),
                              -1);

          if (ACE_OS::strcmp (argv[1], "NameService") == 0)
            {
              
              CosNaming::NamingContext_var naming_context =
                CosNaming::NamingContext::_narrow (objref.in (),
                                                   ACE_TRY_ENV);
              ACE_TRY_CHECK;
              
              ACE_DEBUG ((LM_DEBUG,
                          "Resolved IOR for %s : %s\n",
                          argv[1],
                          orb_->object_to_string (naming_context.in ())));

              // Sanity check to see if the reference to Naming Context is alright.
              CosNaming::Name my_name;
              my_name.length (1);
              my_name[0].id = CORBA::string_dup ("ObjName");

              naming_context->bind (my_name,
                                    objref.in (),
                                    ACE_TRY_ENV);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,
                          "\nBind successful on the resolved Naming Context\n"));

            }
          else
            {
              
              INS_var server = INS::_narrow (objref.in (),
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;

              ACE_DEBUG ((LM_DEBUG,
                          "Resolved IOR for %s : %s\n",
                          argv[1],
                          orb_->object_to_string (server.in ())));
              
              ACE_DEBUG ((LM_DEBUG,
                          "\nResult of Remote Call : %s\n",
                          server->test_ins (ACE_TRY_ENV)));
              
            }
          
        }
    }
  ACE_CATCHANY
    {
       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception:");
    }

  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  
  return 0;
}




