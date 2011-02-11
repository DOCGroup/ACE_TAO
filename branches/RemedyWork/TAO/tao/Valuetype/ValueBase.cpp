// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/Alias_TypeCode_Static.h"
#include "tao/AnyTypeCode/Value_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"

#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/ValueFactory.h"

#include "tao/CDR.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/SystemException.h"

#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/Valuetype/ValueBase.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#define DEFAULT_INDIRECTION_MAP_SIZE 10

#define VERIFY_MAP(CDR, MAPNAME, MAPCLASSNAME)                                               \
  if (strm.get_##MAPNAME ().is_nil ())                                                       \
    {                                                                                        \
      CDR::MAPCLASSNAME##_Handle handle (                                                    \
          new CDR::RC_##MAPCLASSNAME (new CDR::MAPCLASSNAME (DEFAULT_INDIRECTION_MAP_SIZE)));\
      strm.set_##MAPNAME (handle);                                                           \
    }

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

TAO_ChunkInfo::TAO_ChunkInfo (CORBA::Boolean do_chunking,
                              CORBA::Long init_level)
  : chunking_(do_chunking),
    value_nesting_level_(init_level),
    chunk_size_pos_ (0),
    length_to_chunk_octets_pos_ (0),
    chunk_octets_end_pos_ (0)
{
}

CORBA::ValueBase::ValueBase (void)
  : is_truncatable_(0),
    chunking_(0)
{
}

CORBA::ValueBase::ValueBase (const ValueBase& val)
  : is_truncatable_ (val.is_truncatable_),
    chunking_ (val.chunking_)
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

// %! yet much to do ... look for +++ !


  // 1. Is 'this' yet marshalled ? (->1a)
  //    If not then mark 'this' as marshalled. (->2) +++
  //    Or is it null ? (write null_ref and return ok)
  // 1a. Put indirection and return successfull.

  // 2. if (chunking) and we are in a chunk (look in strm),
  //    end the chunk by writing its length at its start.
  //    This is the responsibility of the CDR stream.
  //    But if nothing is writtern in this chunk yet,
  //    we want to overwrite the place of the dummy blocksize-tag
  //    with our <value-tag>.
  //    Increase the nesting level of valuetypes.

  // 3. Build <value-tag>, which states if chunking is used
  //    and if type information ((list of) repository id(s))
  //    is provided. The latter is necessary if the formal_type_id
  //    is unequal the 'true derived' type of this object.

  // 4. Marshal type information.

  // 5. if (chunking) let room for a blocksize-tag. (i.e. write Long)

  // 6. Now marshal state members. (%! Problem when state is empty
  //    and chunked encoding is used.)

  // 7. if (chunking) let strm overwrite the last blocksize tag
  //    with its concrete value.

  // 8. if (chunking) write an end tag, or (optimization) let the CDR
  //    care for collecting all end tags of nested values (e.g. linked
  //    list), so that only one end tag at all must be written.

CORBA::Boolean
CORBA::ValueBase::_tao_marshal (TAO_OutputCDR &strm,
                                const CORBA::ValueBase *this_,
                                ptrdiff_t formal_type_id)
{
  if ( ! _tao_write_special_value (strm, this_))
  {
    return _tao_write_value (strm, this_, formal_type_id);
  }
  else
    return 1;
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

  CORBA::Boolean is_null_object = false;
  CORBA::Boolean is_indirected = false;
  CORBA::Boolean const retval =
    CORBA::ValueBase::_tao_unmarshal_pre (strm,
                                          new_object,
                                          0,
                                          is_null_object,
                                          is_indirected);

  if (!retval)
    {
      return false;
    }

  if (is_null_object || is_indirected)
  {
    return true;
  }

  // In this case, the codebase url and repo id is read,
  // so continue unmarshal values.
  if (new_object && ! new_object->_tao_unmarshal_v (strm))
    {
      return false;
    }

  return retval;
}


CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_pre (TAO_InputCDR &strm,
                                      CORBA::ValueBase *&valuetype,
                                      const char * const repo_id,
                                      CORBA::Boolean& is_null_object,
                                      CORBA::Boolean& is_indirected)
{
  void* pos = strm.rd_ptr();

  // Value factories are reference counted.  When we get a new value
  // factory from the ORB, its reference count is increased.
  CORBA::ValueFactory_var factory;

  // %! yet much to do ... look for +++ !

  // 1. Get the <value-tag> (else it may be <indirection-tag> or <null-ref>).
  //    Is object yet unmarshalled (<indirection-tag> is set) ? (->1a)
  //    Is <null-ref> given ? Set 0 and return ok.
  // 1a. Lookup the real address in memory, which should be aligned +++
  //     to CORBA::ValueBase. Its possible at this point that worse
  //     type mismatch gets by undetected, if the CDR stream fakes.
  //     So the type should be checked ... +++

  // 2. Now at this point it must be a <value-tag> (error else).
  //    if (chunking) check that any last chunk ends with matching
  //    size. If not raise marshal exception.
  //    Increase the nesting level of valuetypes.

  // 3. if (chunking) read and record the blocksize-tag.

  // 4. Unmarshal type information and lookup factory.
  //    If no type information is given in the CDR encoding, as told
  //    from the <value-tag>, then use the repository id parameter
  //    (it _must_ be right).

  CORBA::Long valuetag;
  Repository_Id_List ids;
  ACE_CString codebase_url;

  is_indirected = false;
  is_null_object = false;

  if (! strm.read_long (valuetag))
    {
      return false;
    }

  if (TAO_OBV_GIOP_Flags::is_indirection_tag (valuetag))
    {
      is_indirected = true;

      // value is redirected
      return _tao_unmarshal_value_indirection (strm, valuetype);
    }

  if (TAO_OBV_GIOP_Flags::is_null_ref (valuetag))
    {
      // null reference is unmarshalled.
      valuetype = 0;
      is_null_object = true;
      return true;
    }

  if (TAO_OBV_GIOP_Flags::has_codebase_url (valuetag))
    {
      ACE_CString codebase_url;
      if (! _tao_read_codebase_url (strm, codebase_url))
        {
          return false;
        }
    }

  if (TAO_OBV_GIOP_Flags::has_single_type_info (valuetag))
    {
      ACE_CString id;
      if (! _tao_read_repository_id(strm, id))
        {
          return false;
        }

      ids.push_back (id);
    }
  else if (TAO_OBV_GIOP_Flags::has_list_type_info (valuetag))
    {
      if (! _tao_read_repository_id_list(strm, ids))
        {
          return false;
        }
    }
  else if (TAO_OBV_GIOP_Flags::has_no_type_info (valuetag))
    {
      ids.push_back (repo_id);
    }
  else
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_pre, ")
                      ACE_TEXT ("unknown value tag: %x\n"),
                      valuetag));
        }

      return 0;
    }

  TAO_ORB_Core *orb_core = strm.orb_core ();

  if (orb_core == 0)
    {
      orb_core = TAO_ORB_Core_instance ();

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_pre, ")
                      ACE_TEXT ("WARNING: extracting valuetype using ")
                      ACE_TEXT ("default ORB_Core\n")));
        }
    }

  CORBA::Boolean require_truncation = false;
  CORBA::Boolean const chunking =
    TAO_OBV_GIOP_Flags::is_chunked (valuetag);
  CORBA::ULong const num_ids = ids.size ();

  // Find the registered factory for this unmarshalling valuetype. If any
  // factory for the valuetype in its truncatable derivation hierarchy
  // is registered, the factory is used to create value for unmarshalling.
  for (CORBA::ULong i = 0; i < num_ids; ++i)
    {
      factory = orb_core->orb ()->lookup_value_factory (ids[i].c_str ());

      if (factory.in() != 0)
        {
          if (i != 0 && chunking)
            {
              require_truncation = true;
            }
          break;
        }
    }

  if (factory.in() == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_pre, ")
                      ACE_TEXT ("OBV factory is null, id = %C\n"),
                      repo_id));
        }

      throw ::CORBA::MARSHAL (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    }

  valuetype = factory->create_for_unmarshal ();

  if (require_truncation)
    {
      valuetype->truncation_hook ();
    }

  if (valuetype == 0)
    {
      return 0;  // %! except.?
    }

  valuetype->chunking_ = chunking;

  VERIFY_MAP(TAO_InputCDR, value_map, Value_Map)

  if (strm.get_value_map ()->get()->bind (pos, valuetype ) != 0)
      throw CORBA::INTERNAL ();
  else if (TAO_debug_level > 0)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_pre, bound value %X - %X\n"),
      pos, valuetype));
  }
  return true;
}

CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_post (TAO_InputCDR &)
{
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

  return true;
}


CORBA::Boolean
CORBA::ValueBase::_tao_validate_box_type (TAO_InputCDR &strm,
                                          TAO_InputCDR &indirected_strm,
                                          const char * const repo_id_expected,
                                          CORBA::Boolean & null_object,
                                          CORBA::Boolean & is_indirected)
{
  CORBA::Long value_tag;
  null_object = false;
  is_indirected = false;

  if (!strm.read_long (value_tag))
    {
      return false;
    }

  if (TAO_OBV_GIOP_Flags::is_null_ref (value_tag))
    { // ok, null reference unmarshaled
      null_object = true;
      return true;
    }

  if (TAO_OBV_GIOP_Flags::is_indirection_tag (value_tag))
    {
      is_indirected = true;

      // box value is redirected.
      return _tao_unmarshal_value_indirection_pre (strm, indirected_strm);
    }

  if (!TAO_OBV_GIOP_Flags::is_value_tag (value_tag))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - CORBA::ValueBase::_tao_validate_box_type, ")
                  ACE_TEXT ("not value_tag\n")));
      return false;
    }


  if (TAO_OBV_GIOP_Flags::has_codebase_url (value_tag))
    { // Demarshal the codebase url (but we won't be using it).

      ACE_CString codebase_url;
      if (! _tao_read_codebase_url (strm, codebase_url))
        {
          return false;
        }
    }

  if (TAO_OBV_GIOP_Flags::has_no_type_info (value_tag))
    {
      // No type information so assume it is the correct type.
      return true;
    }

  if (TAO_OBV_GIOP_Flags::has_single_type_info (value_tag))
    {
      // Demarshal the repository id and check if it is the expected one.
      ACE_CString id;
      if (! _tao_read_repository_id (strm, id))
        {
          return false;
        }

      if (!ACE_OS::strcmp (id.c_str(), repo_id_expected))
      {  // Repository ids matched as expected
        return true;
      }
    }

  if (TAO_OBV_GIOP_Flags::has_list_type_info (value_tag))
    {
      // Don't know how to handle a repository id list.  It does not
      // make sense for a value box anyway.
      return false;
    }

  return false;
}

CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_value_indirection_pre (TAO_InputCDR &strm,
                                                        TAO_InputCDR &indirected_strm)
{
  CORBA::Long offset = 0;
  if (!strm.read_long (offset) ||  offset >= 0)
    {
      return false;
    }

  size_t const buffer_size = -(offset) + sizeof (CORBA::Long);
  // Cribbed from tc_demarshal_indirection in Typecode_CDR_Extraction.cpp
  indirected_strm = TAO_InputCDR (strm.rd_ptr () + offset - sizeof (CORBA::Long),
    buffer_size,
    strm.byte_order ());

  indirected_strm.set_repo_id_map (strm.get_repo_id_map ());
  indirected_strm.set_codebase_url_map (strm.get_codebase_url_map ());
  indirected_strm.set_value_map (strm.get_value_map ());
  return indirected_strm.good_bit ();
}

CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_value_indirection (TAO_InputCDR &strm,
                                                    CORBA::ValueBase *&value)
{
  if (strm.get_value_map().is_nil ())
    throw CORBA::INTERNAL ();

  CORBA::Long offset = 0;
  if (!strm.read_long (offset) ||  offset >= 0)
  {
    return 0;
  }

  void* pos = strm.rd_ptr () + offset - sizeof (CORBA::Long);

  if (TAO_debug_level > 9)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) ValueBase::_tao_unmarshal_value_indirection, pos %X \n"), pos));
      TAO_InputCDR::Value_Map* map = strm.get_value_map()->get ();
      for (TAO_InputCDR::Value_Map::ITERATOR it = map->begin (); it != map->end (); ++ it)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) ValueBase::_tao_unmarshal_value_indirection, %X - %X \n"), it->ext_id_, it->int_id_));
        }
    }
  void * v = 0;
  if (strm.get_value_map()->get()->find (pos, v) != 0)
    {
      throw CORBA::INTERNAL ();
    }
  else if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_value_indirection, found %X - %X\n"),
        pos,v));
    }

  value = reinterpret_cast<CORBA::ValueBase *>(v);
  return true;
}


CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_repo_id_indirection (TAO_InputCDR &strm,
                                                      ACE_CString& id)
{
  CORBA::Long offset = 0;
  if (!strm.read_long (offset) ||  offset >= 0)
  {
    return false;
  }

  void* pos = strm.rd_ptr () + offset - sizeof (CORBA::Long);
  if (strm.get_repo_id_map()->get()->find (pos, id) != 0)
    {
      throw CORBA::INTERNAL ();
    }
  else if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_repo_id_indirection, found %X - %C\n"),
        pos, id.c_str ()));
    }

  return 1;
}

CORBA::Boolean
CORBA::ValueBase::_tao_unmarshal_codebase_url_indirection (TAO_InputCDR &strm,
                                                           ACE_CString& codebase_url)
{
  CORBA::Long offset = 0;
  if (!strm.read_long (offset) ||  offset >= 0)
  {
    return false;
  }

  void* pos = strm.rd_ptr () + offset - sizeof (CORBA::Long);

  if (strm.get_codebase_url_map()->get()->find (pos, codebase_url) != 0)
    {
      throw CORBA::INTERNAL ();
    }
  else if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_unmarshal_codebase_url_indirection, found %X - %C\n"),
        pos, codebase_url.c_str ()));
    }

  return 1;
}


#if defined (GEN_OSTREAM_OPS)

std::ostream &
CORBA::ValueBase::_tao_stream (std::ostream &strm,
                               const CORBA::ValueBase *value)
{
  return value->_tao_stream_v (strm);
}

std::ostream &
CORBA::ValueBase::_tao_stream_v (std::ostream &strm) const
{
  return strm << "CORBA::ValueBase";
}

#endif /* GEN_OSTREAM_OPS */

// =================== methods for chunking ====================


CORBA::Boolean
CORBA::ValueBase::_tao_write_special_value (TAO_OutputCDR &strm,
                                            const CORBA::ValueBase *value)
{
  // If the 'value' is null then write the null value to the stream.
  if (value == 0)
  {
    return strm.write_long (TAO_OBV_GIOP_Flags::Null_tag);
  }
  else
  {
#ifdef TAO_HAS_VALUETYPE_OUT_INDIRECTION
    // value indirection

    VERIFY_MAP(TAO_OutputCDR, value_map, Value_Map)

    char* pos = 0;
    if (strm.get_value_map ()->get()->find (
      reinterpret_cast<void*>(const_cast <CORBA::ValueBase *> (value)), pos) == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t)ValueBase::_tao_write_special_value found value %X - %X\n"),
            value, pos));
        }

      if (! strm.write_long (TAO_OBV_GIOP_Flags::Indirection_tag))
        {
          return false;
        }

      CORBA::Long const offset = - strm.offset (pos);
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_write_special_value value, indirection %d\n"), offset));
        }

      return strm.write_long (offset);
    }
    else {
      if (strm.align_write_ptr (ACE_CDR::LONG_SIZE) != 0)
        {
          throw CORBA::INTERNAL ();
        }
      if (strm.get_value_map ()->get()->bind (
        reinterpret_cast<void*>(const_cast <CORBA::ValueBase *> (value)),
        strm.current()->wr_ptr() ) != 0)
        {
          throw CORBA::INTERNAL ();
        }
      else if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t)ValueBase::_tao_marshal bound value %X - %X \n"),
            value, strm.current()->wr_ptr()));
        }

      return false;
    }
#endif

    return false;
  }
}


CORBA::Boolean
CORBA::ValueBase::_tao_write_value (TAO_OutputCDR &strm,
                                    const CORBA::ValueBase * value,
                                    ptrdiff_t formal_type_id)
{
  if (! value->_tao_write_value_header (strm, formal_type_id))
    {
      return false;
    }

  if (! value->_tao_marshal_v (strm))
    {
      return false;
    }

  return true;
}


