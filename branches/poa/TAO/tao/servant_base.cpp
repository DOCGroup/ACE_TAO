#include "tao/corba.h"

TAO_ServantBase::TAO_ServantBase (void)
{
}

TAO_ServantBase::TAO_ServantBase (const TAO_ServantBase &)
{
}

TAO_ServantBase &
TAO_ServantBase::operator= (const TAO_ServantBase &)
{
  return *this;
}

TAO_ServantBase::~TAO_ServantBase (void)
{
}

PortableServer::POA_ptr
TAO_ServantBase::_default_POA (CORBA::Environment &env)
{
  CORBA_Object_ptr root_poa = 
    TAO_ORB_Core_instance ()->orb ()->resolve_initial_references ("RootPOA");

  return PortableServer::POA::_narrow (root_poa, env);
}

CORBA::Boolean 
TAO_ServantBase::_is_a (const char* logical_type_id,
			CORBA::Environment &env)
{
  if (ACE_OS::strcmp (logical_type_id, CORBA::_tc_Object->id (env)) == 0)
    {
      return CORBA::B_TRUE;
    }
  return CORBA::B_FALSE;
}

void
TAO_ServantBase::set_parent (TAO_IUnknown *p)
{
  this->parent_ = p;
  ACE_ASSERT (this->parent_ != 0);
}

TAO_IUnknown *
TAO_ServantBase::get_parent (void) const
{
  return this->parent_;
}

int
TAO_ServantBase::find (const CORBA::String &opname,
                       TAO_Skeleton& skelfunc)
{
  return optable_->find (opname, skelfunc);
}

int
TAO_ServantBase::bind (const CORBA::String &opname,
		       const TAO_Skeleton skel_ptr)
{
  return optable_->bind (opname, skel_ptr);
}

void
TAO_ServantBase::dispatch (CORBA::ServerRequest &req,
			   void *context,
			   CORBA::Environment &env)
{
  // XXXASG - we should check here if the call was for _non_existant, else
  // issue an error. For the time being we issue an error
  CORBA::String opname = req.op_name ();
  ACE_UNUSED_ARG (context);

  // Something really bad happened: the operation was not
  // found in the object, fortunately there is a standard
  // exception for that purpose.
  env.exception (new CORBA_BAD_OPERATION (CORBA::COMPLETED_NO));
  ACE_ERROR ((LM_ERROR,
              "Cannot find operation <%s> in object\n",
              opname));
}

STUB_Object *
TAO_ServantBase::_create_stub (CORBA_Environment &_env)
{
  STUB_Object *stub;

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_POA_Current *poa_current = orb_core->poa_current ();
  if (poa_current->in_upcall ())
    {
      stub = new IIOP_Object (CORBA::string_copy (this->_interface_repository_id ()),
                              IIOP::Profile (orb_core->orb_params ()->addr (),
                                             poa_current->object_key ()));
    }
  else
    {
      PortableServer::POA_var poa = this->_default_POA (_env);
      if (_env.exception () != 0)
	return 0;

      CORBA::Object_var object = poa->servant_to_reference (this, _env);
      if (_env.exception () != 0)
	return 0;

      TAO::ObjectKey_var object_key = object->key (_env);
      stub = new IIOP_Object (CORBA::string_copy (this->_interface_repository_id ()),
                              IIOP::Profile (orb_core->orb_params ()->addr (),
                                             object_key.in ()));
    }

  return stub;
}

STUB_Object *
TAO_Local_ServantBase::_create_stub (CORBA_Environment &_env)
{
  // Note the use of a fake key and no registration with POAs
  return new IIOP_Object (CORBA::string_copy (this->_interface_repository_id ()),
                          IIOP::Profile (TAO_ORB_Core_instance ()->orb_params ()->addr (),
                                         "0"));
}
