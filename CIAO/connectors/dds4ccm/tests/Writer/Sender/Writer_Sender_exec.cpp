// -*- C++ -*-
// $Id$

#include "Writer_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Writer_Sender_Impl
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
      keys_ (5),
      assignment_ (WRITE_UNKEYED),
      last_iteration_ (0)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
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
  Sender_exec_i::create_handles()
  {
    for (Writer_Table::iterator i = this->ktests_.begin ();
         i != this->ktests_.end ();
         ++i)
      {
        ::DDS::InstanceHandle_t hnd = this->writer_->register_instance (i->second);
/*        if (hnd == ::DDS::HANDLE_NIL)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to register handle for %C"),
              i->first.c_str ()));
          }*/
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Registring instance with %C and %d : Valid <%d>\n"),
                    i->second->key.in (),
                    i->second->iteration,
                    hnd.isValid));
        this->handles_[i->first.c_str ()] = hnd;
      }
  }

  void
  Sender_exec_i::start_new_assignment (WRITER_ASSIGNMENT assignment)
  {
    this->last_key = this->ktests_.begin ();
    this->assignment_ = assignment;
    reset_iterations ();
  }

  void
  Sender_exec_i::write_unkeyed ()
  {
    if (this->last_key != this->ktests_.end ())
      {
        try
          {
            ++this->last_key->second->iteration;
            this->writer_->write_one (this->last_key->second, ::DDS::HANDLE_NIL);
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Written unkeyed <%C> with <%d>\n"),
                    this->last_key->first.c_str (),
                    this->last_key->second->iteration));
          }
        catch (CCM_DDS::InternalError& )
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unexpected exception ")
                        ACE_TEXT ("while updating writer info for <%s>.\n"),
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
                //next key
                ++this->last_key;
              }
            else
              {
                break;
              }
          }
        if (this->last_key == this->ktests_.end ())
          {
            start_new_assignment (WRITE_KEYED);
          }
      }
  }
  
  void
  Sender_exec_i::write_keyed ()
  {
    if (this->last_key != this->ktests_.end ())
      {
        bool exception_caught = false;
        try
          {
            ++this->last_key->second->iteration;
            ::DDS::InstanceHandle_t hnd = this->handles_[this->last_key->first.c_str ()];
            this->writer_->write_one (this->last_key->second, hnd);
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Written keyed <%C> with <%d> and handle <%d>\n"),
                    this->last_key->first.c_str (),
                    this->last_key->second->iteration,
                    hnd));
          }
        catch (CCM_DDS::InternalError& )
          {
            exception_caught = true;
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error ")
                        ACE_TEXT ("while updating writer info for <%s>.\n"),
                          this->last_key->first.c_str ()));
          }
        //only the first iterations are registered.
        if (this->last_key->second->iteration != 1 && !exception_caught)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: No exception caught ")
                    ACE_TEXT ("while writing unregistered data\n")));
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
                //next key
                ++this->last_key;
              }
            else
              {
                break;
              }
          }
        if (this->last_key == this->ktests_.end ())
          {
            start_new_assignment (WRITE_MULTI);
          }
      }
  }
  
  void
  Sender_exec_i::write_many ()
  {
    WriterTest_Seq write_many_seq;// = new WriterTest_Seq ();
    write_many_seq.length (this->keys_ * this->iterations_);
    int iter_key = 0;
    for (Writer_Table::iterator iter = this->ktests_.begin ();
         iter != this->ktests_.end ();
         ++iter)
      {
        ++iter_key;
        for (int i = 1; i < this->iterations_ + 1; ++i)
          {
            char key[7];
            WriterTest new_key;// = new WriterTest;
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
    catch (CCM_DDS::InternalError& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error ")
                    ACE_TEXT ("while write many writer info.\n")));
      }
    this->assignment_ = WRITE_NONE;
  }

  void
  Sender_exec_i::tick ()
  {
    switch (this->assignment_)
      {
        case WRITE_UNKEYED:
          write_unkeyed ();
          break;
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
    this->last_key = this->ktests_.begin ();
    create_handles ();
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

