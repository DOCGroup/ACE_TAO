// -*- C++ -*-
//
// $Id$

#include "Reader_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Reader_Test_Receiver_Impl
{
  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iReaderTest_Listener_exec_i ();
  //============================================================
  Starter_exec_i::Starter_exec_i (Receiver_exec_i & callback)
    : callback_ (callback)
  {
  }

  Starter_exec_i::~Starter_exec_i (void)
  {
  }

  void
  Starter_exec_i::start_reader (CORBA::UShort nr_keys,
                                CORBA::UShort nr_iterations)
  {
    this->callback_.start (nr_keys, nr_iterations);
  }


  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      timer_started_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }


  // Supported operations and attributes.
  void
  Receiver_exec_i::read_one_last (void)
  {
    try
      {
        for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
          {
            ReaderTest readertest_info;
            ::CCM_DDS::ReadInfo readinfo;
            char key[100];
            ACE_OS::sprintf (key, "KEY_%d", i);
            readertest_info.key = CORBA::string_dup (key);
            this->reader_->read_one_last (readertest_info, readinfo, ::DDS::HANDLE_NIL);
            if (readertest_info.iteration == this->iterations_)
              {
                CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE LAST: ")
                    ACE_TEXT ("last iteration received for <%C>: iteration <%u>\n"),
                    readertest_info.key.in (),
                    readertest_info.iteration));
              }
            else
              {
                CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE LAST: ")
                    ACE_TEXT ("Didn't receive last iteration for <%C>: ")
                    ACE_TEXT ("expected <%u> - received <%u>\n"),
                    readertest_info.key.in (),
                    this->iterations_,
                    readertest_info.iteration));
              }
          }
      }
    catch(CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            CIAO_DEBUG ((LM_ERROR, ACE_TEXT ("Receiver_exec_i::read_one_last: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch(CCM_DDS::InternalError& ex)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver_exec_i::read_one_last: ")
              ACE_TEXT ("caught exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_one_last : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::read_one_all (void)
  {
    try
      {
        for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
          {
            ReaderTest              readertest_info;
            ReaderTest_Seq          *readertest_info_seq;
            ::CCM_DDS::ReadInfoSeq  *readinfo_seq;
            char key[100];
            ACE_OS::sprintf (key, "KEY_%d", i);
            readertest_info.key = CORBA::string_dup (key);
            this->reader_->read_one_all (
                    readertest_info,
                    readertest_info_seq,
                    readinfo_seq,
                    ::DDS::HANDLE_NIL);

            if (readertest_info_seq->length () != this->iterations_)
              {
                CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE ALL: ")
                    ACE_TEXT ("Didn't receive the expected number of ")
                    ACE_TEXT ("samples : expected <%u> - received <%u>\n"),
                    this->iterations_,
                    readertest_info_seq->length ()));
              }
            CORBA::UShort last_iter = 0;
            for (CORBA::ULong it = 0; it < readertest_info_seq->length (); ++it)
              {
                ++last_iter;
                if (last_iter != (*readertest_info_seq)[it].iteration)
                  {
                    CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE ALL : ")
                        ACE_TEXT ("Didn't receive samples for <%C> in order: iteration <%u>\n"),
                        (*readertest_info_seq)[it].key.in (),
                        (*readertest_info_seq)[it].iteration));
                  }
              }
            if (last_iter != this->iterations_)
              {
                CIAO_DEBUG ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE ALL : ")
                    ACE_TEXT ("Didn't receive all iterations for ")
                    ACE_TEXT ("<%C>: expected <%u> - latest <%u>\n"),
                    key,
                    this->iterations_,
                    last_iter));
              }
            else
              {
                CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE ALL: ")
                    ACE_TEXT ("All iterations received for <%C>: number of iterations <%u>\n"),
                    key,
                    last_iter));
              }
          }
      }
    catch(CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("ReaderTest_Read_One: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch(CCM_DDS::InternalError& ex)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver_exec_i::read_one_all: ")
              ACE_TEXT ("caught exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_one_all : Exception caught\n")));
      }
  }

  void 
  Receiver_exec_i::read_last (void)
  {
    try
      {
        ReaderTest_Seq          *readertest_info_seq;
        ::CCM_DDS::ReadInfoSeq  *readinfo_seq;
        this->reader_->read_last (
                readertest_info_seq,
                readinfo_seq);

        if (readertest_info_seq->length () != this->keys_)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ LAST: ")
                ACE_TEXT ("Didn't receive the expected number of ")
                ACE_TEXT ("instances : expected <%u> - received <%u>\n"),
                this->keys_,
                readertest_info_seq->length ()));
          }
        for (CORBA::ULong it = 0; it < readertest_info_seq->length (); ++it)
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ LAST : ")
                ACE_TEXT ("last iteration received for <%C>: iteration <%u>\n"),
                (*readertest_info_seq)[it].key.in (),
                (*readertest_info_seq)[it].iteration));
          }
      }
    catch(CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::read_last: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_last : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::read_all (void)
  {
    try
      {
        ReaderTest_Seq          *readertest_info_seq;
        ::CCM_DDS::ReadInfoSeq  *readinfo_seq;
        this->reader_->read_all (
                readertest_info_seq,
                readinfo_seq);

        CORBA::ULong nr_samples = this->keys_ * this->iterations_;
        if (readertest_info_seq->length () != nr_samples)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
                ACE_TEXT ("Didn't receive the expected number of ")
                ACE_TEXT ("samples : expected <%u> - received <%u>\n"),
                nr_samples,
                readertest_info_seq->length ()));
          }
        if (readertest_info_seq->length () > 0)
          {
            CORBA::UShort nr_keys_changed = 1;
            CORBA::String_var last_key ((*readertest_info_seq)[0].key.in ());
            CORBA::UShort iterations = 0;
            for (CORBA::ULong it = 0; it < readertest_info_seq->length (); ++it)
              {
                if (ACE_OS::strcmp (last_key, (*readertest_info_seq)[it].key.in ()) != 0)
                  {
                    ACE_OS::strcpy (last_key, (*readertest_info_seq)[it].key.in ());
                    ++nr_keys_changed;
                  }
                ++iterations;
              }
            if (nr_keys_changed != this->keys_)
              {
                CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
                    ACE_TEXT ("Didn't receive the expected number of ")
                    ACE_TEXT ("instances : expected <%u> - received <%u>\n"),
                    this->keys_,
                    nr_keys_changed));
              }
            if (iterations != (this->keys_ * this->iterations_))
              {
                CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
                    ACE_TEXT ("Didn't receive the expected number of ")
                    ACE_TEXT ("samples : expected <%u> - received <%u>\n"),
                    nr_samples,
                    iterations));
              }
          }
      }
    catch(CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::read_all: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::test_exception (void)
  {
    // test exception handling
    bool except_caught = false;
    try
      {
        ReaderTest readertest_info;
        readertest_info.key = CORBA::string_dup ("KEY_0");
        ::CCM_DDS::ReadInfo readinfo;
        this->reader_->read_one_last (readertest_info, readinfo, ::DDS::HANDLE_NIL);
      }
    catch(CCM_DDS::NonExistent& ex)
      {
        except_caught = true;
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("ReaderTest_Read_One: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    if (!except_caught)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : ReaderTest_Read_One: ")
                  ACE_TEXT ("no excep caught while reading non exsistent instance\n")));
      }
  }

  void
  Receiver_exec_i::run ()
  {
    //read_one_last ();
    read_one_all ();
    //read_last ();
    //read_all ();
    //test_exception ();
  }

  void
  Receiver_exec_i::start (CORBA::UShort nr_keys, CORBA::UShort nr_iterations)
  {
    this->keys_ = nr_keys;
    this->iterations_ = nr_iterations;
    run ();
  }
  // Port operations.
  ::CCM_DDS::ReaderTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return 0;
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return 0;
  }

  ::CCM_ReaderStarter_ptr
  Receiver_exec_i::get_reader_start ()
  {
    return new Starter_exec_i (*this);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Reader_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    this->reader_ = this->context_->get_connection_info_out_data();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Reader_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

