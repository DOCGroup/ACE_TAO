// $Id$

#include "ValueBase.h"
#include "ValueFactory.h"

#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/Alias_TypeCode_Static.h"
#include "tao/AnyTypeCode/Value_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/CDR.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "ValueBase.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (Valuetype,
           ValueBase,
           "$Id$")


// Static operations in namespace CORBA.

void
CORBA::add_ref (CORBA::ValueBase *val)
{
  if (val)
    {
      val->_add_ref ();
    }
}

void
CORBA::remove_ref (CORBA::ValueBase *val)
{
  if (val)
    {
      val->_remove_ref ();
    }
}

// ***********************************************************************

CORBA::ValueBase::ValueBase (void)
{
}

CORBA::ValueBase::ValueBase (const ValueBase&)
{
}

CORBA::ValueBase::~ValueBase (void)
{
}

CORBA::ValueBase*
CORBA::ValueBase::_downcast (CORBA::ValueBase *vt)
{
  // Every vt is a CORBA::ValueBase :-).
  return vt;
}

void
CORBA::ValueBase::_tao_any_destructor (void *x)
{
  CORBA::ValueBase *tmp = static_cast<CORBA::ValueBase *> (x);
  CORBA::remove_ref (tmp);
}

// OBV marshaling in principle:
// _tao_marshal () is called from the CDR operator<< ()
// to marshal a valuetype. To marshal the state
// it calls (virtual) _tao_marshal_v () (IDL generated) on itself
// which 'jumps' to the most derived valuetype class. This function
// further calls (inline) _tao_marshal_state, which is generated from
// IDL too and does the marshaling of state members and base classes
// (before, if any) actually.
// Fragmentation (chunking) needs some cooperation with the CDR stream.
// It needs to keep track of the state we're in:
// (outside chunk, beginning of chunk - no data, inside chunk and
// the nesting level of valuetypes. (The chunks itself are not nested.))

// (see CORBA 2.3 GIOP 15.3.4)

CORBA::Boolean
CORBA::ValueBase::_tao_marshal (TAO_OutputCDR &strm,
                                const CORBA::ValueBase *this_,
                                ptrdiff_t /* formal_type_id */)
{
  CORBA::Boolean retval = true;
  // %! yet much to do ... look for +++ !

  // 1. Is 'this' yet marshalled ? (->1a)
  //    If not then mark 'this' as marshalled. (->2) +++
  //    Or is it null ? (write null_ref and return ok)
  // 1a. Put indirection and return successfull.

  if (this_ == 0)
    {
      retval = strm.write_ulong (0);
      // write TAO_OBV_GIOP_Flags::Null_ref
      return retval;
    }

  // 2. if (chunking) and we are in a chunk (look in strm),
  //    end the chunk by writing its length at its start.
  //    This is the responsibility of the CDR stream. +++
  //    But if nothing is writtern in this chunk yet,
  //    we want to overwrite the place of the dummy blocksize-tag
  //    with our <value-tag>.
  //    Increase the nesting level of valuetypes.

  // 3. Build <value-tag>, which states if chunking is used
  //    and if type information ((list of) repository id(s))
  //    is provided. The latter is necessary if the formal_type_id
  //    is unequal the 'true derived' type of this object. +++

  CORBA::ULong value_tag = TAO_OBV_GIOP_Flags::Value_tag_base
                           | TAO_OBV_GIOP_Flags::Type_info_single;

  retval = strm.write_ulong (value_tag);

  if (! retval)
    {
      return retval;
    }

  // 4. Marshal type information.

  retval = strm.write_string (this_->_tao_obv_repository_id ());

  if (! retval)
    {
      return retval;
    }

  // 5. if (chunking) let room for a blocksize-tag. (i.e. write ULong)

  // 6. Now marshal state members. (%! Problem when state is empty
  //    and chunked encoding is used.)

  retval = this_->_tao_marshal_v (strm);

  // 7. if (chunking) let strm overwrite the last blocksize tag
  //    with its concrete value.                                 +++
  // 8. if (chunking) write an end tag, or (optimization) let the CDR
  //    care for collecting all end tags of nested values (e.g. linked
  //    list), so that only one end tag at all must be written.  +++

  return retval;
}


CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal (TAO_InputCDR &strm,
                                  CORBA::ValueBase *&new_object)
{
  // This is for the special case only that one unmarshals in order
  // to assign the newly created object directly to a ValueBase pointer.
  // Implementation is like a specialized one (in TC.cpp, if T.idl is source).
  // basically do:
  //  ValueBase::_tao_unmarshal_pre ()
  //    (Gets factory or possible a null or an existing object.
  //     Then the job is done. On an existing (backreferenced) object
  //     do a cast and a type check)
  //  new_object = factory->create_for_unmarshal ()
  //     (with apropriate cast)
  //  new_object->_tao_unmarshal_v ()
  //  new_object->_tao_unmarshal_post ()

//  CORBA::ValueBase *base = 0;
  CORBA::ValueFactory_var factory;
  CORBA::Boolean retval =
    CORBA::ValueBase::_tao_unmarshal_pre (strm,
                                          factory.out (),
                                          new_object,
                                          0);

  if (!retval)
    {
      return false;
    }

  if (factory.in () != 0)
    {
      new_object = factory->create_for_unmarshal ();

      if (new_object == 0)
        {
          return 0;  // %! except.?
        }

      retval = new_object->_tao_unmarshal_v (strm);

      if (retval == 0)
        {
          return 0;
        }
    }

  // Now base must be null or point to the unmarshaled object.
  // Align the pointer to the right subobject.
//  new_object = CORBA::ValueBase::_downcast (base);
  return retval;
}


CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_pre (TAO_InputCDR &strm,
                                      CORBA::ValueFactory &factory,
                                      CORBA::ValueBase *&valuetype,
                                      const char * const repo_id)
{ // %! dont leak on error case !
  // %! postconditions
  CORBA::Boolean retval = true;
  factory = 0;
  // %! yet much to do ... look for +++ !

  // 1. Get the <value-tag> (else it may be <indirection-tag> or <null-ref>).
  //    Is object yet unmarshalled (<indirection-tag> is set) ? (->1a)
  //    Is <null-ref> given ? Set 0 and return ok.
  // 1a. Lookup the real address in memory, which should be aligned +++
  //     to CORBA::ValueBase. Its possible at this point that worse
  //     type mismatch gets by undetected, if the CDR stream fakes.
  //     So the type should be checked ... +++

  CORBA::ULong value_tag;

  if (!strm.read_ulong (value_tag))
    {
      return false;
    }

  if (TAO_OBV_GIOP_Flags::is_null_ref (value_tag))
    {
      valuetype = 0;
      return 1;
      // ok, null reference unmarshaled
    }
  // 2. Now at this point it must be a <value-tag> (error else).
  //    if (chunking) check that any last chunk ends with matching
  //    size. If not raise marshal exception.
  //    Increase the nesting level of valuetypes.

  if (!TAO_OBV_GIOP_Flags::is_value_tag (value_tag))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("!CORBA::ValueBase::_tao_unmarshal_pre ")
                  ACE_TEXT ("not value_tag\n")));
      return false;
    }

  // 3. if (chunking) read and record the blocksize-tag.

  // 4. Unmarshal type information and lookup factory.
  //    If no type information is given in the CDR encoding, as told
  //    from the <value-tag>, then use the repository id parameter
  //    (it _must_ be right).

  TAO_ORB_Core *orb_core = strm.orb_core ();

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

  if (TAO_OBV_GIOP_Flags::has_no_type_info (value_tag))
    {
      factory = orb_core->orb ()->lookup_value_factory (repo_id);
    }
  else
    {
      CORBA::String_var repo_id_stream;

      CORBA::ULong length;

      if (!strm.read_ulong (length))
        {
          return 0;
        }

      // 'length' may not be the repo id length - it could be the
      // FFFFFFF indirection marker instead
      if (TAO_OBV_GIOP_Flags::is_indirection_tag (length))
        {
          CORBA::Long offset;

          // Read the negative byte offset
          if (!strm.read_long (offset) || offset >= 0)
            {
              return 0;
            }

          // Cribbed from tc_demarshal_indirection in Typecode_CDR_Extraction.cpp
          TAO_InputCDR indir_stream (strm.rd_ptr () + offset - sizeof (CORBA::Long),
                                    (-offset) + sizeof (CORBA::Long),
                                    strm.byte_order ());

          if (!indir_stream.good_bit ())
            {
              return 0;
            }

          indir_stream.read_string(repo_id_stream.inout ());
        }
      else
        {
          if (length > 0 && length <= strm.length ())
            {
              ACE_NEW_RETURN (repo_id_stream.inout (),
                              ACE_CDR::Char[length],
                              0);
              if (!strm.read_char_array (repo_id_stream.inout (), length))
                {
                  return 0;
                }
            }
          else if (length == 0)
            {
              ACE_NEW_RETURN (repo_id_stream.inout (),
                              ACE_CDR::Char[1],
                              0);
              ACE_OS::strcpy (const_cast <char *&> (repo_id_stream.inout ()), "");
            }
          else
            {
              return 0;
            }
        }

      factory =
        orb_core->orb ()->lookup_value_factory (repo_id_stream.in ());
    }

  if (factory == 0) // %! except.!
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("(%N:%l) OBV factory is null !!!\n")));
      return false;
    }

  return retval;
}

CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_post (TAO_InputCDR &)
{
  CORBA::Boolean retval = true;

  // (... called from T::_tao_unmarshal)
  // 7. if (chunking) check the last blocksize tag for correct value.  +++
  //    And if we're gonna to truncate, skip all the state of the more
  //    derived classes. (But it might need to be accessed again,
  //    if there are embedded objects which are referenced later
  //    in this CDR encoding stream.)

  // 8. if (chunking) there must be some end tag. Let the CDR stream deal
  //    with this (and decrease the nesting level of valuetypes).
  //    Also the CDR stream must check for eventually outstanding end tags
  //    at the end of the stream which have to cause a marshal
  //    exception there.

  return retval;
}


CORBA::Boolean
CORBA::ValueBase::_tao_validate_box_type (TAO_InputCDR &strm,
                                          const char * const repo_id_expected,
                                          CORBA::Boolean & null_object)
{
  CORBA::ULong value_tag;

  // todo: no handling for indirection yet

  if (!strm.read_ulong (value_tag))
    {
      return false;
    }

  if (TAO_OBV_GIOP_Flags::is_null_ref (value_tag))
    { // ok, null reference unmarshaled
      null_object = true;
      return true;
    }
  null_object = false;

  if (!TAO_OBV_GIOP_Flags::is_value_tag (value_tag))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("!CORBA::ValueBase::_tao_validate_box_type ")
                  ACE_TEXT ("not value_tag\n")));
      return false;
    }

  if (TAO_OBV_GIOP_Flags::has_codebase_url (value_tag))
    { // Demarshal the codebase url (but we won't be using it).

      CORBA::String_var codebase_url;

      if (!strm.read_string (codebase_url.inout ()))
        {
          return false;
        }
    }

  if (TAO_OBV_GIOP_Flags::has_no_type_info (value_tag))
    { // No type information so assume it is the correct type.
      return true;
    }

  if (TAO_OBV_GIOP_Flags::has_single_type_info (value_tag))
    { // Demarshal the repository id and check if it is the expected one.

     CORBA::String_var repo_id_stream;

     if (!strm.read_string (repo_id_stream.inout ()))
     {
        return false;
     }

     if (!ACE_OS::strcmp (repo_id_stream.in (), repo_id_expected))
       {  // Repository ids matched as expected
        return true;
       }
     else
       { // Unequal repository ids
         return false;
       }
    }

  if (TAO_OBV_GIOP_Flags::has_list_type_info (value_tag))
    { // Don't know how to handle a repository id list.  It does not
      // make sense for a value box anyway.
      return false;
    }

  return false;
}


// ================== Typecode initializations ==================

namespace TAO
{
  namespace TypeCode
  {
    char const tc_value_base_id[]   = "IDL:omg.org/CORBA/ValueBase:1.0";
    char const tc_value_base_name[] = "ValueBase";
    Value<char const *,
          CORBA::TypeCode_ptr const *,
          Value_Field<char const *, CORBA::TypeCode_ptr const *> const *,
          TAO::Null_RefCount_Policy> tc_ValueBase (CORBA::tk_value,
                                                   tc_value_base_id,
                                                   tc_value_base_name,
                                                   CORBA::VM_NONE,
                                                   &CORBA::_tc_null,
                                                   0,  // Field array
                                                   0); // Field count

