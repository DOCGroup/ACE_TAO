/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DOMAINPARTICIPANTLISTENER_H
#define CIAO_RTI_DOMAINPARTICIPANTLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DomainParticipantListener_i :
        public virtual ::DDSDomainParticipantListener
      {
      public:
        // Constructor
        RTI_DomainParticipantListener_i (::DDS::DomainParticipantListener_ptr p);

        // Destructor
        virtual ~RTI_DomainParticipantListener_i (void);

        ::DDS::DomainParticipantListener_ptr get_datareaderlistener (void);
      private:
        ::DDS::DomainParticipantListener_var impl_;
      };
    }
  }
}

#endif
