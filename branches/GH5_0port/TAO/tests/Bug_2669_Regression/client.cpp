
//=============================================================================
/**
 *  $Id$
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include "ChildS.h"

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "");
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
  // Indicates sucessful parsing of the command line
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
main (int argc, char *argv[])
{
  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");
      ACE_TRY_CHECK;

      CORBA::Object_var object_var =
        orb->resolve_initial_references ("RootPOA");
      ACE_TRY_CHECK;

      PortableServer::POA_var poa_var =
        PortableServer::POA::_narrow (object_var.in ());
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager_var =
        poa_var->the_POAManager ();
      ACE_TRY_CHECK;

      poa_manager_var->activate ();
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Create two handlers
      ChildHandler child_handler;
      NonRelatedChildHandler non_related_child_handler;
      
      ChildModule::AMI_ChildInterfaceHandler_var the_child_handler_var =
        child_handler._this ();
      
      // This handler has no relationship with the above in IDL.
      AMI_ChildInterfaceHandler_var the_non_related_child_handler_var =
        non_related_child_handler._this ();
      
      // Check that both handler objects narrow successfully to their parent
      // handler types...
      ParentModule::AMI_ParentInterfaceHandler_var the_parent_handler_var =
        ParentModule::AMI_ParentInterfaceHandler::_narrow (the_child_handler_var.in ());

      if (CORBA::is_nil (the_parent_handler_var.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Error: REGRESSION - Cannot narrow ChildModule::ChildInterface "
                                "reply handler to its parent handler type.\n"));
          result = 1;
        }
      
      AMI_ParentInterfaceHandler_var the_non_related_parent_handler_var =
        AMI_ParentInterfaceHandler::_narrow (the_non_related_child_handler_var.in ());

      if (CORBA::is_nil (the_non_related_parent_handler_var.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Error: REGRESSION - Cannot narrow ChildInterface "
                                "reply handler to its parent handler type.\n"));
          result = 1;
        }

      // Check that both handler objects *won't* narrow to each other's parent
      // handler types...
      the_parent_handler_var =
        ParentModule::AMI_ParentInterfaceHandler::_narrow (the_non_related_child_handler_var.in ());

      if (! CORBA::is_nil (the_parent_handler_var.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Error: REGRESSION - ChildModule::ChildInterface reply handler "
                                "narrows to unrelated type.\n"));
          result = 1;
        }
      
      the_non_related_parent_handler_var =
        AMI_ParentInterfaceHandler::_narrow (the_child_handler_var.in ());

      if (! CORBA::is_nil (the_non_related_parent_handler_var.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "Error: REGRESSION - ChildModule::ChildInterface reply handler "
                                "narrows to unrelated type.\n"));
          result = 1;
        }

      poa_var->destroy (1,0);
      
      orb->destroy ();
      
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return result;
}
