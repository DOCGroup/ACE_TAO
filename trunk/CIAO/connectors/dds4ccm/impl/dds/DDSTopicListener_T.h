/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TOPICLISTENER_H
#define DDS4CCM_TOPICLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::TopicListener DDSTopicListener;
typedef ::DDS::Topic DDSTopic;
typedef ::DDS::InconsistentTopicStatus DDS_InconsistentTopicStatus;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_TopicListener_T :
      public virtual ::DDSTopicListener
    {
    typedef CCM_DDS_Topic_T<DDS_TYPE, CCM_TYPE> Topic_type;
    public:
      /// Constructor
      CCM_DDS_TopicListener_T (::DDS::TopicListener_ptr p);

      /// Destructor
      virtual ~CCM_DDS_TopicListener_T (void);

      virtual void on_inconsistent_topic(
        ::DDSTopic* topic,
        const ::DDS_InconsistentTopicStatus& status);

      ::DDS::TopicListener_ptr get_topiclistener (void);
    private:
      ::DDS::TopicListener_var impl_;
    };
  }
}

#include "dds4ccm/impl/dds/DDSTopicListener_T.cpp"

#endif /* DDS4CCM_TOPICLISTENER_H */
