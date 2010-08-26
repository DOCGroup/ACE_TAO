// -*- C++ -*-
// $Id$

#include "Writer_Sender_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#include "Base/Writer_BaseSupport.h"
#include "Connector/Writer_Connector_conn.h"
#include "dds4ccm/impl/Utils.h"

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
      keys_ (5)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
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
            DDS_InstanceHandle_t hnd = this->handles_[i->first.c_str ()];
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
    for (i = this->ktests_.begin(); i != this->ktests_.end(); ++i)
      {
        DDS_InstanceHandle_t hnd = this->writer_->register_instance (i->second);
        if (!hnd.isValid)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to register handle for <%C> - iteration <%d>\n"),
              i->first.c_str (), i->second->iteration));
          }
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Registering instance for <%C>\n"),
                    i->second->key.in ()));
        this->handles_[i->first.c_str ()] = hnd;
     }
  }

  void
  Sender_exec_i::write_keyed ()
  {
    if (this->last_key != this->ktests_.end ())
      {
        ++this->last_key->second->iteration;
        DDS_InstanceHandle_t hnd = this->handles_[this->last_key->first.c_str ()];
        DDS_ReturnCode_t const retval = this->writer_->write (this->last_key->second, hnd);
        if (retval == DDS_RETCODE_OK)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written keyed <%C> - iteration <%d> - valid handle <%d>\n"),
                    this->last_key->first.c_str (),
                    this->last_key->second->iteration,
                    hnd.isValid));
          }
        else ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error writing key <%C> - retcode %C\n"),
                    CIAO::DDS4CCM::translate_retcode (retval)));

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
            unregister_handles ();
            this->stop ();
          }
      }
  }

  void
  Sender_exec_i::tick ()
  {
    write_keyed ();
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
    if (this->ticker_)
      {
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
        delete this->ticker_;
        this->ticker_ = 0;
      }
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
    ::DDS::DataWriter_var dds_dw =
      this->context_->get_connection_info_write_dds_entity ();

    if (::CORBA::is_nil (dds_dw.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                    "Datawriter connection is NIL.\n"));
        throw ::CORBA::INTERNAL ();
      }

    typedef ::CIAO::DDS4CCM::CCM_DDS_DataWriter_Base DataWriter_type;

    DataWriter_type * typed_ccm_dw = dynamic_cast <DataWriter_type*> (dds_dw.in ());
    if (typed_ccm_dw)
      {
        DDSDataWriter* dds_datawriter = typed_ccm_dw->get_impl ();
        if (dds_datawriter)
          {
            this->writer_ = WriterTestDataWriter::narrow (dds_datawriter);
            if (!this->writer_)
              {
                ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                            "Error narrowing to a typed "
                            "DDS DataWriter.\n"));
                throw ::CORBA::INTERNAL ();
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                        "Error getting DDS DataWriter.\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Sender_exec_i::ccm_activate - "
                    "Error casting DataWriter to typed DataWriter\n"));
        throw ::CORBA::INTERNAL ();
      }

    this->start ();

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, _guard,
                        this->mutex_, CORBA::INTERNAL ());

    for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
      {
        char key[7];
        WriterTest *new_key = new WriterTest;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key->key = CORBA::string_dup(key);
        new_key->iteration = 0;

        this->ktests_[key] = new_key;
      }
    this->last_key = this->ktests_.begin ();
    register_handles ();
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
