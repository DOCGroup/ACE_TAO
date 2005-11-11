#ifndef unbounded_sequence_cdr
#define unbounded_sequence_cdr
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
    template <class sequence>
    bool extract_unbounded_sequence(TAO_InputCDR & strm, sequence & target) {
      ::CORBA::ULong new_length;
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

    template <class sequence>
    bool insert_unbounded_sequence(TAO_OutputCDR & strm, const sequence & source) {
      const ::CORBA::ULong length = source.length ();
      if (!(strm << length)) {
        return false;
      }
      for(CORBA::ULong i = 0; i < length; ++i) {
// Check this
        sequence::value_type element = const_cast <sequence::value_type> (source[i]);
        if (!(strm << element)) {
          return false;
        }
      }
      return true;
    }
  }
}

#endif /* unbounded_sequence_cdr */
