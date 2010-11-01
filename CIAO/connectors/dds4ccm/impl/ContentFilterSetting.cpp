// $Id$

#include "ContentFilterSetting.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/Utils.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_ContentFilterSetting_i::CCM_DDS_ContentFilterSetting_i ()
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::CCM_DDS_ContentFilterSetting_i");
    }

    CCM_DDS_ContentFilterSetting_i::~CCM_DDS_ContentFilterSetting_i ()
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::~CCM_DDS_ContentFilterSetting_i");
    }

    ::CCM_DDS::QueryFilter *
    CCM_DDS_ContentFilterSetting_i::filter (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::filter");
      ::CCM_DDS::QueryFilter* retval = new ::CCM_DDS::QueryFilter;
      *retval = this->filter_;
      return retval;
    }

    void
    CCM_DDS_ContentFilterSetting_i::filter (
      const ::CCM_DDS::QueryFilter & filter)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::filter");

      this->filter_ = filter;
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "CCM_DDS_ContentFilterSetting_i::filter - "));
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    "Expression : <%C>\t\n",
                    filter.expression.in ()));
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "Parameters :\n"));
      for (::CORBA::ULong i = 0; i < filter.parameters.length (); ++i)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "\t<%C>\n",
                        filter.parameters[i].in ()));
        }
    }

    ::DDS::ContentFilteredTopic_ptr
    CCM_DDS_ContentFilterSetting_i::create_contentfilteredtopic (
      ::DDS::Topic_ptr topic,
      ::DDS::Subscriber_ptr subscriber)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::create_contentfilteredtopic");

      ::DDS::DomainParticipant_var dp = subscriber->get_participant ();
      if (::CORBA::is_nil (dp.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "CCM_DDS_ContentFilterSetting_i::create_contentfilteredtopic: "
                        "Unable to get DomainParticipant.\n"));
          throw ::CORBA::INTERNAL ();
        }

      ACE_Auto_Array_Ptr <char> name;
      {
        char *name_bootstrap = 0;
        ACE_NEW_THROW_EX (name_bootstrap,
                          char[32],
                          ::CORBA::NO_MEMORY ());
        name.reset(name_bootstrap);
      }
      ACE_OS::sprintf (name.get (),
                       "DDS4CCM_CFT_%ld",
                       reinterpret_cast <unsigned long> (this));

      this->cft_ = dp->create_contentfilteredtopic (name.get (),
                                                    topic,
                                                    this->filter_.expression,
                                                    this->filter_.parameters);
      if (::CORBA::is_nil (this->cft_.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "CCM_DDS_ContentFilterSetting_i::create_contentfilteredtopic: "
                        "Error creating ContentfilteredTopic.\n"));
          throw ::CORBA::INTERNAL ();
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "CCM_DDS_ContentFilterSetting_i::create_contentfilteredtopic: "
                    "successfully created ContentFilteredTopic <%C>\n",
                    name.get ()));
      return ::DDS::ContentFilteredTopic::_duplicate (this->cft_);
    }

    void
    CCM_DDS_ContentFilterSetting_i::delete_contentfilteredtopic (
      ::DDS::Subscriber_ptr subscriber)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::delete_contentfilteredtopic");

      ::DDS::DomainParticipant_var dp = subscriber->get_participant ();
      if (! ::CORBA::is_nil (dp.in ()) &&
          ! ::CORBA::is_nil (this->cft_.in ()))
        {
          dp->delete_contentfilteredtopic (this->cft_.in ());
        }
    }

    void
    CCM_DDS_ContentFilterSetting_i::set_filter_parameters (
      const ::DDS::StringSeq & parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_ContentFilterSetting_i::set_filter_parameters");
      if (::CORBA::is_nil (this->cft_.in ()) ||
          ACE_OS::strlen (this->filter_.expression.in ()) == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "CCM_DDS_ContentFilterSetting_i::set_filter_parameters: "
                        "Calling set_filter_parameters without having set a filter\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      ::DDS::ReturnCode_t retcode = this->cft_->set_expression_parameters (parameters);
      if (retcode == DDS_RETCODE_OK)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "CCM_DDS_ContentFilterSetting_i::set_filter_parameters: "
                        "successfully set parameters on ContentfilteredTopic\n"));
          this->filter_.parameters = parameters;
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "CCM_DDS_ContentFilterSetting_i::set_filter_parameters: "
                        "Error setting filter parameters: <%C>\n",
                        translate_retcode (retcode)));
          throw ::CCM_DDS::InternalError (retcode, 0);
        }
    }
  }
}
