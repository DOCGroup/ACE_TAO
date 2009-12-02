/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_SUBSCRIBERLISTENER_H
#define CIAO_RTI_SUBSCRIBERLISTENER_H

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_SubscriberListener_i :
        public virtual ::DDSSubscriberListener
      {
      public:
        // Constructor
        RTI_SubscriberListener_i ( ::DDS::SubscriberListener_ptr s);

        // Destructor
        virtual ~RTI_SubscriberListener_i (void);

        ::DDS::SubscriberListener_ptr get_subscriber_listener (void);
      private:
        ::DDS::SubscriberListener_var impl_;
      };
    }
  }
}

#endif
