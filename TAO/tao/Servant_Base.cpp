// $Id$

#include "tao/Servant_Base.h"
#include "tao/Timeprobe.h"
#include "tao/Operation_Table.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/Server_Request.h"

ACE_RCSID(tao, Servant_Base, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/Servant_Base.i"
#endif /* ! __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Servant_Base_Timeprobe_Description[] =
{
  "Servant_Base::_find - start",
  "Servant_Base::_find - end"
};

enum
{
  TAO_SERVANT_BASE_FIND_START = 700,
  TAO_SERVANT_BASE_FIND_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Servant_Base_Timeprobe_Description,
                                  TAO_SERVANT_BASE_FIND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_ServantBase::TAO_ServantBase (void)
  :  optable_ (0)
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
  return TAO_ORB_Core_instance ()->root_poa_reference (env);
}

CORBA::Boolean
TAO_ServantBase::_is_a (const char* logical_type_id,
			CORBA::Environment &env)
{
  if (ACE_OS::strcmp (logical_type_id, CORBA::_tc_Object->id (env)) == 0)
    {
      return 1;
    }
  return 0;
}

int
TAO_ServantBase::_find (const char *opname,
                        TAO_Skeleton& skelfunc)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_BASE_FIND_START);
  return optable_->find (opname, skelfunc);
}

int
TAO_ServantBase::_bind (const char *opname,
                        const TAO_Skeleton skel_ptr)
{
  return optable_->bind (opname, skel_ptr);
}

STUB_Object *
TAO_ServantBase::_create_stub (CORBA_Environment &env)
{
  STUB_Object *stub;

  TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();
  TAO_POA_Current *poa_current = orb_core->poa_current ();

  if (poa_current != 0
      && poa_current->in_upcall ()
      && this == poa_current->servant ())
    {
      stub = orb_core->orb ()->create_stub_object (poa_current->object_key (),
                                                   this->_interface_repository_id (),
                                                   env);
    }
  else
    {
      PortableServer::POA_var poa = this->_default_POA (env);
      if (env.exception () != 0)
	return 0;

      CORBA::Object_var object = poa->servant_to_reference (this, env);
      if (env.exception () != 0)
	return 0;

      // Get the stub object
      stub = object->_stubobj ();

      // Increment the reference count since <object> will zap its
      // stub object on deletion.
      stub->_incr_refcnt ();
    }

  return stub;
}

STUB_Object *
TAO_Local_ServantBase::_create_stub (CORBA_Environment &env)
{
  PortableServer::ObjectId_var invalid_oid =
    PortableServer::string_to_ObjectId ("invalid");

  TAO_ObjectKey tmp_key (invalid_oid->length (),
                         invalid_oid->length (),
                         invalid_oid->get_buffer (),
                         0);
  // Note the use of a fake key and no registration with POAs
  return TAO_ORB_Core_instance ()->orb ()->create_stub_object (tmp_key,
                                                               this->_interface_repository_id (),
                                                               env);
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

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
  return new CORBA::Object (stub, this, 1);
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

  if (poa_current == 0
      || !poa_current->in_upcall ()
      || this != poa_current->servant ())
    {
      CORBA::Exception *exception = new PortableServer::POA::WrongPolicy;
      env.exception (exception);
      return 0;
    }

  PortableServer::POA_var poa = poa_current->get_POA (env);
  if (env.exception () != 0)
    return 0;

  CORBA::RepositoryId interface = this->_primary_interface (poa_current->object_id (),
                                                            poa.in (),
                                                            env);
  if (env.exception () != 0)
    return 0;

  return TAO_ORB_Core_instance ()->orb ()->create_stub_object (poa_current->object_key (),
                                                               interface,
                                                               env);
}

void
TAO_DynamicImplementation::_dispatch (CORBA::ServerRequest &request,
                                      void *context,
                                      CORBA::Environment &env)
{
  ACE_UNUSED_ARG (context);

  // Delegate to user
  this->invoke (&request, env);

  if (request.response_expected ())
    {
      request.init_reply (env);
      request.dsi_marshal (env);
    }
}

#endif /* TAO_HAS_MINIMUM_CORBA */
