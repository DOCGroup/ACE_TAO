// -*- C++ -*-
// $Id$

#include "ConnectorStatusListener_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_ConnectorStatusListener_Test_Sender_Impl
{

//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (void)
  {
  }
  
  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
    
  }
  
  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr the_topic, 
     const DDS::InconsistentTopicStatus & status)
    {
      printf("ConnectorStatusListener_exec_i::on_inconsistent_topic\n");
    }
  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr the_reader,
     const DDS::RequestedIncompatibleQosStatus & status)  {
      printf("ConnectorStatusListener_exec_i::on_requested_incompatible_qos\n");
    }
  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr the_reader, 
     const DDS::SampleRejectedStatus & status)  {
      printf("ConnectorStatusListener_exec_i::on_sample_rejected\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr the_writer,
     const DDS::OfferedDeadlineMissedStatus & status)  {
      printf("ConnectorStatusListener_exec_i::on_offered_deadline_missed\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr the_writer, 
     const DDS::OfferedIncompatibleQosStatus & status)  {
      printf("ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n");
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind  status_kind)  {
      printf("ConnectorStatusListener_exec_i::on_unexpected_status\n");
    }
 
::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_info_out_connector_status (void)
  {
    printf ("*************** out connector status************************\n");
     return new ConnectorStatusListener_exec_i ();
  }
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
  //  this->pulse_callback_.tick ();
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
  /*  if (this->last_key->second->iteration == 0)
      {
        try
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Create key <%C>\n"),
                    this->last_key->first.c_str ()));
            printf ("@@!!@#!@\n");
            this->updater_->create (this->last_key->second);
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
             printf ("1111111111@@!!@#!@\n");
            ++this->last_key->second->iteration;
            this->updater_->update (this->last_key->second);
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
         printf ("1a1a1a1a1a1a1a1a1a1last_key = %d   @@!!@#!@\n", this->last_key);
      }
    else
      {
         printf ("222222222222222222@@!!@#!@\n");
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
                    this->updater_->_cxx_delete (this->last_key->second);
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
      */
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
      ::ConnectorStatusListener_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
   // this->updater_ = this->context_->get_connection_info_update_data ();
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
        TestTopic *new_key = new TestTopic;
        ACE_OS::sprintf (key, "KEY_%d", i);
         printf ("key = %s 1111111111@@!!@#!@\n",key);
        new_key->key = CORBA::string_dup(key);
    //    new_key->iteration = 0;

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
  create_ConnectorStatusListener_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}

