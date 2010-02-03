/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS_PUBLISHERLISTENER_T_H_
#define DDS_PUBLISHERLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

class ACE_Reactor;

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class PublisherListener_T :
      public ::DDS::PublisherListener,
      private ACE_Copy_Disabled
    {
    public:
      /// Constructor
      PublisherListener_T (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
        ACE_Reactor* reactor);

      virtual void on_offered_deadline_missed (
        ::DDS::DataWriter *the_writer,
        const ::DDS::OfferedDeadlineMissedStatus & status);

      virtual void on_liveliness_lost (
        ::DDS::DataWriter *the_writer,
        const ::DDS::LivelinessLostStatus & status);

      virtual void on_offered_incompatible_qos (
        ::DDS::DataWriter *the_writer,
        const ::DDS::OfferedIncompatibleQosStatus & status);

      virtual void on_publication_matched (
        ::DDS::DataWriter *the_writer,
        const ::DDS::PublicationMatchedStatus & status);

      /// Destructor
      virtual ~PublisherListener_T (void);

      static ::DDS::StatusMask get_mask (void);

    private:
      virtual void on_unexpected_status(
        ::DDS::Entity* entity,
        ::DDS::StatusKind status_kind);

      ::CCM_DDS::ConnectorStatusListener_var error_listener_;
      ACE_Reactor* reactor_;
    };
  }
}

#include "dds4ccm/impl/ndds/PublisherListener_T.cpp"

#endif /* DDS_PUBLISHERLISTENER_T_H_ */

