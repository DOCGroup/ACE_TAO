// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ValueBase.cpp
//
// = AUTHOR
//     Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================


#include "tao/CDR.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/ValueBase.h"

#ifdef TAO_HAS_VALUETYPE

#if !defined (__ACE_INLINE__)
# include "tao/ValueBase.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, ValueBase, "$Id$")

// destructor
CORBA_ValueBase::~CORBA_ValueBase (void)
{
}

CORBA_ValueBase*
CORBA_ValueBase::_downcast (CORBA_ValueBase* vt)
{
  return vt;  // every vt is a CORBA::ValueBase :-)
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
CORBA_ValueBase::_tao_marshal (TAO_OutputCDR &strm,
                               CORBA_ValueBase *this_,
                               ptr_arith_t /* formal_type_id */)
{
  CORBA::Boolean retval = 1;
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

  CORBA::ULong value_tag =   TAO_OBV_GIOP_Flags::Value_tag_base
                           | TAO_OBV_GIOP_Flags::Type_info_single;

  retval = strm.write_ulong (value_tag);
  if (!retval)
    return retval;

  // 4. Marshal type information.

  retval = strm.write_string (this_->_tao_obv_repository_id ());
  if (!retval)
    return retval;

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
CORBA_ValueBase::_tao_unmarshal (TAO_InputCDR &,
                                  CORBA_ValueBase *&)
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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("unimpl. CORBA::ValueBase::_tao_unmarshal\n")));
  return 0; // %!
}


CORBA::Boolean
CORBA_ValueBase::_tao_unmarshal_pre (TAO_InputCDR &strm,
                                     CORBA_ValueFactory_ptr &factory,
                                     CORBA_ValueBase *&valuetype,
                                     const char * const /* repo_id */)
{ // %! dont leak on error case !
  // %! postconditions
  CORBA::Boolean retval = 1;
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
      return 0;
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
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("!CORBA::ValueBase::_tao_unmarshal_pre not value_tag\n")));
      return 0;
    }

  // 3. if (chunking) read and record the blocksize-tag.

  // 4. Unmarshal type information and lookup factory.
  //    If no type information is given in the CDR encoding, as told
  //    from the <value-tag>, then use the repository id parameter
  //    (it _must_ be right).

  CORBA::String_var repo_id_stream;

  // It would be more efficient not to copy the string %!)
  if (strm.read_string (repo_id_stream.inout ()) == 0)
    {
      return 0;
    }

  factory = strm.orb_core ()->orb ()
    ->lookup_value_factory (repo_id_stream.in());
  if (factory == 0) // %! except.!
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("(%N:%l) OBV factory is null !!!\n")));
      return 0;
    }

  return retval;
}

CORBA::Boolean
CORBA_ValueBase::_tao_unmarshal_post (TAO_InputCDR &)
{
  CORBA::Boolean retval = 1;

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


// member functions for CORBA_DefaultValueRefCountBase ============

// destructor
CORBA_DefaultValueRefCountBase::~CORBA_DefaultValueRefCountBase (void)
{
}

void
CORBA_DefaultValueRefCountBase::_add_ref (void)
{
  this->_tao_add_ref ();
}

void
CORBA_DefaultValueRefCountBase::_remove_ref (void)
{
  this->_tao_remove_ref ();
}

CORBA::ULong
CORBA_DefaultValueRefCountBase::_refcount_value (void)
{
  return this->_tao_refcount_value ();
}


// some constants

const CORBA::ULong TAO_OBV_GIOP_Flags::Value_tag_base    = 0x7fffff00L;
const CORBA::ULong TAO_OBV_GIOP_Flags::Value_tag_sigbits = 0xffffff00L;
const CORBA::ULong TAO_OBV_GIOP_Flags::Codebase_url      = 1;
const CORBA::ULong TAO_OBV_GIOP_Flags::Type_info_sigbits = 0x00000006L;
const CORBA::ULong TAO_OBV_GIOP_Flags::Type_info_none    = 0;
const CORBA::ULong TAO_OBV_GIOP_Flags::Type_info_single  = 2;
const CORBA::ULong TAO_OBV_GIOP_Flags::Type_info_list    = 6;


#endif /* TAO_HAS_VALUETYPE */
