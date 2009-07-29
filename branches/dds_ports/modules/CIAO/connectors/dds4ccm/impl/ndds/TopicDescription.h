/**
 * @file TopicDescription.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TOPICDESCRIPTION_H
#define CIAO_RTI_TOPICDESCRIPTION_H

#include "tao/LocalObject.h"

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
      class DDS4CCM_NDDS_IMPL_Export RTI_TopicDescription_i :
        public virtual ::DDS::CCM_TopicDescription
      {
      public:
        // Constructor
        RTI_TopicDescription_i (DDSTopicDescription *td);

        // Destructor
        virtual ~RTI_TopicDescription_i (void);

        virtual char *
        get_type_name (void);

        virtual char *
          get_name (void);

        virtual ::DDS::DomainParticipant_ptr
          get_participant (void);

        DDSTopicDescription * get_topicdescription (void);
      private:
        DDSTopicDescription *td_;
      };
    }
  }
}

#endif /* CIAO_RTI_TOPICDESCRIPTION_H */
