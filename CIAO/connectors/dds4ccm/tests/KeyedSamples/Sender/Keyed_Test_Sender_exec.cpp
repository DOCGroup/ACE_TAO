// -*- C++ -*-
// $Id$

#include "Keyed_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Keyed_Test_Sender_Impl
{
  //============================================================
  // Pulse generator
  //============================================================
  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }

  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : rate_ (1),
      iterations_ (10),
      keys_ (5)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::tick ()
  {
    if (this->last_key != this->ktests_.end ())
      {
        try
          {
            KeyedTestConnector::Writer_var writer =
              this->context_->get_connection_info_write_data ()
            ++this->last_key->second->iteration;
            writer->write_one (this->last_key->second, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written key <%C> with <%d>\n"),
                    this->last_key->first.c_str (),
                    this->last_key->second->iteration));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Internal Error while writing KeyedTest info for <%C>.\n",
                        this->last_key->first.c_str ()));
          }
        ++this->last_key;
      }
    else
      {
        //onto the next iteration
        this->last_key = this->ktests_.begin ();
        while (this->last_key != this->ktests_.end ())
          {
            if (this->last_key->second->iteration == this->iterations_)
              {
                ++this->last_key;
              }
            else
              {
                break;
              }
          }
      }
  }

  void
  Sender_exec_i::start (void)
  {
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
    this->ticker_ = 0;
  }

  ::CORBA::ULong
  Sender_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Sender_exec_i::rate (::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::UShort
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Sender_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Sender_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Keyed_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->start ();

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

    for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
      {
        char key[7];
        KeyedTest *new_key = new KeyedTest;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);
        new_key->iteration = 0;

        this->ktests_[key] = new_key;
      }
     this->last_key = this->ktests_.begin ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Keyed_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
