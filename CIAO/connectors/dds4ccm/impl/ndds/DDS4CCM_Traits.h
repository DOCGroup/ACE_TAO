/**
 * @file DDS4CCM_Traits.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Traits necessary for various bits and pieces of the DDS4CCM infrastructure.
 */

#ifndef DDS4CCM_TRAITS_H
#define DDS4CCM_TRAITS_H

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename BASE_TYPE,
              typename SEQ_TYPE,
              typename WRITER_TYPE,
              typename UPDATER_TYPE,
              typename GETTER_TYPE,
              typename READER_TYPE,
              typename CONTEXT_TYPE,
              typename LISTENER_TYPE,
              typename STATELISTENER_TYPE,
              typename CONNECTORSTATUSLISTENER_TYPE
    >

    struct Connector_Traits
    {
      typedef BASE_TYPE base_type;
      typedef SEQ_TYPE seq_type;
      typedef WRITER_TYPE writer_type;
      typedef UPDATER_TYPE updater_type;
      typedef GETTER_TYPE getter_type;
      typedef READER_TYPE reader_type;
      typedef CONTEXT_TYPE context_type;
      typedef LISTENER_TYPE listener_type;
      typedef STATELISTENER_TYPE statelistener_type;
      typedef CONNECTORSTATUSLISTENER_TYPE connectorstatuslistener_type;
    };

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
                typename DDS_SEQ_TYPE,
                typename TYPE_SUPPORT,
                typename DATA_WRITER,
                typename DATA_READER>
      struct Type_Traits
      {
        typedef VALUE_TYPE value_type;
        typedef DDS_SEQ_TYPE dds_seq_type;
        typedef TYPE_SUPPORT type_support;
        typedef DATA_WRITER data_writer;
        typedef DATA_READER data_reader;
      };
    }
  }
}
#endif /* DDS4CCM_TRAITS_H */