CORBA::Boolean
CORBA::ValueBase::_tao_write_value_header (TAO_OutputCDR &strm,
                                           ptrdiff_t formal_type_id) const
{
#ifdef TAO_HAS_OPTIMIZED_VALUETYPE_MARSHALING
  // this case allows TAO to avoid marshaling the typeID for values
  // where the actual type matches the formal type (ie not a derived
  // type).
  //
  // I would much prefer that there be a way to have a -ORB option to
  // control this behavior, but for now there is no reference to the
  // ORB Core available during marshaling (there is during unmarshaling)
  // and no other way to communicate such configuration values.

  CORBA::Boolean const is_formal_type =
    this->_tao_match_formal_type (formal_type_id);
#else
  // Unfortunately, all versions of tao prior to TAO 1.5.2 did not
  // support unmarshaling of valuetypes that did not explicitly
  // marshal the type id. At least it is benign to always encode the
  // typecode value, even if it can be a little verbose.
  CORBA::Boolean const is_formal_type = 0;
  ACE_UNUSED_ARG (formal_type_id);
#endif /* TAO_HAS_OPTIMIZED_VALUETYPE_MARSHALING */

  // Get the list of repository ids for this valuetype.
  Repository_Id_List repository_ids;
  this->_tao_obv_truncatable_repo_ids (repository_ids);
  CORBA::Long const num_ids =
    static_cast <CORBA::Long> (repository_ids.size ());

  // Build <value-tag>, which states if chunking is used
  // and if type information ((list of) repository id(s))
  // is provided. The latter is necessary if the formal_type_id
  // is unequal the 'true derived' type of this object.
  CORBA::Long valuetag = TAO_OBV_GIOP_Flags::Value_tag_base;

  // Truncatable value type, must use chunking and list all repository
  // ids in its "truncatable" derivation hierarchy.
  if (this->is_truncatable_ || this->chunking_)
    valuetag |= TAO_OBV_GIOP_Flags::Chunking_tag_sigbits;

  if (!is_formal_type || this->is_truncatable_)
    valuetag |= TAO_OBV_GIOP_Flags::Type_info_single;

  if (num_ids > 1)
    {
      valuetag |= TAO_OBV_GIOP_Flags::Type_info_list;
    }

  if (! strm.write_long (valuetag)                    // Write <value-tag>.
      || (num_ids > 1 && !strm.write_long (num_ids))) // Write <num-ids>.
    {
      return false;
    }

#ifndef TAO_HAS_OPTIMIMIZED_VALUETYPE_MARSHALING
  if (this->is_truncatable_
      || !is_formal_type  /* Always evaluates to 1 in the
                             !TAO_HAS_OPTIMIMIZED_VALUETYPE_MARSHALING
                             case */
      || num_ids > 1)
    {
#endif  /* !TAO_HAS_OPTIMIMIZED_VALUETYPE_MARSHALING */
      // Marshal type information.
      for (CORBA::Long i = 0; i < num_ids; ++i )
        {
          if (! _tao_write_repository_id (strm, repository_ids[i]))
            {
              return false;
            }
        }
#ifndef TAO_HAS_OPTIMIMIZED_VALUETYPE_MARSHALING
    }
#endif  /* !TAO_HAS_OPTIMIMIZED_VALUETYPE_MARSHALING */

  return true;
}


CORBA::Boolean
CORBA::ValueBase::_tao_write_repository_id (TAO_OutputCDR &strm,
                                            ACE_CString& id)
{
#ifdef TAO_HAS_VALUETYPE_OUT_INDIRECTION

  VERIFY_MAP(TAO_OutputCDR, repo_id_map, Repo_Id_Map)

  if (strm.get_repo_id_map().is_nil ())
    {
      throw CORBA::INTERNAL ();
    }

  char* pos = 0;
  if (strm.get_repo_id_map ()->get()->find (id, pos) == 0)
  {
    if (! strm.write_long (TAO_OBV_GIOP_Flags::Indirection_tag))
      return 0;

    CORBA::Long offset = - strm.offset (pos);

    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_write_repository_id,  id %C indirection %d\n"),
        id.c_str(), offset));
    }

    if (! strm.write_long (offset))
      return false;
  }
  else
  {
    if (strm.align_write_ptr (ACE_CDR::LONG_SIZE) != 0)
      {
        throw CORBA::INTERNAL ();
      }

    if (strm.get_repo_id_map ()->get ()->bind (id, strm.current()->wr_ptr ()) != 0)
      {
        throw CORBA::INTERNAL ();
      }
    else if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_write_repository_id, bound %C - %X\n"),
          id.c_str (), strm.current()->wr_ptr ()));
      }
    if (! strm.write_string (id.c_str ()))
      return false;
  }
