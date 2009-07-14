/**
 * @file NDDS_Traits.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Traits necessary for various bits and pieces of the DDS4CCM infrastructure.
 */

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      /**
       * @struct Type_Traits
       * @brief Convenience traits struct.
       * 
       * This is a convenience struct that is used by most of the
       * DDS4CCM infrastructure; it is not necessary to use this exact
       * struct, so long as all of the needed fields are filled in.
       */
      template <typename VALUE_TYPE,
                typename SEQ_TYPE,
                typename TYPE_SUPPORT,
                typename DATA_WRITER,
                typename DATA_READER>
      struct Type_Traits
      {
        typedef VALUE_TYPE value_type;
        typedef SEQ_TYPE seq_type;
        typedef TYPE_SUPPORT type_support;
        typedef DATA_WRITER data_writer;
        typedef DATA_READER data_reader;
      };
      
      typedef Type_Traits < const char *,
                            DDS_StringSeq,
                            DDSStringTypeSupport,
                            DDSStringDataWriter,
                            DDSStringDataReader > String_Traits;
      
    }
  }
}
