/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
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

    class DDS4CCM_NDDS_Export DDS_TypeSupport_i
    {
    public:
      static void close (void);
      static bool register_type (const char* type, DDS_TypeFactory_i* factory,
                                 ::DDS::DomainParticipant_ptr dp);
      static DDS_TypeFactory_i * unregister_type (const char* type,
                                                  ::DDS::DomainParticipant_ptr dp);
      static ::DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw,
                                                      ::DDS::DomainParticipant_ptr dp,
                                                      ::DDS::Publisher_ptr pub);
      static ::DDS::DataReader_ptr create_datareader (DDSDataReader* dr,
                                                      ::DDS::DomainParticipant_ptr dp,
                                                      ::DDS::Subscriber_ptr sub);
    private:
      DDS_TypeSupport_i (void);
      ~DDS_TypeSupport_i (void);

      typedef std::map < ACE_CString, DDS_TypeFactory_i* > typefactories;

      typedef std::map< ::DDS::DomainParticipant_ptr, typefactories > participantfactories;
      static participantfactories participant_factories;

      static DDS_TypeFactory_i* get_factory (const char* type,
                                            ::DDS::DomainParticipant_ptr dp);

      static bool set_factory (const char* type,
                        DDS_TypeFactory_i* f,
                        ::DDS::DomainParticipant_ptr dp);

      static void remove_participant_factory (const char * type,
                                              ::DDS::DomainParticipant_ptr dp);

    };
  }
}

#endif
