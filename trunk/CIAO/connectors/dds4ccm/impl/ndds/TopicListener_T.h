/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

class ACE_Reactor;

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class TopicListener_T :
      public ::DDS::TopicListener,
      private ACE_Copy_Disabled
    {
    public:
      /// Constructor
      TopicListener_T (::CCM_DDS::ConnectorStatusListener_ptr error_listener,
                       ACE_Reactor* reactor);

      /// Destructor
      virtual ~TopicListener_T (void);

      virtual void on_inconsistent_topic (
        ::DDS::Topic_ptr the_topic,
        const ::DDS::InconsistentTopicStatus & status);

      static ::DDS::StatusMask get_mask (void);

    private:
      CCM_DDS::ConnectorStatusListener_var error_listener_;
      ACE_Reactor* reactor_;
    };
  }
}

#include "dds4ccm/impl/ndds/TopicListener_T.cpp"
