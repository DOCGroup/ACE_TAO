// $Id$

#include "WaitSet.h"

#include "ndds/ndds_cpp.h"

#include "dds4ccm/impl/ndds/Condition.h"
#include "dds4ccm/impl/ndds/QueryCondition.h"
#include "dds4ccm/impl/ndds/ReadCondition.h"
#include "dds4ccm/impl/ndds/convertors/Duration_t.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_WaitSet_i::DDS_WaitSet_i ()
      : rti_entity_ (0)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::DDS_WaitSet_i");
    }

    DDS_WaitSet_i::~DDS_WaitSet_i (void)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::~DDS_WaitSet_i");

      delete this->rti_entity_;
    }

    ::DDS::ReturnCode_t
    DDS_WaitSet_i::wait (::DDS::ConditionSeq & active_conditions,
                         const ::DDS::Duration_t & timeout)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::wait");

      DDS_Duration_t dds_timeout;
      dds_timeout <<= timeout;

      DDSConditionSeq dds_conditions;
      DDS_ReturnCode_t const retcode =
         this->rti_entity ()->wait (dds_conditions, dds_timeout);

      this->convert_conditions (dds_conditions, active_conditions);

      return retcode;
    }

    ::DDS::ReturnCode_t
    DDS_WaitSet_i::attach_condition (::DDS::Condition_ptr cond)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::attach_condition");

      ::DDS::ReadCondition_var rc_cond = ::DDS::ReadCondition::_narrow (cond);
      ::DDS::QueryCondition_var qc_cond = ::DDS::QueryCondition::_narrow (cond);

      if (! ::CORBA::is_nil (qc_cond.in ()))
        {
          DDS_QueryCondition_i * qc_proxy =
            dynamic_cast <DDS_QueryCondition_i *> (qc_cond.in ());
          if (qc_proxy)
            {
              return this->rti_entity ()->attach_condition (qc_proxy->get_rti_entity ());
            }
        }
      else if (! ::CORBA::is_nil (rc_cond.in ()))
        {
          DDS_ReadCondition_i * rc_proxy =
            dynamic_cast <DDS_ReadCondition_i *> (rc_cond.in ());
          if (rc_proxy)
            {
              return this->rti_entity ()->attach_condition (rc_proxy->get_rti_entity ());
            }
        }
      return ::DDS::RETCODE_BAD_PARAMETER;
    }

    ::DDS::ReturnCode_t
    DDS_WaitSet_i::detach_condition (::DDS::Condition_ptr cond)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::detach_condition");

      ::DDS::ReadCondition_var rc_cond = ::DDS::ReadCondition::_narrow (cond);
      ::DDS::QueryCondition_var qc_cond = ::DDS::QueryCondition::_narrow (cond);

       if (! ::CORBA::is_nil (qc_cond.in ()))
        {
          DDS_QueryCondition_i * qc_proxy =
            dynamic_cast <DDS_QueryCondition_i *> (qc_cond.in ());
          if (qc_proxy)
            {
              return this->rti_entity ()->detach_condition (qc_proxy->get_rti_entity ());
            }
        }
      else if (! ::CORBA::is_nil (rc_cond.in ()))
        {
          DDS_ReadCondition_i * rc_proxy =
            dynamic_cast <DDS_ReadCondition_i *> (rc_cond.in ());
          if (rc_proxy)
            {
              return this->rti_entity ()->detach_condition (rc_proxy->get_rti_entity ());
            }
        }
      return ::DDS::RETCODE_BAD_PARAMETER;
    }

    ::DDS::ReturnCode_t
    DDS_WaitSet_i::get_conditions (::DDS::ConditionSeq & attached_conditions)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::get_conditions");

      DDSConditionSeq dds_seq;
      ::DDS::ReturnCode_t retcode = this->rti_entity ()->get_conditions (dds_seq);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_WaitSet_i::get_conditions - "
                        "Error while calling get_conditions: <%C>\n",
                        CIAO::DDS4CCM::translate_retcode (retcode)));
          return retcode;
        }
      this->convert_conditions (dds_seq, attached_conditions);
      return retcode;
    }

    ::DDS::InstanceHandle_t
    DDS_WaitSet_i::check_handle (
      const ::DDS::InstanceHandle_t & instance_handle,
      const ::DDS::InstanceHandle_t & lookup_handle,
      bool & error,
      bool & non_existent)
    {
      ::DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
      hnd <<= instance_handle;

      ::DDS_InstanceHandle_t lookup_hnd = ::DDS_HANDLE_NIL;
      lookup_hnd <<= lookup_handle;

      ::DDS::InstanceHandle_t ret = ::DDS::HANDLE_NIL;

      if (!DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL) &&
          !DDS_InstanceHandle_equals (&hnd, &lookup_hnd))
        {
          error = true;
        }
      else if (DDS_InstanceHandle_equals (&lookup_hnd, &::DDS_HANDLE_NIL))
        {
          non_existent = true;
        }
      else
        {
          ret <<= lookup_hnd;
        }
      return ret;
    }

    bool
    DDS_WaitSet_i::check_condition (
      ::DDS::ReadCondition_ptr rc,
      ::DDS::QueryCondition_ptr qc,
      ::DDS::Condition_ptr condition)
    {
      DDS_ReadCondition_i * dds_rc = dynamic_cast <DDS_ReadCondition_i *> (rc);
      DDS_QueryCondition_i * dds_qc = dynamic_cast <DDS_QueryCondition_i *> (qc);

      DDS_ReadCondition_i * rc_cond = dynamic_cast <DDS_ReadCondition_i *> (condition);
      DDS_QueryCondition_i * qc_cond = dynamic_cast <DDS_QueryCondition_i *> (condition);

      return ((rc_cond && rc_cond->get_rti_entity () == dds_rc->get_rti_entity ()) ||
              (qc_cond && qc_cond->get_rti_entity () == dds_qc->get_rti_entity ()) );
    }

    DDSWaitSet *
    DDS_WaitSet_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_WaitSet_i::init (void)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::init");
      if (!this->rti_entity_)
        {
          ACE_NEW_THROW_EX (this->rti_entity_,
                            DDSWaitSet (),
                            ::CORBA::NO_MEMORY ());
        }
    }

    void
    DDS_WaitSet_i::convert_conditions (const DDSConditionSeq & dds_conditions,
                                       ::DDS::ConditionSeq & conditions)
    {
      DDS4CCM_TRACE ("DDS_WaitSet_i::convert_conditions");

      conditions.length (dds_conditions.length ());

      for (DDS_Long i = 0;
           i < dds_conditions.length();
           ++i)
        {
          DDSQueryCondition * dds_qc =
            dynamic_cast <DDSQueryCondition *> (dds_conditions[i]);
          if (dds_qc)
            {
              ::DDS::QueryCondition_var cond;
              ACE_NEW_THROW_EX (cond,
                                DDS_QueryCondition_i (dds_qc,
                                                      ::DDS::DataReader::_nil ()),
                                ::CORBA::NO_MEMORY ());
              conditions[i] = ::DDS::QueryCondition::_duplicate (cond.in ());
            }
          else
            {
              DDSReadCondition * dds_rc =
                dynamic_cast <DDSReadCondition *> (dds_conditions[i]);
              if (dds_rc)
                {
                  ::DDS::ReadCondition_var cond;
                  ACE_NEW_THROW_EX (cond,
                                    DDS_ReadCondition_i (dds_rc,
                                                         ::DDS::DataReader::_nil ()),
                                    ::CORBA::NO_MEMORY ());
                  conditions[i] = ::DDS::ReadCondition::_duplicate (cond.in ());
                }
            }
        }
    }

    DDSWaitSet *
    DDS_WaitSet_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_WaitSet_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}
