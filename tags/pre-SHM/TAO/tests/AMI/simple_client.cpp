// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI
//
// = FILENAME
//    simple_client.cpp
//
// = DESCRIPTION
//    A very simple client, which uses the AMI callback model.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>,
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Task.h"

#ifdef TAO_HAS_AMI_EXCEPTIONS
#include "ami_testC-hand.h"
#include "ami_testS-hand.h"
#else
#include "ami_testC.h"
#include "ami_testS.h"
#endif


ACE_RCSID(AMI, simple_client, "$Id$")

const char *ior = "file://test.ior";
int niterations = 5;
int shutdown_flag = 0;
int debug = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dk:i:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = 1;
        break;
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'i':
        niterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'x':
        shutdown_flag = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "-x "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  Handler (void) {};
  // Constructor.

    ~Handler (void) {};
  // Destructor.

  void foo (CORBA::Long ami_return_val,
            CORBA::Long out_l,
            CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_UNUSED_ARG (ami_return_val);
      ACE_UNUSED_ARG (out_l);
      ACE_UNUSED_ARG (ACE_TRY_ENV);
      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Callback method <foo> called: result <%d>, out_arg <%d>\n",
                      ami_return_val, 
                      out_l));
        }
    };

  void foo_excep (A::AMI_AMI_TestExceptionHolder * excep_holder,
                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called: \n"));
      ACE_TRY
        {
          excep_holder->raise_foo (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Catched exception:");
        }
      ACE_ENDTRY;
    };

  
  void get_yadda (CORBA::Long result, 
                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_UNUSED_ARG (result);
      ACE_UNUSED_ARG (ACE_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_yadda> called: result <%d>\n",
                  result));
    };

  void get_yadda_excep (A::AMI_AMI_TestExceptionHolder * excep_holder,
                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_UNUSED_ARG (excep_holder);
      ACE_UNUSED_ARG (ACE_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_yadda_excep> called: \n"));
    };
    
  void set_yadda (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_UNUSED_ARG (ACE_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda> called: \n"));
    };

  void set_yadda_excep (A::AMI_AMI_TestExceptionHolder * excep_holder,
                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ACE_UNUSED_ARG (excep_holder);
      ACE_UNUSED_ARG (ACE_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda_excep> called: \n"));
    };

};

int
main (int argc, char *argv[])
{

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object_var =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa_var =
        PortableServer::POA::_narrow (object_var.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager_var =
        poa_var->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      poa_manager_var->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (parse_args (argc, argv) != 0)
        return 1;
      
      // We reuse the object_var smart pointer!
      object_var = orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      A::AMI_Test_var ami_test_var =
        A::AMI_Test::_narrow (object_var.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA. 
      Handler handler;
      A::AMI_AMI_TestHandler_var the_handler_var =
        handler._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Trigger the DidNotWork exception on the server side
      // by sending 0 to it.
      ACE_DEBUG ((LM_DEBUG,
                  "Sending asynch message\n"));
                
      ami_test_var->sendc_foo (the_handler_var.in (),
                               0,
                               "Let's talk AMI.",
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Long l = 931247;
 
      for (ssize_t ni = 0; ni < niterations; ni++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Sending asynch message: %d\n",
                      ni));
                    
          ami_test_var->sendc_foo (the_handler_var.in (),
                                   l,
                                   "Let's talk AMI.",
                                   ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      // Begin test of attributes
      ami_test_var->sendc_get_yadda (the_handler_var.in (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ami_test_var->sendc_set_yadda (the_handler_var.in (),
                                     4711,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ami_test_var->sendc_get_yadda (the_handler_var.in (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // End test of attributes

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "<%d> Asynchronous methods issued\n",
                      niterations));
        }

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Issuing a synchronous method to collect the AMI replies\n"));
        }

      //while (orb->work_pending())
      //  orb->perform_work ();

  
      CORBA::Long number = ami_test_var->foo (l,
                                              l,
                                              "Let's talk SMI.",
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG, 
                      "Received the following number: %d\n",
                      number));
        }
      
      //if (shutdown_flag)
      //  ami_test_var->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;
  
  return 0;
}
