// $Id$
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::Getter_Base_T (void)
  : reader_ (0),
    time_out_ (),
    max_delivered_data_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::Getter_Base_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~Getter_Base_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::~Getter_Base_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::NDDS::DataReader_T<DDS_TYPE> *
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
{
  if (this->reader_)
    {
      return this->reader_;
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    "CIAO::DDS4CCM::Getter_Base_T::impl - "
                    "Throwing BAD_INV_ORDER.\n"));
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::ReturnCode_t
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get (
  typename DDS_TYPE::dds_seq_type & data,
  typename DDS_TYPE::sampleinfo_seq_type & sample_info,
  const DDS_Long & max_samples)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::get");

  ReadCondition_type * rc = this->condition_manager_->get_readcondition ();
  if (rc)
    {
      return this->impl ()->get (data,
                                 sample_info,
                                 max_samples,
                                 rc->get_rti_entity ());
    }
  else
    {
      QueryCondition_type * qc = this->condition_manager_->get_querycondition_getter ();
      if (!qc)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Getter_Base_T::get - "
                        "Unable to retrieve QueryCondition from "
                        "Condition manager\n"));
          return ::DDS::RETCODE_ERROR;
        }
      return this->impl ()->get (data,
                                 sample_info,
                                 max_samples,
                                 qc->get_rti_entity ());
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::check_condition (
  ::DDS::Condition_ptr condition)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::check_condition");

  ReadCondition_type * rc = this->condition_manager_->get_readcondition ();
  QueryCondition_type * qc = this->condition_manager_->get_querycondition_getter ();

  ReadCondition_type * rc_cond = dynamic_cast <ReadCondition_type *> (condition);
  QueryCondition_type * qc_cond = dynamic_cast <QueryCondition_type *> (condition);

  return ((rc_cond && rc_cond->get_rti_entity () == rc->get_rti_entity ()) ||
          (qc_cond && qc_cond->get_rti_entity () == qc->get_rti_entity ()) );
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_many (
  typename CCM_TYPE::seq_type& instances,
  ::CCM_DDS::ReadInfoSeq& infos)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::get_many");

  ::DDS::ConditionSeq active_conditions;
  if (!this->condition_manager_->wait (active_conditions, this->time_out_))
    {
      return false;
    }

  ::DDS_Long max_samples = this->max_delivered_data_;
  if (max_samples == 0)
    {
      max_samples = DDS_LENGTH_UNLIMITED;
    }

  typename DDS_TYPE::sampleinfo_seq_type sample_info;
  typename DDS_TYPE::dds_seq_type data;

  for (::CORBA::ULong i = 0; i < active_conditions.length(); i++)
    {
      if (this->check_condition (active_conditions[i].in ()))
        {
          // Check trigger
          active_conditions[i]->get_trigger_value ();

          ::DDS::ReturnCode_t const retcode = this->get (data,
                                                         sample_info,
                                                         1);

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
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                            ACE_TEXT ("Getter_Base_T::get_many: ")
                            ACE_TEXT ("read <%d> - valid <%d>\n"),
                            sample_info.length (),
                            number_read));
              infos.length (number_read);
              instances.length (number_read);
              number_read = 0;
              for (::DDS_Long j = 0; j < data.length (); j ++)
                {
                  if (sample_info[j].valid_data)
                    {
                      infos.operator[](number_read) <<= sample_info[j];
                      instances.operator[](number_read) = data[j];
                      ++number_read;
                    }
                }
            }
          else
            {
              // RETCODE_NO_DATA should be an error
              // because after a timeout there should be
              // data.
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Getter_Base_T::get_many - "
                    "Error while reading from DDS: <%C>\n",
                    translate_retcode (retcode)));
              DDS_ReturnCode_t const retval =
                this->impl ()->return_loan (data, sample_info);
              if (retval != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Getter_Base_T::get_many - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
                }
              throw ::CCM_DDS::InternalError (retcode, 1);
            }

          DDS_ReturnCode_t const retval =
            this->impl ()->return_loan (data, sample_info);
          if (retval != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                "Getter_Base_T::get_many - "
                "Error returning loan to DDS - <%C>\n",
                translate_retcode (retval)));
            }
        }
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::Duration_t
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::time_out (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::time_out");

  return this->time_out_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::time_out (
  const ::DDS::Duration_t & time_out)
{
  this->time_out_ = time_out;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::DataNumber_t
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::max_delivered_data (void)
{
  return this->max_delivered_data_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  this->max_delivered_data_ = max_delivered_data;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Getter_Base_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (
  DataReader_type * dr,
  ConditionManager_type * condition_manager)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_Base_T::set_impl");

  this->reader_ = dr;
  this->condition_manager_ = condition_manager;
  this->condition_manager_->set_impl (dr);
  this->condition_manager_->init_readcondition ();
}


template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, true, VENDOR_TYPE>::get_one (
  typename DDS_TYPE::value_type::_out_type an_instance,
  ::CCM_DDS::ReadInfo_out info)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_T::get_one");

  ::DDS::ConditionSeq active_conditions;
  if (!this->condition_manager_->wait (active_conditions, this->time_out_))
    {
      return false;
    }

  for (::CORBA::ULong i = 0; i < active_conditions.length(); i++)
    {
      if (this->check_condition (active_conditions[i].in ()))
        {
          bool valid_data_read = false;

          while (!valid_data_read)
            {
              typename DDS_TYPE::sampleinfo_seq_type sample_info;
              typename DDS_TYPE::dds_seq_type data;

              ::DDS::ReturnCode_t const retcode = this->get (data,
                                                          sample_info,
                                                          1);

              if (retcode == ::DDS::RETCODE_NO_DATA)
                {
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                                "Getter_T::get_one - "
                                "DDS returned <%C>. No data available in DDS.\n",
                                translate_retcode (retcode)));
                  return false;
                }
              else if (retcode != ::DDS::RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Getter_T::get_one - "
                        "Error while reading from DDS: <%C>\n",
                        translate_retcode (retcode)));
                  DDS_ReturnCode_t const retval =
                    this->impl ()->return_loan (data, sample_info);
                  if (retval != ::DDS::RETCODE_OK)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Getter_Base_T::get_one - "
                        "Error returning loan to DDS - <%C>\n",
                        translate_retcode (retval)));
                    }

                  throw ::CCM_DDS::InternalError (retcode, 1);
                }
              else if (data.length () == 1 && sample_info[0].valid_data)
                {
                  info <<= sample_info[0];
                  an_instance = data[0];
                  valid_data_read = true;
                }
              else
                {
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "Getter_T::get_one - "
                        "No valid available in DDS.\n"));
                }
              // Return the loan of each read.
              DDS_ReturnCode_t const retval =
                this->impl ()->return_loan (data, sample_info);
              if (retval != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Getter_T::get_one - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
                }
            }
        }
    }

  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
