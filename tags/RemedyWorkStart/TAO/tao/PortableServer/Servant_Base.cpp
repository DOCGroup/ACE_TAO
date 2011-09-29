// -*- C++ -*-
// $Id$

#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Operation_Table.h"
#include "tao/PortableServer/POA_Current_Impl.h"

#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/TSS_Resources.h"
#include "tao/Stub.h"
#include "tao/TAO_Server_Request.h"
#include "tao/IFR_Client_Adapter.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

//@@ TAO_SERVANT_BASE_INCLUDE_ADD_HOOK

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_Base.inl"
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ServantBase::TAO_ServantBase (void)
  : TAO_Abstract_ServantBase ()
  , ref_count_ (1)
  , optable_ (0)
{
}

TAO_ServantBase::TAO_ServantBase (const TAO_ServantBase &rhs)
  : TAO_Abstract_ServantBase ()
  , ref_count_ (1)
  , optable_ (rhs.optable_)
{
}

TAO_ServantBase &
TAO_ServantBase::operator= (const TAO_ServantBase &rhs)
{
  this->optable_ = rhs.optable_;
  return *this;
}

TAO_ServantBase::~TAO_ServantBase (void)
{
}

PortableServer::POA_ptr
TAO_ServantBase::_default_POA (void)
{
  CORBA::Object_var object = TAO_ORB_Core_instance ()->root_poa ();

  return PortableServer::POA::_narrow (object.in ());
}

CORBA::Boolean
TAO_ServantBase::_is_a (const char *logical_type_id)
{
  static char const id[] = "IDL:omg.org/CORBA/Object:1.0";
  return ACE_OS::strcmp (logical_type_id, id) == 0;
}

CORBA::Boolean
TAO_ServantBase::_non_existent (void)
{
  return false;
}

CORBA::InterfaceDef_ptr
TAO_ServantBase::_get_interface (void)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ());

  if (adapter == 0)
    {
      throw ::CORBA::INTF_REPOS ();
    }

  // This doesn't take multiple ORBs into account, but it's being
  // used only to resolve the IFR, so we should be ok.
  return adapter->get_interface (TAO_ORB_Core_instance ()->orb (),
                                 this->_interface_repository_id ());
}

CORBA::Object_ptr
TAO_ServantBase::_get_component (void)
{
  return CORBA::Object::_nil ();
}

char *
TAO_ServantBase::_repository_id (void)
{
  return CORBA::string_dup (this->_interface_repository_id ());
}

int
TAO_ServantBase::_find (const char *opname,
                        TAO_Skeleton& skelfunc,
                        const size_t length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_BASE_FIND_START);
  return this->optable_->find (opname, skelfunc, length);
}

int
TAO_ServantBase::_find (const char *opname,
                        TAO_Collocated_Skeleton& skelfunc,
                        TAO::Collocation_Strategy st,
                        const size_t length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_BASE_FIND_START);
  return this->optable_->find (opname, skelfunc, st, length);
}

TAO_Stub *
TAO_ServantBase::_create_stub (void)
{
  TAO_Stub *stub = 0;

  TAO::Portable_Server::POA_Current_Impl *poa_current_impl =
    static_cast<TAO::Portable_Server::POA_Current_Impl *>
                    (TAO_TSS_Resources::instance ()->poa_current_impl_);

  CORBA::ORB_ptr servant_orb = 0;

  if (poa_current_impl != 0
      && this == poa_current_impl->servant ())
    {
      servant_orb = poa_current_impl->orb_core ().orb () ;

      stub =
        poa_current_impl->poa ()->key_to_stub (
            poa_current_impl->object_key (),
            this->_interface_repository_id (),
            poa_current_impl->priority ());
    }
  else
    {
      PortableServer::POA_var poa = this->_default_POA ();

      CORBA::Object_var object = poa->servant_to_reference (this);

      // Get the stub object
      stub = object->_stubobj ();

      // Increment the reference count since <object> will zap its
      // stub object on deletion.
      stub->_incr_refcnt ();

      servant_orb = stub->orb_core ()->orb ();
    }

  stub->servant_orb (servant_orb);
  return stub;
}

