/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_NDDS_CONTENFILTEREDTOPIC_H
#define CIAO_NDDS_CONTENFILTEREDTOPIC_H

#include "DomainParticipant.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/dds4ccm_dds_impl_export.h"
#include "tao/LocalObject.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::ContentFilteredTopic DDSContentFilteredTopic;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DDS_ContentFilteredTopic_i :
      public virtual ::DDS::CCM_ContentFilteredTopic,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_ContentFilteredTopic_i (DDSContentFilteredTopic * dw);

      /// Destructor
      virtual ~CCM_DDS_ContentFilteredTopic_i (void);

      virtual char * get_filter_expression (void);

      virtual
      ::DDS::ReturnCode_t get_expression_parameters (
          ::DDS::StringSeq & expression_parameters);

      virtual
      ::DDS::ReturnCode_t set_expression_parameters (
          const ::DDS::StringSeq & expression_parameters);

      virtual
      ::DDS::Topic_ptr get_related_topic (void);

      virtual
      char * get_type_name (void);

      virtual
      char * get_name (void);

      virtual
      ::DDS::DomainParticipant_ptr get_participant (void);

      DDSContentFilteredTopic * get_impl (void);

      void set_impl (DDSContentFilteredTopic * dw);

    private:
      DDSContentFilteredTopic * impl_;

      DDSContentFilteredTopic * impl (void);
    };
  }
}

#endif /* CIAO_NDDS_CONTENFILTEREDTOPIC_H */
