// -*- C++ -*-
// $Id$

#include "Writer_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Writer_Sender_Impl
{
  //============================================================
  // pulse_Generator
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
      keys_ (5),
      assignment_ (WRITE_KEYED),
      last_iteration_ (0)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::reset_iterations ()
  {
    for (Writer_Table::iterator i = this->ktests_.begin ();
         i != this->ktests_.end ();
         ++i)
      {
        i->second->iteration = 0;
      }
  }

  void
  Sender_exec_i::unregister_handles ()
  {
    for (Writer_Table::iterator i = this->ktests_.begin ();
         i != this->ktests_.end ();
         ++i)
      {
        try
          {
            ::DDS::InstanceHandle_t const hnd = this->handles_[i->first.c_str ()];
            this->writer_->unregister_instance (i->second, hnd);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Unregistered <%C> - iteration <%d> - valid handle <%d>\n"),
                      i->first.c_str (),
                      i->second->iteration,
                      hnd.isValid));
          }
        catch (...)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
                    ACE_TEXT ("unknown exception caught during unregister_instance.\n")));
          }
      }
  }

  void
  Sender_exec_i::register_handles()
  {
    Writer_Table::iterator i = this->ktests_.begin ();
    ::DDS::InstanceHandle_t hnd = this->writer_->register_instance (i->second);
    if (!hnd.isValid)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to register handle for <%C> - iteration <%d>\n"),
          i->first.c_str (), i->second->iteration));
      }
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Registering instance with <%C> - iteration <%d> - valid handle <%d>\n"),
                i->second->key.in (),
                i->second->iteration,
                hnd.isValid));
    this->handles_[i->first.c_str ()] = hnd;
    ++i;
    //test exception. In Qos, max_instances is set to 1
    //so only one instance may be registered.
    hnd = this->writer_->register_instance (i->second);
    if (hnd.isValid)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Shouldn't be able to register instance for <%C> - iteration <%d>\n"),
          i->first.c_str (), i->second->iteration));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expected : Registering second key not succeeded.\n"),
          i->first.c_str (), i->second->iteration));
      }
  }

  void
  Sender_exec_i::start_new_assignment (WRITER_ASSIGNMENT assignment)
  {
    this->last_key_ = this->ktests_.begin ();
    this->assignment_ = assignment;
    reset_iterations ();
  }

  void
  Sender_exec_i::write_keyed ()
  {
    if (this->last_key_ != this->ktests_.end ())
      {
        bool exception_caught = false;
        try
          {
            ++this->last_key_->second->iteration;
            ::DDS::InstanceHandle_t hnd = this->handles_[this->last_key_->first.c_str ()];
            this->writer_->write_one (this->last_key_->second, hnd);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written keyed <%C> - iteration <%d> - valid handle <%d>\n"),
                    this->last_key_->first.c_str (),
                    this->last_key_->second->iteration,
                    hnd.isValid));
          }
        catch (const CCM_DDS::InternalError& )
          {
            exception_caught = true;
            if (this->last_key_ == this->ktests_.begin ())
              { // the first key should throw this exception; all others
                // shouldn't
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                            ACE_TEXT ("while updating writer info for <%C>.\n"),
                              this->last_key_->first.c_str ()));
              }
          }
        //only the first iterations are registered.
        if (this->last_key_ != this->ktests_.begin () && !exception_caught)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: No exception caught ")
                    ACE_TEXT ("while writing unregistered data\n")));
          }
        ++this->last_key_;
      }
    else
      {
        //onto the next iteration
        this->last_key_ = this->ktests_.begin ();
        while (this->last_key_ != this->ktests_.end ())
          {
            if (this->last_key_->second->iteration == this->iterations_)
              {
                //next key
                ++this->last_key_;
              }
            else
              {
                break;
              }
          }
        if (this->last_key_ == this->ktests_.end ())
          {
            unregister_handles ();
            start_new_assignment (WRITE_MULTI);
          }
      }
  }

  void
  Sender_exec_i::write_many ()
  {
    bool expected_exception_thrown = false;

    WriterTestSeq write_many_seq;
    write_many_seq.length (this->keys_ * this->iterations_);
    ::CORBA::ULong iter_key = 0;
    for (Writer_Table::iterator iter = this->ktests_.begin ();
         iter != this->ktests_.end ();
         ++iter)
      {
        ++iter_key;
        for (int i = 1; i < this->iterations_ + 1; ++i)
          {
            char key[7];
            WriterTest new_key;
            ACE_OS::sprintf (key, "KEY_%d", iter_key);
            new_key.key = CORBA::string_dup(key);
            new_key.iteration = i;
            write_many_seq[iter_key + i - 2] = new_key;
          }
      }
    try
      {
        this->writer_->write_many (write_many_seq);
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        if (ex.index == 0)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal excep ")
                        ACE_TEXT ("while write many for first instance : index <%d> - retval <%d>\n"),
                          ex.index, ex.error_code));
          }
        else
          {
            expected_exception_thrown = true;
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Expected Internal excep ")
                    ACE_TEXT ("while write many writer info: index <%d> - retval <%d>\n"),
                      ex.index, ex.error_code));
          }
      }
    if (!expected_exception_thrown)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Expected Internal excep ")
                ACE_TEXT ("was not thrown\n")));
      }

    this->assignment_ = WRITE_NONE;
  }

  void
  Sender_exec_i::tick ()
  {
    switch (this->assignment_)
      {
        case WRITE_KEYED:
          write_keyed ();
          break;
        case WRITE_MULTI:
          write_many ();
        default:
          break;
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
  }

  ::CORBA::UShort
  Sender_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Sender_exec_i::rate (::CORBA::UShort rate)
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
    this->iterations_ = iterations + 2; //for extra tests.
  }

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Sender_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys + 1; //for extra tests.
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Writer::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->writer_ = this->context_->get_connection_info_write_data ();
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
        WriterTest *new_key = new WriterTest;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);
        new_key->iteration = 1;

        this->ktests_[key] = new_key;
      }
    this->last_key_ = this->ktests_.begin ();
    register_handles ();
    reset_iterations ();
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
  create_Writer_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
