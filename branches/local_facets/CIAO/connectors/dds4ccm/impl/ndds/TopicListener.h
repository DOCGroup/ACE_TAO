/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TOPICLISTENER_H
#define CIAO_RTI_TOPICLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_TopicListener_i :
        public virtual ::DDSTopicListener
      {
      public:
        // Constructor
        RTI_TopicListener_i (::DDS::TopicListener_ptr p);

        // Destructor
        virtual ~RTI_TopicListener_i (void);

        virtual void on_inconsistent_topic(::DDSTopic* topic,
          const ::DDS_InconsistentTopicStatus& status);

        ::DDS::TopicListener_ptr get_topiclistener (void);
      private:
        ::DDS::TopicListener_var impl_;
      };
    }
  }
}

#endif /* CIAO_RTI_TOPICLISTENER_H */
