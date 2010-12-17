/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSTOPICLISTENER_T_H_
#define DDSTOPICLISTENER_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_TopicListener_i :
      public ::DDSTopicListener
    {
    public:
      /// Constructor
      DDS_TopicListener_i (::DDS::Topic_ptr topic,
                           ::DDS::TopicListener_ptr topic_listener);

      /// Destructor
      virtual ~DDS_TopicListener_i (void);

      virtual void on_inconsistent_topic(
        ::DDSTopic* topic,
        const ::DDS_InconsistentTopicStatus& status);

      void set_dds_topic (::DDS::Topic_ptr topic);

    private:
      ::DDS::Topic_var topic_;
      ::DDS::TopicListener_var topic_listener_;
    };
  }
}

#endif /* TOPICLISTENER_T_H_ */
