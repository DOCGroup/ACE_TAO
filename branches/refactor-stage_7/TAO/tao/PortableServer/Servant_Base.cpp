// $Id$

#include "Servant_Base.h"
#include "POA.h"
#include "Operation_Table.h"

#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/TSS_Resources.h"
#include "tao/Stub.h"
#include "tao/Environment.h"
#include "tao/TAO_Server_Request.h"
#include "tao/IFR_Client_Adapter.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID (tao,
           Servant_Base,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "Servant_Base.i"
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
  : TAO_Abstract_ServantBase ()
    , optable_ (0)
{
}

TAO_ServantBase::TAO_ServantBase (const TAO_ServantBase &rhs)
  : TAO_Abstract_ServantBase ()
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
TAO_ServantBase::_default_POA (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var object =
    TAO_ORB_Core_instance ()->root_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  return PortableServer::POA::_narrow (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ServantBase::_is_a (const char *logical_type_id
                        ACE_ENV_ARG_DECL)
{
  const char *id = "IDL:omg.org/CORBA/Object:1.0";
  return ACE_OS::strcmp (logical_type_id, id) == 0;
}

CORBA::Boolean
TAO_ServantBase::_non_existent (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return 0;
}

CORBA::InterfaceDef_ptr
TAO_ServantBase::_get_interface (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        0);
    }

  // This doesn't take multiple ORBs into account, but it's being
  // used only to resolve the IFR, so we should be ok.
  return adapter->get_interface (TAO_ORB_Core_instance ()->orb (),
                                 this->_interface_repository_id ()
                                 ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_ServantBase::_get_component (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return CORBA::Object::_nil ();
}

int
TAO_ServantBase::_find (const char *opname,
                        TAO_Skeleton& skelfunc,
                        const unsigned int length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_BASE_FIND_START);
  return optable_->find (opname, skelfunc, length);
}

int
TAO_ServantBase::_find (const char *opname,
                        TAO_Collocated_Skeleton& skelfunc,
                        TAO::Collocation_Strategy st,
                        const unsigned int length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVANT_BASE_FIND_START);
  return optable_->find (opname, skelfunc, st, length);
}