void TAO_ServantBase::synchronous_upcall_dispatch (TAO_ServerRequest & req,
                                                   void * servant_upcall,
                                                   void * derived_this)
{
  TAO_Skeleton skel;
  char const * const opname = req.operation ();

  // It seems that I might have missed s/g here.  What if
  // it is a one way that is SYNC_WITH_SERVER.
  // Add the following line to handle this reply send as well.

  // Handle the one ways that are SYNC_WITH_SERVER and not collocated
  if (req.sync_with_server () && !req.collocated ())
    {
      req.send_no_exception_reply ();
    }

  // Fetch the skeleton for this operation
  if (this->_find (opname,
                   skel,
                   static_cast <unsigned int> (req.operation_length())) == -1)
    {
      throw ::CORBA::BAD_OPERATION ();
    }

  CORBA::Boolean const send_reply =
    !req.sync_with_server ()
    && req.response_expected ()
    && !req.deferred_reply ();

  try
    {
      // Invoke the skeleton, it will demarshal the arguments, invoke
      // the right operation on the skeleton class, and marshal any
      // results.  De/marshaling will only occur in the uncollocated
      // case.
      skel (req, servant_upcall, derived_this);

      /*
       * Dispatch resolution specialization add hook.
       * Over-ridden with code to handle optimized dispatch.
       */
      //@@ TAO_DISPATCH_RESOLUTION_OPT_ADD_HOOK

      // It is our job to send the already marshaled reply, but only
      // send if it is expected and it has not already been sent

      // We send the reply only if it is NOT a SYNC_WITH_SERVER, a
      // response is expected and if the reply is not deferred.
      if (send_reply)
        {
          req.tao_send_reply ();
        }
    }
  catch (const ::CORBA::Exception& ex)
    {
      // If an exception was raised we should marshal it and send
      // the appropriate reply to the client
      if (send_reply)
        {
          if (req.collocated ())
            {
              // Report the exception to the collocated client.
              throw;
            }
          else
            req.tao_send_reply_exception (ex);
        }
    }

  return;
}

void TAO_ServantBase::asynchronous_upcall_dispatch (TAO_ServerRequest & req,
                                                    void * servant_upcall,
                                                    void * derived_this)
{
  TAO_Skeleton skel;
  const char *opname = req.operation ();

  // It seems that I might have missed s/g here.  What if
  // it is a one way that is SYNC_WITH_SERVER.
  // Add the following line to handle this reply send as well.

  // Handle the one ways that are SYNC_WITH_SERVER
  if (req.sync_with_server ())
    {
      req.send_no_exception_reply ();
    }

  // Fetch the skeleton for this operation
  if (this->_find (opname,
                   skel,
                   static_cast <unsigned int> (req.operation_length())) == -1)
    {
      throw ::CORBA::BAD_OPERATION ();
    }

  try
    {
      // Invoke the skeleton, it will demarshal the arguments, invoke
      // the right operation on the skeleton class, and marshal any
      // results.  De/marshaling will only occur in the uncollocated
      // case.
      skel (req, servant_upcall, derived_this);

      // It is our job to send the already marshaled reply, but only
      // send if it is expected and it has not already been sent

      // Return immediately. Do not send a reply; this is an
      // asynchronous upcall. (unless, of course there is a system
      // exception.

    }
  catch (const ::CORBA::Exception& ex)
    {
      // If an exception was raised we should marshal it and send
      // the appropriate reply to the client
      req.tao_send_reply_exception (ex);
    }

  return;
}

void
TAO_ServantBase::_add_ref (void)
{
  ++this->ref_count_;
}

void
TAO_ServantBase::_remove_ref (void)
{
  unsigned long const new_count = --this->ref_count_;

  if (new_count == 0)
    {
      delete this;
    }
}

CORBA::ULong
TAO_ServantBase::_refcount_value (void) const
{
  return this->ref_count_.value ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
