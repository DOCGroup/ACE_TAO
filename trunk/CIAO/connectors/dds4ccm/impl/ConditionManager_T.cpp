// $Id$
#include "dds4ccm/impl/DataReader_T.h"
#include "dds4ccm/impl/ReadCondition_T.h"
#include "dds4ccm/impl/QueryCondition_T.h"

#include "ace/OS_NS_sys_time.h"

//============================================================
// ConditionManager_T
//============================================================
template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ConditionManager_T ()
  : impl_ (0),
    ws_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::ConditionManager_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~ConditionManager_T ()
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::~ConditionManager_T");

  delete this->ws_;
  this->ws_ = 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDSReadCondition *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_readcondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::get_readcondition");

  if (! ::CORBA::is_nil (this->rd_condition_.in ()))
    {
      ReadCondition_type * rc = dynamic_cast <ReadCondition_type *>
        (this->rd_condition_.in ());
      if (!rc)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO 
                        ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::get_readcondition - ")
                        ACE_TEXT ("Unable to cast ::DDS::ReadCondition to its internal ")
                        ACE_TEXT ("representation.\n")));
          return 0;
        }
      return rc->get_impl ();
    }
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDSQueryCondition *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_querycondition (
  ::DDS::QueryCondition_ptr dds_qc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::get_querycondition");

  QueryCondition_type * qc = dynamic_cast <QueryCondition_type *> (dds_qc);
  if (!qc)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::get_querycondition - ")
                    ACE_TEXT ("Unable to cast ::DDS::QueryCondition to its internal ")
                    ACE_TEXT ("representation.\n")));
      return 0;
    }
  return qc->get_impl ();
}


