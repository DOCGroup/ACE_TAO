// "$Id$"

#include "tao/Valuetype/AbstractBase.h"
#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/ValueFactory.h"
#include "tao/AnyTypeCode/Objref_TypeCode_Static.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/debug.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/Valuetype/AbstractBase.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (Valuetype,
           AbstractBase,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ************************************************************
// These are in CORBA namespace

void
CORBA::release (CORBA::AbstractBase_ptr obj)
{
  if (obj)
    {
      obj->_remove_ref ();
    }
}

CORBA::Boolean
CORBA::is_nil (CORBA::AbstractBase_ptr obj)
{
  return (obj == 0);
}

// ************************************************************

CORBA::AbstractBase::AbstractBase (void)
  : is_objref_ (false)
  , concrete_stubobj_ (0)
  , is_collocated_ (false)
  , servant_ (0)
  , is_local_ (false)
  , equivalent_obj_ (0)
{
}

CORBA::AbstractBase::AbstractBase (const CORBA::AbstractBase &rhs)
  : is_objref_ (rhs.is_objref_)
  , concrete_stubobj_ (rhs.concrete_stubobj_)
  , is_collocated_ (rhs.is_collocated_)
  , servant_ (rhs.servant_)
  , is_local_ (rhs.is_local_)
  , equivalent_obj_ (0)
{
  if (rhs.concrete_stubobj_ != 0)
    {
      rhs.concrete_stubobj_->_incr_refcnt ();
    }

  if (!CORBA::is_nil (rhs.equivalent_obj_))
    {
      this->equivalent_obj_ =
        CORBA::Object::_duplicate (rhs.equivalent_obj_);
    }
}

CORBA::AbstractBase::AbstractBase (TAO_Stub * protocol_proxy,
                                   CORBA::Boolean collocated,
                                   TAO_Abstract_ServantBase * servant)
  : is_objref_ (true)
  , concrete_stubobj_ (protocol_proxy)
  , is_collocated_ (collocated)
  , servant_ (servant)
  , is_local_ (protocol_proxy == 0 ? true : false)
  , equivalent_obj_ (0)
{
  if (this->concrete_stubobj_ != 0)
    {
      TAO_Stub *stub = this->concrete_stubobj_;

      stub->_incr_refcnt ();

      this->equivalent_obj_ =
        stub->orb_core ()->create_object (stub);
    }

}

CORBA::AbstractBase::~AbstractBase (void)
{
  if (this->concrete_stubobj_ != 0)
    {
      this->concrete_stubobj_->_decr_refcnt ();
    }
}

CORBA::AbstractBase_ptr
CORBA::AbstractBase::_duplicate (CORBA::AbstractBase_ptr obj)
{
  if (obj)
    {
      obj->_add_ref ();
    }

  if (!CORBA::is_nil (obj->equivalent_obj_.in ()))
    {
      obj->equivalent_obj_->_add_ref ();
    }

  return obj;
}

// These are non-pure virtual no-ops so we can instantiate the
// class in the CDR extraction operator. The actual management
// of the refcount will always be done in the derived class.
void
CORBA::AbstractBase::_add_ref (void)
{
}

void
CORBA::AbstractBase::_remove_ref (void)
{
}

void
CORBA::AbstractBase::_tao_any_destructor (void *x)
{
  CORBA::AbstractBase_ptr tmp = static_cast<CORBA::AbstractBase_ptr> (x);
  ::CORBA::release (tmp);
}

CORBA::Object_ptr
CORBA::AbstractBase::_to_object (void)
{
  if (!CORBA::is_nil (this->equivalent_obj_.in ()))
    return CORBA::Object::_duplicate (this->equivalent_obj_.in ());

  if (this->concrete_stubobj_ == 0)
    {
      return CORBA::Object::_nil ();
    }

  TAO_ORB_Core *orb_core = this->concrete_stubobj_->orb_core ();
  this->concrete_stubobj_->_incr_refcnt ();

  return orb_core->create_object (this->concrete_stubobj_);
}