#else
  if (! strm.write_string (id.c_str ()))
    return 0;
#endif

  return 1;
}

// this method is called by the IDL generated _tao_marshal_state() method.
CORBA::Boolean
TAO_ChunkInfo::start_chunk(TAO_OutputCDR &strm)
{
  // If chunking, reserve the space for the chunk size of next chunk
  // and increase the nesting level.
  if (this->chunking_)
    {
      if (! reserve_chunk_size(strm))
        {
          return false;
        }

      ++this->value_nesting_level_;
    }

  return true;
}

// this method is called by the IDL generated _tao_marshal_state() method.
CORBA::Boolean
TAO_ChunkInfo::end_chunk(TAO_OutputCDR &strm)
{
  if (this->chunking_)
    {
      // Write actual chunk size at the reserved chunk size place.
      if (! this->write_previous_chunk_size(strm))
        {
          return false;
        }

      // Write an end tag which is negation of value_nesting_level_.
      if (! strm.write_long(- this->value_nesting_level_))
        {
          return false;
        }

      //      -- this->value_nesting_level_;
      if ( -- this->value_nesting_level_ == 0 )
        {
          // ending chunk for outermost value
          this->chunking_ = 0;
        }
    }
  return true;
}


CORBA::Boolean
TAO_ChunkInfo::write_previous_chunk_size(TAO_OutputCDR &strm)
{
  if (this->chunk_size_pos_ != 0)
    {
      // Calculate the chunk size.
      CORBA::Long const chunk_size = strm.total_length () - this->length_to_chunk_octets_pos_;

      // This should not happen since this is called in end_chunk() and
      // the idl generated code always have the matched start_chunk() and
      // end_chunk() pair. There is always data written to the stream between
      // the start_chunk() and end_chunk() calls.
      if (chunk_size == 0)
        {
          return false;
        }

      // Write the actual chunk size to the reserved chunk size position
      // in the stream.
      if (!strm.replace (chunk_size, this->chunk_size_pos_))
        {
          return false;
        }

      // We finish writing the actual chunk size, now we need reset the state.
      this->chunk_size_pos_ = 0;
      this->length_to_chunk_octets_pos_ = 0;
    }

  return true;
}


CORBA::Boolean
TAO_ChunkInfo::reserve_chunk_size(TAO_OutputCDR &strm)
{
  // This is called in the start_chunk().
  // Reserve the chunk size the first time the start_chunk () is called
  // if there are several start_chunk () called continuously without
  // calling end_chunk (). This could happen in the _tao_marshal_state()
  // in the most derived valuetype.

  if (this->chunk_size_pos_ == 0)
    {
      // Align the wr_ptr before we reserve the space for chunk size.
      strm.align_write_ptr (ACE_CDR::LONG_SIZE);
      // Remember begin of the chunk (at chunk size position) that is needed
      // when we write back actual chunk size to the stream.
      this->chunk_size_pos_ = strm.current ()->wr_ptr ();

      // Insert four bytes here as a place-holder, we need to go back
      // later and write the actual size.
      if (! strm.write_long (0))
        {
          return 0;
        }

      // Remember length before writing chunk data. This is used to calculate
      // the actual size of the chunk.
      this->length_to_chunk_octets_pos_ = strm.total_length ();
    }

  return 1;
}

