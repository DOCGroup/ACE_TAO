
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  A very simple client which uses the AMI callback model.
 *
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "ami_testS.h"
#include "tao/Codeset/Codeset.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 5;
int shutdown_flag = 1;
int debug = 0;
int result = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = 1;
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Handler : public POA_A::AMI_AMI_TestHandler
{
public:
  /// Constructor.
  Handler (void) {};

  /// Destructor.
    ~Handler (void) {};

  void foo (void)
    {
    };

  void foo_excep (::Messaging::ExceptionHolder * excep_holder)
    {

      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <foo_excep> called:\n"
                  "Testing proper exception handling ...\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const A::DidTheRightThing& ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... exception received successfully: <%d>\n",
                      ex.id));
#if defined(ACE_HAS_WCHAR) || defined(ACE_HAS_XPG4_MULTIBYTE_CHAR)
          ACE_HEX_DUMP((LM_DEBUG,
                        reinterpret_cast<char const *>(ex.whatDidTheRightThing.in()),
                        ACE_OS::strlen(ex.whatDidTheRightThing.in()) * sizeof(CORBA::WChar)
                        ));

          CORBA::WChar const* wstring = L"Hello world";
#else
          CORBA::WChar const empty[] = { 0 };
          CORBA::WChar const* wstring = empty;
#endif
          if (ACE_OS::strcmp (wstring, ex.whatDidTheRightThing.in()) != 0)
            {
              ex._tao_print_exception ("ERROR");
                 ACE_ERROR ((LM_ERROR,
                           "... caught the exception but got wrong wstring -> ERROR\n"));
              result = 1;
            }
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("ERROR");
          ACE_ERROR ((LM_ERROR,
                      "... caught the wrong exception -> ERROR\n"));
          result = 1;
        }
    };
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object_var =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa_var =
        PortableServer::POA::_narrow (object_var.in ());

      PortableServer::POAManager_var poa_manager_var =
        poa_var->the_POAManager ();

      poa_manager_var->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      // We reuse the object_var smart pointer!
      object_var = orb->string_to_object (ior);

      A::AMI_Test_var ami_test_var =
        A::AMI_Test::_narrow (object_var.in ());

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA.
      Handler handler;
      PortableServer::ObjectId_var id =
        poa_var->activate_object (&handler);

      CORBA::Object_var object = poa_var->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var the_handler_var =
        A::AMI_AMI_TestHandler::_narrow (object.in ());

      // Trigger the DidTheRightThing exception on the server side
      // by sending 0 to it.
      ACE_DEBUG ((LM_DEBUG,
                  "Sending asynch message\n"));

      ami_test_var->sendc_foo (the_handler_var.in (), 0);

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Issuing a synchronous method to collect the AMI replies\n"));
        }

      ami_test_var->foo (1);

      if (shutdown_flag)
        {
          ami_test_var->shutdown ();
        }

      poa_var->destroy (1,  // ethernalize objects
                        0  // wait for completion
                       );

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return result;
}
