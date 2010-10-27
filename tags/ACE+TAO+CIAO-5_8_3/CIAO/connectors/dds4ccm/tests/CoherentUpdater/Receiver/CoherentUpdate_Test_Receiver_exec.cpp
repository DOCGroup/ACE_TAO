// -*- C++ -*-
// $Id$

#include "CoherentUpdate_Test_Receiver_exec.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

namespace CIAO_CoherentUpdate_Test_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback,
                                                int run)
    : callback_ (callback),
      run_ (run)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    ACE_DEBUG ((LM_DEBUG, "Checking if last sample "
                          "is available in DDS...\n"));
    if (this->callback_.check_last ())
      this->callback_.run (this->run_);
    return 0;
  }

  //============================================================
  // Facet Executor Implementation Class: reader_start_exec_i
  //============================================================

  reader_start_exec_i::reader_start_exec_i (
        ::CoherentUpdate_Test::CCM_Receiver_Context_ptr ctx,
        Receiver_exec_i & callback)
    : ciao_context_ (
        ::CoherentUpdate_Test::CCM_Receiver_Context::_duplicate (ctx))
      , callback_ (callback)
  {
  }

  reader_start_exec_i::~reader_start_exec_i (void)
  {
  }

  // Operations from ::CoherentUpdateStarter

  void
  reader_start_exec_i::set_reader_properties (
    ::CORBA::UShort nr_iterations)
  {
    this->callback_.iterations (nr_iterations);
  }

  void
  reader_start_exec_i::start_read (
    ::CORBA::UShort run)
  {
    this->callback_.start_read (run);
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : nr_runs_ (5),
      iterations_ (10),
      run_ (0),
      last_iter_ (0),
      ticker_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  bool
  Receiver_exec_i::check_last ()
  {
    try
      {
        ::CoherentUpdate_Test::Reader_var reader =
          this->ciao_context_->get_connection_info_out_data ();
        if (::CORBA::is_nil (reader.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_last - "
                                  "Unable to retrieve reader from context.\n"));
            return false;
          }
        CoherentUpdateTest coherentwrite_info;
        ::CCM_DDS::ReadInfo readinfo;
        coherentwrite_info.symbol = CORBA::string_dup ("KEY_1");
        reader->read_one_last (
                coherentwrite_info,
                readinfo,
                ::DDS::HANDLE_NIL);
                ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_last - "
                              "last iteration <%d> - <%d>\n",
                               coherentwrite_info.iteration,
                               (this->run_ + 1) * this->iterations_ - 1));
        return coherentwrite_info.iteration >= (this->run_ + 1) * this->iterations_ - 1;
      }
    catch (...)
      {
        // No need to catch. An error is given
        // when this example didn't run at all.
        ACE_DEBUG ((LM_INFO, "CRASH !\n"));
      }
    return false;
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::read_all (::CoherentUpdate_Test::Reader_ptr reader)
  {
    try
      {
        CoherentUpdateTestSeq coherentwrite_info_seq;
        ::CCM_DDS::ReadInfoSeq readinfo_seq;
        reader->read_all (coherentwrite_info_seq, readinfo_seq);

        for (CORBA::ULong it = 0; it < coherentwrite_info_seq.length (); ++it)
          {
            if (coherentwrite_info_seq[it].iteration > this->last_iter_)
              {
                if (coherentwrite_info_seq[it].iteration == ++this->last_iter_)
                  {
                    ACE_DEBUG ((LM_DEBUG, "OK: "));
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR: "));
                  }
                ACE_DEBUG ((LM_DEBUG, "expected: <%u> - "
                                      "received <%d>\n",
                                      this->last_iter_,
                                      coherentwrite_info_seq[it].iteration));
               }
          }
        if (this->run_ < this->nr_runs () + 1)
          {
             CoherentUpdateRestarter_var restarter =
              this->ciao_context_->get_connection_updater_restart ();
            if (! ::CORBA::is_nil (restarter))
              {
                restarter->restart_update ();
              }
            else
              {
                ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::read_all -"
                                      "Unable to restart.\n"));
              }
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "Finished: wait for shutdown\n"));
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ ALL: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::start_read (CORBA::UShort run)
  {
    ACE_NEW_THROW_EX (this->ticker_,
                      read_action_Generator (*this, run),
                      ::CORBA::INTERNAL ());
    if (this->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(1, 0),
                                          ACE_Time_Value(1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  void
  Receiver_exec_i::run (CORBA::UShort run)
  {
    if (this->ticker_)
      {
        this->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
      }
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::run - "
                          "Starting run number <%d>\n",
                          run));
    this->run_ = run;
    ::CoherentUpdate_Test::Reader_var reader =
      this->ciao_context_->get_connection_info_out_data ();
    if (! ::CORBA::is_nil (reader.in ()))
      {
        this->read_all (reader);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::run - "
                              "Unable to start: reader is nil.\n"));
      }
  }

  // Supported operations and attributes.

  ACE_Reactor*
  Receiver_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->ciao_context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  // Component attributes and port operations.

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_CoherentUpdateStarter_ptr
  Receiver_exec_i::get_reader_start (void)
  {
    if ( ::CORBA::is_nil (this->ciao_reader_start_.in ()))
      {
        reader_start_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          reader_start_exec_i (
            this->ciao_context_.in (),
            *this),
          ::CCM_CoherentUpdateStarter::_nil ());

        this->ciao_reader_start_ = tmp;
      }

    return
      ::CCM_CoherentUpdateStarter::_duplicate (
        this->ciao_reader_start_.in ());
  }

  ::CORBA::UShort
  Receiver_exec_i::nr_runs (void)
  {
    return
      this->nr_runs_;
  }

  void
  Receiver_exec_i::nr_runs (
    const ::CORBA::UShort nr_runs)
  {
    this->nr_runs_ =
      nr_runs;
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  // Operations from Components::SessionComponent.

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::CoherentUpdate_Test::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (this->ticker_)
      {
        this->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
      }
    if (this->run_ == 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
            ACE_TEXT ("Test did not run")
            ACE_TEXT ("events.\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished coherent update test.\n")));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentUpdate_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();
    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);
    return retval;
  }
}
