//$Id$

#include "ace/SString.h"
#include "Manager.h"
#include "test_i.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *control_ior = 0;
const ACE_TCHAR *proxy_ior = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("c:p:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'c':
        control_ior = get_opts.opt_arg ();
        break;
      case 'p':
        proxy_ior = get_opts.opt_arg ();
        break;
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Manager manager;

      // Initilaize the ORB, POA etc.
      manager.init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return -1;

      manager.activate_servant ();

      CORBA::ORB_var orb = manager.orb ();
      CORBA::Object_var server_ref = manager.server ();

      CORBA::String_var ior =
        orb->object_to_string (server_ref.in ());

      FILE *output_file = 0;

      if (proxy_ior != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Writing the servant locator object ref out to file %s\n",
                      proxy_ior));
          output_file = ACE_OS::fopen (proxy_ior, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               proxy_ior),
                               1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // this is only to shutdown the manager afterwards
      Simple_Server_i server_impl (orb.in ());

      Simple_Server_var server = server_impl._this ();

      ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the proxy_ior exists, output the ior to it
      if (control_ior != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Writing the root poa servant server IOR out to file %s\n",
                      control_ior));
          output_file = ACE_OS::fopen (control_ior, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               control_ior),
                               1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      manager.run ();
    }
  catch (const CORBA::Exception & ex)
    {
      ex._tao_print_exception ("Exception caught in manager:");
      return -1;
    }

  return 0;
}

Manager::Manager (void)
  : orb_ (0),
    new_poa_var_ (0)
{
  //no-op
}

Manager::~Manager (void)
{
  this->orb_->destroy ();
}

CORBA::ORB_ptr
Manager::orb (void)
{
  return CORBA::ORB::_duplicate (this->orb_.in ());
}

CORBA::Object_ptr
Manager::server (void)
{
  return CORBA::Object::_duplicate (this->server_.in ());
}

int
Manager::init (int argc, ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  // Obtain the RootPOA.
  CORBA::Object_var obj_var =
    this->orb_->resolve_initial_references ("RootPOA");

  // Get the POA_var object from Object_var.
  PortableServer::POA_var root_poa_var =
    PortableServer::POA::_narrow (obj_var.in ());

  // Get the POAManager of the RootPOA.
  PortableServer::POAManager_var poa_manager_var =
    root_poa_var->the_POAManager ();

  poa_manager_var->activate ();

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (4);
  policies.length (4);

  // The next two policies are common to both
  // Id Assignment Policy
  policies[0] =
    root_poa_var->create_id_assignment_policy (PortableServer::USER_ID);

  // Lifespan policy
  policies[1] =
    root_poa_var->create_lifespan_policy (PortableServer::PERSISTENT);

  // Tell the POA to use a servant manager
  policies[2] =
    root_poa_var->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

  // Servant Retention Policy -> Use a locator
  policies[3] =
    root_poa_var->create_servant_retention_policy (PortableServer::NON_RETAIN);

  ACE_CString name = "newPOA";

  this->new_poa_var_ =
    root_poa_var->create_POA (name.c_str (),
                              poa_manager_var.in (),
                              policies);

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy ();
    }

  return 0;
}

int
Manager::activate_servant ()
{
  Servant_Locator *tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    Servant_Locator,
                    CORBA::NO_MEMORY ());

  this->servant_locator_ = tmp;

  // Set ServantLocator object as the servant Manager of
  // secondPOA.
  this->new_poa_var_->set_servant_manager (this->servant_locator_.in ());

  // Try to create a reference with user created ID in new_poa
  // which uses ServantLocator.
  PortableServer::ObjectId_var oid_var =
    PortableServer::string_to_ObjectId ("Simple_Server");

  this->server_ =
    new_poa_var_->create_reference_with_id (oid_var.in (),
                                            "IDL:Simple_Server:1.0");

  return 0;
}

int
Manager::run ()
{
  this->orb_->run ();

  return 0;
}
