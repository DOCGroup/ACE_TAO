// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/TopicListener_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_TopicBase_Connector_T (void) :
    DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>()
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (void)
{
  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (
  const char * topic_name)
{
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->topic_name_ = topic_name;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (const ::DDS::StringSeq & key_fields)
{
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->key_fields_.length (key_fields.length ());
      for (CORBA::ULong i = 0; i < key_fields.length (); ++i)
        {
          this->key_fields_.operator [](i) = this->key_fields_[i];
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (void)
{
/*
    For future use, DDS_TYPE doesn't have get_typecode yet
    ::DDS_TypeCode* ptr = ::DDS_TYPE::get_typecode ();
    DDS_ExceptionCode_t ex;
    DDS_UnsignedLong number = ptr->member_count (ex);
    for (DDS_UnsignedLong i = 0; i < number; i++)
    {
     if (ptr->is_member_key (i, ex))
     {
       const char* name = ptr->member_name (i, ex);
       printf ("Name: %s\n", name);
     }
    }
 */
  ::DDS::StringSeq *retval =
    new ::DDS::StringSeq (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    {
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);
    }
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_topic (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_TopicBase_Connector_T::configure_default_topic - "
                "Configuring default topic\n"));

  this->configure_default_domain ();

  if (CORBA::is_nil (this->topic_))
    {
      try
        {
          CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *part =
            dynamic_cast< CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i * > (this->domain_participant_.in ());
          DDS_ReturnCode_t const retcode = DDS_TYPE::type_support::register_type(
                part->get_participant (), DDS_TYPE::type_support::get_type_name ());
          if (retcode == DDS_RETCODE_OK)
            {
              this->topiclistener_ = new ::CIAO::DDS4CCM::TopicListener_T
                <DDS_TYPE, CCM_TYPE> (
                    this->context_);

              ::DDS::TopicQos tqos;
              this->topic_ =
                this->domain_participant_->create_topic (this->topic_name_.in (),
                                             DDS_TYPE::type_support::get_type_name (),
                                             tqos,
                                             this->topiclistener_.in (),
                                             DDS_INCONSISTENT_TOPIC_STATUS);
            }
          else
            {
              throw CCM_DDS::InternalError (retcode, 0);
            }
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, "Caught unknown error while configuring default topic\n"));
          throw CORBA::INTERNAL ();
        }
    }
}


