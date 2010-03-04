/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_DDS4CCM_TOPICDESCRIPTION_H
#define CIAO_DDS4CCM_TOPICDESCRIPTION_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/dds4ccm_ndds_impl_export.h"

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#include "tao/LocalObject.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::TopicDescription DDSTopicDescription;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_IMPL_Export CCM_DDS_TopicDescription_i :
      public virtual ::DDS::CCM_TopicDescription,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_TopicDescription_i (DDSTopicDescription * rc);

      /// Destructor
      virtual ~CCM_DDS_TopicDescription_i (void);

      virtual char * get_type_name (void);

      virtual char * get_name (void);

      virtual ::DDS::DomainParticipant_ptr get_participant (void);

      DDSTopicDescription * get_impl (void);

      void set_impl (DDSTopicDescription * rc);

    private:
      DDSTopicDescription * impl_;

      DDSTopicDescription * impl (void);
    };
  }
}

#endif /* CIAO_DDS4CCM_TOPICDESCRIPTION_H */
