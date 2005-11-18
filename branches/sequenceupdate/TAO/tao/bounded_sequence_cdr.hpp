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

namespace TAO {
  namespace details {
    template <class stream, class sequence>
    bool extract_bounded_sequence(stream & strm, sequence & target) {
      ::CORBA::ULong new_length;
      if (!(strm >> new_length)) {
        return false;
      }
      if (new_length > strm.length()) {
         return false;
      }
      if (new_length > target.maximum ()) {
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

    template <typename stream, CORBA::ULong MAX>
    bool insert_bounded_sequence(TAO_OutputCDR & strm, const TAO::bounded_value_sequence <CORBA::Short, MAX> & source) {
      const ::CORBA::ULong length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_short_array (source.get_buffer (), source.length ());
    }

    template <typename stream, CORBA::ULong MAX>
    bool insert_bounded_sequence(TAO_OutputCDR & strm, const TAO::bounded_value_sequence <CORBA::Long, MAX> & source) {
      const ::CORBA::ULong length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      return strm.write_long_array (source.get_buffer (), source.length ());
    }

    template <typename stream, typename object_t, typename object_t_var, CORBA::ULong MAX>
    bool insert_bounded_sequence(stream & strm, const TAO::bounded_object_reference_sequence<object_t, object_t_var, MAX> & source) {
      const ::CORBA::ULong length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      for(CORBA::ULong i = 0; i < length; ++i) {
        if (!TAO::Objref_Traits<typename TAO::unbounded_object_reference_sequence<object_t, object_t_var, MAX>::object_type>::marshal (source[i], strm)) {
          return false;
        }
      }
      return true;
    }

    template <typename stream, typename sequence>
    bool insert_bounded_sequence(stream & strm, const sequence & source) {
      const ::CORBA::ULong length = source.length ();
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

#endif /* guard_bounded_sequence_cdr */
