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
//
// ============================================================================
#include "tao_util.h"

// Initialize the ORB, using the supplied command line arguments.
// the poa_name is a user-supplied string that is used to name the
// POA created.
int
TAO_ORB_Manager::init (int argc,
                       char **argv,
                       CORBA::Environment &env)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                0,
                                env);
  TAO_CHECK_ENV_RETURN (env, 1);

  // Get the POA from the ORB
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");
  if (CORBA::is_nil(poa_object.in()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize the POA.\n"),
                      1);

  // Get the POA object
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in (), env);

  TAO_CHECK_ENV_RETURN (env, 1);

  // Get the POA_Manager
  this->poa_manager_ =
    this->root_poa_->the_POAManager (env);

  TAO_CHECK_ENV_RETURN (env, 1);

  return 0;
}

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

int
TAO_ORB_Manager::run (CORBA::Environment &env)
{
  this->poa_manager_->activate (env);
  TAO_CHECK_ENV_RETURN (env, 1);

  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"), -1);

  this->root_poa_->destroy (CORBA::B_TRUE,
                           CORBA::B_TRUE,
                           env);
  TAO_CHECK_ENV_RETURN (env, 1);

  return 0;
}

CORBA::ORB_ptr
TAO_ORB_Manager::orb (void)
{
  return this->orb_;
}
