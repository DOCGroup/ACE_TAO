#ifndef TAO_UNBOUNDED_SEQUENCE_CDR_T_H
#define TAO_UNBOUNDED_SEQUENCE_CDR_T_H
/**
 * @file
 *
 * @brief Extract the sequence
 *
 * @author Carlos O'Ryan
 * @author Johnny Willemsen
 */

#include "tao/orbconf.h"
#include "tao/CORBA_String.h"
#include "tao/SystemException.h"
#include "tao/Basic_Types_IDLv4.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {
  template <typename stream>
  bool demarshal_sequence(stream & strm, unbounded_value_sequence <CORBA::Short> & target) {
    using sequence = TAO::unbounded_value_sequence<CORBA::Short>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Long>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::ULong>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::UShort>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Octet>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Octet>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Char>;
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

# if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)
  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::WChar> & target) {
    using sequence = TAO::unbounded_value_sequence<CORBA::WChar>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Float>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Double>;
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
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::LongLong> & target) {
    using sequence = TAO::unbounded_value_sequence<CORBA::LongLong>;
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
    if (!strm.read_longlong_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::ULongLong> & target) {
    using sequence = TAO::unbounded_value_sequence<CORBA::ULongLong>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::LongDouble>;
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
    using sequence = TAO::unbounded_value_sequence<CORBA::Boolean>;
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

  template <typename stream>
  bool demarshal_sequence(stream & strm,
      TAO::unbounded_value_sequence <CORBA::IDLv4::UInt8, CORBA::IDLv4::UInt8_tag> & target) {
    using sequence = TAO::unbounded_value_sequence<CORBA::IDLv4::UInt8, CORBA::IDLv4::UInt8_tag>;
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
    if (!strm.read_uint8_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream>
  bool demarshal_sequence(stream & strm,
      TAO::unbounded_value_sequence <CORBA::IDLv4::Int8, CORBA::IDLv4::Int8_tag> & target) {
    using sequence = TAO::unbounded_value_sequence<CORBA::IDLv4::Int8, CORBA::IDLv4::Int8_tag>;
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
    if (!strm.read_int8_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename value_t>
  bool demarshal_sequence(stream & strm, TAO::unbounded_value_sequence <value_t> & target) {
    using sequence = TAO::unbounded_value_sequence<value_t>;
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
  bool demarshal_sequence(stream & strm, TAO::unbounded_basic_string_sequence <charT> & target) {
    using sequence = TAO::unbounded_basic_string_sequence<charT>;
    using string_var = typename sequence::element_traits::string_var;
    using sequence_allocation_traits = typename sequence::allocation_traits;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
      return false;
    }
    sequence tmp(new_length, new_length,
                 sequence_allocation_traits::allocbuf_noinit(new_length),
                 true);
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      string_var string;
      if (!(strm >> string.inout ())) {
        return false;
      }
      else {
        tmp[i] = string._retn ();
      }
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename charT, CORBA::ULong BD_STR_MAX>
  bool demarshal_sequence(stream & strm, TAO::unbounded_bd_string_sequence <charT, BD_STR_MAX> & target) {
    using sequence = TAO::unbounded_bd_string_sequence<charT, BD_STR_MAX>;
    using string_var = typename sequence::element_traits::string_var;
    using sequence_allocation_traits = typename sequence::allocation_traits;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
      return false;
    }
    sequence tmp(new_length, new_length,
                 sequence_allocation_traits::allocbuf_noinit(new_length),
                 true);
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      string_var string;
      if (!(strm >> string.inout ())) {
        return false;
      }
      else {
        if (string.in () != 0 &&
            ACE_OS::strlen (string.in ()) > tmp.bd_string_maximum ()) {
          throw ::CORBA::BAD_PARAM ();
        }
        tmp[i] = string._retn ();
      }
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename object_t, typename object_t_var>
  bool demarshal_sequence(stream & strm, TAO::unbounded_object_reference_sequence<object_t, object_t_var> & target) {
    using sequence = TAO::unbounded_object_reference_sequence<object_t, object_t_var>;
    using sequence_allocation_traits = typename sequence::allocation_traits;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if (new_length > strm.length()) {
      return false;
    }
    sequence tmp(new_length, new_length,
                 sequence_allocation_traits::allocbuf_noinit(new_length),
                 true);
    typename sequence::value_type * buffer = tmp.get_buffer();
    for(CORBA::ULong i = 0; i < new_length; ++i) {
      if (!(strm >> buffer[i])) {
        return false;
      }
    }
    tmp.swap(target);
    return true;
  }

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

# if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)
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
  bool marshal_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::LongLong> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_longlong_array (source.get_buffer (), length);
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

  template <typename stream>
  bool marshal_sequence(stream & strm,
      const TAO::unbounded_value_sequence <CORBA::IDLv4::UInt8, CORBA::IDLv4::UInt8_tag> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_uint8_array (source.get_buffer (), length);
  }

  template <typename stream>
  bool marshal_sequence(stream & strm,
      const TAO::unbounded_value_sequence <CORBA::IDLv4::Int8, CORBA::IDLv4::Int8_tag> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_int8_array (source.get_buffer (), length);
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
  bool marshal_sequence(stream & strm, const TAO::unbounded_basic_string_sequence <charT> & source) {
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

  template <typename stream, typename charT, CORBA::ULong BD_STR_MAX>
  bool marshal_sequence(stream & strm, const TAO::unbounded_bd_string_sequence <charT, BD_STR_MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      if (source[i].in () != 0 &&
          ACE_OS::strlen (source[i]) > source.bd_string_maximum ()) {
        throw ::CORBA::BAD_PARAM ();
      }
      if (!(strm << source[i])) {
        return false;
      }
    }
    return true;
  }

  template <typename stream, typename object_t, typename object_t_var>
  bool marshal_sequence(stream & strm, const TAO::unbounded_object_reference_sequence<object_t, object_t_var> & source) {
    using objec_t = typename TAO::unbounded_object_reference_sequence<object_t, object_t_var>::object_type;
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
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_UNBOUNDED_SEQUENCE_CDR_T_H */