template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDSQueryCondition *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_querycondition_getter (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::get_querycondition_getter");

  if (! ::CORBA::is_nil (this->qc_getter_.in ()))
    {
      return this->get_querycondition (this->qc_getter_.in ());
    }
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDSQueryCondition *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_querycondition_listener (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::get_querycondition_listener");

  if (! ::CORBA::is_nil (this->qc_listener_.in ()))
    {
      return this->get_querycondition (this->qc_listener_.in ());
    }
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDSQueryCondition *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_querycondition_reader (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::get_querycondition_reader");

  if (! ::CORBA::is_nil (this->qc_reader_.in ()))
    {
      return this->get_querycondition (this->qc_reader_.in ());
    }
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::init_readcondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::init_readcondition");

  if (!this->ws_)
    {
      // Waitset is created when a query condition is attached.
      // when this is the case, no need to create a read condition
      // (including its proxy).
      ACE_NEW_THROW_EX (this->ws_,
                        DDSWaitSet (),
                        CORBA::NO_MEMORY ());

      if ( ::CORBA::is_nil (this->rd_condition_.in ()))
        {
          this->rd_condition_ = this->impl ()->create_readcondition (
                                  ::DDS::NOT_READ_SAMPLE_STATE,
                                  ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                  ::DDS::ALIVE_INSTANCE_STATE | ::DDS::NOT_ALIVE_INSTANCE_STATE);
          if ( ::CORBA::is_nil (this->rd_condition_.in ()))
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::init_readcondition - ")
                            ACE_TEXT ("Error creating read condition.\n")));
              return;
            }
        }
      DDS_ReturnCode_t const retcode = this->ws_->attach_condition (
        this->get_readcondition ());

      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::init_readcondition - ")
                        ACE_TEXT ("Unable to attach read condition to waitset.\n")));
          throw CCM_DDS::InternalError (retcode, 1);
        }
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::init_readcondition - ")
                    ACE_TEXT ("Read condition created and attached to Waitset.\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::QueryFilter *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::query (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::query");

  if (!this->get_querycondition_reader ())
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::query - ")
                    ACE_TEXT ("Error: No QueryCondition set yet. ")
                    ACE_TEXT ("First set a filter.\n")));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  ::CCM_DDS::QueryFilter_var filter;
  ACE_NEW_THROW_EX (filter,
                    ::CCM_DDS::QueryFilter(),
                    CORBA::NO_MEMORY ());
  filter->expression= this->qc_reader_->get_query_expression ();
  this->qc_reader_->get_query_parameters (filter->parameters);
  return filter._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::query (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::query");

  // Since the readcondition for the getter is always created (at start up),
  // this should be removed first. Instead of a readcondition, there should
  // be a QueryCondition attached to the waitset
  if (this->get_readcondition ())
    { // Getter functionality
      // First remove the existing conditions from the waitset
      // Than create a new (query) condition and attach it to the waitset
      // instead of the readcondition
      this->remove_conditions ();
    }

  if ( ::CORBA::is_nil (this->qc_reader_.in ()))
    {
      // We're sure the query is not set yet.
      this->qc_reader_ = this->impl ()->create_querycondition (
                            ::DDS::READ_SAMPLE_STATE | ::DDS::NOT_READ_SAMPLE_STATE,
                            ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                            ::DDS::ALIVE_INSTANCE_STATE,
                            filter.expression,
                            filter.parameters);
      if ( ::CORBA::is_nil (this->qc_getter_.in ()))
        {
          this->qc_getter_ = this->impl ()->create_querycondition (
                                ::DDS::NOT_READ_SAMPLE_STATE,
                                ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                ::DDS::ALIVE_INSTANCE_STATE | ::DDS::NOT_ALIVE_INSTANCE_STATE,
                                filter.expression,
                                filter.parameters);
          // attach the query condition for the getter to the waitset.
          // creates a waitset if not already created.
          this->attach_querycondition ();
        }
      if ( ::CORBA::is_nil (this->qc_listener_.in ()))
        {
          this->qc_listener_ = this->impl ()->create_querycondition (
                                      ::DDS::NOT_READ_SAMPLE_STATE,
                                      ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                      ::DDS::ANY_INSTANCE_STATE,
                                      filter.expression,
                                      filter.parameters);
        }
      if (::CORBA::is_nil (this->qc_reader_.in ()) ||
          ::CORBA::is_nil (this->qc_getter_.in ()) ||
          ::CORBA::is_nil (this->qc_listener_.in ()))
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::query - ")
                        ACE_TEXT ("Error creating query conditions ")
                        ACE_TEXT ("reader <%@> - getter <%@> - listener <%@>.\n"),
                        this->qc_reader_.in (),
                        this->qc_getter_.in (),
                        this->qc_listener_.in ()));

          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::query - ")
                        ACE_TEXT ("All query conditions created successfully\n")));
        }
    }
  else if (ACE_OS::strlen (filter.expression.in ()) > 0)
    {
      this->set_parameters (filter, this->qc_reader_);
      this->set_parameters (filter, this->qc_getter_);
      this->set_parameters (filter, this->qc_listener_);
    }
  else
    {
      //remove query conditions
      this->remove_conditions ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_parameters (
  const ::CCM_DDS::QueryFilter & filter,
  ::DDS::QueryCondition_ptr qc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::set_parameters");

  ::DDS::ReturnCode_t const retval = qc->set_query_parameters (filter.parameters);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::set_parameters - ")
                    ACE_TEXT ("Error setting expression_parameters. ")
                    ACE_TEXT ("Retval is %C\n"),
                    translate_retcode(retval)));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::attach_querycondition (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::attach_querycondition");

  if (!this->ws_)
    {
      ACE_NEW_THROW_EX (this->ws_,
                        DDSWaitSet (),
                        CORBA::NO_MEMORY ());
    }
  DDS_ReturnCode_t const retcode =
    this->ws_->attach_condition (this->get_querycondition_getter ());

  if (retcode != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::ConditionManager_T::attach_querycondition - "
                                          "Unable to attach query condition to waitset.\n"));
      throw CCM_DDS::InternalError (retcode, 1);
    }
  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CIAO::DDS4CCM::Getter_Base_T::attach_querycondition - "
                                      "Query condition created and attached to Waitset.\n"));
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
bool
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::wait (
    DDSConditionSeq & active_conditions,
    DDS_Duration_t & time_out)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::wait");

