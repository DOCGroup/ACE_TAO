/**
 * @file Table_Adapter.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */

#include "Table_Adapter.h"
#include "IOR_Table_Impl.h"
#include "tao/ORB_Core.h"
#include "tao/Object.h"

ACE_RCSID (IORTable,
           Table_Adapter,
           "$Id$")

TAO_Table_Adapter::TAO_Table_Adapter (TAO_ORB_Core *orb_core)
  :  orb_core_ (orb_core)
  ,  root_ (0)
{
}

TAO_Table_Adapter::~TAO_Table_Adapter (void)
{
  CORBA::release (this->root_);
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
  CORBA::release (this->root_);
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
      return TAO_Adapter::DS_MISMATCHED_KEY;
    }
  ACE_ENDTRY;
  return TAO_Adapter::DS_FORWARD;
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
  return new CORBA::Object (stub);
}

CORBA::Long
TAO_Table_Adapter::initialize_collocated_object (TAO_Stub *,
                                                 CORBA::Object_ptr )
{
  return 0;
}
// ****************************************************************

TAO_Table_Adapter_Factory::TAO_Table_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_Table_Adapter_Factory::create (TAO_ORB_Core *oc)
{
  return new TAO_Table_Adapter (oc);
}

ACE_FACTORY_DEFINE (TAO_IORTable, TAO_Table_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Table_Adapter_Factory,
                       ACE_TEXT ("TAO_IORTable"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Table_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
