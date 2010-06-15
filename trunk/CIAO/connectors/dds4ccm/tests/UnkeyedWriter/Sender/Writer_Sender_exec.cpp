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
      iterations_ (15),
      keys_ (5),
      assignment_ (WRITE_UNKEYED)
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
            ::Writer::WriterTestConnector::Writer_var writer =
              this->context_->get_connection_info_write_data ();

            ++this->last_key->second->iteration;
            OctetSeq_var reply_mesg = new OctetSeq (1);
            reply_mesg->length (1);
            this->last_key->second->data = *reply_mesg._retn ();
            writer->write_one (this->last_key->second, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written keyed <%C> - iteration <%d>\n"),
                    this->last_key->first.c_str (),
                    this->last_key->second->iteration));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
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
            start_new_assignment (WRITE_MULTI);
          }
      }
  }

  void
  Sender_exec_i::write_many ()
  {
    CORBA::ULong nr_samples = this->keys_ * this->iterations_;
    WriterTestSeq write_many_no_excep;
    write_many_no_excep.length (nr_samples);
    //write with no exception
    for (CORBA::ULong i = 1; i < nr_samples - 1; ++i)
      {
        WriterTest new_key;
        new_key.key = CORBA::string_dup("KEY_1");
        new_key.iteration = i;
        OctetSeq_var reply_mesg = new OctetSeq (1);
        reply_mesg->length (1);
        new_key.data = *reply_mesg._retn ();
        write_many_no_excep[i-1] = new_key;
      }
    try
      {
        ::Writer::WriterTestConnector::Writer_var writer =
          this->context_->get_connection_info_write_data ();

        writer->write_many (write_many_no_excep);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("write_many : written <%u> samples\n"),
              write_many_no_excep.length ()));
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                    ACE_TEXT ("while write many writer info: index <%d> - retval <%d>\n"),
                      ex.index, ex.error_code));
      }
    start_new_assignment (TEST_EXCEPTION);
  }

  void
  Sender_exec_i::test_exception ()
  {
    if (this->last_key != this->ktests_.end ())
      {
        try
          {
            ::Writer::WriterTestConnector::Writer_var writer =
              this->context_->get_connection_info_write_data ();

            ++this->last_key->second->iteration;
            long const length = 100000;
            OctetSeq_var reply_mesg = new OctetSeq (length);
            reply_mesg->length (length);
            this->last_key->second->data = *reply_mesg._retn ();
            writer->write_one (this->last_key->second, ::DDS::HANDLE_NIL);
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: No InternalError ")
                        ACE_TEXT ("caught while writing a large amount of data.\n")));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::test_exception: ")
                        ACE_TEXT ("OK: Expected InternalErr received\n")));
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
            start_new_assignment (WRITE_NONE);
          }
      }
  }

  void
  Sender_exec_i::tick ()
  {
    switch (this->assignment_)
      {
        case WRITE_UNKEYED:
          write_unkeyed ();
          break;
        case WRITE_MULTI:
          write_many ();
          break;
        case TEST_EXCEPTION:
          test_exception ();
          break;
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
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
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
   this->start ();
   this->last_key = this->ktests_.begin ();
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

