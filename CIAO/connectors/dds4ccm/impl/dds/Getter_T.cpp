// $Id$
#include "dds4ccm/impl/dds/DataReader.h"
#include "dds4ccm/impl/dds/Utils.h"

#include "dds4ccm/impl/dds/ndds/Duration_t.h"
#include "dds4ccm/impl/dds/ndds/SampleInfo.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::Getter_Base_T (void) :
   impl_ (0),
   condition_(0),
   time_out_ (),
   max_delivered_data_ (0),
   ws_ (0),
   rd_condition_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_Base_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::~Getter_Base_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::~Getter_Base_T");
  delete ws_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::impl (void)
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

template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::wait (
          DDSConditionSeq& active_conditions)
{
  DDS_Duration_t timeout;
  timeout <<= this->time_out_;
  DDS_ReturnCode_t const retcode = this->ws_->wait (active_conditions, timeout);
  if (retcode == DDS_RETCODE_TIMEOUT)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("Getter: No data available after timeout.\n")));
      return false;
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::get_many (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  instances = 0;
  ACE_NEW_THROW_EX (instances,
                    typename CCM_TYPE::seq_type,
                    CORBA::NO_MEMORY ());
  infos = 0;
  ACE_NEW_THROW_EX (infos,
                    ::CCM_DDS::ReadInfoSeq,
                    CORBA::NO_MEMORY ());

  DDSConditionSeq active_conditions;
  if (!this->wait (active_conditions))
    {
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
      if (active_conditions[i] == this->rd_condition_)
        {
          // Check trigger
          active_conditions[i]->get_trigger_value ();

          // Take read condition
          DDS_ReturnCode_t retcode = this->impl ()->read_w_condition (
                                    data,
                                    sample_info,
                                    max_samples,
                                    this->rd_condition_);

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
              DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("Getter_Base_T::get_many: ")
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
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_Base_T::get_many - "
                    "Error while reading from DDS: <%C>\n",
                    translate_retcode (retcode)));
              this->impl ()->return_loan(data,sample_info);
              throw CCM_DDS::InternalError (retcode, 1);
            }

          retcode = this->impl ()->return_loan(data,sample_info);
          if (retcode != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, ACE_TEXT ("return loan error %C\n"),
                          translate_retcode (retcode)));
            }
        }
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::Duration_t
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::time_out (void)
{
  return this->time_out_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::time_out (
  const ::DDS::Duration_t & time_out)
{
  this->time_out_ = time_out;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::DataNumber_t
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::max_delivered_data (void)
{
  return this->max_delivered_data_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  this->max_delivered_data_ = max_delivered_data;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  DDS_ReturnCode_t retcode = this->ws_->detach_condition (this->rd_condition_);
  if (retcode != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "Getter_Base_T::passivate - "
                      "Unable to detach read condition from waitset.\n"));
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "Getter_Base_T::passivate - "
                      "Read condition succesfully detached from waitset.\n"));
    }
  if (retcode != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "Getter_Base_T::passivate - "
                      "Unable to detach guard condition from waitset.\n"));
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "Getter_Base_T::passivate - "
                      "Guard condition succesfully detached from waitset.\n"));
    }
  retcode = this->impl ()->delete_readcondition (this->rd_condition_);
  if (retcode != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "Getter_Base_T::passivate - "
                      "Unable to delete read condition from DDSDataReader.\n"));
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "Getter_Base_T::passivate - "
                      "Read condition succesfully deleted from DDSDataReader.\n"));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::set_impl (
  ::DDS::DataReader_ptr reader)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::set_impl");

  if (::CORBA::is_nil (reader))
    {
      impl_ = 0;
      delete this->ws_;
      this->ws_ = 0;
    }
  else
    {
      CCM_DDS_DataReader_i *rdr = dynamic_cast <CCM_DDS_DataReader_i *> (reader);
      if (!rdr)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::data_reader - "
                       "Unable to cast provided DataReader to servant\n"));
          throw CORBA::INTERNAL ();
        }

      this->impl_ =  DDS_TYPE::data_reader::narrow (rdr->get_impl ());

      if (!this->impl_)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::data_reader - "
                       "Unable to narrow the provided writer entity to the specific "
                       "type necessary to publish messages\n"));
          throw CORBA::INTERNAL ();
        }

      // Now create the waitset conditions
      ACE_NEW_THROW_EX (this->ws_,
                        DDSWaitSet (),
                        CORBA::NO_MEMORY ());
      this->rd_condition_ = this->impl ()->create_readcondition (DDS_NOT_READ_SAMPLE_STATE,
                                                       DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                                       DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE);
      DDS_ReturnCode_t retcode = this->ws_->attach_condition (this->rd_condition_);
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "GETTER: Unable to attach read condition to waitset.\n"));
          throw CCM_DDS::InternalError (retcode, 1);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one (
  typename DDS_TYPE::value_type::_out_type an_instance,
  ::CCM_DDS::ReadInfo_out info)
{
  DDSConditionSeq active_conditions;
  if (!this->wait (active_conditions))
    {
      return false;
    }

  for (::DDS_Long i = 0; i < active_conditions.length(); i++)
    {
      if (active_conditions[i] == this->rd_condition_)
        {
          bool valid_data_read = false;

          while (!valid_data_read)
            {
              DDS_SampleInfoSeq sample_info;
              typename DDS_TYPE::dds_seq_type data;
              DDS_ReturnCode_t retcode = this->impl ()->read_w_condition (
                                                                  data,
                                                                  sample_info,
                                                                  1,
                                                                  this->rd_condition_);
              if (retcode == DDS_RETCODE_NO_DATA)
                {
                  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "DDS returned DDS_RETCODE_NO_DATA. No data available in DDS.\n"));
                  return false;
                }
              else if (retcode != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "Error while reading from DDS: <%C>\n",
                        translate_retcode (retcode)));
                  if (this->impl ()->return_loan (data, sample_info) !=
                      DDS_RETCODE_OK)
                    {
                      DDS4CCM_ERROR (1, (LM_ERROR,
                                  ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Getter_T")
                                  ACE_TEXT ("<DDS_TYPE, CCM_TYPE, true>::get_one - ")
                                  ACE_TEXT ("return loan error\n")));
                    }
                  throw CCM_DDS::InternalError (retcode, 1);
                }
              else if (data.length () == 1 &&
                       sample_info[0].valid_data)
                {
                  info <<= sample_info[0];
                  an_instance = data[0];
                  valid_data_read = true;
                }
              else
                {
                  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "No valid available in DDS.\n"));
                }
              //return the loan of each read.
              if (this->impl ()->return_loan (data, sample_info) !=
                  DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR,
                              ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Getter_T")
                              ACE_TEXT ("<DDS_TYPE, CCM_TYPE, true>::get_one - ")
                              ACE_TEXT ("return loan error\n")));
                }
            }
        }
    }

  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, false>::get_one (
  typename DDS_TYPE::value_type::_out_type an_instance,
  ::CCM_DDS::ReadInfo_out info)
{
  an_instance = 0;
  ACE_NEW_THROW_EX (an_instance,
                    typename DDS_TYPE::value_type,
                    CORBA::NO_MEMORY ());
  DDSConditionSeq active_conditions;
  if (!this->wait (active_conditions))
    {
      return false;
    }

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;
  for (::DDS_Long i = 0; i < active_conditions.length(); i++)
    {
      if (active_conditions[i] == this->rd_condition_)
        {
          bool valid_data_read = false;

          while (!valid_data_read)
            {
              DDS_SampleInfoSeq sample_info;
              typename DDS_TYPE::dds_seq_type data;
              DDS_ReturnCode_t retcode = this->impl ()->read_w_condition (
                                                                  data,
                                                                  sample_info,
                                                                  1,
                                                                  this->rd_condition_);
              if (retcode == DDS_RETCODE_NO_DATA)
                {
                  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "DDS returned DDS_RETCODE_NO_DATA. No data available in DDS.\n"));
                  return false;
                }
              else if (retcode != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "Error while reading from DDS: <%C>\n",
                        translate_retcode (retcode)));
                  if (this->impl ()->return_loan (data, sample_info) !=
                      DDS_RETCODE_OK)
                    {
                      DDS4CCM_ERROR (1, (LM_ERROR,
                                  ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Getter_T")
                                  ACE_TEXT ("<DDS_TYPE, CCM_TYPE, true>::get_one - ")
                                  ACE_TEXT ("return loan error\n")));
                    }
                  throw CCM_DDS::InternalError (retcode, 1);
                }
              else if (data.length () == 1 &&
                       sample_info[0].valid_data)
                {
                  info <<= sample_info[0];
                  *an_instance = data[0];
                  valid_data_read = true;
                }
              else
                {
                  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "No valid available in DDS.\n"));
                }
              // Return the loan of each read.
              if (this->impl ()->return_loan (data, sample_info) !=
                  DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR,
                              ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Getter_T")
                              ACE_TEXT ("<DDS_TYPE, CCM_TYPE, true>::get_one - ")
                              ACE_TEXT ("return loan error\n")));
                }
            }
        }
    }

  return true;
}

