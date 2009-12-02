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
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
      class PublisherListener_T :
        public ::DDS::PublisherListener,
        private ACE_Copy_Disabled
      {
      public:
        /// Constructor
        PublisherListener_T (
          ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

        /// Destructor
        virtual ~PublisherListener_T (void);

      private:
        ::CCM_DDS::ConnectorStatusListener_var error_listener_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/PublisherListener_T.cpp"

#endif /* DDS_PUBLISHERLISTENER_T_H_ */

