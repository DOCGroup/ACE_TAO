// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     AbstractBase.cpp
//
// = AUTHOR
//     Jeff Parsons  <parsons@cs.wustl.edu>
//
// ============================================================================

#include "tao/AbstractBase.h"
#include "tao/Any.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/ValueFactory.h"

#if defined (TAO_HAS_VALUETYPE)

#if !defined (__ACE_INLINE__)
# include "tao/AbstractBase.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao, 
           AbstractBase, 
           "$Id$")

int CORBA_AbstractBase::_tao_class_id = 0;

CORBA_AbstractBase::CORBA_AbstractBase (void)
  : is_objref_ (0),
    concrete_stubobj_ (0),
    is_collocated_ (0),
    servant_ (0),
    is_local_ (0),
    refcount_ (1),
    refcount_lock_ (0)
{
  ACE_NEW (this->refcount_lock_, 
           TAO_SYNCH_MUTEX);
}

CORBA_AbstractBase::CORBA_AbstractBase (const CORBA_AbstractBase &rhs)
  : is_objref_ (rhs.is_objref_),
    concrete_stubobj_ (rhs.concrete_stubobj_),
    is_collocated_ (rhs.is_collocated_),
    servant_ (rhs.servant_),
    is_local_ (rhs.is_local_),
    refcount_ (1),
    refcount_lock_ (0)
{
  if (this->concrete_stubobj_ != 0)
    {
      (void) this->concrete_stubobj_->_incr_refcnt ();

      // Only instantiate a lock if the object is unconstrained.
      // Locality-constrained objects have no-op reference counting by
      // default.  Furthermore locality-constrained objects may be
      // instantiated in the critical path.  Instantiating a lock for
      // unconstrained objects alone optimizes instantiation of such
      // locality-constrained objects.
      ACE_NEW (this->refcount_lock_, 
               TAO_SYNCH_MUTEX);
    }
}

CORBA_AbstractBase::CORBA_AbstractBase (TAO_Stub * protocol_proxy,
                                        CORBA::Boolean collocated,
                                        TAO_Abstract_ServantBase * servant)
  : is_objref_ (1),
    concrete_stubobj_ (protocol_proxy),
    is_collocated_ (collocated),
    servant_ (servant),
    is_local_ (protocol_proxy == 0 ? 1 : 0),
    refcount_ (1),
    refcount_lock_ (0)
{
  if (this->concrete_stubobj_ != 0)
    {
      (void) this->concrete_stubobj_->_incr_refcnt ();

      // Only instantiate a lock if the object is unconstrained.
      // Locality-constrained objects have no-op reference counting by
      // default.  Furthermore locality-constrained objects may be
      // instantiated in the critical path.  Instantiating a lock for
      // unconstrained objects alone optimizes instantiation of such
      // locality-constrained objects.
      ACE_NEW (this->refcount_lock_, 
               TAO_SYNCH_MUTEX);
    }
}

void *
CORBA_AbstractBase::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  
  if (type == ACE_reinterpret_cast (
              ptr_arith_t,
              &CORBA_AbstractBase::_tao_class_id)
            )
    {
      retv = ACE_reinterpret_cast (void*, this);
    }
  
  if (retv != 0)
    {
      this->_add_ref ();
    }
  
  return retv;
}

void
CORBA_AbstractBase::_add_ref (void)
{
  if (this->refcount_lock_ != 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, mon, *this->refcount_lock_);

      this->refcount_++;
    }
}

void
CORBA_AbstractBase::_remove_ref (void)
{
  if (this->refcount_lock_ != 0)
    {
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, mon, *this->refcount_lock_);

        this->refcount_--;

        if (this->refcount_ != 0)
          {
            return;
          }
      }

      delete this;
    }
}

CORBA::Object_ptr
CORBA_AbstractBase::_to_object (void)
{
  if (this->concrete_stubobj_ == 0)
    {
      return CORBA::Object::_nil ();
    }

  TAO_ORB_Core *orb_core = this->concrete_stubobj_->orb_core ();
  this->concrete_stubobj_->_incr_refcnt ();

  return orb_core->create_object (this->concrete_stubobj_);
}

CORBA::ValueBase *
CORBA_AbstractBase::_to_value (void)
{
  if (this->is_objref_)
    {
      return 0;
    }

  CORBA::ValueBase *retval = this->_tao_to_value ();

  if (retval == 0)
    {
      return retval;
    }

  retval->_add_ref ();
  return retval;
}

