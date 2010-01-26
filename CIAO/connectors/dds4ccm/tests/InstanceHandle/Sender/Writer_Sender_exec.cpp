// -*- C++ -*-
// $Id$

#include "Writer_Sender_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#include "Base/Writer_BaseSupport.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/InstanceHandle_t.h"

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
            ::DDS::InstanceHandle_t hnd = this->handles_[i->first.c_str ()];
            this->ccm_writer_->unregister_instance (i->second, hnd);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Unregistered <%C> - valid handle <%d>\n"),
                      i->first.c_str (),
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
        DDS::InstanceHandle_t hnd = this->ccm_writer_->register_instance (i->second);
        if (!hnd.isValid)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to register handle for <%C>\n"),
              i->first.c_str ()));
          }
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Registering instance for <%C>\n"),
                    i->second->key.in ()));
        this->handles_[i->first.c_str ()] = hnd;
     }
  }

  void
  Sender_exec_i::test_equality ()
  {
    if (this->last_key_ != this->ktests_.end ())
      {
        ::DDS::InstanceHandle_t ccm_hnd =
          this->handles_[this->last_key_->first.c_str ()];

        DDS_InstanceHandle_t dds_hnd =
          this->dds_writer_->lookup_instance (this->last_key_->second);
        if (dds_hnd == ccm_hnd)
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::test_equality - "
                          "== operator seems to work for DDS and "
                          "CCM handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::test_equality - "
                          "== operator doesn't seem to work for DDS and "
                          "CCM handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        DDS::InstanceHandle_t ccm_dds_hnd;
        ccm_dds_hnd <<= dds_hnd;
        if (ccm_hnd == ccm_dds_hnd)
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::test_equality - "
                          "== operator seems to work for CCM handles "
                          "for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::test_equality - "
                          "== operator doesn't seem to work for CCM "
                          "handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
      }
  }

  void
  Sender_exec_i::test_non_equality ()
  {
    if (this->last_key_ != this->ktests_.end ())
      {
        ::DDS::InstanceHandle_t ccm_hnd =
          this->handles_[this->last_key_->first.c_str ()];

        Writer_Table::iterator unequal = this->last_key_;
        ++unequal;
        if (unequal == this->ktests_.end ())
          unequal = this->ktests_.begin ();

        DDS_InstanceHandle_t dds_hnd =
          this->dds_writer_->lookup_instance (unequal->second);
        //compare
        if (dds_hnd != ccm_hnd)
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::test_equality - "
                          "!= operator seems to work for DDS and "
                          "CCM handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::test_equality - "
                          "!= operator doesn't seem to work for DDS and "
                          "CCM handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        DDS::InstanceHandle_t ccm_dds_hnd;
        ccm_dds_hnd <<= dds_hnd;
        if (ccm_dds_hnd != ccm_hnd)
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::test_equality - "
                          "!= operator seems to work for CCM "
                          "handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::test_equality - "
                          "!= operator doesn't seem to work for CCM "
                          "handles for key <%C>\n",
                          this->last_key_->first.c_str ()));
          }
        ++this->last_key_;
      }
    else
      {
        unregister_handles ();
        this->stop ();
      }
  }

  void
  Sender_exec_i::tick ()
  {
    test_equality ();
    test_non_equality ();
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
    DDS::DataWriter_var dds_dw =
      this->context_->get_connection_info_write_dds_entity ();
    CIAO::DDS4CCM::RTI::RTI_DataWriter_i *rti_rd =
      dynamic_cast <CIAO::DDS4CCM::RTI::RTI_DataWriter_i *> (dds_dw.in ());
    DDSDataWriter * p = rti_rd->get_impl ();
    this->dds_writer_ = dynamic_cast <WriterTestDataWriter *> (p);
    this->ccm_writer_ = this->context_->get_connection_info_write_data ();
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

        this->ktests_[key] = new_key;
      }
    this->last_key_ = this->ktests_.begin ();
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
