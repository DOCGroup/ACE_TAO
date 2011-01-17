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

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"

class DDSTopicDescription;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_TopicDescription_i :
      public virtual ::DDS::TopicDescription,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_TopicDescription_i (::DDSTopicDescription * rc,
                            ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_TopicDescription_i (void);

      virtual char * get_type_name (void);

      virtual char * get_name (void);

      virtual ::DDS::DomainParticipant_ptr get_participant (void);

      ::DDSTopicDescription * get_rti_entity (void);

    private:
      ::DDSTopicDescription * rti_entity_;
      ::DDS::DomainParticipant_var dp_;

      ::DDSTopicDescription * rti_entity (void);
    };
  }
}

#endif /* TOPICDESCRIPTION_T_H_ */
