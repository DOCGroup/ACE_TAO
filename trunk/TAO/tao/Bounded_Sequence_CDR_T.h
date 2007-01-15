#ifndef guard_bounded_sequence_cdr
#define guard_bounded_sequence_cdr
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

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {
  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Short, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Short, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_short_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Long, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Long, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_long_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::ULong, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::ULong, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_ulong_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::UShort, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::UShort, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_ushort_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Octet, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Octet, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_octet_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Char, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Char, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_char_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

# if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)
  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::WChar, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::WChar, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_wchar_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }
#endif

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Float, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Float, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_float_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Double, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Double, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_double_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::ULongLong, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::ULongLong, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_ulonglong_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::LongDouble, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::LongDouble, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_longdouble_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <CORBA::Boolean, MAX> & target) {
    typedef TAO::bounded_value_sequence <CORBA::Boolean, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
    typename sequence::value_type * buffer = tmp.get_buffer();
    if (!strm.read_boolean_array (buffer, new_length)) {
      return false;
    }
    tmp.swap(target);
    return true;
  }

  template <typename stream, typename value_t, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_value_sequence <value_t, MAX> & target) {
    typedef TAO::bounded_value_sequence <value_t, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
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

  template <typename stream, typename charT, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_basic_string_sequence <charT, MAX> & target) {
    typedef typename TAO::bounded_basic_string_sequence <charT, MAX> sequence;
    typedef typename sequence::element_traits::string_var string_var;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
    tmp.length(new_length);
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

  template <typename stream, typename object_t, typename object_t_var, CORBA::ULong MAX>
  bool demarshal_sequence(stream & strm, TAO::bounded_object_reference_sequence<object_t, object_t_var, MAX> & target) {
    typedef typename TAO::bounded_object_reference_sequence<object_t, object_t_var, MAX> sequence;
    ::CORBA::ULong new_length = 0;
    if (!(strm >> new_length)) {
      return false;
    }
    if ((new_length > strm.length()) || (new_length > target.maximum ())) {
      return false;
    }
    sequence tmp;
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
  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Short, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_short_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Long, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_long_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::ULong, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_ulong_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::UShort, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_ushort_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Octet, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_octet_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Char, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_char_array (source.get_buffer (), length);
  }

# if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)
  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::WChar, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_wchar_array (source.get_buffer (), length);
  }
#endif

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Float, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_float_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Double, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_double_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::ULongLong, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_ulonglong_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::LongDouble, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_longdouble_array (source.get_buffer (), length);
  }

  template <typename stream, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <CORBA::Boolean, MAX> & source) {
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    return strm.write_boolean_array (source.get_buffer (), length);
  }

  template <typename stream, typename value_t, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_value_sequence <value_t, MAX> & source) {
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

  template <typename stream, typename charT, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_basic_string_sequence <charT, MAX> & source) {
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

  template <typename stream, typename object_t, typename object_t_var, CORBA::ULong MAX>
  bool marshal_sequence(stream & strm, const TAO::bounded_object_reference_sequence<object_t, object_t_var, MAX> & source) {
    typedef typename TAO::bounded_object_reference_sequence<object_t, object_t_var, MAX>::object_type object_type;
    ::CORBA::ULong const length = source.length ();
    if (!(strm << length)) {
      return false;
    }
    for(CORBA::ULong i = 0; i < length; ++i) {
      if (!TAO::Objref_Traits<object_type>::marshal (source[i], strm)) {
        return false;
      }
    }
    return true;
  }
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* guard_bounded_sequence_cdr */
