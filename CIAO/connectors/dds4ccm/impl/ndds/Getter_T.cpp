// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/Duration_t.h"
#include "dds4ccm/impl/ndds/SampleInfo.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::Getter_T (void) :
   impl_ (0),
   condition_(0),
   time_out_ (),
   max_delivered_data_ (0)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::Getter_T");
}

template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::~Getter_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::~Getter_T");
  delete gd_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::impl (void)
{
  if (this->impl_)
    {
      return this->impl_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::wait (
          DDSWaitSet* ws,
          DDSConditionSeq& active_conditions)
{
  DDS_Duration_t timeout;
  timeout <<= this->time_out_;
  DDS_ReturnCode_t const retcode = ws->wait (active_conditions, timeout);
  if (retcode == DDS_RETCODE_TIMEOUT)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Getter: No data available after timeout.\n")));
      return false;
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::configure_waitset (DDSWaitSet* ws)
{
  DDS_ReturnCode_t retcode = ws->attach_condition (gd_);
  if (retcode != DDS_RETCODE_OK)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "GETTER: Unable to attach guard condition to waitset.\n"));
      throw CCM_DDS::InternalError (retcode, 0);
    }
  retcode = ws->attach_condition (rd_condition_);
  if (retcode != DDS_RETCODE_OK)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "GETTER: Unable to attach read condition to waitset.\n"));
      throw CCM_DDS::InternalError (retcode, 1);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_many (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  instances = new typename CCM_TYPE::seq_type;
  infos = new ::CCM_DDS::ReadInfoSeq;

//  DDS_WaitSetProperty_t wsp;
//  wsp.max_event_count = this->max_delivered_data_;
  DDSWaitSet* ws = new DDSWaitSet ();
  configure_waitset (ws);

  DDSConditionSeq active_conditions;
  if (!this->wait (ws, active_conditions))
    {
      delete ws;
      return false;
    }

  ::DDS_Long max_samples = this->max_delivered_data_;
  if (max_samples == 0)
    {
      max_samples = DDS_LENGTH_UNLIMITED;
    }

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;
  for (::DDS_Long i = 0; i < active_conditions.length(); i++)
    {
      if (active_conditions[i] == gd_)
        {
          gd_->set_trigger_value (false);
        }

      if (active_conditions[i] == rd_condition_)
        {
          // Check trigger
          active_conditions[i]->get_trigger_value ();

          // Take read condition
          DDS_ReturnCode_t retcode = this->impl ()->read (data,
                                    sample_info,
                                    max_samples,
                                    DDS_NOT_READ_SAMPLE_STATE ,
                                    DDS_ANY_VIEW_STATE,
                                    DDS_ANY_INSTANCE_STATE);

          if (retcode == DDS_RETCODE_OK && data.length () >= 1)
            {
              ::CORBA::ULong number_read = 0;
              for (::DDS_Long index = 0; index < sample_info.length (); index ++)
                {
                  if (sample_info[index].valid_data)
                    {
                      ++number_read;
                    }
                }
              CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Getter_T::get_many: ")
                                ACE_TEXT ("read <%d> - valid <%d>\n"),
                                sample_info.length (),
                                number_read));
              infos->length (number_read);
              instances->length (number_read);
              number_read = 0;
              for (::DDS_Long j = 0; j < data.length (); j ++)
                {
                  if (sample_info[j].valid_data)
                    {
                      infos->operator[](number_read) <<= sample_info[j];
                      instances->operator[](number_read) = data[j];
                      ++number_read;
                    }
                }
            }
          else
            {
              // RETCODE_NO_DATA should be an error
              // because after a timeout there should be
              // data.
              CIAO_ERROR ((LM_ERROR, CLINFO
                    "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                    "Error while reading from DDS: <%C>\n",
                    translate_retcode (retcode)));
              this->impl ()->return_loan(data,sample_info);
              delete ws;
              throw CCM_DDS::InternalError (retcode, 1);
            }

          retcode = this->impl ()->return_loan(data,sample_info);
          if (retcode != DDS_RETCODE_OK)
            {
              CIAO_ERROR ((LM_ERROR, ACE_TEXT ("return loan error %C\n"),
                          translate_retcode (retcode)));
            }
        }
    }
  delete ws;
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_one (
          typename DDS_TYPE::value_type::_out_type an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
  an_instance = new typename DDS_TYPE::value_type;

  DDSWaitSet* ws = new DDSWaitSet ();
  configure_waitset (ws);

  DDSConditionSeq active_conditions;
  if (!this->wait (ws, active_conditions))
    return false;

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;
  for (::DDS_Long i = 0; i < active_conditions.length(); i++)
    {
      if (active_conditions[i] == gd_)
        {
          gd_->set_trigger_value (false);
        }

      if (active_conditions[i] == rd_condition_)
        {
          // Check trigger
          active_conditions[i]->get_trigger_value ();

          // Take read condition
          DDS_ReturnCode_t retcode = this->impl ()->read (data,
                                    sample_info,
                                    DDS_LENGTH_UNLIMITED,
                                    DDS_NOT_READ_SAMPLE_STATE ,
                                    DDS_ANY_VIEW_STATE,
                                    DDS_ANY_INSTANCE_STATE);

          if (retcode == DDS_RETCODE_OK && data.length () >= 1)
            {
              info <<= sample_info[0]; //retrieves the last sample.
              *an_instance = data[0];
            }
          else
            {
              // RETCODE_NO_DATA should be an error
              // because after a timeout there should be
              // data.
              CIAO_ERROR ((LM_ERROR, CLINFO
                    "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                    "Error while reading from DDS: <%C>\n",
                    translate_retcode (retcode)));
              this->impl ()->return_loan(data,sample_info);
              delete ws;
              throw CCM_DDS::InternalError (retcode, 1);
            }

          retcode = this->impl ()->return_loan(data,sample_info);
          if (retcode != DDS_RETCODE_OK)
            {
              CIAO_ERROR ((LM_ERROR,
                          ACE_TEXT ("return loan error %C\n"),
                          translate_retcode (retcode)));
            }
        }
    }
  delete ws;
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
::DDS::Duration_t
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::time_out (void)
{
  return this->time_out_;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::time_out (
  const ::DDS::Duration_t & time_out)
{
  this->time_out_ = time_out;
}

template <typename DDS_TYPE, typename CCM_TYPE >
::CCM_DDS::DataNumber_t
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::max_delivered_data (void)
{
  return this->max_delivered_data_;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  this->max_delivered_data_ = max_delivered_data;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::data_reader (
  ::DDS::DataReader_ptr reader)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::InstanceHandleManager_T::data_reader");

  if (::CORBA::is_nil (reader))
    {
      impl_ = 0;
      delete gd_;
      gd_ = 0;
    }
  else
    {
      RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (reader);
      if (rdr == 0)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::data_reader - "
                       "Unable to cast provided DataReader to servant\n"));
          throw CORBA::INTERNAL ();
        }

      this->impl_ =  DDS_TYPE::data_reader::narrow (rdr->get_datareader ());

      if (!this->impl_)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::data_reader - "
                       "Unable to narrow the provided writer entity to the specific "
                       "type necessary to publish messages\n"));
          throw CORBA::INTERNAL ();
        }

      // Now create the waitset conditions
      gd_ = new DDSGuardCondition ();
      rd_condition_ = this->impl_->create_readcondition (DDS_NOT_READ_SAMPLE_STATE,
                                                         DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                                         DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE);
    }
}

