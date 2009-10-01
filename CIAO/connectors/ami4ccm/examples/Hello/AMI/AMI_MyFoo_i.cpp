// $Id$

#include "AMI_MyFoo_i.h"
#include "tao/Messaging/ExceptionHolder_i.h"
#include "tao/Exception_Data.h"

namespace CCM_CORBA_AMI_MyFoo_Impl
{
  //============================================================
  // Implementation of the AMI CORBA FOO reply handler
  //============================================================
  AMI_MyFoo_reply_handler::AMI_MyFoo_reply_handler (::CCM_AMI::AMI_MyFooCallback_ptr foo_callback)
  : foo_callback_ (::CCM_AMI::AMI_MyFooCallback::_duplicate (foo_callback))
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
    foo_callback_->foo (result, CORBA::string_dup (out_str));
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::foo_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo_excep\n");
    foo_callback_->foo_excep (excep_holder);
    this->_remove_ref ();
  }
  
  // HELLO methods
  void
  AMI_MyFoo_reply_handler::hello (
    CORBA::Long answer)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::hello\n");
    foo_callback_->hello (answer);
    this->_remove_ref ();
  }
  
  void
  AMI_MyFoo_reply_handler::hello_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::hello_excep\n");
    foo_callback_->hello_excep (excep_holder);
    this->_remove_ref ();
  }
  
  //GET rw_attrib Reply Handler
  void 
  AMI_MyFoo_reply_handler::get_rw_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_rw_atrrib\n");
    foo_callback_->get_rw_attrib (ami_return_val);
    this->_remove_ref ();
  }
  
  void 
  AMI_MyFoo_reply_handler::get_rw_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_rw_attrib_excep\n");
    foo_callback_->get_rw_attrib_excep (excep_holder);
    this->_remove_ref ();
  }
    
  //SET rw_attrib Reply Handler
  void 
  AMI_MyFoo_reply_handler::set_rw_attrib ()
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::set_rw_attrib\n");
    foo_callback_->set_rw_attrib ();
    this->_remove_ref ();
  }
  
  void 
  AMI_MyFoo_reply_handler::set_rw_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::set_rw_attrib_excep\n");
    foo_callback_->set_rw_attrib_excep (excep_holder);
    this->_remove_ref ();
  }
  
  //ro_attrib Reply Handler
  void 
  AMI_MyFoo_reply_handler::get_ro_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_ro_attrib\n");
    foo_callback_->get_ro_attrib (ami_return_val);
    this->_remove_ref ();
  }
    
  void 
  AMI_MyFoo_reply_handler::get_ro_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_ro_atrrib_excep\n");
    foo_callback_->get_ro_attrib_excep (excep_holder);
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
    printf ("AMI CORBA (FOO) :\tReceived string <%s>. Try passing it to the Receiver component\n", in_str);
    CORBA::Long result = foo_receiver_->foo (CORBA::string_dup (in_str), out_str);
    return result;
  }

  void
  AMI_MyFoo_i::hello (CORBA::Long_out answer)
  {
    printf ("AMI CORBA (FOO) :\tHello. Try calling the Receiver component\n");
    foo_receiver_->hello (answer);
  }
  
  CORBA::Short
  AMI_MyFoo_i::rw_attrib ()
  {
    printf ("AMI CORBA (FOO) :\trw_attrib. Try calling the Receiver component\n");
    CORBA::Short result = foo_receiver_->rw_attrib ();
    return result;
  }

  void
  AMI_MyFoo_i::rw_attrib (CORBA::Short rw_attrib)
  {
    printf ("AMI CORBA (FOO) :\tSet rw_attrib. Try calling the Receiver component\n");
    foo_receiver_->rw_attrib (rw_attrib);
  }
  
  CORBA::Short
  AMI_MyFoo_i::ro_attrib ()
  {
    printf ("AMI CORBA (FOO) :\tro_attrib. Try calling the Receiver component\n");
    CORBA::Short result = foo_receiver_->ro_attrib ();
    return result;
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
