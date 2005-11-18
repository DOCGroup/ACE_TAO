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

namespace TAO {
  namespace details {
    template <class stream>
    bool extract_unbounded_sequence(stream & strm, TAO::unbounded_value_sequence <CORBA::Short> & target) {
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

    template <class stream, class sequence>
    bool extract_unbounded_sequence(stream & strm, sequence & target) {
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

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Short> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_short_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Long> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_long_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::ULong> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_ulong_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::UShort> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_ushort_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Octet> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_octet_array (source.get_buffer (), source.length ()); // todo specialization fo octet
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Char> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_char_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::WChar> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_wchar_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Float> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_float_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Double> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_double_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::ULongLong> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_ulonglong_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::LongDouble> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_longdouble_array (source.get_buffer (), source.length ());
    }

    template <typename stream>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_value_sequence <CORBA::Boolean> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_boolean_array (source.get_buffer (), source.length ());
    }

    template <typename stream, typename object_t, typename object_t_var>
    bool insert_unbounded_sequence(stream & strm, const TAO::unbounded_object_reference_sequence<object_t, object_t_var> & source) {
      ::CORBA::ULong const length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      for(CORBA::ULong i = 0; i < length; ++i) {
        if (!TAO::Objref_Traits<typename TAO::unbounded_object_reference_sequence<object_t, object_t_var>::object_type>::marshal (source[i], strm)) {
          return false;
        }
      }
      return true;
    }

    template <typename stream, typename sequence>
    bool insert_unbounded_sequence(stream & strm, const sequence & source) {
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
  }
}

#endif /* guard_unbounded_sequence_cdr */
