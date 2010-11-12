/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef TOPICLISTENER_T_H_
#define TOPICLISTENER_T_H_

#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export TopicListener :
      public ::DDS::TopicListener
    {
    public:
      /// Constructor
      TopicListener (::CCM_DDS::ConnectorStatusListener_ptr error_listener,
                       ::ACE_Reactor* reactor);

      /// Destructor
      virtual ~TopicListener (void);

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

#endif /* TOPICLISTENER_T_H_ */
