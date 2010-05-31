// -*- C++ -*-
// $Id$

// test of different return values
#include "InterReturnT_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterReturnT_Sender_Impl
{
  CORBA::Short nr_of_received = 0;

  //============================================================
  // Facet Executor Implementation Class: MyFoo_callback_exec_i
  //============================================================
  MyFoo_callback_exec_i::MyFoo_callback_exec_i (void)
  {
  }

  MyFoo_callback_exec_i::~MyFoo_callback_exec_i (void)
  {
  }

  //============================================================
  // Operations from ::CCM_AMI::MyFoo_callback
  //============================================================
  // FOO methods
  void
  MyFoo_callback_exec_i::ret_void (const char * /*answer*/, CORBA::Long l_cmd)
  {
    if (l_cmd != 1)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_void: "
                              "received the wrong long, expected 1,"
                              " received %lu\n",
                              l_cmd));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_void_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::ret_long (::CORBA::Long ami_return_val, 
                           const char * /*answer*/, CORBA::Long l_cmd)
  {
    if ((l_cmd != 2) || (ami_return_val != 200))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_long: "
                              "received the wrong longs, expected 2 and 200,"
                              " received %lu and %lu\n",
                              l_cmd, ami_return_val));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_long_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::ret_double (::CORBA::Double ami_return_val,
                              const char * /*answer*/, CORBA::Long l_cmd)
  {
    if ((l_cmd != 3) || (ami_return_val != 3.14))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_double: "
                              "received the wrong returns, expected 3 "
                              "and 3.14,received %u and %2.2f\n",
                              l_cmd, ami_return_val));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_double_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
    MyFoo_callback_exec_i::ret_struct (
                            const InterReturnT::TestStruct &ami_return_val,
                            const char *  /*answer*/, CORBA::Long l_cmd)
  {
    if (l_cmd != 4)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_struct: "
                              "received the wrong long, expected 4,"
                              " received %u\n",
                              l_cmd));
      }
    else if ((ACE_OS::strcmp (ami_return_val.key.in(), "aaa") != 0) ||
             (ami_return_val.x != 10))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_struct: "
                              "received the struct return, expected 'aaa' "
                              "and 10, received %C and %u\n",
                              ami_return_val.key.in(), ami_return_val.x));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_struct_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
    MyFoo_callback_exec_i::ret_array (
                       const InterReturnT::TestArr ami_return_val[],
                       const char * /*answer*/, CORBA::Long l_cmd)
  {
     if ((l_cmd != 5) || (ami_return_val[1].x_array[2] != 102))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_array: "
                              "received the wrong long or array, expected 5 "
                              "and 102,received %u and %u\n",
                              l_cmd, ami_return_val[1].x_array[2]));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_array_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
    MyFoo_callback_exec_i::ret_seq (const InterReturnT::TestSeq &ami_return_val,
                           const char * /*answer*/, CORBA::Long l_cmd)
  {
    if (l_cmd != 6)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_seq: "
                              "received the wrong long, expected 6,"
                              " received %lu\n",
                              l_cmd));
      }
    else if ((ACE_OS::strcmp (ami_return_val[0].x_teststr, "ccc") != 0) ||
             (ami_return_val[1].x_test != 12))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_seq: "
                              "didn't receive the expected sequence,"
                              " received %C and %lu\n",
                              ami_return_val[0].x_teststr.in(),
                              ami_return_val[1].x_test));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_seq_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
    MyFoo_callback_exec_i::ret_union (
                          const InterReturnT::X_Union &ami_return_val,
                          const char * /*answer*/, CORBA::Long l_cmd)
  {     
    if ((l_cmd != 7) || (ami_return_val.x_long() != 11))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_union: "
                              "received the wrong long or union, expected 7"
                              " and 11, received %u and %u\n",
                              l_cmd, ami_return_val.x_long()));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_union_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::ret_enum(
                           const InterReturnT::test_enum ami_return_val,
                           const char * /*answer*/, CORBA::Long l_cmd)
  {
    if ((l_cmd != 8) || (ami_return_val != InterReturnT::TWO))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::ret_enum: "
                              "received the wrong long or enum value, expected 8"
                              " and TWO, received %u and %u\n",
                              l_cmd, ami_return_val));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::ret_enum_excep(
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::InterReturnT::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::InterReturnT::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    if (CORBA::is_nil (my_foo_ami_))
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender (ASYNCH) :\tfoo_ami is NIL !\n"));  
       return 1;
      }
    else
      {
        //Invoke Asynchronous calls to test 
        my_foo_ami_->sendc_ret_void (new MyFoo_callback_exec_i (), "Send me a void." );
        my_foo_ami_->sendc_ret_long (new MyFoo_callback_exec_i (),"Send me a long.");
        my_foo_ami_->sendc_ret_double (new MyFoo_callback_exec_i (),"Send me a double.");
        my_foo_ami_->sendc_ret_struct (new MyFoo_callback_exec_i (), "Send me a struct.");
        my_foo_ami_->sendc_ret_array (new MyFoo_callback_exec_i (),"Send me an array.");
        my_foo_ami_->sendc_ret_seq (new MyFoo_callback_exec_i (),"Send me a sequence.");
        my_foo_ami_->sendc_ret_union (new MyFoo_callback_exec_i (),"Send me a union.");
        my_foo_ami_->sendc_ret_enum (new MyFoo_callback_exec_i (),"Send me a enum value.");

      }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::InterReturnT::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::InterReturnT::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    //run some synch calls
    InterReturnT::TestStruct_var test_topic = 0;
    InterReturnT::TestArray_var topic_arr = 0;
    CORBA::Long l_cmd = 0;
    char * out_str = 0;
    try
      {
        CORBA::Double ret = 
          my_foo_ami_->ret_double ("Send me synch double",out_str, l_cmd);
        if ( (ret == 3.14 ) && (l_cmd == 3))
          {
            ++nr_of_received;
          }
      } 
    catch (const InterReturnT::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::ret_double: "
                              "Unexpected exception.\n"));  
      }
    try
      {
        InterReturnT::TestArray_slice *arr =  
          my_foo_ami_->ret_array ("Send me synch array",out_str, l_cmd);
        arr++;
        if ((l_cmd != 5) || (arr->x_array[2] != 102))
          {
            ACE_ERROR ((LM_ERROR, "ERROR MyFoo synchroon ret_array: "
                              "received the wrong long or array, expected 5 "
                              "and 102, received %u and %u\n",
                              l_cmd, arr->x_array[2]));
          } 
        else
         {
           ++nr_of_received;
         }
       }
  
    catch (const InterReturnT::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::ret_array: "
                              "Unexpected exception.\n"));  
     }
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
  : global_foo_callback_ (0)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.
  // Component attributes.
  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::InterReturnT::CCM_Sender_Context::_narrow (ctx);
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
    ::InterReturnT::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::InterReturnT::MyFoo_var synch_foo =
        this->context_->get_connection_run_my_foo ();
    synch_foo_generator* synch_foo_gen =
      new synch_foo_generator (synch_foo);
    synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (nr_of_received == 10)
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender received all expected return data"
                              " for syn- and asynchronous calls\n"));  
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Sender didn't receive all expected"
                              " return data for syn- and asynchronous"
                              " calls\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterReturnT_AMI_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      Sender_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}
