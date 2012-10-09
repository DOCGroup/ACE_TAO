/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CONTENFILTEREDTOPIC_T_H_
#define CONTENFILTEREDTOPIC_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "tao/LocalObject.h"

class DDSContentFilteredTopic;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_ContentFilteredTopic_i :
      public virtual ::DDS::ContentFilteredTopic,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      explicit DDS_ContentFilteredTopic_i (::DDSContentFilteredTopic * cft,
                                         ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_ContentFilteredTopic_i (void);

      virtual char * get_filter_expression (void);

      virtual ::DDS::ReturnCode_t get_expression_parameters (
          ::DDS::StringSeq & expression_parameters);

      virtual ::DDS::ReturnCode_t set_expression_parameters (
          const ::DDS::StringSeq & expression_parameters);

      virtual ::DDS::Topic_ptr get_related_topic (void);

      virtual char * get_type_name (void);

      virtual char * get_name (void);

      virtual ::DDS::DomainParticipant_ptr get_participant (void);

      ::DDSContentFilteredTopic * get_rti_entity (void);

      void set_rti_entity (::DDSContentFilteredTopic * cft);

    private:
      ::DDSContentFilteredTopic * rti_entity_;
      ::DDS::DomainParticipant_var dp_;

      DDSContentFilteredTopic * rti_entity (void);
    };
  }
}

#endif /* CONTENFILTEREDTOPIC_T_H_ */
