// // $Id$
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::Getter_Base_T (void)
      : time_out_ (),
        max_delivered_data_ (0)
    {
      DDS4CCM_TRACE ("Getter_Base_T::Getter_Base_T");
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::~Getter_Base_T (void)
    {
      DDS4CCM_TRACE ("Getter_Base_T::~Getter_Base_T");
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    typename DDS_READER_TYPE::_ptr_type
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::dds_reader (void)
    {
      if (this->dds_reader_)
        {
          return this->dds_reader_;
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "CIAO::DDS4CCM::Getter_Base_T::impl - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::ReturnCode_t
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::get (
      SEQ_VALUE_TYPE & data,
      ::DDS::SampleInfoSeq & sample_info,
      const ::CORBA::Long & max_samples)
    {
      DDS4CCM_TRACE ("Getter_Base_T::get");

      ::DDS::ReadCondition_var rc = this->condition_manager_->get_readcondition ();
      if (! ::CORBA::is_nil (rc.in ()))
        {
          return this->get (data,
                            sample_info,
                            max_samples,
                            rc.in ());
        }
      else
        {
          ::DDS::QueryCondition_var qc = this->condition_manager_->get_querycondition_getter ();
          if (::CORBA::is_nil (qc.in ()))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "Getter_Base_T::get - "
                            "Unable to retrieve QueryCondition from "
                            "Condition manager\n"));
              return ::DDS::RETCODE_ERROR;
            }
          return this->get (data,
                            sample_info,
                            max_samples,
                            qc.in ());
        }
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::ReturnCode_t
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::get (
      SEQ_VALUE_TYPE & data,
      ::DDS::SampleInfoSeq & sample_info,
      const ::CORBA::Long & max_samples,
      ::DDS::QueryCondition_ptr qc)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get");

      if (! ::CORBA::is_nil (qc))
        {
          return this->dds_reader ()->read_w_condition (data,
                                                  sample_info,
                                                  max_samples,
                                                  qc);
      }
      return ::DDS::RETCODE_ERROR;
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::ReturnCode_t
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::get (
      SEQ_VALUE_TYPE & data,
      ::DDS::SampleInfoSeq & sample_info,
      const ::CORBA::Long & max_samples,
      ::DDS::ReadCondition_ptr rd)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get");

      if (! ::CORBA::is_nil (rd))
        {
          return this->dds_reader ()->read_w_condition (data,
                                                  sample_info,
                                                  max_samples,
                                                  rd);
        }
      return DDS_RETCODE_ERROR;
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    bool
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::get_many (
      SEQ_VALUE_TYPE& instances,
      ::CCM_DDS::ReadInfoSeq& infos)
    {
      DDS4CCM_TRACE ("Getter_Base_T::get_many");

      ::DDS::ConditionSeq active_conditions;
      if (!this->condition_manager_->wait (active_conditions, this->time_out_))
        {
          return false;
        }

      ::CORBA::Long max_samples = this->max_delivered_data_;
      if (max_samples == 0)
        {
          max_samples = DDS_LENGTH_UNLIMITED;
        }

      ::DDS::SampleInfoSeq sample_info;
      SEQ_VALUE_TYPE data;

      for (::CORBA::ULong i = 0; i < active_conditions.length(); i++)
        {
          if (this->condition_manager_->check_condition (active_conditions[i].in ()))
            {
              // Check trigger
              active_conditions[i]->get_trigger_value ();

              ::DDS::ReturnCode_t const retcode = this->get (data,
                                                              sample_info,
                                                              1);

              if (retcode == ::DDS::RETCODE_OK && data.length () >= 1)
                {
                  ::CORBA::ULong number_read = 0;
                  for (::CORBA::ULong index = 0; index < sample_info.length (); index ++)
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
                  for (::CORBA::ULong j = 0; j < data.length (); j ++)
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

                  ::DDS::ReturnCode_t const retval =
                    this->dds_reader ()->return_loan (data, sample_info);
                  if (retval != ::DDS::RETCODE_OK)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Getter_Base_T::get_many - "
                        "Error returning loan to DDS - <%C>\n",
                        translate_retcode (retval)));
                    }
                  throw ::CCM_DDS::InternalError (retcode, 1);
                }

              ::DDS::ReturnCode_t const retval =
                this->dds_reader ()->return_loan (data, sample_info);
              if (retval != ::DDS::RETCODE_OK)
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

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::Duration_t
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::time_out (void)
    {
      DDS4CCM_TRACE ("Getter_Base_T::time_out");

      return this->time_out_;
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::time_out (
      const ::DDS::Duration_t & time_out)
    {
      this->time_out_ = time_out;
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::CCM_DDS::DataNumber_t
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::max_delivered_data (void)
    {
      return this->max_delivered_data_;
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::max_delivered_data (
      ::CCM_DDS::DataNumber_t max_delivered_data)
    {
      this->max_delivered_data_ = max_delivered_data;
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::set_dds_reader (
      ::DDS::DataReader_ptr dr,
      ::CIAO::DDS4CCM::ConditionManager * condition_manager)
    {
      DDS4CCM_TRACE ("Getter_Base_T::set_dds_reader");

      this->dds_reader_ = DDS_READER_TYPE::_narrow (dr);
      this->condition_manager_ = condition_manager;
      this->condition_manager_->set_dds_entity (dr);
      this->condition_manager_->init_readcondition ();
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::DataReader_ptr
    Getter_Base_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE>::get_dds_reader (void)
    {
      return ::DDS::DataReader::_duplicate (this->dds_reader_.in ());
    }

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    bool
    Getter_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE, true>::get_one (
      typename VALUE_TYPE::_out_type an_instance,
      ::CCM_DDS::ReadInfo_out info)
    {
      DDS4CCM_TRACE ("Getter_T::get_one");

      ::DDS::ConditionSeq active_conditions;
      if (!this->condition_manager_->wait (active_conditions, this->time_out_))
        {
          return false;
        }

      for (::CORBA::ULong i = 0; i < active_conditions.length(); i++)
        {
          if (this->condition_manager_->check_condition (active_conditions[i].in ()))
            {
              bool valid_data_read = false;

              while (!valid_data_read)
                {
                  ::DDS::SampleInfoSeq sample_info;
                  SEQ_VALUE_TYPE data;

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

                      ::DDS::ReturnCode_t const retval =
                        this->dds_reader ()->return_loan (data, sample_info);
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
                  ::DDS::ReturnCode_t const retval =
                    this->dds_reader ()->return_loan (data, sample_info);
                  if (retval != ::DDS::RETCODE_OK)
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

    template <typename GETTER_TYPE, typename DDS_READER_TYPE, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    bool
    Getter_T<GETTER_TYPE, DDS_READER_TYPE, VALUE_TYPE, SEQ_VALUE_TYPE, false>::get_one (
      typename VALUE_TYPE::_out_type an_instance,
      ::CCM_DDS::ReadInfo_out info)
    {
      DDS4CCM_TRACE ("Getter_T::get_one");

      an_instance = 0;
      ACE_NEW_THROW_EX (an_instance,
                        VALUE_TYPE,
                        ::CORBA::NO_MEMORY ());
      ::DDS::ConditionSeq active_conditions;
      if (!this->condition_manager_->wait (active_conditions,
                                            this->time_out_))
        {
          return false;
        }
      ::DDS::SampleInfoSeq sample_info;
      SEQ_VALUE_TYPE data;
      for (::CORBA::ULong i = 0; i < active_conditions.length(); i++)
        {
          if (this->condition_manager_->check_condition (active_conditions[i].in ()))
            {
              bool valid_data_read = false;

              while (!valid_data_read)
                {
                  ::DDS::SampleInfoSeq sample_info;
                  SEQ_VALUE_TYPE data;
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

                      ::DDS::ReturnCode_t const retval =
                        this->dds_reader ()->return_loan (data, sample_info);
                      if (retval != ::DDS::RETCODE_OK)
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
                  ::DDS::ReturnCode_t const retval = this->dds_reader ()->return_loan (data, sample_info);
                  if (retval != ::DDS::RETCODE_OK)
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
  }
}

