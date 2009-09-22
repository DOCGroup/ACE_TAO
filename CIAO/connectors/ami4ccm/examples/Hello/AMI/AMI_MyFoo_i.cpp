// $Id$

#include "AMI_MyFoo_i.h"

namespace CCM_CORBA_AMI_MyFoo_Impl
{
  //============================================================
  // Implementation of the AMI CORBA FOO reply handler
  //============================================================
  AMI_MyFoo_reply_handler::AMI_MyFoo_reply_handler (::CCM_AMI::AMI_MyFoo_callback_ptr foo_callback)
  : foo_callback_ (::CCM_AMI::AMI_MyFoo_callback::_duplicate (foo_callback))
  {
  }

  AMI_MyFoo_reply_handler::~AMI_MyFoo_reply_handler ()
  {
  }

  // FOO methods
  void
  AMI_MyFoo_reply_handler::foo (
    CORBA::Long result,
    const char * out_str)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo\n");
    foo_callback_->foo_callback_handler (result, CORBA::string_dup (out_str));
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::foo_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo_excep\n");
    foo_callback_->foo_callback_excep (excep_holder);
    this->_remove_ref ();
  }
  
  // HELLO methods
  void
  AMI_MyFoo_reply_handler::hello (
    CORBA::Long answer)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::foo\n");
    foo_callback_->hello_callback_handler (answer);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::hello_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    foo_callback_->hello_callback_excep (excep_holder);
    this->_remove_ref ();
  }

  //============================================================
  // Implementation of the AMI CORBA FOO interface
  //============================================================
  AMI_MyFoo_i::AMI_MyFoo_i (CORBA::ORB_ptr orb, ::CCM_AMI::MyFoo_ptr foo_receiver)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      foo_receiver_ (::CCM_AMI::MyFoo::_duplicate (foo_receiver))
  {
  }

  CORBA::Long
  AMI_MyFoo_i::foo (const char * in_str,
                  CORBA::String_out out_str)
  {
    try
      {
        printf ("AMI CORBA (FOO) :\tReceived string <%s>. Try passing it to the Receiver component\n", in_str);
        CORBA::Long result = foo_receiver_->foo (CORBA::string_dup (in_str), out_str);
        return result;
      }
    catch (CCM_AMI::InternalError& ex)
      {
        printf ("AMI CORBA (FOO) :\tCORRECT EXCEPT -> re-throwing\n");
        throw ex;
      }
    catch (...)
    {
        printf ("AMI CORBA (FOO) :\t!!!!!UNKNOWN EXCEPT!!!!!\n");
        CCM_AMI::InternalError excep;
        excep.id = 43;
        excep.error_string = CORBA::string_dup ("UNKNOWN");
        throw excep;
    }
  }

  void
  AMI_MyFoo_i::hello (CORBA::Long_out answer)
  {
    try
      {
        printf ("AMI CORBA (FOO) :\tHello. Try calling the Receiver component\n");
        foo_receiver_->hello (answer);
      }
    catch (CCM_AMI::InternalError& ex)
      {
        printf ("AMI CORBA (FOO) :\tCORRECT EXCEPT -> re-throwing\n");
        throw ex;
      }
    catch (...)
    {
        printf ("AMI CORBA (FOO) :\t!!!!!UNKNOWN EXCEPT!!!!!\n");
        CCM_AMI::InternalError excep;
        excep.id = 43;
        excep.error_string = CORBA::string_dup ("UNKNOWN");
        throw CCM_AMI::InternalError (excep);
    }
  }

  //============================================================
  // Worker thread to service the AMI CORBA FOO interface
  //============================================================
  CORBA_MyFoo_server::CORBA_MyFoo_server (::CCM_AMI::MyFoo_ptr foo_receiver)
    : foo_receiver_ (::CCM_AMI::MyFoo::_duplicate (foo_receiver))
  {
  }

  int CORBA_MyFoo_server::svc ()
  {
    try
    {
      int argc = 2;
      ACE_TCHAR **argv = new ACE_TCHAR *[argc];
      argv[0] = ACE::strnew (ACE_TEXT (""));
      argv[1] = ACE::strnew (ACE_TEXT (""));
      CORBA::ORB_var orb =
          CORBA::ORB_init (argc, argv, ACE_TEXT ("CORBA_MyFoo_server"));

      CORBA::Object_var poa_object =
          orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
      {
        printf ("Server :\tERROR creating POA \n");
        return 1;
      }
      PortableServer::POA_var root_poa =
          PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
          root_poa->the_POAManager ();

      AMI_MyFoo_i AMI_MyFoo_i (orb.in (), foo_receiver_);

      PortableServer::ObjectId_var id =
          root_poa->activate_object (&AMI_MyFoo_i);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      CCM_AMI::MyFoo_var ami_foo_var =
          CCM_AMI::MyFoo::_narrow (object.in ());

      CORBA::String_var ior =
          orb->object_to_string (ami_foo_var.in ());

        // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen ("foo.ior", "w");
      if (output_file == 0)
      {
        printf ("Cannot open output file for writing IOR: server.ior\n");
        return 1;
      }
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      printf ("AMI CORBA (FOO) :\tFOO Server is activated\n");

      orb->run ();
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught except:");
      return 1;
    }

    return 0;
  }
}
