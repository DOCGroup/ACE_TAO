// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
//
// = FILENAME
//   tao_util.cpp
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "tao_util.h"

// constructor
TAO_ORB_Manager::TAO_ORB_Manager (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr root_poa,
                                  PortableServer::POAManager_ptr poa_manager)
  : orb_ (orb),
    root_poa_ (root_poa),
    poa_manager_ (poa_manager)
{
}

// Initialize the ORB, using the supplied command line arguments.  the
// poa_name is a user-supplied string that is used to name the POA
// created.
int
TAO_ORB_Manager::init (int argc,
                       char **argv,
                       CORBA::Environment &env)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0,
                                    env);
      TAO_CHECK_ENV_RETURN (env, -1);
    }

  if (CORBA::is_nil (this->root_poa_.in ()))
    {
      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      // Get the POA object.
      this->root_poa_ =
        PortableServer::POA::_narrow (poa_object.in (), env);

      TAO_CHECK_ENV_RETURN (env, -1);
    }

  if (CORBA::is_nil (this->poa_manager_.in ()))
    {

      // Get the POA_Manager.
      this->poa_manager_ =
        this->root_poa_->the_POAManager (env);

      TAO_CHECK_ENV_RETURN (env, -1);
    }

  return 0;
}

// Initialize the child poa.

int
TAO_ORB_Manager::init_child_poa (int argc,
				 char** argv,       
				 char* poa_name,
				 CORBA_Environment &env)
{
  int init_result;
  
  // check to see if root poa has to be created.
  init_result = this->init (argc,argv,env);

  if (init_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Error in init.\n"),
                      -1);

  // Create the default policies - user-supplied ID, and persistent
  // objects.
  PortableServer::PolicyList policies (2);
  policies.length (2);
  policies[0] =
    this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                                  env);
  TAO_CHECK_ENV_RETURN (env, -1);

  policies[1] =
    this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT,
                                             env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // We use a different POA, otherwise the user would have to change
  // the object key each time it invokes the server.

  this->child_poa_ =
    this->root_poa_->create_POA (poa_name,
                                 this->poa_manager_.in (),
                                 policies,
                                 env);
  TAO_CHECK_ENV_RETURN (env, -1);
  return 0;
}

// Activate servant in the POA.

CORBA::String
TAO_ORB_Manager::activate (PortableServer::Servant servant,
                           CORBA_Environment &env)
{
  PortableServer::ObjectId_var id =
    this->root_poa_->activate_object (servant,
                                      env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::Object_var obj =
    this->root_poa_->id_to_reference (id.in (),
                                      env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::String str =
    this->orb_->object_to_string (obj.in (),
                                  env);
  TAO_CHECK_ENV_RETURN (env, 0);

  return str;
}

// Activate the object with the object_name under the child POA.

CORBA::String
TAO_ORB_Manager::activate_under_child_poa (const char* object_name,
                                           PortableServer::Servant servant,
                                           CORBA_Environment& env)
{
  if (object_name == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\n(%P|%t) TAO_ORB_Manager::register: "
                       "object_name is null!"),
                      0);

  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (object_name);

  this->child_poa_->activate_object_with_id (id.in (),
                                             servant,
                                             env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::Object_var obj =
    this->child_poa_->id_to_reference (id.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env, 0);

  CORBA::String str =
    this->orb_->object_to_string (obj.in (),
                                  env);

  TAO_CHECK_ENV_RETURN (env, 0);

  return str;
}
  
// Enter the ORB event loop.

int
TAO_ORB_Manager::run (CORBA_Environment &env,
                      ACE_Time_Value *tv)
{
  this->poa_manager_->activate (env);

  TAO_CHECK_ENV_RETURN (env, -1);

  if (this->orb_->run (tv) == -1)
    ACE_ERROR_RETURN ( (LM_ERROR,
                       "%p\n",
                       "run"),
                       -1);

  TAO_CHECK_ENV_RETURN (env, -1);
  return 0;
}

// Return the corba orb reference.

CORBA::ORB_ptr
TAO_ORB_Manager::orb (void)
{
  return CORBA_ORB::_duplicate (this->orb_.in ());
}

// Destructor.

TAO_ORB_Manager::~TAO_ORB_Manager (void)
{
  CORBA::Environment env;
  if (CORBA::is_nil (this->root_poa_.in ()) == 0)
    this->root_poa_->destroy (CORBA::B_TRUE,
                              CORBA::B_TRUE,
                              env);
}
