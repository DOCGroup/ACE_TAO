// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/Duration_t.h"
#include "dds4ccm/impl/ndds/SampleInfo.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::Getter_T (::DDS::DataReader_ptr reader)
: impl_ (0),
  condition_(0),
  time_out_ ()
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::Getter_T");

  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (reader);
  if (rdr == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                   "Unable to cast provided DataReader to servant\n"));
      throw CORBA::INTERNAL ();
    }

  this->impl_ =  DDS_TYPE::data_reader::narrow (rdr->get_datareader ());

  if (this->impl_ == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                   "Unable to narrow the provided writer entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
  //now create the waitset conditions
  ws_ = new DDSWaitSet ();
  gd_ = new DDSGuardCondition ();
  rd_condition_ = this->impl_->create_readcondition (DDS_NOT_READ_SAMPLE_STATE,
                                                     DDS_ANY_VIEW_STATE,
                                                     DDS_ANY_INSTANCE_STATE);
  DDS_ReturnCode_t retcode = ws_->attach_condition (gd_);
  if (retcode != DDS_RETCODE_OK)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "GETTER:Unable to attach guard condition to waitset.\n"));
      throw CCM_DDS::InternalError (retcode, 0);
    }
  retcode = ws_->attach_condition (rd_condition_);
  if (retcode != DDS_RETCODE_OK)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "GETTER: Unable to attach read condition to waitset.\n"));
      throw CCM_DDS::InternalError (retcode, 1);
    }
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::~Getter_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::~Getter_T");
  delete ws_;
  delete gd_;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::wait (
          DDSConditionSeq& active_conditions)
{
  DDS_Duration_t timeout;
  timeout <<= this->time_out_;
  DDS_ReturnCode_t const retcode = ws_->wait (active_conditions, timeout);
  if (retcode == DDS_RETCODE_TIMEOUT)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Getter: No data available after timeout.\n")));
      return false;
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_all (
          typename DDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_all_history (
          typename DDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  DDSConditionSeq active_conditions;
  DDS_SampleInfoSeq sample_info;
  if (!this->wait (active_conditions))
    return false;

  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
  return false;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_one (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
  DDSConditionSeq active_conditions;
  DDS_SampleInfoSeq sample_info;
  if (!this->wait (active_conditions))
    return false;
  typename DDS_TYPE::dds_seq_type data;
  for (int i = 0; i < active_conditions.length(); i++)
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
          DDS_ReturnCode_t retcode = this->impl_->read (data,
                                    sample_info,
                                    DDS_LENGTH_UNLIMITED,
                                    DDS_NOT_READ_SAMPLE_STATE ,
                                    DDS_ANY_VIEW_STATE,
                                    DDS_ANY_INSTANCE_STATE);

          if (retcode == DDS_RETCODE_NO_DATA)
            {
              throw CCM_DDS::InternalError (retcode, 1);
            }
          else if (retcode != DDS_RETCODE_OK)
            {
              CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                    "Unable to return the loan to DDS: <%d>\n", retcode));
              break;
            }
          info <<= sample_info; //retrieves the last sample.
          if (retcode == DDS_RETCODE_OK && data.length () >= 0)
            {
              an_instance = data[0];
            }
          else
            {
              throw CCM_DDS::InternalError (retcode, 0);
            }

          retcode = this->impl_->return_loan(data,sample_info);
          if (retcode != DDS_RETCODE_OK)
            {
              CIAO_ERROR ((LM_ERROR, ACE_TEXT ("return loan error %d\n"), retcode));
            }
        }
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_one_history (
          const typename DDS_TYPE::value_type& an_instance,
          typename DDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (an_instance);
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
bool
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::get_next (
          typename DDS_TYPE::value_type::_out_type  an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
  ACE_UNUSED_ARG (an_instance);
  ACE_UNUSED_ARG (info);
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE >
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::filter (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::filter (const ::CCM_DDS::QueryFilter & filter)
{
  this->query_filter_ = filter;
}

template <typename DDS_TYPE, typename CCM_TYPE >
::DDS::Duration_t
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::time_out (void)
{
  return this->time_out_;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE>::time_out (const ::DDS::Duration_t & time_out)
{
  this->time_out_ = time_out;
}
