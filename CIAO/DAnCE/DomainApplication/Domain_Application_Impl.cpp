// $Id$

#include "Domain_Application_Impl.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  DomainApplication_Impl::DomainApplication_Impl (
    const char* plan_uuid,
    PortableServer::POA_ptr poa,
    TNam2Nm & nams,
    TNm2Id& node_ids)
      : poa_ (PortableServer::POA::_duplicate (poa)),
        planUUID_ (plan_uuid),
        nams_ (nams),
        node_ids_ (node_ids)
  {
    DANCE_TRACE("DomainApplication_Impl::DomainApplication_Impl");
  }

  DomainApplication_Impl::~DomainApplication_Impl()
  {
    DANCE_TRACE ("DomainApplication_Impl::~DomainApplication_Impl()");

    this->node_applications_.unbind_all();
  }

  void
  DomainApplication_Impl::finishLaunch (::Deployment::AMH_ApplicationResponseHandler_ptr _tao_rh,
                                        const ::Deployment::Connections & connections,
                                        CORBA::Boolean start)
  {
    DANCE_TRACE ("DomainApplication_Impl::finishLaunch");

    try
      {
        DANCE_DEBUG (9, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplication_impl::finishLaunch - ")
                  ACE_TEXT("Plan %C DomainApplication[%@] : %u applications to finish launching\n"),
                  this->planUUID_.c_str (),
                  this,
                  this->node_applications_.current_size ()));

        // create completion counter
        DA_NA_ReplyHandlerImpl::Counter* _cp = 0;
        ACE_NEW_THROW_EX (_cp,
                          DA_NA_ReplyHandlerImpl::Counter (node_applications_.current_size (),
                                                           this,
                                                           _tao_rh),
                          CORBA::NO_MEMORY ());
        DA_NA_ReplyHandlerImpl::Counter_AutoPtr _counter_ptr (_cp);

        for (TApp2Mgr::iterator iter = this->node_applications_.begin();
            iter != this->node_applications_.end();
            ++iter)
          {
            ACE_CString node_id;
            this->app_node_ids_.find ((*iter).ext_id_, node_id);
            try
              {
                // create and activate reply handler
                DA_NA_ReplyHandlerImpl* da_na_rh_servant = 0;
                ACE_NEW_THROW_EX (da_na_rh_servant,
                                  DA_NA_ReplyHandlerImpl (this,
                                                          node_id.c_str (),
                                                          _counter_ptr),
                                  CORBA::NO_MEMORY ());

                PortableServer::ObjectId_var id = this->poa_->activate_object (da_na_rh_servant);

                CORBA::Object_var ref = this->poa_->id_to_reference (id.in());

                ::Deployment::AMI_ApplicationHandler_var da_na_handler =
                    ::Deployment::AMI_ApplicationHandler::_narrow (ref.in ());

                DANCE_DEBUG (6, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::finishLaunch - ")
                                ACE_TEXT("Plan %C DomainApplication[%@] node %C : ")
                                ACE_TEXT("calling finishLaunch on a node application\n"),
                                this->planUUID_.c_str (),
                                this,
                                node_id.c_str ()));

                (*iter).ext_id_->sendc_finishLaunch (da_na_handler.in (),
                                                    connections,
                                                    start);

                DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::finishLaunch - ")
                              ACE_TEXT("finishLaunch has been called on a node application\n")));
              }
            catch (CORBA::Exception& ex)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT("DomainApplication_Impl::finishLaunch - ")
                              ACE_TEXT("DomainApplication[%@] caught a CORBA exception handling node %C : %C\n"),
                              this,
                              node_id.c_str (),
                              ex._info ().c_str ()));
                // mark failure
                _counter_ptr->increment_fail_count ();
                // mark off node
                _counter_ptr->decrement_exec_count ();
                // continue for next node
              }
            catch (...)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT("DomainApplication_Impl::finishLaunch - ")
                              ACE_TEXT("DomainApplication[%@] caught a unknown exception handling node %C\n"),
                              this,
                              node_id.c_str ()));
                // mark failure
                _counter_ptr->increment_fail_count ();
                // mark off node
                _counter_ptr->decrement_exec_count ();
                // continue for next node
              }
          }
      }
    catch (CORBA::Exception& ex)
      {
        CORBA::Exception* local_ex = ex._tao_duplicate ();
        ::Deployment::AMH_ApplicationExceptionHolder amh_exholder (local_ex);
        _tao_rh->finishLaunch_excep (&amh_exholder);
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplication_Impl::finishLaunch - ")
                    ACE_TEXT("Propagating StartError for unknown exception caught here\n")));
        CORBA::Exception* start_ex = new Deployment::StartError();
        ::Deployment::AMH_ApplicationExceptionHolder amh_exholder (start_ex);
        _tao_rh->finishLaunch_excep (&amh_exholder);
      }
  }

  void
  DomainApplication_Impl::start (::Deployment::AMH_ApplicationResponseHandler_ptr _tao_rh)
  {
    DANCE_TRACE ("DomainApplication_Impl::start");
    try
      {
        DANCE_DEBUG (9, (LM_DEBUG, DLINFO ACE_TEXT("DomainApplication_impl::start - ")
                  ACE_TEXT("Plan %C DomainApplication[%@] : %u applications to start\n"),
                  this->planUUID_.c_str (),
                  this,
                  this->node_applications_.current_size ()));

        // create completion counter
        DA_NA_ReplyHandlerImpl::Counter* _cp = 0;
        ACE_NEW_THROW_EX (_cp,
                          DA_NA_ReplyHandlerImpl::Counter (node_applications_.current_size (),
                                                           this,
                                                           _tao_rh),
                          CORBA::NO_MEMORY ());
        DA_NA_ReplyHandlerImpl::Counter_AutoPtr _counter_ptr (_cp);

        for (TApp2Mgr::iterator iter = this->node_applications_.begin();
            iter != this->node_applications_.end();
            ++iter)
          {
            ACE_CString node_id;
            this->app_node_ids_.find ((*iter).ext_id_, node_id);

            try
              {
                // create and activate reply handler
                DA_NA_ReplyHandlerImpl* da_na_rh_servant = 0;
                ACE_NEW_THROW_EX (da_na_rh_servant,
                                  DAnCE::DA_NA_ReplyHandlerImpl (this,
                                                                node_id.c_str (),
                                                                _counter_ptr),
                                  CORBA::NO_MEMORY ());

                PortableServer::ObjectId_var id = this->poa_->activate_object (da_na_rh_servant);

                CORBA::Object_var ref = this->poa_->id_to_reference (id.in());

                ::Deployment::AMI_ApplicationHandler_var da_na_handler =
                    ::Deployment::AMI_ApplicationHandler::_narrow (ref.in ());

                DANCE_DEBUG (6, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::start - ")
                                ACE_TEXT("Plan %C DomainApplication[%@] node %C : ")
                                ACE_TEXT("calling start on a node application\n"),
                                this->planUUID_.c_str (),
                                this,
                                node_id.c_str ()));

                (*iter).ext_id_->sendc_start (da_na_handler.in ());

                DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::start - ")
                              ACE_TEXT("start has been called on an application\n")));
              }
            catch (CORBA::Exception& ex)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT("DomainApplication_Impl::start - ")
                              ACE_TEXT("DomainApplication[%@] caught a CORBA exception handling node %C : %C\n"),
                              this,
                              node_id.c_str (),
                              ex._info ().c_str ()));
                // mark failure
                _counter_ptr->increment_fail_count ();
                // mark off node
                _counter_ptr->decrement_exec_count ();
                // continue for next node
              }
            catch (...)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                              ACE_TEXT("DomainApplication_Impl::start - ")
                              ACE_TEXT("DomainApplication[%@] caught a unknown exception handling node %C\n"),
                              this,
                              node_id.c_str ()));
                // mark failure
                _counter_ptr->increment_fail_count ();
                // mark off node
                _counter_ptr->decrement_exec_count ();
                // continue for next node
              }
          }
      }
    catch (CORBA::Exception& ex)
      {
        CORBA::Exception* local_ex = ex._tao_duplicate ();
        ::Deployment::AMH_ApplicationExceptionHolder amh_exholder (local_ex);
        _tao_rh->start_excep (&amh_exholder);
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                    ACE_TEXT("DomainApplication_Impl::start - ")
                    ACE_TEXT("Propagating StartError for unknown exception caught here\n")));
        CORBA::Exception* start_ex = new Deployment::StartError();
        ::Deployment::AMH_ApplicationExceptionHolder amh_exholder (start_ex);
        _tao_rh->start_excep (&amh_exholder);
      }
  }

  void
  DomainApplication_Impl::startLaunch (
    const Deployment::Properties & configProperty,
    DA_CompletionHandler* completion_handler)
  {
    DANCE_TRACE ("DomainApplication_Impl::startLaunch");

    DA_CompletionHandler_AutoPtr _ch_ptr (completion_handler); // manage handler

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                     ACE_TEXT("Plan %C DomainApplication[%@] : %u nodes to launch\n"),
                     this->planUUID_.c_str (),
                     this,
                     nams_.current_size ()));

    // create completion counter
    DA_NAM_ReplyHandlerImpl::Counter* _cp = 0;
    ACE_NEW_THROW_EX (_cp,
                      DA_NAM_ReplyHandlerImpl::Counter (nams_.current_size (),
                                                        this,
                                                        _ch_ptr),
                      CORBA::NO_MEMORY ());
    DA_NAM_ReplyHandlerImpl::Counter_AutoPtr _counter_ptr (_cp);

    for (TNam2Nm::iterator iter = this->nams_.begin();
         iter != this->nams_.end();
         ++iter)
      {
        ACE_CString node_id;
        this->node_ids_.find ((*iter).int_id_, node_id);

        try
          {
            // create and activate reply handler
            DA_NAM_ReplyHandlerImpl* da_nam_rh_servant = 0;
            ACE_NEW_THROW_EX (da_nam_rh_servant,
                              DA_NAM_ReplyHandlerImpl (this,
                                                      (*iter).ext_id_,
                                                      node_id.c_str (),
                                                      _counter_ptr),
                              CORBA::NO_MEMORY ());

            PortableServer::ObjectId_var id = this->poa_->activate_object (da_nam_rh_servant);

            CORBA::Object_var ref = this->poa_->id_to_reference (id.in());

            ::Deployment::AMI_NodeApplicationManagerHandler_var da_nam_handler =
                ::Deployment::AMI_NodeApplicationManagerHandler::_narrow (ref.in ());

            DANCE_DEBUG (6, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                            ACE_TEXT("Plan %C DomainApplication[%@] : ")
                            ACE_TEXT("calling startLaunch on node application manager for node %C\n"),
                            this->planUUID_.c_str (),
                            this,
                            node_id.c_str ()));

            (*iter).ext_id_->sendc_startLaunch (da_nam_handler.in (),
                                                configProperty);

            DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                          ACE_TEXT("startLaunch has been called on an application\n")));
          }
        catch (CORBA::Exception& ex)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                          ACE_TEXT("DomainApplication[%@] caught a CORBA exception handling node %C : %C\n"),
                          this,
                          node_id.c_str (),
                          ex._info ().c_str ()));
            // mark failure
            _counter_ptr->increment_fail_count ();
            // mark off node
            _counter_ptr->decrement_exec_count ();
            // continue for next node
          }
        catch (...)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DomainApplication_Impl::startLaunch - ")
                          ACE_TEXT("DomainApplication[%@] caught a unknown exception handling node %C\n"),
                          this,
                          node_id.c_str ()));
            // mark failure
            _counter_ptr->increment_fail_count ();
            // mark off node
            _counter_ptr->decrement_exec_count ();
            // continue for next node
          }
      }
  }

  void
  DomainApplication_Impl::destroyApplication (
    const DA_CompletionHandler_AutoPtr& _ch_ptr)
  {
    DANCE_TRACE ("DomainApplication_Impl::destroyApplication");

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::destroyApplication - ")
                     ACE_TEXT("Plan %C DomainApplication[%@] : %u node applications to destroy\n"),
                     this->planUUID_.c_str (),
                     this,
                     this->node_applications_.current_size ()));

    if (this->node_applications_.current_size () > 0)
      {
      // create completion counter
      DA_NAM_ReplyHandlerImpl::Counter* _cp = 0;
      ACE_NEW_THROW_EX (_cp,
                        DA_NAM_ReplyHandlerImpl::Counter (this->node_applications_.current_size (),
                                                          this,
                                                          _ch_ptr),
                        CORBA::NO_MEMORY ());
      DA_NAM_ReplyHandlerImpl::Counter_AutoPtr _counter_ptr (_cp);

      for (TApp2Mgr::iterator iter = this->node_applications_.begin();
            iter != this->node_applications_.end();
            ++iter)
        {
          ACE_CString node_id;
          this->app_node_ids_.find ((*iter).ext_id_, node_id);

          try
            {
              // create and activate reply handler
              DA_NAM_ReplyHandlerImpl* da_nam_rh_servant = 0;
              ACE_NEW_THROW_EX (da_nam_rh_servant,
                                DAnCE::DA_NAM_ReplyHandlerImpl (this,
                                                                (*iter).int_id_,
                                                                node_id.c_str (),
                                                                _counter_ptr),
                                CORBA::NO_MEMORY ());

              PortableServer::ObjectId_var id = this->poa_->activate_object (da_nam_rh_servant);

              CORBA::Object_var ref = this->poa_->id_to_reference (id.in());

              ::Deployment::AMI_NodeApplicationManagerHandler_var da_nam_handler =
                  ::Deployment::AMI_NodeApplicationManagerHandler::_narrow (ref.in ());

              DANCE_DEBUG (6, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::destroyApplication - ")
                              ACE_TEXT("Plan %C DomainApplication[%@] : ")
                              ACE_TEXT("calling destroyApplication on node application manager for node %C\n"),
                              this->planUUID_.c_str (),
                              this,
                              node_id.c_str ()));

              (*iter).int_id_->sendc_destroyApplication (da_nam_handler.in (),
                                                        (*iter).ext_id_.in());

              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::destroyApplication - ")
                            ACE_TEXT("destroyApplication has been called on node application manager\n")));
            }
          catch (CORBA::Exception& ex)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("DomainApplication_Impl::destroyApplication - ")
                            ACE_TEXT("DomainApplication[%@] caught a CORBA exception handling node %C : %C\n"),
                            this,
                            node_id.c_str (),
                            ex._info ().c_str ()));
              // mark failure
              _counter_ptr->increment_fail_count ();
              // mark off node
              _counter_ptr->decrement_exec_count ();
              // continue for next node
            }
          catch (...)
            {
              DANCE_ERROR (1, (LM_ERROR, DLINFO
                            ACE_TEXT("DomainApplication_Impl::destroyApplication - ")
                            ACE_TEXT("DomainApplication[%@] caught a unknown exception handling node %C\n"),
                            this,
                            node_id.c_str ()));
              // mark failure
              _counter_ptr->increment_fail_count ();
              // mark off node
              _counter_ptr->decrement_exec_count ();
              // continue for next node
            }
        }
      }
    else
      {
        // no node applications to clean up so finish immediately
        _ch_ptr->handle_completion (this);
      }

    DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DomainApplication_Impl::destroyApplication - finished\n")));
  }

  void
  DomainApplication_Impl::getConnections (Deployment::Connections& conn)
    {
      conn = this->connections_;
    }

  const char*
  DomainApplication_Impl::getPlanUUID (void)
    {
      return this->planUUID_.c_str ();
    }

  DA_NAM_ReplyHandlerImpl::Counter::Counter (
      unsigned int exec_count,
      DomainApplication_Impl* da_servant,
      const DA_CompletionHandler_AutoPtr& dam_completion_handler)
    : Completion_Counter_Base<ACE_Null_Mutex> (exec_count, 0),
      da_servant_ (DomainApplication_Impl_var::_duplicate (da_servant)),
      dam_ch_ptr_ (dam_completion_handler)
    {
    }

  void
  DA_NAM_ReplyHandlerImpl::Counter::on_all_completed ()
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                  ACE_TEXT("DA_NAM_ReplyHandlerImpl::Counter::on_all_completed - ")
                  ACE_TEXT("Plan %C DomainApplication[%@] successfully finished all nodes\n"),
                  this->da_servant_->getPlanUUID (),
                  this->da_servant_.in ()));

      this->dam_ch_ptr_->handle_completion (da_servant_.in ());
    }

  void
  DA_NAM_ReplyHandlerImpl::Counter::on_all_completed_with_failure ()
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("DA_NAM_ReplyHandlerImpl::Counter::on_all_completed_with_failure - ")
                  ACE_TEXT("Plan %C DomainApplication[%@] : %u errors from nodemanagers\n"),
                  this->da_servant_->getPlanUUID (),
                  this->da_servant_.in (),
                  this->fail_count ()));

      this->dam_ch_ptr_->handle_exception (new Deployment::StartError());
    }

  DA_NAM_ReplyHandlerImpl::DA_NAM_ReplyHandlerImpl (
      DomainApplication_Impl* da_servant,
      ::Deployment::NodeApplicationManager_ptr nam,
      const char* node_id,
      const Counter_AutoPtr& counter)
    : da_servant_ (DomainApplication_Impl_var::_duplicate (da_servant)),
      node_id_ (node_id),
      nam_ (::Deployment::NodeApplicationManager::_duplicate (nam)),
      counter_ (counter)
    {
    }

  void
  DA_NAM_ReplyHandlerImpl::startLaunch (
    ::Deployment::Application_ptr na,
    const ::Deployment::Connections & conn)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch called.\n")));

      if (CORBA::is_nil (na))
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch -")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C startLaunch returned nil object reference.\n"),
                        this->da_servant_->getPlanUUID (),
                        this->da_servant_.in (),
                        this->node_id_.c_str ()));

          this->counter_->increment_fail_count ();
        }
      else
        {
          DANCE_DEBUG (6, (LM_TRACE, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C startLaunch succeeded, ")
                        ACE_TEXT("received %u connections\n"),
                        this->da_servant_->getPlanUUID (),
                        this->da_servant_.in (),
                        this->node_id_.c_str (),
                        conn.length ()));

          this->da_servant_->node_applications_.bind (
              ::Deployment::Application::_duplicate (na),
              ::Deployment::NodeApplicationManager::_duplicate (this->nam_.in ()));
          this->da_servant_->app_node_ids_.bind (
              ::Deployment::Application::_duplicate (na),
              this->node_id_);

          CORBA::ULong const before = this->da_servant_->connections_.length();
          this->da_servant_->connections_.length (before + conn.length());
          for (CORBA::ULong i = 0; i < conn.length(); ++i)
            {
              this->da_servant_->connections_[before+i] = conn[i];
            }
        }

      // mark off one node
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }
  void
  DA_NAM_ReplyHandlerImpl::startLaunch_excep (
    ::Messaging::ExceptionHolder * excep_holder)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch_excep called\n")));

      try
        {
          excep_holder->raise_exception ();
        }
      catch (Deployment::StartError &)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch_excep - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C : StartError exception caught.\n"),
                        this->da_servant_->getPlanUUID (),
                        this->da_servant_.in (),
                        this->node_id_.c_str ()));
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch_excep - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught a CORBA exception: %C\n"),
                        this->da_servant_->getPlanUUID (),
                        this->da_servant_.in (),
                        this->node_id_.c_str (),
                        ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::startLaunch_excep - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught unknown exception.\n"),
                        this->da_servant_->getPlanUUID (),
                        this->da_servant_.in (),
                        this->node_id_.c_str ()));
        }

      // add failure
      this->counter_->increment_fail_count ();

      // mark off node
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }

  void
  DA_NAM_ReplyHandlerImpl::destroyApplication (void)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NAM_ReplyHandlerImpl::destroyApplication called.\n")));

      DANCE_DEBUG (9, (LM_TRACE, DLINFO
                    ACE_TEXT("DA_NAM_ReplyHandlerImpl::destroyApplication - ")
                    ACE_TEXT("Plan %C DomainApplication[%@] node %C : Node application destroy succeeded\n"),
                    this->da_servant_->getPlanUUID (),
                    this->da_servant_.in (),
                    this->node_id_.c_str ()));

      // mark off one application
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }
  void
  DA_NAM_ReplyHandlerImpl::destroyApplication_excep (
    ::Messaging::ExceptionHolder * excep_holder)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NAM_ReplyHandlerImpl::destroyApplication_excep called\n")));

      try
        {
          excep_holder->raise_exception ();
        }
      catch (Deployment::StopError &)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::destroyApplication_excep - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C : StopError exception caught.\n"),
                        this->da_servant_->planUUID_.c_str (),
                        this->da_servant_.in (),
                        this->node_id_.c_str ()));
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::destroyApplication_excep - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught a CORBA exception: %C\n"),
                        this->da_servant_->planUUID_.c_str (),
                        this->da_servant_.in (),
                        this->node_id_.c_str (),
                        ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                        ACE_TEXT("DA_NAM_ReplyHandlerImpl::destroyApplication_excep - ")
                        ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught unknown exception.\n"),
                        this->da_servant_->planUUID_.c_str (),
                        this->da_servant_.in (),
                        this->node_id_.c_str ()));
        }

      // add failure
      this->counter_->increment_fail_count ();

      // mark off application
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }

  DA_NA_ReplyHandlerImpl::Counter::Counter (
      unsigned int exec_count,
      DomainApplication_Impl* da_servant,
      ::Deployment::AMH_ApplicationResponseHandler_ptr _tao_rh)
    : Completion_Counter_Base<ACE_Null_Mutex> (exec_count, 0),
      da_servant_ (DomainApplication_Impl_var::_duplicate (da_servant)),
      da_rh_ (::Deployment::AMH_ApplicationResponseHandler::_duplicate (_tao_rh))
    {
    }

  void
  DA_NA_ReplyHandlerImpl::Counter::on_all_completed ()
    {
      DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                  ACE_TEXT("DA_NA_ReplyHandlerImpl::finishLaunch - ")
                  ACE_TEXT("Plan %C DomainApplication[%@] : Successfully finished all nodes\n"),
                  this->da_servant_->getPlanUUID (),
                  this->da_servant_.in ()));

      this->da_rh_->finishLaunch ();
    }

  void
  DA_NA_ReplyHandlerImpl::Counter::on_all_completed_with_failure ()
    {
      DANCE_ERROR (1, (LM_ERROR, DLINFO
                  ACE_TEXT("DA_NA_ReplyHandlerImpl::Counter::on_all_completed_with_failure - ")
                  ACE_TEXT("Plan %C DomainApplication[%@] : %u errors from node applications\n"),
                  this->da_servant_->getPlanUUID (),
                  this->da_servant_.in (),
                  this->fail_count ()));

      CORBA::Exception* start_ex = new Deployment::StartError();
      ::Deployment::AMH_ApplicationExceptionHolder amh_exholder (start_ex);
      this->da_rh_->finishLaunch_excep (&amh_exholder);
    }

  DA_NA_ReplyHandlerImpl::DA_NA_ReplyHandlerImpl (
      DomainApplication_Impl* da_servant,
      const char* node_id,
      const Counter_AutoPtr& counter)
    : da_servant_ (DomainApplication_Impl_var::_duplicate (da_servant)),
      node_id_ (node_id),
      counter_ (counter)
    {
    }

  void DA_NA_ReplyHandlerImpl::finishLaunch ()
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NA_ReplyHandlerImpl::finishLaunch - ")
                       ACE_TEXT("Plan %C DomainApplication[%@] node %C.\n"),
                       this->da_servant_->planUUID_.c_str (),
                       this->da_servant_.in (),
                       this->node_id_.c_str ()));

      // mark off one node
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }

  void DA_NA_ReplyHandlerImpl::finishLaunch_excep (
    ::Messaging::ExceptionHolder * excep_holder)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NA_ReplyHandlerImpl::finishLaunch_excep called\n")));

      try
        {
          excep_holder->raise_exception ();
        }
      catch (Deployment::StartError &)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DA_NA_ReplyHandlerImpl::finishLaunch_excep - ")
                          ACE_TEXT("Plan %C DomainApplication[%@] node %C : StartError exception caught.\n"),
                          this->da_servant_->planUUID_.c_str (),
                          this->da_servant_.in (),
                          this->node_id_.c_str ()));
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DA_NA_ReplyHandlerImpl::finishLaunch_excep - ")
                          ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught a CORBA exception: %C\n"),
                          this->da_servant_->planUUID_.c_str (),
                          this->da_servant_.in (),
                          this->node_id_.c_str (),
                          ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DA_NA_ReplyHandlerImpl::finishLaunch_excep - ")
                          ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught unknown exception.\n"),
                          this->da_servant_->planUUID_.c_str (),
                          this->da_servant_.in (),
                          this->node_id_.c_str ()));
        }

      // add failure
      this->counter_->increment_fail_count ();

      // mark off node
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }

  void DA_NA_ReplyHandlerImpl::start (void)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NA_ReplyHandlerImpl::start - ")
                       ACE_TEXT("Plan %C DomainApplication[%@] node %C.\n"),
                       this->da_servant_->planUUID_.c_str (),
                       this->da_servant_.in (),
                       this->node_id_.c_str ()));

      // mark off one node
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }

  void DA_NA_ReplyHandlerImpl::start_excep (
    ::Messaging::ExceptionHolder * excep_holder)
    {
      DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("DA_NA_ReplyHandlerImpl::start_excep called\n")));

      try
        {
          excep_holder->raise_exception ();
        }
      catch (Deployment::StartError &)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DA_NA_ReplyHandlerImpl::start_excep - ")
                          ACE_TEXT("Plan %C DomainApplication[%@] node %C : StartError exception caught.\n"),
                          this->da_servant_->planUUID_.c_str (),
                          this->da_servant_.in (),
                          this->node_id_.c_str ()));
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DA_NA_ReplyHandlerImpl::start_excep - ")
                          ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught a CORBA exception: %C\n"),
                          this->da_servant_->planUUID_.c_str (),
                          this->da_servant_.in (),
                          this->node_id_.c_str (),
                          ex._info ().c_str ()));
        }
      catch (...)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                          ACE_TEXT("DA_NA_ReplyHandlerImpl::start_excep - ")
                          ACE_TEXT("Plan %C DomainApplication[%@] node %C : Caught unknown exception.\n"),
                          this->da_servant_->planUUID_.c_str (),
                          this->da_servant_.in (),
                          this->node_id_.c_str ()));
        }

      // add failure
      this->counter_->increment_fail_count ();

      // mark off node
      this->counter_->decrement_exec_count ();

      // deactivate AMI response handler
      PortableServer::ObjectId_var oid = this->da_servant_->poa_->servant_to_id (this);
      this->da_servant_->poa_->deactivate_object (oid.in ());
    }

} // DAnCE

