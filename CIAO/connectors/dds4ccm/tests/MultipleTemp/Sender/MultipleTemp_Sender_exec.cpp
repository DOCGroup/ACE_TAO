// -*- C++ -*-
// $Id$

//Test Multiple templates connector
#include "MultipleTemp_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_MultipleTemp_Sender_Impl
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
  : test_nr_(UPDATE_CREATE),
    test_ok_(true)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }
  ACE_Reactor*
  Sender_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->context_->get_CCM_object();
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
  // Supported operations and attributes.
  CORBA::Boolean
  Sender_exec_i::create_one_topic()
  {
    CORBA::Boolean result = true;
    TestTopicOne i = this->topic_one_seq_one_[0];
    TestTopicTwo u = this->topic_two_seq_one_[0];
  try
    {
      if ( ::CORBA::is_nil (this->updater_one_) ||
           ::CORBA::is_nil (this->updater_two_))
        {
           ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: updater_ is nil")));
           result= false;
         }
      else
        {
          this->updater_one_->create_one(i);
          this->updater_two_->create_one(u);
          ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("MultipleTemp: create_one for both topics ")
                       ACE_TEXT ("<%C> and <%C>\n"),
                       i.keyOne.in(), u.keyTwo.in()));
        }
    }
    catch(const CCM_DDS::AlreadyCreated &)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unexpected exception: ")
                            ACE_TEXT ("AlreadyCreated with test updater")
                            ACE_TEXT (" create_one <%C> or <%C>.\n"),
                            i.keyOne.in(), u.keyTwo.in()));
      result= false;
    }
    catch (const CCM_DDS::InternalError& )
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while create_one.\n")));
      result = false;;
    }
    return result;
  }


  CORBA::Boolean
  Sender_exec_i::update_one_topic()
  {
    //update already created instance with handle nil
    CORBA::Boolean result = true;
    TestTopicOne i = this->topic_one_seq_one_[0];
    TestTopicTwo u = this->topic_two_seq_one_[0];

    try
    {
      this->updater_one_->update_one(i, DDS::HANDLE_NIL);
      this->updater_two_->update_one(u, DDS::HANDLE_NIL);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("MultipleTemp: update_one with already")
                  ACE_TEXT (" existing instance with DDS::HANDLE_NIL,")
                  ACE_TEXT (" key <%C> and key <%C>\n"),
                  i.keyOne.in(), u.keyTwo.in()));
    }
    catch(const CCM_DDS::NonExistent &)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unexpected exception: ")
                            ACE_TEXT ("NonExistent with test updater ")
                            ACE_TEXT ("update_one.\n")));
      result = false;
    }
    catch (const CCM_DDS::InternalError& )
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while update_one.\n")));
      result = false;
    }
    return result;
  }

  CORBA::Boolean
  Sender_exec_i::create_many_topics()
  {
    //create many with no exception
    CORBA::Boolean result = false;
    try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("create_many both topics : <%u> samples\n"),
                  this->topic_one_seq_many_.length ()));
      this->updater_one_->create_many (this->topic_one_seq_many_);
      this->updater_two_->create_many (this->topic_two_seq_many_);

      result = true;
    }
    catch(const CCM_DDS::NonExistent& )
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR Unexpected exception:")
                  ACE_TEXT (" NonExistent with test updater create_many\n")));
    }
    catch (const CCM_DDS::InternalError& ex)
    {
       ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error in updater:")
                   ACE_TEXT (" create_many: index <%d> - retval <%d>\n"),
                   ex.index, ex.error_code));

    }
    return result;
  }

  CORBA::Boolean
  Sender_exec_i::update_many_topics()
  {
    CORBA::Boolean result = false;
    try
    {
      this->updater_one_->update_many (this->topic_one_seq_many_);
      this->updater_two_->update_many (this->topic_two_seq_many_);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("update_many : written <%u> samples\n"),
                 this->topic_one_seq_many_.length ()));
      result = true;
    }
    catch(const CCM_DDS::NonExistent& )
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Unexpected exception: NonExistent ")
                  ACE_TEXT ("with test updater update_many.\n")));
      result = false;
    }
    catch (const CCM_DDS::InternalError& ex)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                   ACE_TEXT ("whit update_many: index <%d> - retval <%d>\n"),
                   ex.index, ex.error_code));
      result = false;
    }
    return result;
  }

  void
  Sender_exec_i::add_instances_of_topic ()
  {
    //make 4 sequences of instances as testdata
    CORBA::UShort total = 3;
    this->topic_one_seq_one_.length (total);
    //sequence for tests with .._one
    for (int i = 1; i < (total + 1); i++)
      {
        char key[9];
        TestTopicOne new_key;
        ACE_OS::sprintf (key, "keyONE_%d", i);
        new_key.keyOne = CORBA::string_dup(key);
        new_key.x = i;
        this->topic_one_seq_one_[i-1] = new_key;
      }
    //sequence for tests with .._many
    this->topic_one_seq_many_.length (total);
    for (int i = 1; i < (total + 1); i++)
      {
        char key[10];
        TestTopicOne new_key;
        ACE_OS::sprintf (key, "manyOne_%d", i);
        new_key.keyOne = CORBA::string_dup(key);
        new_key.x = i;
        this->topic_one_seq_many_[i-1] = new_key;
      }
    this->topic_two_seq_one_.length (total);
    //sequence for tests with .._one
    for (int i = 1; i < (total + 1); i++)
      {
        char key[9];
        TestTopicTwo new_key;
        ACE_OS::sprintf (key, "keyTWO_%d", i);
        new_key.keyTwo = CORBA::string_dup(key);
        new_key.y = i;
        new_key.z = i;
        this->topic_two_seq_one_[i-1] = new_key;
      }
     //sequence for tests with .._many
    this->topic_two_seq_many_.length (total);
    for (int i = 1; i < (total + 1); i++)
      {
        char key[10];
        TestTopicTwo new_key;
        ACE_OS::sprintf (key, "manyTwo_%d", i);
        new_key.keyTwo = CORBA::string_dup(key);
        new_key.y = i;
        new_key.z = i;
        this->topic_two_seq_many_[i-1] = new_key;
      }
  }

  void
  Sender_exec_i::tick ()
  {
    if(this->test_ok_.value())
    {
      switch (this->test_nr_)
      {
        case UPDATE_CREATE:
          this->test_ok_ = this->create_one_topic();
          if(this->test_ok_.value())
            {
              this->test_nr_ = UPDATE_ONE;
            }
          else
            {
              this->test_nr_ = END_TEST;
            }
          break;
        case UPDATE_ONE:
          this->test_ok_ = this->update_one_topic();
          this->test_nr_ = CREATE_MANY;
          break;
        case CREATE_MANY:
          this->test_ok_ = this->create_many_topics();
          if(this->test_ok_.value())
            {
              this->test_nr_ = UPDATE_MANY;
            }
          else
            {
              this->test_nr_ = END_TEST;
            }
          this->test_nr_ = UPDATE_MANY;
          break;
        case UPDATE_MANY:
          this->test_ok_ = this->update_many_topics();
          this->test_nr_ = END_TEST;
          break;
        default:
          break;
      }
    }
  }
  void
  Sender_exec_i::start (void)
  {
    long sec = 3L;
    long const usec = 0L;
    if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (sec, usec),
                ACE_Time_Value (sec, usec)) == -1)
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
    delete this->ticker_;
  }

  void
  Sender_exec_i::set_session_context (
      ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::MultipleTemp::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->updater_one_  =
        this->context_->get_connection_test_topic_one_update_data ();
    this->updater_two_  =
        this->context_->get_connection_test_topic_two_update_data ();
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->add_instances_of_topic ();
    this->start ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if(!this->test_ok_.value())
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Error in MultipleTemp in Sender")));
      }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_MultipleTemp_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
