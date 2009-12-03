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
        typename CCM_TYPE::context_type::_ptr_type context,
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

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
      typename CCM_TYPE::context_type::_var_type context_;
      ::CCM_DDS::ConnectorStatusListener_var error_listener_;
    };
  }
}

#include "dds4ccm/impl/ndds/PublisherListener_T.cpp"

#endif /* DDS_PUBLISHERLISTENER_T_H_ */

