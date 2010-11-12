// $Id$

#include "dds4ccm/impl/ConditionManager.h"
#include "dds4ccm/impl/Utils.h"

#include "ace/OS_NS_sys_time.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {

    ConditionManager::ConditionManager ()
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::ConditionManager");
    }

    ConditionManager::~ConditionManager ()
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::~ConditionManager");
    }


    ::DDS::ReadCondition_ptr
    ConditionManager::get_readcondition (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::get_readcondition");

      return ::DDS::ReadCondition::_duplicate (this->rd_condition_.in ());
    }


    ::DDS::QueryCondition_ptr
    ConditionManager::get_querycondition_getter (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::get_querycondition_getter");

      return ::DDS::QueryCondition::_duplicate (this->qc_getter_.in ());
    }


    ::DDS::QueryCondition_ptr
    ConditionManager::get_querycondition_listener (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::get_querycondition_listener");

      return ::DDS::QueryCondition::_duplicate (this->qc_listener_.in ());
    }


    ::DDS::QueryCondition_ptr
    ConditionManager::get_querycondition_reader (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::get_querycondition_reader");

      return ::DDS::QueryCondition::_duplicate (this->qc_reader_.in ());
    }


    bool
    ConditionManager::check_condition (
      ::DDS::Condition_ptr condition)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::check_condition");

      ::DDS::ReadCondition_var rc = this->get_readcondition ();
      ::DDS::QueryCondition_var qc = this->get_querycondition_getter ();

      return this->ws_.check_condition (rc.in (), qc.in (), condition);
    }


    void
    ConditionManager::init_readcondition (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::init_readcondition");

      if (!this->ws_.get_rti_entity ())
        {
          // Waitset is created when a query condition is attached.
          // when this is the case, no need to create a read condition
          // (including its proxy).
          this->ws_.init ();
        }
      if ( ::CORBA::is_nil (this->rd_condition_.in ()))
        {
          this->rd_condition_ = this->dds_entity ()->create_readcondition (
                                  ::DDS::NOT_READ_SAMPLE_STATE,
                                  ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                  ::DDS::ALIVE_INSTANCE_STATE | ::DDS::NOT_ALIVE_INSTANCE_STATE);
          if ( ::CORBA::is_nil (this->rd_condition_.in ()))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::init_readcondition - ")
                            ACE_TEXT ("Error creating read condition.\n")));
              return;
            }
        }

      ::DDS::ReturnCode_t retcode =
        this->ws_.attach_condition (this->rd_condition_.in ());

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("ConditionManager::init_readcondition - ")
                        ACE_TEXT ("Unable to attach read condition to waitset. Error <%C>\n"),
                        translate_retcode (retcode)));
          throw ::CCM_DDS::InternalError (retcode, 1);
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("ConditionManager::init_readcondition - ")
                    ACE_TEXT ("Read condition created and attached to Waitset.\n")));
    }


    ::CCM_DDS::QueryFilter *
    ConditionManager::query (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::query");

      if (! ::CORBA::is_nil (this->rd_condition_.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("ConditionManager::query - ")
                        ACE_TEXT ("Error: No QueryCondition set yet. ")
                        ACE_TEXT ("First set a filter.\n")));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      ::CCM_DDS::QueryFilter_var filter;
      ACE_NEW_THROW_EX (filter,
                        ::CCM_DDS::QueryFilter(),
                        ::CORBA::NO_MEMORY ());
      filter->expression= this->qc_reader_->get_query_expression ();
      this->qc_reader_->get_query_parameters (filter->parameters);
      return filter._retn ();
    }


    void
    ConditionManager::query (
      const ::CCM_DDS::QueryFilter & filter)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::query");

      // Since the readcondition for the getter is always created (at start up),
      // this should be removed first. Instead of a readcondition, there should
      // be a QueryCondition attached to the waitset
      if (! ::CORBA::is_nil (this->rd_condition_.in ()))
        { // Getter functionality
          // First remove the existing conditions from the waitset
          // Than create a new (query) condition and attach it to the waitset
          // instead of the readcondition
          this->remove_conditions ();
        }

      if ( ::CORBA::is_nil (this->qc_reader_.in ()))
        {
          // We're sure the query is not set yet.
          this->qc_reader_ = this->dds_entity ()->create_querycondition (
                                ::DDS::READ_SAMPLE_STATE | ::DDS::NOT_READ_SAMPLE_STATE,
                                ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                ::DDS::ALIVE_INSTANCE_STATE,
                                filter.expression,
                                filter.parameters);
          if ( ::CORBA::is_nil (this->qc_getter_.in ()))
            {
              this->qc_getter_ = this->dds_entity ()->create_querycondition (
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
              this->qc_listener_ = this->dds_entity ()->create_querycondition (
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
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::query - ")
                            ACE_TEXT ("Error creating query conditions ")
                            ACE_TEXT ("reader <%@> - getter <%@> - listener <%@>.\n"),
                            this->qc_reader_.in (),
                            this->qc_getter_.in (),
                            this->qc_listener_.in ()));

              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::query - ")
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
          // Remove query conditions
          this->remove_conditions ();
          this->init_readcondition ();
        }
    }


    void
    ConditionManager::set_parameters (
      const ::CCM_DDS::QueryFilter & filter,
      ::DDS::QueryCondition_ptr qc)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::set_parameters");

      ::DDS::ReturnCode_t const retval = qc->set_query_parameters (filter.parameters);
      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("ConditionManager::set_parameters - ")
                        ACE_TEXT ("Error setting expression_parameters. ")
                        ACE_TEXT ("Retval is %C\n"),
                        translate_retcode(retval)));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
        }
    }


    void
    ConditionManager::attach_querycondition (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::attach_querycondition");

      if (!this->ws_.get_rti_entity ())
        {
          this->ws_.init ();
        }

      ::DDS::ReturnCode_t retcode =
        this->ws_.attach_condition (this->qc_getter_.in ());
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "ConditionManager::attach_querycondition - "
                        "Unable to attach query condition to waitset. "
                        "Error <%C>\n",
                        translate_retcode (retcode)));
          throw ::CCM_DDS::InternalError (retcode, 1);
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "ConditionManager::attach_querycondition - "
                        "Query condition created and attached to Waitset.\n"));
        }
    }


    bool
    ConditionManager::wait (
        ::DDS::ConditionSeq & active_conditions,
        ::DDS::Duration_t & time_out)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::wait");

    #if !defined (DDS4CCM_NLOGGING)
      ACE_Time_Value const start = ACE_OS::gettimeofday ();
    #endif

      ::DDS::ReturnCode_t const retcode =
         this->ws_.wait (active_conditions, time_out);

    #if !defined (DDS4CCM_NLOGGING)
      ACE_Time_Value const waited = ACE_OS::gettimeofday () - start;
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("ConditionManager::wait - ")
                    ACE_TEXT ("waited <%#T>\n"),
                    &waited));
    #endif

      if (retcode == ::DDS::RETCODE_TIMEOUT)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        ACE_TEXT ("ConditionManager::wait - ")
                        ACE_TEXT ("No data available after timeout.\n")));
          return false;
        }
      return true;
    }


    void
    ConditionManager::remove_condition (
      ::DDS::QueryCondition_ptr dds_qc,
      const char * type)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::remove_condition");
      if (! ::CORBA::is_nil (dds_qc))
        {
          ::DDS::ReturnCode_t retcode = this->dds_entity ()->delete_readcondition (dds_qc);
          if (retcode == ::DDS::RETCODE_OK)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_DEBUG, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::remove_condition - ")
                            ACE_TEXT ("Succesfully removed query condition for <%C>.\n"),
                            type));
            }
          else
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::remove_condition - ")
                            ACE_TEXT ("Unable to remove query condition for <%C>.")
                            ACE_TEXT ("Error <%C>\n"),
                            type, translate_retcode (retcode)));
            }
        }
    }



    void
    ConditionManager::remove_conditions ()
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::remove_conditions");

      ::DDS::ReturnCode_t retcode = ::DDS::RETCODE_OK;
      if (! ::CORBA::is_nil (this->qc_reader_.in ()))
        {
          this->remove_condition (this->qc_reader_.in (), "reader");
          this->qc_reader_ = ::DDS::QueryCondition::_nil ();
        }
      if (! ::CORBA::is_nil (this->qc_listener_.in ()))
        {
          this->remove_condition (this->qc_listener_.in (), "listener");
          this->qc_listener_ = ::DDS::QueryCondition::_nil ();
        }

      if (this->ws_.get_rti_entity ())
        {
          if (! ::CORBA::is_nil (this->qc_getter_.in ()))
            {
              retcode = this->ws_.detach_condition (this->qc_getter_.in ());
              if (retcode == ::DDS::RETCODE_OK)
                {
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                                ACE_TEXT ("ConditionManager::remove_conditions - ")
                                ACE_TEXT ("Query condition successfully detached from waitset.\n")));
                  this->remove_condition (this->qc_getter_.in (), "getter");
                  this->qc_getter_ = ::DDS::QueryCondition::_nil ();
                }
              else
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                ACE_TEXT ("ConditionManager::remove_conditions - ")
                                ACE_TEXT ("Unable to detach query condition from waitset: ")
                                ACE_TEXT ("Error: <%C>\n"),
                                translate_retcode (retcode) ));
                }
            }
          else
            {
              ::DDS::QueryCondition_var q_condition = this->get_querycondition_getter ();
              if (::CORBA::is_nil (q_condition.in ()))
                {
                  retcode = this->ws_.detach_condition (this->rd_condition_.in ());
                  if (retcode != ::DDS::RETCODE_OK)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                    ACE_TEXT ("ConditionManager::remove_conditions - ")
                                    ACE_TEXT ("Unable to detach read condition ")
                                    ACE_TEXT ("from waitset. Error <%C>\n"),
                                    translate_retcode (retcode)));
                    }
                  else
                    {
                      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                                    ACE_TEXT ("ConditionManager::remove_conditions - ")
                                    ACE_TEXT ("Read condition successfully detached from waitset.\n")));
                    }
                }
            }
        }
      if (! ::CORBA::is_nil (this->rd_condition_.in ()))
        {
          retcode = this->dds_entity ()->delete_readcondition (this->rd_condition_.in ());
          if (retcode != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::remove_conditions - ")
                            ACE_TEXT ("Unable to delete read condition from DDSDataReader.")
                            ACE_TEXT ("Error <%C>\n"),
                            translate_retcode (retcode)));
            }
          else
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                            ACE_TEXT ("ConditionManager::remove_conditions - ")
                            ACE_TEXT ("Read condition successfully deleted from DDSDataReader.\n")));
            }
          this->rd_condition_ = ::DDS::ReadCondition::_nil ();
        }
    }


    void
    ConditionManager::passivate ()
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::passivate");

      this->remove_conditions ();
    }


    void
    ConditionManager::set_dds_entity (
      ::DDS::DataReader_ptr dr)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::ConditionManager::set_dds_entity");

      this->dr_ = dr;
    }


    ::DDS::DataReader_ptr
    ConditionManager::get_dds_entity (void)
    {
      return ::DDS::DataReader::_duplicate (this->dr_.in ());
    }


    ::DDS::DataReader_ptr
    ConditionManager::dds_entity (void)
    {
      if (! ::CORBA::is_nil (this->dr_.in ()))
        {
          return this->dr_.in ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "ConditionManager::dds_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
    }
  }
}
