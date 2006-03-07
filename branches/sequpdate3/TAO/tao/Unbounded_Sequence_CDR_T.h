#ifndef guard_unbounded_sequence_cdr
#define guard_unbounded_sequence_cdr
/**
 * @file
 *
 * @brief Extract the sequence
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 * @author Johnny Willemsen
 */

#include "tao/Unbounded_Basic_String_Sequence_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {
  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Short> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Short> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_short_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Long> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Long> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_long_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::ULong> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::ULong> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_ulong_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::UShort> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::UShort> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_ushort_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Octet> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Octet> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    if (ACE_BIT_DISABLED (strm.start ()->flags (), ACE_Message_Block::DONT_DELETE))
    {
      TAO_ORB_Core* orb_core = strm.orb_core ();
      if (orb_core != 0 && strm.orb_core ()->resource_factory ()->
        input_cdr_allocator_type_locked () == 1)
      {
        tmp.replace (new_length, strm.start ());
        tmp.mb ()->wr_ptr (tmp.mb()->rd_ptr () + new_length);
        strm.skip_bytes (new_length);
        tmp.swap(target);
        return true;
      }
    }
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_octet_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }
#else
  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Octet> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Octet> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_octet_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }
#endif

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Char> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Char> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_char_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

# if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::WChar> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::WChar> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_wchar_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }
#endif

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Float> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Float> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_float_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Double> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Double> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_double_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::ULongLong> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::ULongLong> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_ulonglong_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::LongDouble> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::LongDouble> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_longdouble_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Boolean> & target) {
    typedef TAO::unbounded_value_sequence <CORBA::Boolean> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_boolean_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename value_t>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <value_t> & target) {
    typedef TAO::unbounded_value_sequence <value_t> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      if (!(strm >> buffer[i])) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename charT>
  bool demarshal_sequence(stream & strm, TAO::details::unbounded_basic_string_sequence <charT> & target) {
    typedef TAO::details::unbounded_basic_string_sequence <charT> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      if (!(strm >> buffer[i])) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename object_t, typename object_t_var>
  bool demarshal_sequence(stream & strm, TAO::unbounded_object_reference_sequence<object_t, object_t_var> & target) {
    typedef TAO::unbounded_object_reference_sequence<object_t, object_t_var> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
        return false;
    }
    sequence tmp(new_length);
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      if (!(strm >> buffer[i])) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }
}

namespace TAO {
  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Short> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_short_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Long> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_long_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::ULong> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_ulong_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::UShort> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_ushort_array (source.get_buffer (), length);
  }

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Octet> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    if (source.mb ()) {
      return strm.write_octet_array_mb (source.mb ());
    }
    return strm.write_octet_array (source.get_buffer (), length);
  }
#else
  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Octet> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_octet_array (source.get_buffer (), length);
  }
#endif

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Char> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_char_array (source.get_buffer (), length);
  }

# if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)
  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::WChar> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_wchar_array (source.get_buffer (), length);
  }
#endif

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Float> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_float_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Double> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_double_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::ULongLong> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_ulonglong_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::LongDouble> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_longdouble_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Boolean> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_boolean_array (source.get_buffer (), length);
  }

  template <typename stream, typename value_t>
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <value_t> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      if (!(strm << source[i])) {
        return false;
      }
    }
    return true;
  }

  template <typename stream, typename charT>
  bool marshal_sequence(stream & strm, const TAO::details::unbounded_basic_string_sequence <charT> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      if (!(strm << source[i])) {
        return false;
      }
    }
    return true;
  }

  template <typename stream, typename object_t, typename object_t_var>
  bool marshal_sequence(stream & strm, const TAO::unbounded_object_reference_sequence<object_t, object_t_var> & source) {
    typedef typename TAO::unbounded_object_reference_sequence<object_t, object_t_var>::object_type objec_t;
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      if (!TAO::Objref_Traits<objec_t>::marshal (source[i], strm)) {
        return false;
      }
    }
    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* guard_unbounded_sequence_cdr */
