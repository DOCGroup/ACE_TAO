// -*- C++ -*-
// $Id$

#include "Writer_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

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
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : rate_ (1)
      , iterations_ (10)
      , keys_ (5)
      , assignment_ (WRITE_KEYED)
      , last_iteration_ (0)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->ticker_;
  }

  // Supported operations and attributes.

  ACE_Reactor*
  Sender_exec_i::reactor (void)
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

  ::CORBA::UShort
  Sender_exec_i::rate (void)
  {
    return
      this->rate_;
  }

  void
  Sender_exec_i::rate (
    const ::CORBA::UShort rate)
  {
    this->rate_ =
      rate;
  }

  ::CORBA::UShort
  Sender_exec_i::iterations (void)
  {
    return
      this->iterations_;
  }

  void
  Sender_exec_i::iterations (
    const ::CORBA::UShort iterations)
  {
    this->iterations_ = iterations + 2; //for extra tests.
  }

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return
      this->keys_;
  }

  void
  Sender_exec_i::keys (
    const ::CORBA::UShort keys)
  {
    this->keys_ = keys + 1; //for extra tests.
  }

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Writer::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
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
            WriterTestConnector::Writer_var writer =
              this->ciao_context_->get_connection_info_write_data ();
            writer->unregister_instance (i->second, hnd);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Unregistered <%C> - iteration <%d> - ")
                      DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ACE_TEXT("\n"),
                      i->first.c_str (),
                      i->second->iteration,
                      DDS_INSTANCE_HANDLE_LOG(hnd)));
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
    WriterTestConnector::Writer_var writer =
      this->ciao_context_->get_connection_info_write_data ();

    Writer_Table::iterator i = this->ktests_.begin ();
    ::DDS::InstanceHandle_t hnd = writer->register_instance (i->second);
    if (hnd == ::DDS::HANDLE_NIL)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to register handle for <%C> - iteration <%d>\n"),
          i->first.c_str (), i->second->iteration));
      }
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Registering instance with <%C> - iteration <%d> - ")
                DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ACE_TEXT("\n"),
                i->second->key.in (),
                i->second->iteration,
                DDS_INSTANCE_HANDLE_LOG(hnd)));
    this->handles_[i->first.c_str ()] = hnd;
    ++i;
    // Test exception. In Qos, max_instances is set to 1
    // so only one instance may be registered.
    hnd = writer->register_instance (i->second);
    if (hnd != ::DDS::HANDLE_NIL)
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
    WriterTestConnector::Writer_var writer =
      this->ciao_context_->get_connection_info_write_data ();

    if (this->last_key_ != this->ktests_.end ())
      {
        bool exception_caught = false;
        try
          {
            ++this->last_key_->second->iteration;
            ::DDS::InstanceHandle_t hnd = this->handles_[this->last_key_->first.c_str ()];
            writer->write_one (this->last_key_->second, hnd);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written keyed <%C> - iteration <%d> - ")
                    DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ACE_TEXT ("\n"),
                    this->last_key_->first.c_str (),
                    this->last_key_->second->iteration,
                    DDS_INSTANCE_HANDLE_LOG(hnd)));
          }
        catch (const CCM_DDS::InternalError& )
          {
            exception_caught = true;
            if (this->last_key_ == this->ktests_.begin ())
              {
                // The first key should throw this exception; all others
                // shouldn't
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                            ACE_TEXT ("while updating writer info for <%C>.\n"),
                              this->last_key_->first.c_str ()));
              }
          }
        // Only the first iterations are registered.
        if (this->last_key_ != this->ktests_.begin () && !exception_caught)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: No exception caught ")
                    ACE_TEXT ("while writing unregistered data\n")));
          }
        ++this->last_key_;
      }
    else
      {
        // Onto the next iteration
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
    WriterTestConnector::Writer_var writer =
      this->ciao_context_->get_connection_info_write_data ();

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
        writer->write_many (write_many_seq);
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
    long const usec = 1000000 / this->rate_;
    if (this->reactor ()->schedule_timer (
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
    this->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
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
