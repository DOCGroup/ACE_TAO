// -*- C++ -*-
// $Id$

#include "Keyed_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
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
  // Component Executor Implementation Class: Sender_exec_i
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

  // Supported operations and attributes.

  void
  Sender_exec_i::tick ()
  {
    if (this->last_key->second->iteration == 0)
      {
        try
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Create key <%C>\n"),
                    this->last_key->first.c_str ()));
            this->updater_->create_one (this->last_key->second);
          }
        catch (CCM_DDS::AlreadyCreated& )
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("keyedtest_info for <%C> already created.\n"),
                        this->last_key->first.c_str ()));
          }
        catch (CCM_DDS::InternalError& )
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while creating keyedtest_info for <%C>.\n"),
                        this->last_key->first.c_str ()));
          }
      }
    if (this->last_key != this->ktests_.end ())
      {
        try
          {
            ++this->last_key->second->iteration;
            this->updater_->update_one (this->last_key->second, ::DDS::HANDLE_NIL);
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Updated key <%C> with <%d>\n"),
                    this->last_key->first.c_str (),
                    this->last_key->second->iteration));
          }
        catch (CCM_DDS::NonExistent& )
          {
            printf ("Stock_info for <%s> not updated: <%s> didn't exist.\n",
                        this->last_key->first.c_str (), this->last_key->first.c_str ());
          }
        catch (CCM_DDS::InternalError& )
          {
            printf ("Internal Error while updating Stock_info for <%s>.\n",
                        this->last_key->first.c_str ());
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
                //we're done with this one -> unregister it.
                try
                  {
                    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Deleting key <%C> with <%d>\n"),
                            this->last_key->first.c_str (),
                            this->last_key->second->iteration));
                    this->updater_->delete_one (this->last_key->second, ::DDS::HANDLE_NIL);
                  }
                catch (CCM_DDS::NonExistent& )
                  {
                    CIAO_ERROR ((LM_ERROR,
                                ACE_TEXT ("keyedtest_info for <%C> not deleted: <%C> didn't exist.\n"),
                                this->last_key->first.c_str (), this->last_key->first.c_str ()));
                  }
                catch (CCM_DDS::InternalError& )
                  {
                    CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while deleting keyedtest_info for <%C>.\n"),
                                this->last_key->first.c_str ()));
                  }
                //next key
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
    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (0, usec),
                ACE_Time_Value (0, usec)) == -1)
    {
      CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                             ACE_TEXT ("Error scheduling timer")));
    }
  }

  void
  Sender_exec_i::stop (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
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
    this->updater_ = this->context_->get_connection_info_update_data ();
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
  create_Shape_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