#if !defined (DDS4CCM_NLOGGING)
  ACE_Time_Value const start = ACE_OS::gettimeofday ();
#endif

  DDS_ReturnCode_t const retcode =
     this->ws_->wait (active_conditions, time_out);

#if !defined (DDS4CCM_NLOGGING)
  ACE_Time_Value const waited = ACE_OS::gettimeofday () - start;
  DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::wait - ")
                ACE_TEXT ("waited <%#T>\n"),
                &waited));
#endif

  if (retcode == DDS_RETCODE_TIMEOUT)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::wait - ")
                    ACE_TEXT ("No data available after timeout.\n")));
      return false;
    }
  return true;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove_condition (
  ::DDS::QueryCondition_ptr dds_qc,
  const char * type)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::remove_condition");
  if (! ::CORBA::is_nil (dds_qc))
    {
      QueryCondition_type * qc = dynamic_cast <QueryCondition_type *>(dds_qc);
      if (qc)
        {
          ::DDS::ReturnCode_t retcode = this->impl ()->delete_readcondition (qc);
          if (retcode == ::DDS::RETCODE_OK)
            {
              qc->set_impl (0);
              DDS4CCM_DEBUG (8, (LM_DEBUG, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_condition - ")
                            ACE_TEXT ("Succesfully removed query condition for <%C>.\n"),
                            type));
            }
          else
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_condition - ")
                            ACE_TEXT ("Unable to remove query condition for <%C>.")
                            ACE_TEXT ("Error <%C>\n"),
                            type, translate_retcode (retcode)));
            }
        }
    }
}


template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove_conditions ()
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions");

  ::DDS::ReturnCode_t retcode = ::DDS::RETCODE_OK;
  this->remove_condition (this->qc_reader_.in (), "reader");
  this->remove_condition (this->qc_listener_.in (), "listener");

  if (this->ws_)
    {
      if (! ::CORBA::is_nil (this->qc_getter_.in ()))
        {
          if (this->ws_->detach_condition (this->get_querycondition_getter ()) == DDS_RETCODE_OK)
            {
              DDS4CCM_DEBUG (6, (LM_INFO, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions - ")
                            ACE_TEXT ("Query condition successfully detached from waitset.\n")));
              this->remove_condition (this->qc_getter_.in (), "getter");
            }
          else
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions - ")
                            ACE_TEXT ("Unable to detach query condition from waitset.\n")));
            }
        }
      else
        {
          retcode = this->ws_->detach_condition (this->get_readcondition ());
          if (retcode != DDS_RETCODE_OK)
            {
              DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions - ")
                            ACE_TEXT ("Unable to detach read condition ")
                            ACE_TEXT ("from waitset. Error <%C>\n"),
                            translate_retcode (retcode)));
            }
          else
            {
              DDS4CCM_DEBUG (6, (LM_INFO, CLINFO
                            ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions - ")
                            ACE_TEXT ("Read condition successfully detached from waitset.\n")));
            }
        }
    }
  if (this->get_readcondition ())
    {
      retcode = this->impl ()->delete_readcondition (this->rd_condition_.in ());
      if (retcode != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions - ")
                        ACE_TEXT ("Unable to delete read condition from DDSDataReader.")
                        ACE_TEXT ("Error <%C>\n"),
                        translate_retcode (retcode)));
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO
                        ACE_TEXT ("CIAO::DDS4CCM::ConditionManager_T::remove_conditions - ")
                        ACE_TEXT ("Read condition successfully deleted from DDSDataReader.\n")));
        }
    }
  delete this->ws_;
  this->ws_ = 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::passivate");

  this->remove_conditions ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (
  DataReader_type * impl)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager_T::set_impl");

  this->impl_ = impl;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_impl ()
{
  return this->impl_;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *
CIAO::DDS4CCM::ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
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
