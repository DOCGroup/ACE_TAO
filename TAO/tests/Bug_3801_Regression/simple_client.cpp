
//=============================================================================
/**
 *  @file    simple_client.cpp
 *
 *  A very simple client which uses the AMI callback model.
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "ami_testS.h"

#include <cstring>
#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior,
            bool &shutdown_flag,
            bool &debug)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("dk:x"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'd':
        debug = true;
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'x':
        shutdown_flag = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-x "
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
  Handler () = default;

  /// Destructor.
  ~Handler () override = default;
  Handler (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler &operator= (Handler &&) = delete;

  bool test_failed () const
    {
      return this->test_failed_;
    }

  void get_yadda (CORBA::Long value) override
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Callback method <get_yadda> called: result <%d>\n",
                  value));
      this->test_failed_ = true;
    };

  void get_yadda_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_yadda_excep> called:\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (A::DidTheRightThingB const &ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... exception received successfully\n"));
          if (ex.id != 42)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: ex.id not 42: %d\n",
                          ex.id));
              this->test_failed_ = true;
            }
          if (std::strcmp (ex.whatDidTheRightThing.in (), "Hello world") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: ex.whatDidTheRightThing not ok: <%C>\n",
                          ex.whatDidTheRightThing.in ()));
              this->test_failed_ = true;
            }
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("ERROR");
          ACE_ERROR ((LM_ERROR,
                      "... caught the wrong exception -> ERROR\n"));
        }
    };

  void set_yadda () override
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Callback method <set_yadda> called:\n"));
      this->test_failed_ = true;
    };

  void set_yadda_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <set_yadda_excep> called:\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (A::DidTheRightThing const &ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... exception received successfully\n"));
          if (ex.id != 42)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: ex.id not 42: %d\n",
                          ex.id));
              this->test_failed_ = true;
            }
          if (std::strcmp (ex.whatDidTheRightThing.in (), "Hello world") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: ex.whatDidTheRightThing not ok: <%C>\n",
                          ex.whatDidTheRightThing.in ()));
              this->test_failed_ = true;
            }
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("ERROR");
          ACE_ERROR ((LM_ERROR,
                      "... caught the wrong exception -> ERROR\n"));
        }
    };

  void get_dadda (CORBA::Long value) override
    {
      ACE_ERROR ((LM_ERROR,
                  "Error: Callback method <get_dadda> called: result <%d>\n",
                  value));
      this->test_failed_ = true;
    };

  void get_dadda_excep (::Messaging::ExceptionHolder *excep_holder) override
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Callback method <get_dadda_excep> called:\n"));
      try
        {
          excep_holder->raise_exception ();
        }
      catch (A::DidTheRightThing const &ex)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "... exception received successfully\n"));
          if (ex.id != 42)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: ex.id not 42: %d\n",
                          ex.id));
              this->test_failed_ = true;
            }
          if (std::strcmp (ex.whatDidTheRightThing.in (), "Hello world") != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "ERROR: ex.whatDidTheRightThing not ok: <%C>\n",
                          ex.whatDidTheRightThing.in ()));
              this->test_failed_ = true;
            }
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("ERROR");
          ACE_ERROR ((LM_ERROR,
                      "... caught the wrong exception -> ERROR\n"));
        }
    };

  void inout_arg_test (char const *) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "Callback method <set_yadda_excep> called:\n"));
  }

  void inout_arg_test_excep (::Messaging::ExceptionHolder *) override
  {
  }

private:
  bool test_failed_ {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Handler handler;
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var object_var =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const poa_var =
        PortableServer::POA::_narrow (object_var.in ());

      PortableServer::POAManager_var const poa_manager_var =
        poa_var->the_POAManager ();

      poa_manager_var->activate ();

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      bool shutdown_flag {};
      bool debug {};
      if (parse_args (argc, argv, ior, shutdown_flag, debug) != 0)
        return 1;

      // We reuse the object_var smart pointer!
      object_var = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      A::AMI_Test_var const ami_test_var =
        A::AMI_Test::_narrow (object_var.in ());

      if (CORBA::is_nil (ami_test_var.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior.c_str ()),
                            1);
        }

      // Instantiate the ReplyHandler and register that with the POA.
      PortableServer::ObjectId_var const id =
        poa_var->activate_object (&handler);

      CORBA::Object_var const object = poa_var->id_to_reference (id.in ());

      A::AMI_AMI_TestHandler_var const the_handler_var =
        A::AMI_AMI_TestHandler::_narrow (object.in ());

      // Begin test of attributes
      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      ami_test_var->sendc_set_yadda (the_handler_var.in (), 4711);

      ami_test_var->sendc_get_yadda (the_handler_var.in ());

      ami_test_var->sendc_get_dadda (the_handler_var.in ());
      // End test of attributes

      if (debug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Issuing a synchronous method to collect the AMI replies\n"));
        }

      CORBA::String_var mystring = CORBA::string_dup ("Hello");

      ami_test_var->inout_arg_test (mystring.inout ());

      if (shutdown_flag)
        {
          ami_test_var->shutdown ();
        }

      poa_var->destroy (1,  // ethernalize objects
                        0); // wait for completion

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return handler.test_failed () ? 1 : 0;
}
