/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef TOPICDESCRIPTION_T_H_
#define TOPICDESCRIPTION_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_DomainParticipant;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_TopicDescription_T :
      public virtual ::DDS::TopicDescription,
      public virtual ::CORBA::LocalObject
    {
    typedef DDS_DomainParticipant<DDS_TYPE, VENDOR_TYPE> DomainParticipant_type;

    public:
      /// Constructor
      DDS_TopicDescription_T (DDSTopicDescription * rc);

      /// Destructor
      virtual ~DDS_TopicDescription_T (void);

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

#include "dds4ccm/impl/ndds/TopicDescription_T.cpp"

#endif /* TOPICDESCRIPTION_T_H_ */
