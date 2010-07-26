/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CONTENFILTEREDTOPIC_T_H_
#define CONTENFILTEREDTOPIC_T_H_

#include "DomainParticipant_T.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::ContentFilteredTopic DDSContentFilteredTopic;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_ContentFilteredTopic_T :
      public virtual ::DDS::CCM_ContentFilteredTopic,
      public virtual ::CORBA::LocalObject,
      private ACE_Copy_Disabled
    {
    typedef CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> Topic_type;
    typedef CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> DomainParticipant_type;
    public:
      /// Constructor
      CCM_DDS_ContentFilteredTopic_T (DDSContentFilteredTopic * dw);

      /// Destructor
      virtual ~CCM_DDS_ContentFilteredTopic_T (void);

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

#include "dds4ccm/impl/ContentFilteredTopic_T.cpp"

#endif /* CONTENFILTEREDTOPIC_T_H_ */
