/**
 * @file Table_Adapter.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */

#include "tao/IORTable/Table_Adapter.h"
#include "tao/IORTable/IOR_Table_Impl.h"

#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/ORB.h"
#include "tao/Profile.h"
#include "tao/TAO_Server_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Table_Adapter::TAO_Table_Adapter (TAO_ORB_Core &orb_core)
  :  orb_core_ (orb_core),
     root_ (),
     closed_ (true),
     thread_lock_ (),
     lock_ (TAO_Table_Adapter::create_lock (thread_lock_))
{
}

TAO_Table_Adapter::~TAO_Table_Adapter (void)
{
  delete this->lock_;
}

/* static */
ACE_Lock *
TAO_Table_Adapter::create_lock (TAO_SYNCH_MUTEX &thread_lock)
{
  ACE_Lock *the_lock = 0;
  ACE_NEW_RETURN (the_lock,
                  ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (thread_lock),
                  0);
  return the_lock;
}

void
TAO_Table_Adapter::open (void)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  TAO_IOR_Table_Impl *impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO_IOR_Table_Impl (),
                    CORBA::NO_MEMORY ());

  this->root_ = impl;
  this->closed_ = false;
}

void
TAO_Table_Adapter::close (int)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
  this->closed_ = true;
  this->root_ = 0;
}

void
TAO_Table_Adapter::check_close (int)
{
}

int
TAO_Table_Adapter::priority (void) const
{
  return static_cast<int> (TAO_DEFAULT_ADAPTER_REGISTRY_SIZE);
}

int
TAO_Table_Adapter::dispatch (TAO::ObjectKey &key,
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
      return TAO_Adapter::DS_MISMATCHED_KEY;
    rootref = this->root_;
  }

  if (this->find_object (key, forward_to))
    {
      request.forward_location (forward_to);
      return TAO_Adapter::DS_FORWARD;
    }
  else
    return TAO_Adapter::DS_MISMATCHED_KEY;
}

const char *
TAO_Table_Adapter::name (void) const
{
  return "IORTable";
}

CORBA::Object_ptr
TAO_Table_Adapter::root (void)
{
  return CORBA::Object::_duplicate (this->root_.in());
}

CORBA::Object_ptr
TAO_Table_Adapter::create_collocated_object (TAO_Stub *stub,
                                             const TAO_MProfile &)
{
  CORBA::Object_ptr result = CORBA::Object::_nil ();

  if (! this->initialize_collocated_object (stub)) // 0 == success
    {
      // A reference was found in the table. The stub has been forwarded
      // to this. The collocation indicators are now correct on the stub
      // (although they may well now indicate that the stub is not in fact
      // collocated at all).
      ACE_NEW_RETURN (result,
                      CORBA::Object (stub,
                                     stub->is_collocated (),
                                     stub->collocated_servant ()),
                      CORBA::Object::_nil ());

    }

  return result;
}

CORBA::Long
TAO_Table_Adapter::initialize_collocated_object (TAO_Stub *stub)
{
  // Get the effective profile set.
  const TAO_MProfile &mp = stub->forward_profiles () ? *(stub->forward_profiles ())
                                                     : stub->base_profiles ();
  TAO_PHandle j = 0;
  // We only look at the key from the 0th profile but we only really care about
  // corbaloc's here where all profiles share a single object key
  TAO::ObjectKey_var key = mp.get_profile (j)->_key ();

  CORBA::Object_var forward_to = CORBA::Object::_nil ();
  CORBA::Boolean found = false;

  try
    {
      found = this->find_object (key, forward_to.out ());
    }
  catch (const ::CORBA::Exception&)
    {
    }

  if (found)
    {
      // This call will set the appropriate collocation values
      // to correspond to the reference we found in the table.
      stub->add_forward_profiles (forward_to->_stubobj ()->base_profiles ());
      stub->next_profile ();
    }

  // 0 for success
  return ! found;
}

bool
TAO_Table_Adapter::find_object (TAO::ObjectKey &key,
                                CORBA::Object_out forward_to)
{
  CORBA::String_var object_key;
  TAO::ObjectKey::encode_sequence_to_string (object_key.out (), key);
  try
    {
      CORBA::String_var ior = root_->find (object_key.in ());
      forward_to = this->orb_core_.orb ()->string_to_object (ior.in ());
    }
  catch (const ::IORTable::NotFound&)
    {
      return false;
    }
  return true;
}

// ****************************************************************

TAO_Table_Adapter_Factory::TAO_Table_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_Table_Adapter_Factory::create (TAO_ORB_Core *oc)
{
  TAO_Adapter* ptr = 0;
  ACE_NEW_RETURN (ptr,
                 TAO_Table_Adapter (*oc),
                 0);
  return ptr;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_IORTable, TAO_Table_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Table_Adapter_Factory,
                       ACE_TEXT ("TAO_IORTable"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Table_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