    char const tc_event_base_id[]   = "IDL:omg.org/CORBA/EventBase:1.0";
    char const tc_event_base_name[] = "EventBase";
    Value<char const *,
          CORBA::TypeCode_ptr const *,
          Value_Field<char const *, CORBA::TypeCode_ptr const *> const *,
          TAO::Null_RefCount_Policy> tc_EventBase (CORBA::tk_event,
                                                   tc_event_base_id,
                                                   tc_event_base_name,
                                                   CORBA::VM_NONE,
                                                   &CORBA::_tc_null,
                                                   0,  // Field array
                                                   0); // Field count

    Alias<char const *,
          CORBA::TypeCode_ptr const *,
          TAO::Null_RefCount_Policy> tc_ValueModifier (
      CORBA::tk_alias,
     "IDL:omg.org/CORBA/ValueModifier:1.0",
     "ValueModifier",
     &CORBA::_tc_short);
  }
}

namespace CORBA
{
  TypeCode_ptr const _tc_ValueBase     = &TAO::TypeCode::tc_ValueBase;
  TypeCode_ptr const _tc_EventBase     = &TAO::TypeCode::tc_EventBase;
  TypeCode_ptr const _tc_ValueModifier = &TAO::TypeCode::tc_ValueModifier;
}

// member functions for CORBA::DefaultValueRefCountBase ============

// destructor
CORBA::DefaultValueRefCountBase::~DefaultValueRefCountBase (void)
{
}

void
CORBA::DefaultValueRefCountBase::_add_ref (void)
{
  this->_tao_add_ref ();
}

void
CORBA::DefaultValueRefCountBase::_remove_ref (void)
{
  this->_tao_remove_ref ();
}

CORBA::ULong
CORBA::DefaultValueRefCountBase::_refcount_value (void)
{
  return this->_tao_refcount_value ();
}

// ===========================================================

CORBA::DefaultValueRefCountBase::DefaultValueRefCountBase (void)
  : refcount_ (1)
{
}


// Copy constructor
CORBA::DefaultValueRefCountBase::DefaultValueRefCountBase
  (const DefaultValueRefCountBase& rhs)
  : ValueBase (rhs),
    refcount_ (1)

{
}


void
CORBA::DefaultValueRefCountBase::_tao_add_ref (void)
{
  ++this->refcount_;
}

void
CORBA::DefaultValueRefCountBase::_tao_remove_ref (void)
{
  const CORBA::ULong new_count = --this->refcount_;

  if (new_count == 0)
    delete this;
}

CORBA::ULong
CORBA::DefaultValueRefCountBase::_tao_refcount_value (void)
{
  return this->refcount_.value ();
}

// ===========================================================

CORBA::Boolean
operator<< (TAO_OutputCDR &strm,
            const CORBA::ValueBase *_tao_valuetype)
{
  return CORBA::ValueBase::_tao_marshal (
             strm,
             const_cast<CORBA::ValueBase *> (_tao_valuetype),
             (ptrdiff_t) &CORBA::ValueBase::_downcast
           );
}

CORBA::Boolean
operator>> (TAO_InputCDR &strm,
            CORBA::ValueBase *&_tao_valuetype)
{
  return CORBA::ValueBase::_tao_unmarshal (strm,
                                           _tao_valuetype);
}

// =============== Template Specializations =====================
namespace TAO
{
  void
  Value_Traits<CORBA::ValueBase>::add_ref (
      CORBA::ValueBase *p)
  {
    CORBA::add_ref (p);
  }

  void
  Value_Traits<CORBA::ValueBase>::remove_ref (
      CORBA::ValueBase * p)
  {
    CORBA::remove_ref (p);
  }

  void
  Value_Traits<CORBA::ValueBase>::release (
      CORBA::ValueBase * p)
  {
    CORBA::remove_ref (p);
  }
}
