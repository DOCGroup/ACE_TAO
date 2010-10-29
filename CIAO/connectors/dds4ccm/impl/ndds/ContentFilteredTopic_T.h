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

#include "ndds/ndds_cpp.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    class DDS_ContentFilteredTopic_T :
      public virtual ::DDS::ContentFilteredTopic,
      public virtual ::CORBA::LocalObject,
      private ACE_Copy_Disabled
    {
    typedef DDS_Topic_T<DDS_TYPE> Topic_type;
    typedef DDS_DomainParticipant_T<DDS_TYPE> DomainParticipant_type;
    public:
      /// Constructor
      explicit DDS_ContentFilteredTopic_T (DDSContentFilteredTopic * dw = 0);

      /// Destructor
      virtual ~DDS_ContentFilteredTopic_T (void);

      virtual char * get_filter_expression (void);

      virtual ::DDS::ReturnCode_t get_expression_parameters (
          ::DDS::StringSeq & expression_parameters);

      virtual ::DDS::ReturnCode_t set_expression_parameters (
          const ::DDS::StringSeq & expression_parameters);

      virtual ::DDS::Topic_ptr get_related_topic (void);

      virtual char * get_type_name (void);

      virtual char * get_name (void);

      virtual ::DDS::DomainParticipant_ptr get_participant (void);

      DDSContentFilteredTopic * get_impl (void);

      void set_impl (DDSContentFilteredTopic * dw);

    private:
      DDSContentFilteredTopic * impl_;

      DDSContentFilteredTopic * impl (void);
    };
  }
}

#include "dds4ccm/impl/ndds/ContentFilteredTopic_T.cpp"

#endif /* CONTENFILTEREDTOPIC_T_H_ */
