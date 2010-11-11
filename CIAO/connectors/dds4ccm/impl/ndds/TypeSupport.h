/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * NDDS TypeSupport
 */

#ifndef NDDS_TYPESUPPORT_H_
#define NDDS_TYPESUPPORT_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"

#include <map>

namespace CIAO
{
  namespace NDDS
  {
    class DDS_TypeFactory_i
    {
    public:
      virtual DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw, ::DDS::DomainParticipant_ptr dp) = 0;
      virtual DDS::DataReader_ptr create_datareader (DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp) = 0;
    };

    template <typename DDS_TYPE>
    class DDS_TypeFactory_T : public DDS_TypeFactory_i
    {
    public:
      DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw, ::DDS::DomainParticipant_ptr dp)
      {
        typedef CIAO::NDDS::DataWriter_T<DDS_TYPE> DataWriter_type;
        return new DataWriter_type (dw, dp);
      }
      DDS::DataReader_ptr create_datareader (DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp)
      {
        typedef CIAO::NDDS::DataReader_T<DDS_TYPE> DataReader_type;
        return new DataReader_type (dr, dp);
      }
    };

    class DDS4CCM_NDDS_Export DDS_TypeSupport_i
    {
    public:
      static void close (void);
      static void register_type (const char* type, DDS_TypeFactory_i* factory, ::DDS::DomainParticipant_ptr dp);
      static ::DDS::DataWriter_ptr create_datawriter (DDSDataWriter* dw, ::DDS::DomainParticipant_ptr dp);
      static ::DDS::DataReader_ptr create_datareader (DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp);
    private:
      DDS_TypeSupport_i (void);
      ~DDS_TypeSupport_i (void);
      typedef std::map <ACE_CString, DDS_TypeFactory_i*> typefactories;
      static typefactories type_factories;
    };
  }
}

#endif
