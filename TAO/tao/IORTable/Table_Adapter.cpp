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
#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/ORB.h"
#include "tao/Profile.h"

ACE_RCSID (IORTable,
           Table_Adapter,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Table_Adapter::TAO_Table_Adapter (TAO_ORB_Core *orb_core)
  :  orb_core_ (orb_core)
  ,  root_ (0)
{
}

TAO_Table_Adapter::~TAO_Table_Adapter (void)
{
  ::CORBA::release (this->root_);
}

void
TAO_Table_Adapter::open (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_THROW_EX (this->root_,
                    TAO_IOR_Table_Impl (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
}

void
TAO_Table_Adapter::close (int  ACE_ENV_ARG_DECL_NOT_USED)
{
  ::CORBA::release (this->root_);
  this->root_ = 0;
}

void
TAO_Table_Adapter::check_close (int  ACE_ENV_ARG_DECL_NOT_USED)
{
}

int
TAO_Table_Adapter::priority (void) const
{
  return 16; // @@
}

int
TAO_Table_Adapter::dispatch (TAO::ObjectKey &key,
                             TAO_ServerRequest &,
                             CORBA::Object_out forward_to
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->find_object (key, forward_to) ? TAO_Adapter::DS_FORWARD
                                             : TAO_Adapter::DS_MISMATCHED_KEY;
}

const char *
TAO_Table_Adapter::name (void) const
{
  return "IORTable";
}

CORBA::Object_ptr
TAO_Table_Adapter::root (void)
{
  return CORBA::Object::_duplicate (this->root_);
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
TAO_Table_Adapter::initialize_collocated_object (TAO_Stub * stub)
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

  ACE_TRY_NEW_ENV
    {
      found = this->find_object (key, forward_to.out ());
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

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

CORBA::Long
TAO_Table_Adapter::find_object (TAO::ObjectKey &key,
                                CORBA::Object_out forward_to
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var object_key;
  TAO::ObjectKey::encode_sequence_to_string (object_key.out (),
                                            key);
  ACE_TRY
    {
      CORBA::String_var ior =
        this->root_->find (object_key.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      forward_to =
        this->orb_core_->orb ()->string_to_object (ior.in ()
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (IORTable::NotFound, nf_ex)
    {
      return 0;
    }
  ACE_ENDTRY;
  return 1;
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
                 TAO_Table_Adapter (oc),
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
