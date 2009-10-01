// $Id$

#include "AMI_MyInterface_i.h"

namespace CCM_CORBA_AMI_MyInterface_Impl
{
  //============================================================
  // Implementation of the AMI CORBA INTERFACE reply handler
  //============================================================
  AMI_MyInterface_reply_handler::AMI_MyInterface_reply_handler (
    ::CCM_AMI::AMI_MyInterfaceCallback_ptr interface_callback)
    : interface_callback_ (::CCM_AMI::AMI_MyInterfaceCallback::_duplicate (interface_callback))
  {
  }
    
  AMI_MyInterface_reply_handler::~AMI_MyInterface_reply_handler ()
  {
  }
    
  void
  AMI_MyInterface_reply_handler::do_something_with_something (
    CORBA::Float result)
  {
    printf ("AMI CORBA :\tMyInterface Reply Handler::do_something_with_something\n");
    interface_callback_->do_something_with_something (result);
    this->_remove_ref ();
  }

  void
  AMI_MyInterface_reply_handler::do_something_with_something_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (INTERFACE) :\tMyInterface Reply Handler::do_something_with_something_excep\n");
    interface_callback_->do_something_with_something_excep (excep_holder);
    this->_remove_ref ();
  }
  
  //============================================================
  // Implementation of the AMI CORBA FOO interface
  //============================================================
  AMI_MyInterface_i::AMI_MyInterface_i (CORBA::ORB_ptr orb, ::CCM_AMI::MyInterface_ptr interface_receiver)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      interface_receiver_ (::CCM_AMI::MyInterface::_duplicate (interface_receiver))
  {
  }

  CORBA::Float
  AMI_MyInterface_i::do_something_with_something (CORBA::Short something)
  {
    try
      {
        printf ("AMI CORBA (INTERFACE) :\tReceived short <%d>. Try passing it to the Receiver component\n", something);
        CORBA::Float result = interface_receiver_->do_something_with_something (something);
        return result;
      }
    catch (CCM_AMI::InternalError& ex)
      {
        printf ("AMI CORBA (INTERFACE) :\tCORRECT EXCEPT -> re-throwing\n");
        throw CCM_AMI::InternalError (ex);
      }
    catch (...)
    {
        printf ("AMI CORBA (INTERFACE) :\t!!!!!UNKNOWN EXCEPT!!!!!\n");
        CCM_AMI::InternalError excep;
        excep.id = 43;
        excep.error_string = CORBA::string_dup ("UNKNOWN");
        throw CCM_AMI::InternalError (excep);
    }
  }

  //============================================================
  // Worker thread to service the AMI CORBA FOO interface
  //============================================================
  CORBA_MyInterface_server::CORBA_MyInterface_server (::CCM_AMI::MyInterface_ptr interface_receiver)
    : interface_receiver_ (::CCM_AMI::MyInterface::_duplicate (interface_receiver))
  {
  }

  int CORBA_MyInterface_server::svc ()
  {
    try
    {
      int argc = 2;
      ACE_TCHAR **argv = new ACE_TCHAR *[argc];
      argv[0] = ACE::strnew (ACE_TEXT (""));
      argv[1] = ACE::strnew (ACE_TEXT (""));
      CORBA::ORB_var orb =
          CORBA::ORB_init (argc, argv, ACE_TEXT ("CORBA_MyInterface_server"));

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

      AMI_MyInterface_i AMI_MyInterface_i (orb.in (), interface_receiver_);

      PortableServer::ObjectId_var id =
          root_poa->activate_object (&AMI_MyInterface_i);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      CCM_AMI::MyInterface_var ami_interface_var =
          CCM_AMI::MyInterface::_narrow (object.in ());

      CORBA::String_var ior =
          orb->object_to_string (ami_interface_var.in ());

        // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen ("interface.ior", "w");
      if (output_file == 0)
      {
        printf ("Cannot open output file for writing IOR: interface.ior\n");
        return 1;
      }
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      printf ("AMI CORBA (INTERFACE) :\tServer is activated\n");

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