/*int
TAO_ServantBase::_bind (const char *opname,
                        const TAO_Skeleton skel_ptr)
{
  return optable_->bind (opname, skel_ptr);
}
*/
TAO_Stub *
TAO_ServantBase::_create_stub (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Stub *stub = 0;

  TAO_POA_Current_Impl *poa_current_impl =
    ACE_static_cast(TAO_POA_Current_Impl *,
                    TAO_TSS_RESOURCES::instance ()->poa_current_impl_);

  CORBA::ORB_ptr servant_orb = 0;

  if (poa_current_impl != 0
      && this == poa_current_impl->servant ())
    {
      servant_orb = poa_current_impl->orb_core ().orb () ;


      stub =
        poa_current_impl->poa ()->key_to_stub (
            poa_current_impl->object_key (),
            this->_interface_repository_id (),
            poa_current_impl->priority ()
            ACE_ENV_ARG_PARAMETER
          );
      ACE_CHECK_RETURN (0);
    }
  else
    {
      PortableServer::POA_var poa =
        this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CORBA::Object_var object =
        poa->servant_to_reference (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

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

void TAO_ServantBase::synchronous_upcall_dispatch (TAO_ServerRequest &req,
                                                   void *servant_upcall,
                                                   void *derived_this
                                                   ACE_ENV_ARG_DECL)
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
  if (this->_find (opname, skel,
         ACE_static_cast (unsigned int, req.operation_length())) == -1)
    {
      ACE_THROW (CORBA::BAD_OPERATION ());
    }

  CORBA::Boolean send_reply = !req.sync_with_server ()
    && req.response_expected ()
    && !req.deferred_reply ();

  ACE_TRY
    {
      // Invoke the skeleton, it will demarshal the arguments,
      // invoke the right operation on the skeleton class
      // (<derived_this>), and marshal any results.
      skel (req,
            derived_this,
            servant_upcall
            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // It is our job to send the already marshaled reply, but only
      // send if it is expected and it has not already been sent

      // We send the reply only if it is NOT a SYNC_WITH_SERVER, a
      // response is expected and if the reply is not deferred.
      if (send_reply)
        {
          req.tao_send_reply ();
        }

    }
  ACE_CATCH (CORBA::Exception,ex)
    {
      // If an exception was raised we should marshal it and send
      // the appropriate reply to the client
      if (send_reply)
        {
          req.tao_send_reply_exception (ex);
        }
    }
  ACE_ENDTRY;
  ACE_CHECK;

  return;
}

void TAO_ServantBase::asynchronous_upcall_dispatch (TAO_ServerRequest &req,
                                                    void *servant_upcall,
                                                    void *derived_this
                                                    ACE_ENV_ARG_DECL)
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
  if (this->_find (opname, skel,
         ACE_static_cast (unsigned int, req.operation_length())) == -1)
    {
      ACE_THROW (CORBA::BAD_OPERATION ());
    }

  ACE_TRY
    {
      // Invoke the skeleton, it will demarshal the arguments,
      // invoke the right operation on the skeleton class
      // (<derived_this>), and marshal any results.
      skel (req,
            derived_this,
            servant_upcall
            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // It is our job to send the already marshaled reply, but only
      // send if it is expected and it has not already been sent

      // Return immediately. Do not send a reply; this is an
      // asunchronous upcall. (unless, of course there is a system
      // exception.

    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      // If an exception was raised we should marshal it and send
      // the appropriate reply to the client
      req.tao_send_reply_exception (ex);
    }
  ACE_ENDTRY;
  ACE_CHECK;

  return;
}


TAO_RefCountServantBase::~TAO_RefCountServantBase (void)
{
}

void
TAO_RefCountServantBase::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  ++this->ref_count_;
}

void
TAO_RefCountServantBase::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  const CORBA::ULong new_count = --this->ref_count_;

  if (new_count == 0)
    delete this;
}

long
TAO_RefCountServantBase::_ref_count (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return this->ref_count_.value ();
}

TAO_RefCountServantBase::TAO_RefCountServantBase (void)
  : TAO_Abstract_ServantBase ()
    , TAO_ServantBase ()
    , ref_count_ (1)
{
}

TAO_RefCountServantBase::TAO_RefCountServantBase (
    const TAO_RefCountServantBase &
  )
  : TAO_Abstract_ServantBase ()
    , TAO_ServantBase ()
    , ref_count_ (1)
{
}

TAO_RefCountServantBase &
TAO_RefCountServantBase::operator= (const TAO_RefCountServantBase &)
{
  return *this;
}

TAO_ServantBase_var::TAO_ServantBase_var (void)
  : ptr_ (0)
{
}

TAO_ServantBase_var::TAO_ServantBase_var (TAO_ServantBase *p)
  : ptr_ (p)
{
}

TAO_ServantBase_var::TAO_ServantBase_var (const TAO_ServantBase_var &b)
  : ptr_ (b.ptr_)
{
  if (this->ptr_ != 0)
    {
      ACE_TRY_NEW_ENV
        {
          this->ptr_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
    }
}

TAO_ServantBase_var::~TAO_ServantBase_var (void)
{
  if (this->ptr_ != 0)
    {
      // We shold not allow exceptions to pass through
      ACE_TRY_NEW_ENV
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
        }
      ACE_ENDTRY;
    }
}

TAO_ServantBase_var &
TAO_ServantBase_var::operator= (TAO_ServantBase *p)
{
  if (this->ptr_ == p)
    return *this;

  ACE_TRY_NEW_ENV
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  this->ptr_ = p;

  return *this;
}

TAO_ServantBase_var &
TAO_ServantBase_var::operator= (const TAO_ServantBase_var &b)
{
  if (this->ptr_ != b.ptr_)
    {
      ACE_TRY_NEW_ENV
        {
          if (this->ptr_ != 0)
            {
              this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if ((this->ptr_ = b.ptr_) != 0)
            {
              this->ptr_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCHALL
        {
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
    }

  return *this;
}

TAO_ServantBase *
TAO_ServantBase_var::operator->() const
{
  return this->ptr_;
}

TAO_ServantBase *
TAO_ServantBase_var::in (void) const
{
  return this->ptr_;
}

TAO_ServantBase *&
TAO_ServantBase_var::inout (void)
{
  return this->ptr_;
}

TAO_ServantBase *&
TAO_ServantBase_var::out (void)
{
  ACE_TRY_NEW_ENV
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  this->ptr_ = 0;

  return this->ptr_;
}

TAO_ServantBase *
TAO_ServantBase_var::_retn (void)
{
  TAO_ServantBase *retval = this->ptr_;
  this->ptr_ = 0;
  return retval;
}

void
TAO_Local_ServantBase::_dispatch (TAO_ServerRequest &,
                                  void *
                                  ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::BAD_OPERATION ());
}