CORBA::Boolean
TAO_ChunkInfo::handle_chunking (TAO_InputCDR &strm)
{
  if (!this->chunking_)
    {
      return 1;
    }

  char* the_rd_ptr = strm.start()->rd_ptr ();

  //This case could happen if a handle_chunking() reads a chunk size
  //and then calls the handle_chunking() again without reading the chunk data.
  //The handle_chunking() called continuously without reading the chunk data
  //only happens at the beginning of _tao_unmarshal_state() in a valuetype
  //that has parents.
  if (the_rd_ptr < this->chunk_octets_end_pos_)
    {
      ++this->value_nesting_level_;
      return 1;
    }

  //Safty check if reading is out of range of current chunk.
  if (this->chunk_octets_end_pos_ != 0
      && the_rd_ptr > this->chunk_octets_end_pos_)
    {
      return 0;
    }

  // Read a long value that might be an endtag, the chunk size or the value tag
  // of the nested valuetype.
  CORBA::Long tag;

  if (!strm.read_long (tag))
    {
      return 0;
    }

  if (tag < 0)
    {
      // tag is an end tag
      if (-tag > this->value_nesting_level_)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) - TAO_ChunkInfo::handle_chunking, received end tag ")
                             ACE_TEXT ("%d > value_nesting_level %d\n"),
                             -tag,
                             this->value_nesting_level_),
                            0);
        }

      this->value_nesting_level_ = - tag;
      --this->value_nesting_level_;

      this->chunk_octets_end_pos_ = 0;

      // Continue reading so that we can read the outmost endtag. This
      // would simplify the implementation in the derived valuetype.
      if (this->value_nesting_level_ > 0)
        {
          this->handle_chunking(strm);
        }
    }
  else if (tag < TAO_OBV_GIOP_Flags::Value_tag_base)
    {
      // Read the chunk size of another chunk.
      this->chunk_octets_end_pos_ = strm.rd_ptr () + tag;
      ++this->value_nesting_level_;
    }
  else // (tag >= 0x7fffff00)
    {
      // This should not happen since the valuetag of the nested
      // values are always unmarshalled in the
      // ValueBase::_tao_unmarshal_pre().
      return 0;
    }

  return 1;
}


CORBA::Boolean
TAO_ChunkInfo::skip_chunks (TAO_InputCDR &strm)
{
  if (!this->chunking_)
    {
      return 1;
    }

  // This function is called after reading data of the truncated parent and
  // skips the remaining chunks until the outmost endtag (-1).
  // The tag read here is suppoused to be an endtag.
  CORBA::Long tag;
  if (!strm.read_long(tag))
    {
      return 0;
    }

  // end of the whole valuetype.
  if (tag == -1)
    {
      return 1;
    }
  else if (tag < 0)
    {
      // continue skip the chunk.
      return this->skip_chunks (strm);
    }
  else if (tag < TAO_OBV_GIOP_Flags::Value_tag_base)
    {
      // Read the chunk size and move forward to skip the data.
      ACE_Message_Block* current =
        const_cast<ACE_Message_Block*>(strm.start ());
      current->rd_ptr (tag);
      return this->skip_chunks (strm);
    }
  else
    return 0;
}

CORBA::Boolean
CORBA::ValueBase::_tao_read_repository_id_list (TAO_InputCDR& strm,
                                                Repository_Id_List& ids)
{
  CORBA::Long num_ids = 0;

  if (!strm.read_long (num_ids))
    {
      return 0;
    }

  if (num_ids == TAO_OBV_GIOP_Flags::Indirection_tag)
    {
      // Multiple repo id is not indirected.
      return 0;
    }
  else
    {
      for (CORBA::Long i = 0; i < num_ids; ++i)
        {
          ACE_CString id;
          if (!_tao_read_repository_id (strm, id))
            {
              return 0;
            }
          ids.push_back (id);
        }
    }

  return 1;
}

CORBA::Boolean
CORBA::ValueBase::_tao_read_repository_id (TAO_InputCDR& strm,
                                           ACE_CString& id)
{
  CORBA::ULong length = 0;

  size_t buffer_size = strm.length();

  if (!strm.read_ulong (length))
    {
      return 0;
    }

  VERIFY_MAP(TAO_InputCDR, repo_id_map, Repo_Id_Map)

  if (strm.get_repo_id_map ().is_nil ())
    throw CORBA::INTERNAL ();

  char * pos = strm.rd_ptr();

  // 'length' may not be the repo id length - it could be the
  // FFFFFFF indirection marker instead. If it is an indirection marker, we
  // get the offset following the indirection marker, otherwise we can follow
  // the same logic using the offset to simply rewind to the start of length
  // and re-read the length as part of the string
  if (TAO_OBV_GIOP_Flags::is_indirection_tag (length))
    {
      return _tao_unmarshal_repo_id_indirection (strm, id);
    }

  pos -= sizeof (CORBA::ULong);

  // Cribbed from tc_demarshal_indirection in Typecode_CDR_Extraction.cpp
  TAO_InputCDR id_stream (pos,
                          buffer_size,
                          strm.byte_order ());

  if (!id_stream.good_bit ())
    {
      return 0;
    }

  if (! id_stream.read_string (id))
    return 0;

  // It's possible the id is read again from an indirection stream,
  // so make sure the id is the same.
  ACE_CString mapped_id;
  if (strm.get_repo_id_map ()->get()->find (pos, mapped_id) == 0)
  {
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_read_repository_id, found %X - %C\n"),
        pos, mapped_id.c_str ()));
    }

    if (ACE_OS::strcmp (mapped_id.c_str (), id.c_str ()) != 0)
      throw CORBA::INTERNAL ();
  }
  else if (strm.get_repo_id_map ()->get ()->bind (pos, id) != 0)
  {
    throw CORBA::INTERNAL ();
  }
  else if (TAO_debug_level > 0)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_read_repository_id, bound %X - %C\n"),
      pos, id.c_str ()));
  }

  // Since the ID is always read from the indirection cdr we have to skip
  // the main CDR forward if we were in fact reading from the current
  // location and not rewinding back some offset.

  strm.skip_bytes (length);

  return 1;
}