CORBA::ValueBase *
CORBA::AbstractBase::_to_value (void)
{
  if (this->is_objref_)
    {
      return 0;
    }

  CORBA::ValueBase *retval = this->_tao_to_value ();

  if (retval != 0)
    {
      retval->_add_ref ();
    }

  return retval;
}

CORBA::Boolean
CORBA::AbstractBase::_is_a (const char *type_id)
{
  if (! CORBA::is_nil (this->equivalent_obj_.in ()))
    {
      return this->equivalent_obj_->_is_a (type_id);
    }

  return (ACE_OS::strcmp (type_id, "IDL:omg.org/CORBA/AbstractBase:1.0") == 0);
}

const char *
CORBA::AbstractBase::_interface_repository_id (void) const
{
  return "IDL:omg.org/CORBA/AbstractBase:1.0";
}

const char *
CORBA::AbstractBase::_tao_obv_repository_id (void) const
{
  return "IDL:omg.org/CORBA/AbstractBase:1.0";
}

CORBA::Boolean
operator<< (TAO_OutputCDR &strm, const CORBA::AbstractBase_ptr abs)
{
  CORBA::Boolean discriminator = true;

  // We marshal a null abstract interface ptr as a discriminator
  // plus null object reference (see CORBA::Object::marshal()
  // and operator << for CORBA::Object).
  if (CORBA::is_nil (abs))
    {
      // Marshal discriminator, then empty type hint.
      strm << ACE_OutputCDR::from_boolean (discriminator);
      strm.write_ulong (1);
      strm.write_char ('\0');
      strm.write_ulong (0);
      return (CORBA::Boolean) strm.good_bit ();
    }

  if (abs->_is_objref ())
    {
      if (strm << ACE_OutputCDR::from_boolean (discriminator))
        {
          TAO_Stub *stubobj = abs->_stubobj ();

          if (stubobj == 0)
            {
              return false;
            }

          // STRING, a type ID hint
          if ((strm << stubobj->type_id.in ()) == 0)
            {
              return false;
            }

          const TAO_MProfile& mprofile = stubobj->base_profiles ();

          CORBA::ULong const profile_count = mprofile.profile_count ();

          if ((strm << profile_count) == 0)
            {
              return false;
            }

          // @@ The MProfile should be locked during this iteration, is there
          // anyway to achieve that?
          for (CORBA::ULong i = 0; i < profile_count; ++i)
            {
              const TAO_Profile *p = mprofile.get_profile (i);

              if (p->encode (strm) == 0)
                {
                  return false;
                }
            }

          return (CORBA::Boolean) strm.good_bit ();
        }
    }
  else
    {
      discriminator = false;

      if (strm << ACE_OutputCDR::from_boolean (discriminator))
        {
          CORBA::Boolean retval = true;

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

  return false;
}

CORBA::Boolean
operator>> (TAO_InputCDR &strm, CORBA::AbstractBase_ptr &abs)
{
  abs = 0;
  CORBA::Boolean discriminator = false;
  ACE_InputCDR::to_boolean tb (discriminator);
  TAO_ORB_Core *orb_core = 0;

  if (strm >> tb)
    {
      if (!discriminator)
        {
          CORBA::ULong value_tag;

          if (!strm.read_ulong (value_tag))
            {
              return false;
            }

          if (TAO_OBV_GIOP_Flags::is_null_ref (value_tag))
            {
              // Ok, null reference unmarshaled.
              return true;
            }

          if (!TAO_OBV_GIOP_Flags::is_value_tag (value_tag))
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("operator>> CORBA::AbstractBase ")
                          ACE_TEXT ("not value_tag\n")));
              return false;
            }

          CORBA::String_var repo_id_stream;

          // It would be more efficient not to copy the string)
          if (strm.read_string (repo_id_stream.inout ()) == 0)
            {
              return false;
            }

          orb_core = strm.orb_core ();

          if (orb_core == 0)
            {
              orb_core = TAO_ORB_Core_instance ();

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_WARNING,
                              "TAO (%P|%t) WARNING: extracting "
                              "valuetype using default ORB_Core\n"));
                }
            }

          CORBA::ValueFactory_var factory =
            orb_core->orb ()->lookup_value_factory (repo_id_stream.in ());

          // We should throw an exception, if there were an appropriate one.
          if (factory.in() == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%N:%l): The following unknown type was received: `%s'."),
                          repo_id_stream.in ()));
              return false;
            }

          abs = factory->create_for_unmarshal_abstract ();

          return (abs == 0 ? false : abs->_tao_unmarshal_v (strm));
        }
      else
        {
          CORBA::Object_var generic_objref;

          if (strm >> generic_objref.inout ())
            {
              TAO_Stub *concrete_stubobj = generic_objref->_stubobj ();

              CORBA::Boolean const stores_orb =
                ! CORBA::is_nil (concrete_stubobj->servant_orb_var ().in ());

              if (stores_orb)
                {
                  orb_core =
                    concrete_stubobj->servant_orb_var ()->orb_core ();
                }

              CORBA::Boolean const collocated =
                orb_core != 0
                && orb_core->optimize_collocation_objects ()
                && generic_objref->_is_collocated ();

              ACE_NEW_RETURN (abs,
                              CORBA::AbstractBase (
                                concrete_stubobj,
                                collocated,
                                generic_objref->_servant ()),
                              false);
              return true;
            }
        }
    }

  return false;
}

#if defined (GEN_OSTREAM_OPS)

std::ostream&
operator<< (std::ostream &strm,
            CORBA::AbstractBase_ptr _tao_objref)
{
  return CORBA::AbstractBase::_tao_stream (strm, _tao_objref);
}

#endif /* GEN_OSTREAM_OPS */

CORBA::Boolean
CORBA::AbstractBase::_tao_marshal_v (TAO_OutputCDR &) const
{
  return false;
}

CORBA::Boolean
CORBA::AbstractBase::_tao_unmarshal_v (TAO_InputCDR &)
{
  return false;
}

CORBA::Boolean
CORBA::AbstractBase::_tao_match_formal_type (ptrdiff_t ) const
{
  return false;
}

#if defined (GEN_OSTREAM_OPS)

std::ostream &
CORBA::AbstractBase::_tao_stream (std::ostream &strm,
                                  const CORBA::AbstractBase_ptr _tao_objref)
{
  return _tao_objref->_tao_stream_v (strm);
}

std::ostream &
CORBA::AbstractBase::_tao_stream_v (std::ostream &strm) const
{
  return strm << "\"IDL:omg.org/CORBA/AbstractBase:1.0\"";
}

#endif /* GEN_OSTREAM_OPS */

CORBA::ValueBase *
CORBA::AbstractBase::_tao_to_value (void)
{
  return 0;
}

CORBA::Object_ptr
CORBA::AbstractBase::equivalent_objref (void)
{
  if (CORBA::is_nil (this->equivalent_obj_.in ()))
    {
      if (this->concrete_stubobj_ != 0)
        {
          TAO_ORB_Core *orb_core =
            this->concrete_stubobj_->orb_core ();

          this->concrete_stubobj_->_incr_refcnt ();

          this->equivalent_obj_ =
            orb_core->create_object (this->concrete_stubobj_);
        }
    }

  return this->equivalent_obj_.in ();
}

// ================== Typecode initializations ==================

namespace TAO
{
  namespace TypeCode
  {
    char const tc_object_id[]   = "IDL:omg.org/CORBA/AbstractBase:1.0";
    char const tc_object_name[] = "AbstractBase";
    Objref<char const *,
           TAO::Null_RefCount_Policy>
      tc_AbstractBase (CORBA::tk_abstract_interface,
                       tc_object_id,
                       tc_object_name);
  }
}

namespace CORBA
{
  TypeCode_ptr const _tc_AbstractBase = &TAO::TypeCode::tc_AbstractBase;
}

TAO_END_VERSIONED_NAMESPACE_DECL
