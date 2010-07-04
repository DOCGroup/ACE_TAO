/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CCM_DDS_CONTENTFILTERSETTING_H
#define CCM_DDS_CONTENTFILTERSETTING_H

#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/idl/dds4ccm_BaseEC.h"
#include "dds4ccm/impl/ContentFilteredTopic_T.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DDS_ContentFilterSetting_i
      : public virtual ::CCM_DDS::CCM_ContentFilterSetting,
        public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_ContentFilterSetting_i ();

      /// Destructor
      virtual ~CCM_DDS_ContentFilterSetting_i (void);

      virtual void set_filter_parameters (const ::DDS::StringSeq & parameters);

      ::CCM_DDS::QueryFilter *filter (void);

      void filter (const ::CCM_DDS::QueryFilter & filter);

      ::DDS::ContentFilteredTopic_ptr
      create_contentfilteredtopic (::DDS::Topic_ptr topic,
                                   ::DDS::Subscriber_ptr subscriber);

      void delete_contentfilteredtopic (::DDS::Subscriber_ptr subscriber);
    private:
      ::CCM_DDS::QueryFilter filter_;
      ::DDS::ContentFilteredTopic_var cft_;
    };
  }
}

#endif
