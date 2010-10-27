// -*- C++ -*-
// $Id$

//Test combination of ami4ccm and dds4ccm
#include "AmiDds_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_AmiDds_Sender_Impl
{
  //============================================================
  // Facet Executor Implementation Class: MyFoo_callback_exec_i
  //============================================================
  MyFoo_callback_exec_i::MyFoo_callback_exec_i (void)
  {
  }

  MyFoo_callback_exec_i::~MyFoo_callback_exec_i (void)
  {
  }
  void
  MyFoo_callback_exec_i::hello (
    ::CORBA::Long answer)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Sender (AMI4CCM)) :\treceived ASYNCHRONOUS answer ")
                ACE_TEXT("from Receiver <%u>\n"), answer));
  }

  void
  MyFoo_callback_exec_i::hello_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
        excep_holder->raise_exception ();
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
    this->pulse_callback_.tick ();
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
  : test_nr_(UPDATE_ONE),
    test_ok_(true)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
  CORBA::Boolean
  Sender_exec_i::create_one(CORBA::Short nr)
  {
    CORBA::Boolean result = true;
    TestTopic i = this->topic_seq_one_[nr];
    i.x = nr + 1;
    try
    {
      if (! ::CORBA::is_nil (this->updater_) ) {
        this->updater_->create_one(i);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender (DDS4CCM)) :\t")
                              ACE_TEXT ("create_one with instance key <%C>\n"),
                              i.key.in()));
      }
      else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: updater_ is nil")));
        result= false;
      }
    }
    catch(const CCM_DDS::AlreadyCreated &)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected exception: AlreadyCreated ")
                            ACE_TEXT ("with test updater create_one <%C>.\n"),
                            i.key.in()));
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
  Sender_exec_i::update_one(CORBA::Short nr)
  {
    //update already created instance with handle nil
    CORBA::Boolean result = true;
    TestTopic i = this->topic_seq_one_[nr];
    i.x = nr + 1;
    try
    {
      this->updater_->update_one(i, DDS::HANDLE_NIL);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender (DDS4CCM)) :\t")
                            ACE_TEXT ("update_one with key <%C>\n"),
                            i.key.in()));
    }
    catch(const CCM_DDS::NonExistent &)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected exception: NonExistent with")
                            ACE_TEXT (" test updater update_one <%C>.\n"),
                            i.key.in()));
      result = false;
    }
    catch (const CCM_DDS::InternalError& )
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Internal Error while update_one.\n")));
      result = false;
    }
    return result;
  }

  void
  Sender_exec_i::add_instances_of_topic ()
  {
    //make sequence of instances as testdata
    CORBA::UShort total = 2;
    this->topic_seq_one_.length (total);
    //sequence for tests with .._one
    for (int i = 1; i < (total + 1); i++)
      {
        char key[7];
        TestTopic new_key;
        ACE_OS::sprintf (key, "KEY_%d", i);
        new_key.key = CORBA::string_dup(key);
        new_key.x = 0;
        this->topic_seq_one_[i-1] = new_key;
      }
  }

  CORBA::Boolean
  Sender_exec_i::send_asynch_call()
  {
    //ami4ccm send asynch call
    CORBA::Boolean result = true;
    ::AmiDds::AMI4CCM_MyFoo_var my_foo_ami_  =
       context_->get_connection_sendc_run_my_foo();

    for (int i = 0; i < 2; ++i)
      {
        if (CORBA::is_nil (my_foo_ami_))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("ERROR Sender (ASYNCH) :")
                        ACE_TEXT("\tmy_foo_ami is NIL !!!\n")));
            result = false;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Sender (AMI4CCM)) :\t")
                        ACE_TEXT("Invoke Asynchronous call nr %u\n"),(i + 1)));
            my_foo_ami_->sendc_hello (new MyFoo_callback_exec_i ());
          }
      }
    return result;

  }

  CORBA::Boolean
  Sender_exec_i::send_synch_call()
  {
    //send synch call
    CORBA::Boolean result = true;
    ::AmiDds::MyFoo_var my_foo_ami_  =
    context_->get_connection_run_my_foo();
    for (CORBA::Short i = 0; i < 2; ++i)
      {
        if (CORBA::is_nil (my_foo_ami_))
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("ERROR Sender (SYNCH) :")
                        ACE_TEXT("\tmy_foo_ami is NIL !!!\n")));
            result = false;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Sender (AMI4CCM)) :\t")
                        ACE_TEXT("Invoke SYNCHRONOUS call nr %u\n"),(i + 1)));
            CORBA::Long answer;
            my_foo_ami_->hello (answer);
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Sender (AMI4CCM)) :\treceived SYNCHRONOUS")
                        ACE_TEXT(" answer from Receiver <%u>\n"),
                        answer));
          }
      }
    return result;
  }

  void
  Sender_exec_i::tick ()
  {
    if(this->test_ok_.value())
    {
      switch (this->test_nr_)
      {
        case UPDATE_ONE:
          this->test_ok_ = this->create_one(0);
          if(this->test_ok_.value())
            {
              this->test_ok_ = this->update_one(0);
            }
          if(this->test_ok_.value())
            {
              this->test_nr_ = SEND_ASYNCH;
            }
          else
            {
              this->test_nr_ = END_TEST;
            }
          break;
        case SEND_ASYNCH:
          this->test_ok_ = this->send_asynch_call();
          if(this->test_ok_.value())
            {
              this->test_nr_ = UPDATE_TWO;
            }
          else
            {
              this->test_nr_ = END_TEST;
            }
          break;
        case UPDATE_TWO:
          this->test_ok_ = this->create_one(1);
          if(this->test_ok_.value())
            {
              this->test_ok_ = this->update_one(1);
            }
          if(this->test_ok_.value())
            {
              this->test_nr_ = SEND_SYNCH;
            }
          else
            {
              this->test_nr_ = END_TEST;
            }
          break;
        case SEND_SYNCH:
          this->test_ok_ = this->send_synch_call();
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
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
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
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::AmiDds::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->updater_  = this->context_->get_connection_test_topic_update_data ();
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
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Error in AmiDds test")));
      }
  }

  extern "C" AMIDDS_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_AmiDds_AMI_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
