/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef TOPICLISTENER_T_H_
#define TOPICLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "ace/Copy_Disabled.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class TopicListener_T :
      public ::DDS::TopicListener,
      private ACE_Copy_Disabled
    {
    public:
      /// Constructor
      TopicListener_T (::CCM_DDS::ConnectorStatusListener_ptr error_listener,
                       ::ACE_Reactor* reactor);

      /// Destructor
      virtual ~TopicListener_T (void);

      virtual void on_inconsistent_topic (
        ::DDS::Topic_ptr the_topic,
        const ::DDS::InconsistentTopicStatus & status);

      static ::DDS::StatusMask get_mask (
        CCM_DDS::ConnectorStatusListener_ptr error_listener);

    private:
      CCM_DDS::ConnectorStatusListener_var error_listener_;
      ACE_Reactor* reactor_;
    };
  }
}

#include "dds4ccm/impl/TopicListener_T.cpp"

#endif /* TOPICLISTENER_T_H_ */
