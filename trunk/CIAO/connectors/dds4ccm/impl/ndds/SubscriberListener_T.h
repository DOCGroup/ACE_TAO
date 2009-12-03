/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS_SUBSCRIBERLISTENER_T_H_
#define DDS_SUBSCRIBERLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class SubscriberListener_T :
        public ::DDS::SubscriberListener,
        private ACE_Copy_Disabled
      {
      public:
        /// Constructor
        SubscriberListener_T (
          ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

        /// Destructor
        virtual ~SubscriberListener_T (void);

        static ::DDS::StatusMask get_mask (void);
      private:
        ::CCM_DDS::ConnectorStatusListener_var error_listener_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/SubscriberListener_T.cpp"

#endif /* DDS_SUBSCRIBERLISTENER_T_H_ */
