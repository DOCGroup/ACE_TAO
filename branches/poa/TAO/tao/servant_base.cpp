#include "tao/corba.h"

TAO_ServantBase::TAO_ServantBase (void)
  :  optable_ (0),
     parent_ (0)
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
  TAO_POA *poa = TAO_ORB_Core_instance ()->root_poa ();
  PortableServer::POA_var result = poa->_this (env);
  if (env.exception () != 0)
    return PortableServer::POA::_nil ();
  else
    return result._retn ();  
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
TAO_ServantBase::_set_parent (TAO_IUnknown *p)
{
  this->parent_ = p;
  ACE_ASSERT (this->parent_ != 0);
}

TAO_IUnknown *
TAO_ServantBase::_get_parent (void) const
{
  return this->parent_;
}

int
TAO_ServantBase::_find (const CORBA::String &opname,
                        TAO_Skeleton& skelfunc)
{
  return optable_->find (opname, skelfunc);
}

int
TAO_ServantBase::_bind (const CORBA::String &opname,
                        const TAO_Skeleton skel_ptr)
{
  return optable_->bind (opname, skel_ptr);
}

void
TAO_ServantBase::_dispatch (CORBA::ServerRequest &req,
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
TAO_ServantBase::_create_stub (CORBA_Environment &env)
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
      PortableServer::POA_var poa = this->_default_POA (env);
      if (env.exception () != 0)
	return 0;

      CORBA::Object_var object = poa->servant_to_reference (this, env);
      if (env.exception () != 0)
	return 0;

      TAO::ObjectKey_var object_key = object->key (env);
      stub = new IIOP_Object (CORBA::string_copy (this->_interface_repository_id ()),
                              IIOP::Profile (orb_core->orb_params ()->addr (),
                                             object_key.in ()));
    }

  return stub;
}

STUB_Object *
TAO_Local_ServantBase::_create_stub (CORBA_Environment &env)
{
  // Note the use of a fake key and no registration with POAs
  return new IIOP_Object (CORBA::string_copy (this->_interface_repository_id ()),
                          IIOP::Profile (TAO_ORB_Core_instance ()->orb_params ()->addr (),
                                         "0"));
}

CORBA::Object_ptr 
TAO_DynamicImplementation::_this (CORBA::Environment &env)
{
  // The _this() function returns a CORBA::Object_ptr for the target
  // object. Unlike _this() for static skeletons, its return type is
  // not interface-specific because a DSI servant may very well
  // incarnate multiple CORBA objects of different types. 
  STUB_Object *stub = this->_create_stub (env);
  if (env.exception () != 0)
    return CORBA::Object::_nil ();
  
  // Create a object
  return new CORBA::Object (stub, this, CORBA::B_TRUE);
}

const char *
TAO_DynamicImplementation::_interface_repository_id (void) const
{
  // This should never be called
  return 0;
}

void *
TAO_DynamicImplementation::_downcast (const char *repository_id)
{
  ACE_UNUSED_ARG (repository_id);

  // Don't know enough to do better
  return this;
}

STUB_Object *
TAO_DynamicImplementation::_create_stub (CORBA::Environment &env)
{
  // If DynamicImplementation::_this() is invoked outside of the
  // context of a request invocation on a target object being served
  // by the DSI servant, it raises the PortableServer::WrongPolicy
  // exception.
  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_POA_Current *poa_current = orb_core->poa_current ();
  if (!poa_current->in_upcall ())
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  PortableServer::POA_var poa = poa_current->get_POA (env);
  if (env.exception () != 0)
    return 0;

  // CORBA::RepositoryId interface = this->_primary_interface (poa_current->object_id (),
  //                                                           poa.in (),
  //                                                           env);
  PortableServer::RepositoryId interface = this->_primary_interface (poa_current->object_id (),
                                                                     poa.in (),
                                                                     env);
  if (env.exception () != 0)
    return 0;

  return new IIOP_Object (interface,
                          IIOP::Profile (orb_core->orb_params ()->addr (),
                                         poa_current->object_key ()));
}

void 
TAO_DynamicImplementation::_dispatch (CORBA::ServerRequest &request,
                                      void *context,
                                      CORBA::Environment &env) 
{
  ACE_UNUSED_ARG (context);

  // Delegate to user
  this->invoke (&request, env);
}
