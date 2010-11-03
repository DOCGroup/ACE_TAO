/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSTOPICLISTENER_T_H_
#define DDSTOPICLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

# include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    class DDS_TopicListener_T :
      public virtual ::DDSTopicListener
    {
    typedef DDS_Topic_T<DDS_TYPE> Topic_type;
    public:
      /// Constructor
      DDS_TopicListener_T (::DDS::TopicListener_ptr p);

      /// Destructor
      virtual ~DDS_TopicListener_T (void);

      virtual void on_inconsistent_topic(
        ::DDSTopic* topic,
        const ::DDS_InconsistentTopicStatus& status);

      ::DDS::TopicListener_ptr get_topiclistener (void);
    private:
      ::DDS::TopicListener_var impl_;
    };
  }
}

#include "dds4ccm/impl/ndds/DDSTopicListener_T.cpp"

#endif /* TOPICLISTENER_T_H_ */
