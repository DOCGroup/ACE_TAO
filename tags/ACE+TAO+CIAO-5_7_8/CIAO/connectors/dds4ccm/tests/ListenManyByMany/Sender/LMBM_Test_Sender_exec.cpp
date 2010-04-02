// -*- C++ -*-
// $Id$

#include "LMBM_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Date_Time.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_LMBM_Test_Sender_Impl
{
  //============================================================
  // WriteManyHandler
  //============================================================
  WriteTicker::WriteTicker (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteTicker::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->callback_.write_one ();
    return 0;
  }

  //============================================================
  // WriteManyHandler
  //============================================================
  WriteManyHandler::WriteManyHandler (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteManyHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.write_one ();
    return 0;
  }


  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (10),
      keys_ (5)
  {
    this->ticker_ = new WriteTicker (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::write_one (void)
  {
    if (this->last_key_ != this->samples_.end ())
      {
        try
          {
            ++this->last_key_->second->iteration;
            this->writer_->write_one (this->last_key_->second, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, "Written key <%C> - <%u>\n",
                          this->last_key_->first.c_str (),
                          this->last_key_->second->iteration));
          }
        catch (const CCM_DDS::InternalError& )
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                        ACE_TEXT ("while updating writer info for <%C>.\n"),
                          this->last_key_->first.c_str ()));
          }
        ++this->last_key_;
      }
    else
      {
        //onto the next iteration
        this->last_key_ = this->samples_.begin ();
        while (this->last_key_ != this->samples_.end ())
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
        if (this->last_key_ == this->samples_.end ())
          {
            this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
          }
      }
  }


  void
  Sender_exec_i::start (void)
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

    for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
      {
        char key[7];
        ListenManyByManyTest *new_key = new ListenManyByManyTest;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);
        new_key->iteration = 0;

        this->samples_[key] = new_key;
      }
   this->last_key_ = this->samples_.begin ();
   if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (5, 50000),
                ACE_Time_Value (0, 50000)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
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
      ::LMBM_Test::CCM_Sender_Context::_narrow (ctx);

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
    try
      {
        this->writer_ = this->context_->get_connection_info_write_data ();
        start ();
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Exception caught\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Unknown exception caught\n")));
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LMBM_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