CORBA::Boolean
operator<< (TAO_OutputCDR &strm, const CORBA_AbstractBase_ptr abs)
{
  CORBA::Boolean discriminator = 0;

  if (abs->_is_objref ())
    {
      discriminator = 1;

      if (strm << ACE_OutputCDR::from_boolean (discriminator))
        {
          TAO_Stub *stubobj = abs->_stubobj ();

          if (stubobj == 0)
            {
              return 0;
            }

          // STRING, a type ID hint
          if ((strm << stubobj->type_id.in ()) == 0)
            {
              return 0;
            }

          const TAO_MProfile& mprofile = stubobj->base_profiles ();

          CORBA::ULong profile_count = mprofile.profile_count ();

          if ((strm << profile_count) == 0)
            {
              return 0;
            }

          // @@ The MProfile should be locked during this iteration, is there
          // anyway to achieve that?
          for (CORBA::ULong i = 0; i < profile_count; ++i)
            {
              const TAO_Profile *p = mprofile.get_profile (i);

              if (p->encode (strm) == 0)
                {
                  return 0;
                }
            }

          return (CORBA::Boolean) strm.good_bit ();
        }
    }
  else
    {
      discriminator = 0;

      if (strm << ACE_OutputCDR::from_boolean (discriminator))
        {
          CORBA::Boolean retval = 1;

          CORBA::ULong value_tag = TAO_OBV_GIOP_Flags::Value_tag_base
                                   | TAO_OBV_GIOP_Flags::Type_info_single;

          retval = strm.write_ulong (value_tag);

          if (retval == 0)
            {
              return retval;
            }

          retval = strm << abs->_tao_obv_repository_id ();

          if (retval == 0)
            {
              return retval;
            }

          return abs->_tao_marshal_v (strm);
        }
    }

  return 0;
}

CORBA::Boolean
operator>> (TAO_InputCDR &strm, CORBA_AbstractBase_ptr &abs)
{
  abs = 0;
  CORBA::Boolean discriminator = 0;
  ACE_InputCDR::to_boolean tb (discriminator);
  TAO_ORB_Core *orb_core = 0;

  if (strm >> tb)
    {
      if (discriminator == 0)
        {
          CORBA::ULong value_tag;

          if (!strm.read_ulong (value_tag))
            {
              return 0;
            }

          if (TAO_OBV_GIOP_Flags::is_null_ref (value_tag))
            {
              // Ok, null reference unmarshaled.
              return 1;
            }

          if (!TAO_OBV_GIOP_Flags::is_value_tag (value_tag))
            {
              ACE_DEBUG ((LM_DEBUG, 
                          ACE_TEXT ("operator>> CORBA::AbstractBase ")
                          ACE_TEXT ("not value_tag\n")));
              return 0;
            }

          CORBA::String_var repo_id_stream;

          // It would be more efficient not to copy the string)
          if (strm.read_string (repo_id_stream.inout ()) == 0)
            {
              return 0;
            }

          orb_core = strm.orb_core ();

          if (orb_core == 0)
            {
              orb_core = TAO_ORB_Core_instance ();

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_WARNING,
                              "TAO (%P|%t) WARNING: extracting valuetype using "
                              "default ORB_Core\n"));
                }
            }

          CORBA::ValueFactory_var factory = 
            orb_core->orb ()->lookup_value_factory (repo_id_stream.in ());

          // We should throw an exception, if there were an appropriate one.
          if (factory == 0)
            {
              ACE_DEBUG ((LM_ERROR, 
                          ACE_TEXT ("(%N:%l) OBV factory is null !!!\n")));
              return 0;
            }

          abs = factory->create_for_unmarshal_abstract ();

          return abs->_tao_unmarshal_v (strm);
        }
      else
        {
          CORBA::Object_var generic_objref;

          if (strm >> generic_objref.inout ())
            {
              TAO_Stub *concrete_stubobj = generic_objref->_stubobj ();

              CORBA::Boolean stores_orb =
                ! CORBA::is_nil (concrete_stubobj->servant_orb_var ().ptr ());

              if (stores_orb)
                {
                  orb_core = 
                    concrete_stubobj->servant_orb_var ()->orb_core ();
                }

              CORBA::Boolean collocated =
                orb_core != 0
                && orb_core->optimize_collocation_objects ()
                && generic_objref->_is_collocated ();

              ACE_NEW_RETURN (abs,
                              CORBA_AbstractBase (concrete_stubobj,
                                                  collocated,
                                                  generic_objref->_servant ()),
                              0);
              return 1;
            }
        }
    }

  return 0;
}

CORBA::Boolean
CORBA_AbstractBase::_tao_marshal_v (TAO_OutputCDR &)
{
  return 0;
}

CORBA::Boolean
CORBA_AbstractBase::_tao_unmarshal_v (TAO_InputCDR &)
{
  return 0;
}

CORBA::ValueBase *
CORBA_AbstractBase::_tao_to_value (void)
{
  return 0;
}

#endif /* TAO_HAS_VALUETYPE */

