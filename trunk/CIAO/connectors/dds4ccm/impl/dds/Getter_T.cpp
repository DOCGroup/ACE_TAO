// $Id$
#include "dds4ccm/impl/dds/DataReader.h"
#include "dds4ccm/impl/dds/Utils.h"

#include "dds4ccm/impl/dds/ndds/Duration_t.h"
#include "dds4ccm/impl/dds/ndds/SampleInfo.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::Getter_Base_T (void)
  : reader_ (0),
    time_out_ (),
    max_delivered_data_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_Base_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::~Getter_Base_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::~Getter_Base_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE> *
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::impl (void)
{
  if (this->reader_)
    {
      return this->reader_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
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
  if (!this->impl ()->wait (active_conditions,
                            this->time_out_))
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
      if (active_conditions[i] == this->impl ()->get_readcondition () ||
          active_conditions[i] == this->impl ()->get_querycondition ())
        {
          // Check trigger
          active_conditions[i]->get_trigger_value ();

          // Take read condition
          DDS_ReturnCode_t retcode =
            this->impl ()->read_w_condition (data,
                                             sample_info,
                                             max_samples);

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
              DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
              if (retval != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_Base_T::get_many - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
                }
              throw CCM_DDS::InternalError (retcode, 1);
            }

          DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
          if (retval != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_Base_T::get_many - "
                "Error returning loan to DDS - <%C>\n",
                translate_retcode (retval)));
            }
        }
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::Duration_t
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::time_out (void)
{
  ::DDS::Duration_t timeout;
  timeout <<= this->time_out_;

  return timeout;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::time_out (
  const ::DDS::Duration_t & time_out)
{
  this->time_out_ <<= time_out;
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
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE>::set_impl (
  DataReader_T<DDS_TYPE, CCM_TYPE> * dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::set_impl");

  this->reader_ = dr;
  this->impl ()->create_readcondition ();
}


template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one (
  typename DDS_TYPE::value_type::_out_type an_instance,
  ::CCM_DDS::ReadInfo_out info)
{
  DDSConditionSeq active_conditions;
  if (!this->impl ()->wait (active_conditions,
                            this->time_out_))
    {
      return false;
    }

  for (::DDS_Long i = 0; i < active_conditions.length(); i++)
    {
      if (active_conditions[i] == this->impl ()->get_readcondition () ||
          active_conditions[i] == this->impl ()->get_querycondition ())
        {
          bool valid_data_read = false;

          while (!valid_data_read)
            {
              DDS_SampleInfoSeq sample_info;
              typename DDS_TYPE::dds_seq_type data;

              DDS_ReturnCode_t retcode =
                this->impl ()->read_w_condition (data,
                                                 sample_info);
              if (retcode == ::DDS::RETCODE_NO_DATA)
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
                  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
                  if (retval != DDS_RETCODE_OK)
                    {
                      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                        "Error returning loan to DDS - <%C>\n",
                        translate_retcode (retval)));
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
              DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
              if (retval != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_T<DDS_TYPE, CCM_TYPE, true>::get_one - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
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
  if (!this->impl ()->wait (active_conditions,
                            this->time_out_))
    {
      return false;
    }

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;
  for (::DDS_Long i = 0; i < active_conditions.length(); i++)
    {
      if (active_conditions[i] == this->impl ()->get_readcondition () ||
          active_conditions[i] == this->impl ()->get_querycondition ())
        {
          bool valid_data_read = false;

          while (!valid_data_read)
            {
              DDS_SampleInfoSeq sample_info;
              typename DDS_TYPE::dds_seq_type data;
              DDS_ReturnCode_t retcode =
                this->impl ()->read_w_condition (data,
                                                 sample_info);
              if (retcode == ::DDS::RETCODE_NO_DATA)
                {
                  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, false>::get_one - "
                        "DDS returned DDS_RETCODE_NO_DATA. No data available in DDS.\n"));
                  return false;
                }
              else if (retcode != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, false>::get_one - "
                        "Error while reading from DDS: <%C>\n",
                        translate_retcode (retcode)));
                  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
                  if (retval != DDS_RETCODE_OK)
                    {
                      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_T<DDS_TYPE, CCM_TYPE, false>::get_one - "
                        "Error returning loan to DDS - <%C>\n",
                        translate_retcode (retval)));
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
                        "CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, false>::get_one - "
                        "No valid available in DDS.\n"));
                }
              // Return the loan of each read.
              DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
              if (retval != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    "CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_T<DDS_TYPE, CCM_TYPE, false>::get_one - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
                }
            }
        }
    }

  return true;
}

