// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Reactor.h"

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::DDS_TopicBase_Connector_T (void) :
    BaseConnector ()
  , late_binding_ (false)
{
}

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding");
  return this->late_binding_;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding (bool late_binding)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding");
  this->late_binding_ |= late_binding;
}

template <typename CCM_TYPE, typename DDS_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binded (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binded");

  this->late_binding (ACE_OS::strlen (topic_name) == 0);

  if (this->late_binding () &&
      ACE_OS::strlen (topic_name) > 0 &&
      ACE_OS::strlen (this->topic_name_.in ()) == 0)
    {
      DDS_TopicBase_Connector_T::topic_name (topic_name);
      return true;
    }
  else if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      DDS_TopicBase_Connector_T::topic_name (topic_name);
    }
  return false;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::configuration_complete");

  BaseConnector::configuration_complete ();
  const char* typesupport_name = DDS_TYPE::type_support::get_type_name ();

  if (::CORBA::is_nil (this->topic_.in ()))
    {
      this->register_type (typesupport_name);
      this->init_topic (this->topic_.inout () ,
                        this->topic_name_.in (),
                        typesupport_name);
    }
  this->init_subscriber (this->subscriber_.inout ());
  this->init_publisher (this->publisher_.inout ());
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_activate (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_activate");
  BaseConnector::ccm_activate ();

  this->activate_topic (reactor,
                        this->topic_.in (),
                        this->topiclistener_.inout ());
  this->activate_subscriber (reactor,
                             this->subscriber_.in (),
                             this->subscriber_listener_.inout ());
  this->activate_publisher (reactor,
                            this->publisher_.in (),
                            this->publisher_listener_.inout ());
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_passivate");
  this->passivate_topic (this->topic_.in (),
                         this->topiclistener_.inout ());
  this->passivate_subscriber (this->subscriber_.in (),
                              this->subscriber_listener_.inout ());
  this->passivate_publisher (this->publisher_.in (),
                             this->publisher_listener_.inout ());
  BaseConnector::ccm_passivate ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_remove");
  this->remove_topic (this->topic_.inout ());

  const char* typesupport_name = DDS_TYPE::type_support::get_type_name ();
  this->unregister_type (typesupport_name);
  this->remove_subscriber (this->subscriber_.inout ());
  this->remove_publisher (this->publisher_.inout ());
  BaseConnector::ccm_remove ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
char *
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::topic_name (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::topic_name");

  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::topic_name (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::topic_name");

  this->topic_name_ = topic_name;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields (
  const ::DDS::StringSeq & key_fields)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->key_fields_.length (key_fields.length ());
      for (CORBA::ULong i = 0; i < key_fields.length (); ++i)
        {
          this->key_fields_.operator [](i) = CORBA::string_dup (key_fields[i]);
        }
    }
}

template <typename CCM_TYPE, typename DDS_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields");
/*
    For future use, DDS_TYPE doesn't have get_typecode yet
    ::DDS_TypeCode* ptr = ::DDS_TYPE::type_support::get_typecode ();
    DDS_ExceptionCode_t ex;
    DDS_UnsignedLong const number = ptr->member_count (ex);
    for (DDS_UnsignedLong i = 0; i < number; i++)
    {
     if (ptr->is_member_key (i, ex))
     {
       const char* name = ptr->member_name (i, ex);
       ACE_OS::printf ("Name: %s\n", name);
     }
    }
 */
  ::DDS::StringSeq_var retval = 0;
  ACE_NEW_THROW_EX (retval,
                    ::DDS::StringSeq (this->key_fields_.length ()),
                    ::CORBA::NO_MEMORY ());
  retval->length (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    {
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);
    }
  return retval._retn ();
}