bool
CIAO::DDS4CCM::DDS_CCM::Getter_T<DDS_TYPE, CCM_TYPE, false, VENDOR_TYPE>::get_one (
  typename DDS_TYPE::value_type::_out_type an_instance,
  ::CCM_DDS::ReadInfo_out info)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Getter_T::get_one");

  an_instance = 0;
  ACE_NEW_THROW_EX (an_instance,
                    typename DDS_TYPE::value_type,
                    ::CORBA::NO_MEMORY ());
  ::DDS::ConditionSeq active_conditions;
  if (!this->condition_manager_->wait (active_conditions,
                                       this->time_out_))
    {
      return false;
    }
  typename DDS_TYPE::sampleinfo_seq_type sample_info;
  typename DDS_TYPE::dds_seq_type data;
  for (::CORBA::ULong i = 0; i < active_conditions.length(); i++)
    {
      if (this->check_condition (active_conditions[i].in ()))
        {
          bool valid_data_read = false;

          while (!valid_data_read)
            {
              typename DDS_TYPE::sampleinfo_seq_type sample_info;
              typename DDS_TYPE::dds_seq_type data;
              ::DDS::ReturnCode_t const retcode = this->get (data,
                                                          sample_info,
                                                          1);
              if (retcode == ::DDS::RETCODE_NO_DATA)
                {
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "Getter_T::get_one - "
                        "DDS returned <%C>. No data available in DDS.\n",
                        translate_retcode (retcode)));
                  return false;
                }
              else if (retcode != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Getter_T::get_one - "
                        "Error while reading from DDS: <%C>\n",
                        translate_retcode (retcode)));
                  DDS_ReturnCode_t const retval =
                    this->impl ()->return_loan (data, sample_info);
                  if (retval != DDS_RETCODE_OK)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Getter_T::get_one - "
                        "Error returning loan to DDS - <%C>\n",
                        translate_retcode (retval)));
                    }
                  throw ::CCM_DDS::InternalError (retcode, 1);
                }
              else if (data.length () == 1 && sample_info[0].valid_data)
                {
                  info <<= sample_info[0];
                  *an_instance = data[0];
                  valid_data_read = true;
                }
              else
                {
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "Getter_T::get_one - "
                        "No valid available in DDS.\n"));
                }
              // Return the loan of each read.
              ::DDS::ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
              if (retval != DDS_RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Getter_T::get_one - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
                }
            }
        }
    }

  return true;
}

