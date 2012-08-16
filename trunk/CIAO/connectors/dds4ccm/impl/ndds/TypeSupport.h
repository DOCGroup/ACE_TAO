/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * NDDS TypeSupport
 */

#ifndef NDDS_TYPESUPPORT_H_
#define NDDS_TYPESUPPORT_H_

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"

#include <map>

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_TypeFactory_i
    {
    public:
      virtual ~DDS_TypeFactory_i (void);
      virtual DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw,
                                                     ::DDS::DomainParticipant_ptr dp,
                                                     ::DDS::Publisher_ptr pub) = 0;
      virtual DDS::DataReader_ptr create_datareader (DDSDataReader* dr,
                                                     ::DDS::DomainParticipant_ptr dp,
                                                     ::DDS::Subscriber_ptr sub) = 0;
    };

    template <typename DDS_TYPE, typename SEQ_TYPE>
    class DDS_TypeFactory_T : public DDS_TypeFactory_i
    {
    public:
      DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw,
                                             ::DDS::DomainParticipant_ptr dp,
                                             ::DDS::Publisher_ptr pub)
      {
        typedef CIAO::NDDS::DataWriter_T<
            typename DDS_TYPE::datawriter_type,
            typename DDS_TYPE::typed_writer_type,
            typename DDS_TYPE::value_type>
          DataWriter_type;
        return new DataWriter_type (dw, dp, pub);
      }
      DDS::DataReader_ptr create_datareader (DDSDataReader* dr,
                                             ::DDS::DomainParticipant_ptr dp,
                                             ::DDS::Subscriber_ptr sub)
      {
        typedef CIAO::NDDS::DataReader_T<
            typename DDS_TYPE::datareader_type,
            typename DDS_TYPE::typed_reader_type,
            typename DDS_TYPE::value_type,
            SEQ_TYPE,
            typename DDS_TYPE::dds_seq_type>
          DataReader_type;
        return new DataReader_type (dr, dp, sub);
      }
    };

    /**
     * Helper class to administrate the type factory and how many times
     * it has been registered
     */
    class DDS_TypeFactory_i_var
    {
      public:
        DDS_TypeFactory_i_var (DDS_TypeFactory_i* tf);
        ~DDS_TypeFactory_i_var (void);

        DDS_TypeFactory_i* get_factory ();

        /**
         * Increments the reference count
         */
        long _inc_ref ();
        /**
         * Decrements the reference count
         */
        long _dec_ref ();

      private:
        long ref_count_;
        DDS_TypeFactory_i* tf_;
    };

    class DDS4CCM_NDDS_Export DDS_TypeSupport_i
    {
    public:
      /**
        * Clears all internal maps, freeing the memory.
        */
      static void close (void);

      /**
        * Registering a type-factory combination per DomainParticipant.
        * @retval false Insertions to one of the maps fails or if there's
        * already a factory for the given type registered.
        * @retval true Insertions succeeded
        */
      static bool register_type (::DDS::DomainParticipant_ptr dp,
                                 const char* type, DDS_TypeFactory_i* factory);

      /**
        * Unregistering a type-factory combination for a specific
        * DomainParticipant. At the moment the type factory isn't used anymore
        * it is automatically destructed
        */
      static void unregister_type (::DDS::DomainParticipant_ptr dp,
                                   const char* type);

      /**
        * Create a type specific datawriter
        */
      static ::DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw,
                                                      ::DDS::DomainParticipant_ptr dp,
                                                      ::DDS::Publisher_ptr pub);
      /**
        * Create a type specific datareader
        */
      static ::DDS::DataReader_ptr create_datareader (DDSDataReader* dr,
                                                      ::DDS::DomainParticipant_ptr dp,
                                                      ::DDS::Subscriber_ptr sub);
    private:
      DDS_TypeSupport_i (void);
      ~DDS_TypeSupport_i (void);

      /// Store for a specific DDS type a type factory var with its own refcount
      typedef std::map < ACE_CString, DDS_TypeFactory_i_var* > typefactories;

      /// For each domain participant we store a map with type factories for the
      /// types that participant has
      typedef std::map< ::DDS::DomainParticipant_ptr, typefactories > participantfactories;
      static participantfactories participant_factories;

      /**
        * Searches for a TypeFactory, based on a type and DomainParticipant
        */
      static DDS_TypeFactory_i* get_factory_i (::DDS::DomainParticipant_ptr dp,
                                               const char* type);
    };
  }
}

#endif
