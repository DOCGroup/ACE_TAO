// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
//
// = FILENAME
//   TAO.cpp
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "tao/TAO.h"
#include "tao/Environment.h"

ACE_RCSID(tao, TAO, "$Id$")

// constructor
TAO_ORB_Manager::TAO_ORB_Manager (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa,
                                  PortableServer::POAManager_ptr poa_manager)
  : orb_ (orb),
    poa_ (poa),
    poa_manager_ (poa_manager)
{
}

int
TAO_ORB_Manager::init (int &argc,
                       char **argv,
                       CORBA::Environment &ACE_TRY_ENV)
{
  return this->init (argc,
                     argv,
                     0,
                     ACE_TRY_ENV);
}

int
TAO_ORB_Manager::init (int &argc,
                       char **argv,
                       const char *orb_name,
                       CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    orb_name,
                                    ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }

  if (CORBA::is_nil (this->poa_.in ()))
    {
      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA,
                                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      // Get the POA object.
      this->poa_ =
        PortableServer::POA::_narrow (poa_object.in (),
                                      ACE_TRY_ENV);

      ACE_CHECK_RETURN (-1);
    }

  if (CORBA::is_nil (this->poa_manager_.in ()))
    {
      // Get the POA_Manager.
      this->poa_manager_ =
        this->poa_->the_POAManager (ACE_TRY_ENV);

      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

int
TAO_ORB_Manager::init_child_poa (int& argc,
                                 char **argv,
                                 const char *poa_name,
                                 CORBA_Environment &ACE_TRY_ENV)
{
  return this->init_child_poa (argc,
                               argv,
                               poa_name,
                               0,
                               ACE_TRY_ENV);
}

int
TAO_ORB_Manager::init_child_poa (int& argc,
                                 char **argv,
                                 const char *poa_name,
                                 const char *orb_name,
                                 CORBA_Environment &ACE_TRY_ENV)
{
  int init_result;

  // check to see if root poa has to be created.
  init_result = this->init (argc,
                            argv,
                            orb_name,
                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (init_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT (" (%P|%t) Error in init.\n")),
                      -1);

  // Create the default policies - user-supplied ID, and persistent
  // objects.
  CORBA::PolicyList policies (2);
  policies.length (2);

  // Id Assignment policy
  policies[0] =
    this->poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Lifespan policy
  policies[1] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // We use a different POA, otherwise the user would have to change
  // the object key each time it invokes the server.

  this->child_poa_ =
    this->poa_->create_POA (poa_name,
                            this->poa_manager_.in (),
                            policies,
                            ACE_TRY_ENV);
  // Warning!  If create_POA fails, then the policies won't be
  // destroyed and there will be hell to pay in memory leaks!
  ACE_CHECK_RETURN (-1);

  // Creation of the new POAs over, so destroy the Policy_ptr's.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

// Activate POA manager.

int
TAO_ORB_Manager::activate_poa_manager (CORBA_Environment &ACE_TRY_ENV)
{
  this->poa_manager_->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  return 0;
}

// Activate servant in the POA.

char *
TAO_ORB_Manager::activate (PortableServer::Servant servant,
                           CORBA_Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var id =
    this->poa_->activate_object (servant,
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var obj =
    this->poa_->id_to_reference (id.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in (),
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return str._retn ();
}

void
TAO_ORB_Manager::deactivate (const char *id,
                             CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::Object_var object =
    this->orb_->string_to_object (id,
                                  ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var object_id =
    this->poa_->reference_to_id (object.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;

  this->poa_->deactivate_object (object_id.in (),
                                 ACE_TRY_ENV);
}

// Activate the object with the object_name under the child POA.

char *
TAO_ORB_Manager::activate_under_child_poa (const char* object_name,
                                           PortableServer::Servant servant,
                                           CORBA_Environment& ACE_TRY_ENV)
{
  if (object_name == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("\n(%P|%t) TAO_ORB_Manager::register: ")
                       ASYS_TEXT ("object_name is null!")),
                      0);

  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (object_name);

  this->child_poa_->activate_object_with_id (id.in (),
                                             servant,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var obj =
    this->child_poa_->id_to_reference (id.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  char * str =
    this->orb_->object_to_string (obj.in (),
                                  ACE_TRY_ENV);

  ACE_CHECK_RETURN (0);

  return str;
}

void
TAO_ORB_Manager::deactivate_under_child_poa (const char *id,
                                             CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::Object_var object =
    this->orb_->string_to_object (id,
                                  ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var object_id =
    this->child_poa_->reference_to_id (object.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK;

  this->child_poa_->deactivate_object (object_id.in (),
                                       ACE_TRY_ENV);
}

// Enter the ORB event loop.

int
TAO_ORB_Manager::run (CORBA_Environment &ACE_TRY_ENV,
                      ACE_Time_Value *tv)
{
  this->poa_manager_->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  int status = this->orb_->run (tv, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (status == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) TAO_ORB_Manager %p\n"),
                       ASYS_TEXT ("run")),
                      -1);
  return 0;
}

int
TAO_ORB_Manager::run (ACE_Time_Value &tv,
                      CORBA_Environment &ACE_TRY_ENV)
{
  this->poa_manager_->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  int status = this->orb_->run (tv, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (status == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) TAO_ORB_Manager %p\n"),
                       ASYS_TEXT ("run")),
                      -1);
  return 0;
}

int
TAO_ORB_Manager::fini (CORBA_Environment &ACE_TRY_ENV)
{
  this->poa_->destroy (1, 1, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->poa_ = 0;

  this->orb_->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->orb_ = 0;
  return 0;
}

int
TAO_ORB_Manager::run (CORBA_Environment &ACE_TRY_ENV)
{
  this->poa_manager_->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  int status = this->orb_->run (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (status == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) TAO_ORB_Manager %p\n"),
                       ASYS_TEXT ("run")),
                      -1);
  return 0;
}

// Return the corba orb reference.

CORBA::ORB_ptr
TAO_ORB_Manager::orb (void)
{
  return CORBA_ORB::_duplicate (this->orb_.in ());
}

// Return the root POA reference
PortableServer::POA_ptr
TAO_ORB_Manager::root_poa (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Return the child POA reference
PortableServer::POA_ptr
TAO_ORB_Manager::child_poa (void)
{
  return PortableServer::POA::_duplicate (this->child_poa_.in ());
}

PortableServer::POAManager_ptr
TAO_ORB_Manager::poa_manager (void)
{
  return PortableServer::POAManager::_duplicate (this->poa_manager_.in ());
}

// Destructor.

TAO_ORB_Manager::~TAO_ORB_Manager (void)
{
  ACE_TRY_NEW_ENV
    {
      if (!CORBA::is_nil (this->poa_.in ()))
        {
          this->poa_->destroy (1,
                               1,
                               ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      // ignore any exceptions..
    }
  ACE_ENDTRY;
}
