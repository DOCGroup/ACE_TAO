/**
 * @file Async_Table_Adapter.cpp
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 *
 */

#include "tao/IORTable/Async_Table_Adapter.h"
#include "tao/IORTable/Async_IOR_Table_Impl.h"
#include "tao/IORTable/Locate_ResponseHandler.h"

#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/ORB.h"
#include "tao/Profile.h"
#include "tao/TAO_Server_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Async_Table_Adapter::TAO_Async_Table_Adapter (TAO_ORB_Core &orb_core)
  :  TAO_Table_Adapter (orb_core)
{
}

TAO_Async_Table_Adapter::~TAO_Async_Table_Adapter (void)
{
}

void
TAO_Async_Table_Adapter::open (void)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  TAO_Async_IOR_Table_Impl *impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO_Async_IOR_Table_Impl (),
                    CORBA::NO_MEMORY ());

  this->root_ = impl;
  this->closed_ = false;
}

int
TAO_Async_Table_Adapter::dispatch (TAO::ObjectKey &key,
                                   TAO_ServerRequest &request,
                                   CORBA::Object_out forward_to)
{
  TAO_IOR_Table_Impl_var rootref;
  {
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      TAO_Adapter::DS_MISMATCHED_KEY);
    if (this->closed_)
      {
        return TAO_Adapter::DS_MISMATCHED_KEY;
      }
    rootref = this->root_;
  }

  if (this->root_->async_available ())
    {
      // construct a callback handler
      TAO_AMH_Locate_ResponseHandler_ptr rhp;
      ACE_NEW_RETURN (rhp,
                      TAO_AMH_Locate_ResponseHandler (request),
                      DS_MISMATCHED_KEY);
      TAO_AMH_Locate_ResponseHandler_var rh (rhp);
      this->find_object (rh, key);
      return 0;
    }

  if (this->TAO_Table_Adapter::find_object (key, forward_to))
    {
      request.forward_location (forward_to);
      return TAO_Adapter::DS_FORWARD;
    }
  else
    return TAO_Adapter::DS_MISMATCHED_KEY;
}

void
TAO_Async_Table_Adapter::find_object (IORTable::Locate_ResponseHandler rh,
                                      TAO::ObjectKey &key)
{
  CORBA::String_var object_key;
  TAO::ObjectKey::encode_sequence_to_string (object_key.out (), key);
  TAO_Async_IOR_Table_Impl * aroot =
    dynamic_cast<TAO_Async_IOR_Table_Impl *>(this->root_.ptr());
  if (aroot != 0)
    {
      aroot->async_find (rh, object_key.in ());
    }
  else
    {
      rh->raise_excep (IORTable::NotFound ());
    }
}

// ****************************************************************

TAO_Async_Table_Adapter_Factory::TAO_Async_Table_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_Async_Table_Adapter_Factory::create (TAO_ORB_Core *oc)
{
  TAO_Adapter* ptr = 0;
  ACE_NEW_RETURN (ptr,
                 TAO_Async_Table_Adapter (*oc),
                 0);
  return ptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_Async_IORTable, TAO_Async_Table_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Async_Table_Adapter_Factory,
                       ACE_TEXT ("TAO_Async_IORTable"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Async_Table_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