CORBA::Boolean
CORBA::ValueBase::_tao_read_codebase_url (TAO_InputCDR& strm,
                                          ACE_CString& codebase_url)
{
  CORBA::ULong length = 0;

  size_t buffer_size = strm.length();

  if (!strm.read_ulong (length))
    {
      return 0;
    }

  VERIFY_MAP(TAO_InputCDR, codebase_url_map, Codebase_URL_Map)

  char * pos = strm.rd_ptr();

  // 'length' may not be the codebase url length - it could be the
  // FFFFFFF indirection marker instead. If it is an indirection marker, we
  // get the offset following the indirection marker, otherwise we can follow
  // the same logic using the offset to simply rewind to the start of length
  // and re-read the length as part of the string
  if (TAO_OBV_GIOP_Flags::is_indirection_tag (length))
    {
      return _tao_unmarshal_codebase_url_indirection (strm, codebase_url);
    }

  pos -= sizeof (CORBA::ULong);

  // Cribbed from tc_demarshal_indirection in Typecode_CDR_Extraction.cpp
  TAO_InputCDR url_stream (pos,
                          buffer_size,
                          strm.byte_order ());

  if (!url_stream.good_bit ())
    {
      return 0;
    }

  if (! url_stream.read_string (codebase_url))
    return 0;

  // It's possible the codebase url is read again from an indirection stream,
  // so make sure the codebase url is the same.
  ACE_CString mapped_url;
  if (strm.get_codebase_url_map ()->get()->find (pos, mapped_url) == 0)
  {
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_read_codebase_url, found %X - %C\n"),
        pos, mapped_url.c_str ()));
    }
    if (ACE_OS::strcmp (mapped_url.c_str (), codebase_url.c_str ()) != 0)
      throw CORBA::INTERNAL ();
  }
  else if (strm.get_codebase_url_map ()->get()->bind (pos, codebase_url) != 0)
  {
    throw CORBA::INTERNAL ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO (%P|%t) - ValueBase::_tao_read_codebase_url, bound %X - %C\n"),
        pos, codebase_url.c_str ()));
    }
  }

  // Since the codebase url is always read from the indirection cdr we have to skip
  // the main CDR forward if we were in fact reading from the current
  // location and not rewinding back some offset.

  strm.skip_bytes (length);

  return 1;
}


void
CORBA::ValueBase::truncation_hook ()
{
  throw ::CORBA::INTERNAL ();
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
  }
}

namespace CORBA
{
  TypeCode_ptr const _tc_ValueBase = &TAO::TypeCode::tc_ValueBase;
  TypeCode_ptr const _tc_EventBase = &TAO::TypeCode::tc_EventBase;
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
  CORBA::ULong const new_count = --this->refcount_;

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
             _tao_valuetype,
             reinterpret_cast<ptrdiff_t> (&CORBA::ValueBase::_downcast)
           );
}

CORBA::Boolean
operator>> (TAO_InputCDR &strm,
            CORBA::ValueBase *&_tao_valuetype)
{
  return CORBA::ValueBase::_tao_unmarshal (strm, _tao_valuetype);
}

#if defined (GEN_OSTREAM_OPS)

std::ostream&
operator<< (std::ostream &strm,
            CORBA::ValueBase *_tao_valuetype)
{
  return CORBA::ValueBase::_tao_stream (strm, _tao_valuetype);
}

#endif /* GEN_OSTREAM_OPS */

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

TAO_END_VERSIONED_NAMESPACE_DECL
