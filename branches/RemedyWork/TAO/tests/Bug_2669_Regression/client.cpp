
//=============================================================================
/**
 *  $Id$
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_string.h"

#include "ChildS.h"

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT(""));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class ChildHandler : public POA_ChildModule::AMI_ChildInterfaceHandler
{
public:
  ChildHandler (void) {};
  ~ChildHandler (void) {};
};

class NonRelatedChildHandler : public POA_AMI_ChildInterfaceHandler
{
public:
  NonRelatedChildHandler (void) {};
  ~NonRelatedChildHandler (void) {};
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;


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

      // Create two handlers
      ChildHandler child_handler;
      NonRelatedChildHandler non_related_child_handler;

      PortableServer::ObjectId_var id =
        poa_var->activate_object (&child_handler);

      CORBA::Object_var object = poa_var->id_to_reference (id.in ());

      ChildModule::AMI_ChildInterfaceHandler_var the_child_handler_var =
        ChildModule::AMI_ChildInterfaceHandler::_narrow (object.in ());

      const char * expectedid = "IDL:child.pragma.prefix/ChildModule/AMI_ChildInterfaceHandler:1.0";
      if (ACE_OS::strcmp (the_child_handler_var->_interface_repository_id (), expectedid) != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error: REGRESSION - ChildModule::repository id is "
                                "incorrectly generated, received %s\n",
                                the_child_handler_var->_interface_repository_id ()));
          result = 1;
        }

      // This handler has no relationship with the above in IDL.
      id = poa_var->activate_object (&non_related_child_handler);

      object = poa_var->id_to_reference (id.in ());

      AMI_ChildInterfaceHandler_var the_non_related_child_handler_var =
        AMI_ChildInterfaceHandler::_narrow (object.in ());

      // Check that both handler objects narrow successfully to their parent
      // handler types...
      ParentModule::AMI_ParentInterfaceHandler_var the_parent_handler_var =
        ParentModule::AMI_ParentInterfaceHandler::_narrow (the_child_handler_var.in ());

      if (CORBA::is_nil (the_parent_handler_var.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: REGRESSION - Cannot narrow ChildModule::ChildInterface "
                                "reply handler to its parent handler type.\n"));
          result = 1;
        }

      AMI_ParentInterfaceHandler_var the_non_related_parent_handler_var =
        AMI_ParentInterfaceHandler::_narrow (the_non_related_child_handler_var.in ());

      if (CORBA::is_nil (the_non_related_parent_handler_var.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: REGRESSION - Cannot narrow ChildInterface "
                                "reply handler to its parent handler type.\n"));
          result = 1;
        }

      // Check that both handler objects *won't* narrow to each other's parent
      // handler types...
      the_parent_handler_var =
        ParentModule::AMI_ParentInterfaceHandler::_narrow (the_non_related_child_handler_var.in ());

      if (! CORBA::is_nil (the_parent_handler_var.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: REGRESSION - ChildModule::ChildInterface reply handler "
                                "narrows to unrelated type.\n"));
          result = 1;
        }

      the_non_related_parent_handler_var =
        AMI_ParentInterfaceHandler::_narrow (the_child_handler_var.in ());

      if (! CORBA::is_nil (the_non_related_parent_handler_var.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Error: REGRESSION - ChildModule::ChildInterface reply handler "
                                "narrows to unrelated type.\n"));
          result = 1;
        }

      poa_var->destroy (1,0);

      orb->destroy ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return result;
}
