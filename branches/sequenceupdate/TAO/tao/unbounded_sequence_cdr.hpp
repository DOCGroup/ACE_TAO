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

/*    template <class stream>
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
    }*/

/*    template <>
    bool insert_unbounded_sequence(TAO_OutputCDR & strm, const TAO::unbounded_value_sequence <CORBA::Short> & source) {
      const ::CORBA::ULong length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      // Encode all elements.
      return strm.write_short_array (source.get_buffer (), source.length ());
    }*/

    template <class stream, class sequence>
    bool insert_unbounded_sequence(stream & strm, const sequence & source) {
      const ::CORBA::ULong length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      for(CORBA::ULong i = 0; i < length; ++i) {
        if (!TAO::Objref_Traits<typename sequence::object_type>::marshal (source[i], strm)) {
          return false;
        }
      }
      return true;
    }
  }
}

#endif /* guard_unbounded_sequence_cdr */
