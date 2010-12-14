// $Id$

// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>

#include "tao/Utils/ORB_Manager.h"
#include "tao/Utils/PolicyList_Destroyer.h"

#include "tao/PortableServer/POAManagerC.h"
#include "tao/PortableServer/IdAssignmentPolicyC.h"
#include "tao/PortableServer/LifespanPolicyC.h"


#include "tao/ORBInitializer_Registry.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// constructor
TAO_ORB_Manager::TAO_ORB_Manager (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa,
                                  PortableServer::POAManager_ptr poa_manager)
  : orb_ (CORBA::ORB::_duplicate(orb)),
    poa_ (PortableServer::POA::_duplicate(poa)),
    poa_manager_ (PortableServer::POAManager::_duplicate(poa_manager))
{
}

int
TAO_ORB_Manager::init (int &argc,
                       ACE_TCHAR *argv[],
                       const char *orb_name)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_ = CORBA::ORB_init (argc, argv, orb_name);
    }

  if (CORBA::is_nil (this->poa_.in ()))
    {
      // Get the POA from the ORB.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      // Get the POA object.
      this->poa_ = PortableServer::POA::_narrow (poa_object.in ());
    }

  if (CORBA::is_nil (this->poa_manager_.in ()))
    {
      // Get the POA_Manager.
      this->poa_manager_ = this->poa_->the_POAManager ();
    }

  return 0;
}

#if !defined (CORBA_E_MICRO)
int
TAO_ORB_Manager::init_child_poa (int& argc,
                                 ACE_TCHAR **argv,
                                 const char *poa_name,
                                 const char *orb_name)
{
  // Check to see if root poa has to be created.
  if (this->init (argc, argv, orb_name) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Error in init.\n")),
                      -1);

  // Create the default policies - user-supplied ID, and persistent
  // objects.
  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  // Id Assignment policy
  policies[0] =
    this->poa_->create_id_assignment_policy (PortableServer::USER_ID);

  // Lifespan policy
  policies[1] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT);

  // We use a different POA, otherwise the user would have to change
  // the object key each time it invokes the server.
  this->child_poa_ =
    this->poa_->create_POA (poa_name, this->poa_manager_.in (), policies);

  return 0;
}
#endif /* CORBA_E_MICRO */

// Activate POA manager.

int
TAO_ORB_Manager::activate_poa_manager (void)
{
  this->poa_manager_->activate ();
  return 0;
}

// Activate servant in the POA.

char *
TAO_ORB_Manager::activate (PortableServer::Servant servant)
{
  PortableServer::ObjectId_var id = this->poa_->activate_object (servant);

  CORBA::Object_var obj = this->poa_->id_to_reference (id.in ());

  CORBA::String_var str = this->orb_->object_to_string (obj.in ());

  return str._retn ();
}

void
TAO_ORB_Manager::deactivate (const char *id)
{
  CORBA::Object_var object = this->orb_->string_to_object (id);

  PortableServer::ObjectId_var object_id =
    this->poa_->reference_to_id (object.in ());

  this->poa_->deactivate_object (object_id.in ());
}

#if !defined (CORBA_E_MICRO)
// Activate the object with the object_name under the child POA.
char *
TAO_ORB_Manager::activate_under_child_poa (const char *object_name,
                                           PortableServer::Servant servant)
{
  if (object_name == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\n(%P|%t) TAO_ORB_Manager::register: ")
                       ACE_TEXT ("object_name is null!")),
                      0);

  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (object_name);

  this->child_poa_->activate_object_with_id (id.in (), servant);

  CORBA::Object_var obj = this->child_poa_->id_to_reference (id.in ());

  CORBA::String_var str = this->orb_->object_to_string (obj.in ());

  return str._retn();
}

void
TAO_ORB_Manager::deactivate_under_child_poa (const char *id)
{
  CORBA::Object_var object = this->orb_->string_to_object (id);

  PortableServer::ObjectId_var object_id =
    this->child_poa_->reference_to_id (object.in ());

  this->child_poa_->deactivate_object (object_id.in ());
}
#endif /* CORBA_E_MICRO */

// Enter the ORB event loop.

int
TAO_ORB_Manager::run (ACE_Time_Value &tv)
{
  this->poa_manager_->activate ();

  this->orb_->run (tv);

  return 0;
}

int
TAO_ORB_Manager::fini (void)
{
  this->poa_->destroy (1, 1);

  this->child_poa_   = PortableServer::POA::_nil();
  this->poa_         = PortableServer::POA::_nil();
  this->poa_manager_ = PortableServer::POAManager::_nil();

  this->orb_->destroy ();

  this->orb_ = CORBA::ORB::_nil();

  return 0;
}

int
TAO_ORB_Manager::run (void)
{
  this->poa_manager_->activate ();

  this->orb_->run ();

  return 0;
}

// Return the corba orb reference.

CORBA::ORB_ptr
TAO_ORB_Manager::orb (void)
{
  return CORBA::ORB::_duplicate (this->orb_.in ());
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
  try
    {
      if (!CORBA::is_nil (this->poa_.in ()))
        {
          this->poa_->destroy (1,1);
        }
      if (!CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_->destroy ();
        }
    }
  catch (const ::CORBA::Exception&)
    {
      // ignore any exceptions..
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
