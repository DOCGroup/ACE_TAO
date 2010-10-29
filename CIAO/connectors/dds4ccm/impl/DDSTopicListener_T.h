/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSTOPICLISTENER_T_H_
#define DDSTOPICLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "dds4ccm_dds_impl_export.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_TopicListener_T :
      public virtual ::DDSTopicListener
    {
    typedef DDS_Topic_T<DDS_TYPE, VENDOR_TYPE> Topic_type;
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

#include "dds4ccm/impl/DDSTopicListener_T.cpp"

#endif /* TOPICLISTENER_T_H_ */
