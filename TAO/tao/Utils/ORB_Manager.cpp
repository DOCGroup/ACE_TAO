#include "tao/Utils/ORB_Manager.h"
#include "tao/Utils/PolicyList_Destroyer.h"

#include "tao/PortableServer/POAManagerC.h"
#include "tao/PortableServer/IdAssignmentPolicyC.h"
#include "tao/PortableServer/LifespanPolicyC.h"

#include "tao/ORBInitializer_Registry.h"

#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ORB_Manager::TAO_ORB_Manager (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa,
                                  PortableServer::POAManager_ptr poa_manager)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    poa_manager_ (PortableServer::POAManager::_duplicate (poa_manager))
{
}

int
TAO_ORB_Manager::init (int &argc,
                       ACE_TCHAR *argv[],
                       char const *orb_name)
{
  if (CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_ = CORBA::ORB_init (argc, argv, orb_name);
    }

  if (CORBA::is_nil (this->poa_.in ()))
    {
      // Get the POA from the ORB.
      CORBA::Object_var const poa_object =
        this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

      if (CORBA::is_nil (poa_object.in ()))
        TAOLIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      // Get the POA object.
      this->poa_ = PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (this->poa_.in ()))
        TAOLIB_ERROR_RETURN ((LM_ERROR,
                              ACE_TEXT (" (%P|%t) Unable to narrow the RootPOA reference.\n")),
                             -1);
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
TAO_ORB_Manager::init_child_poa (int &argc,
                                 ACE_TCHAR **argv,
                                 char const *poa_name,
                                 char const *orb_name)
{
  // Check to see if root poa has to be created.
  if (this->init (argc, argv, orb_name) == -1)
    TAOLIB_ERROR_RETURN ((LM_ERROR,
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

int
TAO_ORB_Manager::activate_poa_manager ()
{
  this->poa_manager_->activate ();
  return 0;
}

char *
TAO_ORB_Manager::activate (PortableServer::Servant servant)
{
  PortableServer::ObjectId_var const id = this->poa_->activate_object (servant);

  CORBA::Object_var const obj = this->poa_->id_to_reference (id.in ());

  CORBA::String_var str = this->orb_->object_to_string (obj.in ());

  return str._retn ();
}

void
TAO_ORB_Manager::deactivate (char const *id)
{
  CORBA::Object_var const object = this->orb_->string_to_object (id);

  PortableServer::ObjectId_var const object_id =
    this->poa_->reference_to_id (object.in ());

  this->poa_->deactivate_object (object_id.in ());
}

#if !defined (CORBA_E_MICRO)
/// Activate the object with the object name under the child POA.
char *
TAO_ORB_Manager::activate_under_child_poa (char const *object_name,
                                           PortableServer::Servant servant)
{
  if (object_name == nullptr)
    {
      TAOLIB_ERROR ((LM_ERROR,
                     ACE_TEXT ("\n(%P|%t) TAO_ORB_Manager::activate_under_child_poa: ")
                     ACE_TEXT ("object_name is null!")));
      return nullptr;
    }

  PortableServer::ObjectId_var const id =
    PortableServer::string_to_ObjectId (object_name);

  this->child_poa_->activate_object_with_id (id.in (), servant);

  CORBA::Object_var const obj = this->child_poa_->id_to_reference (id.in ());

  CORBA::String_var str = this->orb_->object_to_string (obj.in ());

  return str._retn ();
}

void
TAO_ORB_Manager::deactivate_under_child_poa (char const *id)
{
  CORBA::Object_var const object = this->orb_->string_to_object (id);

  PortableServer::ObjectId_var const object_id =
    this->child_poa_->reference_to_id (object.in ());

  this->child_poa_->deactivate_object (object_id.in ());
}
#endif /* CORBA_E_MICRO */

int
TAO_ORB_Manager::run (ACE_Time_Value &tv)
{
  this->poa_manager_->activate ();

  this->orb_->run (tv);

  return 0;
}

int
TAO_ORB_Manager::fini ()
{
  if (!CORBA::is_nil (this->poa_.in ()))
    this->poa_->destroy (true, true);

  this->child_poa_   = PortableServer::POA::_nil ();
  this->poa_         = PortableServer::POA::_nil ();
  this->poa_manager_ = PortableServer::POAManager::_nil ();

  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->destroy ();

  this->orb_ = CORBA::ORB::_nil ();

  return 0;
}

int
TAO_ORB_Manager::run ()
{
  this->poa_manager_->activate ();

  this->orb_->run ();

  return 0;
}

/// Return a duplicate of the ORB reference.
CORBA::ORB_ptr
TAO_ORB_Manager::orb () const
{
  return CORBA::ORB::_duplicate (this->orb_.in ());
}

/// Return a duplicate of the root POA reference.
PortableServer::POA_ptr
TAO_ORB_Manager::root_poa () const
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

/// Return a duplicate of the child POA reference.
PortableServer::POA_ptr
TAO_ORB_Manager::child_poa () const
{
  return PortableServer::POA::_duplicate (this->child_poa_.in ());
}

PortableServer::POAManager_ptr
TAO_ORB_Manager::poa_manager () const
{
  return PortableServer::POAManager::_duplicate (this->poa_manager_.in ());
}

TAO_ORB_Manager::~TAO_ORB_Manager ()
{
  try
    {
      if (!CORBA::is_nil (this->poa_.in ()))
        {
          this->poa_->destroy (true, true);
        }
      if (!CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_->destroy ();
        }
    }
  catch (::CORBA::Exception const &)
    {
      // Ignore exceptions during destruction.
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
