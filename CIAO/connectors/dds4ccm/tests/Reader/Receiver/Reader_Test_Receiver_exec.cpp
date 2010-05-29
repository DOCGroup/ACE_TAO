// -*- C++ -*-
//
// $Id$

#include "Reader_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Reader_Test_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    ACE_DEBUG ((LM_DEBUG, "Checking if last sample "
                          "is available in DDS...\n"));
    if (this->callback_.check_last ())
      this->callback_.run ();
    return 0;
  }

  //============================================================
  // Starter_exec_i
  //============================================================
  Starter_exec_i::Starter_exec_i (Receiver_exec_i & callback)
    : callback_ (callback)
  {
  }

  Starter_exec_i::~Starter_exec_i (void)
  {
  }

  void
  Starter_exec_i::set_reader_properties (CORBA::UShort nr_keys,
                                         CORBA::UShort nr_iterations)
  {
    this->callback_.keys (nr_keys);
    this->callback_.iterations (nr_iterations);
  }

  void
  Starter_exec_i::start_read ()
  {
    this->callback_.start_read ();
  }

  void
  Starter_exec_i::read_no_data ()
  {
    this->callback_.read_no_data ();
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      has_run_ (false)
  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  bool
  Receiver_exec_i::check_last ()
  {
    try
      {
        if (! ::CORBA::is_nil (this->reader_))
          {
            ReaderTest readertest_info;
            ::CCM_DDS::ReadInfo readinfo;
            char key[100];
            ACE_OS::sprintf (key, "KEY_%d", this->keys_);
            readertest_info.key = CORBA::string_dup (key);
            this->reader_->read_one_last (
                    readertest_info,
                    readinfo,
                    ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_last: "
                                  "key <%C> - iteration <%d>\n",
                                  readertest_info.key.in (),
                                  readertest_info.iteration));
            return readertest_info.iteration == this->iterations_;
          }
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::check_last: "
                              "Unexpected exception caught\n"));
      }
    return false;
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::read_one_last (bool test_handles)
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
            DDS::InstanceHandle_t hnd = test_handles
                ? this->handles_[key]
                : ::DDS::HANDLE_NIL;
            this->reader_->read_one_last (
                    readertest_info,
                    readinfo,
                    hnd);
            if (readertest_info.iteration == this->iterations_)
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE LAST: ")
                    ACE_TEXT ("last iteration received for <%C>: iteration <%u>\n"),
                    readertest_info.key.in (),
                    readertest_info.iteration));
              }
            else
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE LAST: ")
                    ACE_TEXT ("Didn't receive last iteration for <%C>: ")
                    ACE_TEXT ("expected <%u> - received <%u>\n"),
                    readertest_info.key.in (),
                    this->iterations_,
                    readertest_info.iteration));
              }
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_ERROR, ACE_TEXT ("READ ONE LAST: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE LAST: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ ONE LAST:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_one_last : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::read_one_all (bool test_handles)
  {
    try
      {
        for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
          {
            ReaderTest              readertest_info;
            ReaderTestSeq          *readertest_info_seq = 0;
            ::CCM_DDS::ReadInfoSeq *readinfo_seq = 0;
            char key[100];
            ACE_OS::sprintf (key, "KEY_%d", i);
            readertest_info.key = CORBA::string_dup (key);
            DDS::InstanceHandle_t hnd = test_handles
                ? this->handles_[key]
                : ::DDS::HANDLE_NIL;
            this->reader_->read_one_all (
                    readertest_info,
                    readertest_info_seq,
                    readinfo_seq,
                    hnd);

            if (readertest_info_seq->length () != this->iterations_)
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE ALL: ")
                    ACE_TEXT ("Didn't receive the expected number of ")
                    ACE_TEXT ("samples for <%C>: expected <%u> - received <%u>\n"),
                    key,
                    this->iterations_,
                    readertest_info_seq->length ()));
              }
            else
              {
                this->handles_[key] = (*readinfo_seq)[0].instance_handle;
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE ALL: ")
                    ACE_TEXT ("All iterations received for <%C>: number of iterations <%u>\n"),
                    key,
                    readertest_info_seq->length ()));
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE ALL: ")
                    ACE_TEXT ("Handle created for <%C>: length <%u> - isValid <%d>\n"),
                    key,
                    (*readinfo_seq)[0].instance_handle.length,
                    (*readinfo_seq)[0].instance_handle.isValid));
              }
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE ALL: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        if (ex.error_code == 0)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE ALL: ")
                  ACE_TEXT ("caught InternalError exception: handles differ\n")));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ONE ALL: ")
                  ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
                  ex.error_code));
          }
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ ONE ALL: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_one_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::read_last (void)
  {
    try
      {
        ReaderTestSeq          *readertest_info_seq = 0;
        ::CCM_DDS::ReadInfoSeq *readinfo_seq = 0;
        this->reader_->read_last (
                readertest_info_seq,
                readinfo_seq);

        if (readertest_info_seq->length () != this->keys_)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ LAST: ")
                ACE_TEXT ("Didn't receive the expected number of ")
                ACE_TEXT ("instances : expected <%u> - received <%u>\n"),
                this->keys_,
                readertest_info_seq->length ()));
          }
        for (CORBA::ULong it = 0; it < readertest_info_seq->length (); ++it)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ LAST : ")
                ACE_TEXT ("last iteration received for <%C>: iteration <%u>\n"),
                (*readertest_info_seq)[it].key.in (),
                (*readertest_info_seq)[it].iteration));
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ LAST: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ LAST: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ LAST: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_last : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::read_all (void)
  {
    try
      {
        ReaderTestSeq          *readertest_info_seq = 0;
        ::CCM_DDS::ReadInfoSeq *readinfo_seq = 0;
        this->reader_->read_all (
                readertest_info_seq,
                readinfo_seq);

        CORBA::ULong nr_samples = this->keys_ * this->iterations_;
        if (readertest_info_seq->length () != nr_samples)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
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
                // check readinfo struct.
                if (!(*readinfo_seq)[it].instance_handle.isValid)
                  {
                    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
                            ACE_TEXT ("received instance handle should be valid ")
                            ACE_TEXT ("for keyed data: ")
                            ACE_TEXT ("key <%C> - iteration <%u>\n"),
                            (*readertest_info_seq)[it].key.in (),
                            (*readertest_info_seq)[it].iteration));
                  }
                if ((*readinfo_seq)[it].source_timestamp.sec == 0 &&
                    (*readinfo_seq)[it].source_timestamp.nanosec == 0)
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR: READ ALL: "
                                        "source timestamp seems to be invalid (nil) "
                                        "key <%C> - iteration <%d>\n",
                                        (*readertest_info_seq)[it].key.in (),
                                        (*readertest_info_seq)[it].iteration));
                  }
                ++iterations;
              }
            if (nr_keys_changed != this->keys_)
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
                    ACE_TEXT ("Didn't receive the expected number of ")
                    ACE_TEXT ("instances : expected <%u> - received <%u>\n"),
                    this->keys_,
                    nr_keys_changed));
              }
            if (iterations != (this->keys_ * this->iterations_))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
                    ACE_TEXT ("Didn't receive the expected number of ")
                    ACE_TEXT ("samples : expected <%u> - received <%u>\n"),
                    nr_samples,
                    iterations));
              }
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ ALL: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::test_exception ()
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
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Receiver_exec_i::test_exception: ")
                  ACE_TEXT ("Wrong exception caught while reading non existent instance\n")));
      }
    catch (const CCM_DDS::NonExistent& )
      {
        except_caught = true;
      }
    if (!except_caught)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Receiver_exec_i::test_exception: ")
                  ACE_TEXT ("no excep caught while reading non existent instance\n")));
      }
  }

  void
  Receiver_exec_i::test_exception_with_handles ()
  {
    // test exception handling
    bool except_caught = false;
    try
      {
        ReaderTest readertest_info;
        readertest_info.key = CORBA::string_dup ("KEY_1");
        ::CCM_DDS::ReadInfo readinfo;
        DDS::InstanceHandle_t hnd = this->handles_["KEY_2"];
        this->reader_->read_one_last (
            readertest_info,
            readinfo,
            hnd);
      }
    catch (const CCM_DDS::InternalError& )
      {
        except_caught = true;
        ACE_ERROR ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::test_exception_with_handles: ")
                  ACE_TEXT ("Expected exception caught while reading non existent instance\n")));
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Receiver_exec_i::test_exception_with_handles: ")
                  ACE_TEXT ("Wrong exception caught while reading non existent instance\n")));
      }
    if (!except_caught)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Receiver_exec_i::test_exception_with_handles: ")
                  ACE_TEXT ("no excep caught while reading non existent instance\n")));
      }
  }

  void
  Receiver_exec_i::read_no_data ()
  {
    try
      {
        ReaderTestSeq          *readertest_info_seq = 0;
        ::CCM_DDS::ReadInfoSeq *readinfo_seq = 0;
        this->reader_->read_all (
                readertest_info_seq,
                readinfo_seq);

        if (readertest_info_seq->length () > 0)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ NO DATA: ")
                ACE_TEXT ("Didn't receive the expected number of ")
                ACE_TEXT ("samples : expected <%u> - received <%u>\n"),
                0,
                readertest_info_seq->length ()));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ NO DATA: ")
                ACE_TEXT ("No data available.\n")));
          }
      }
    catch (const CCM_DDS::NonExistent& )
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ERROR: READ NO DATA: ")
              ACE_TEXT ("Caught NonExistent exception")));
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ NO DATA: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ NO DATA: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_no_data : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::run ()
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    delete this->ticker_;
    this->ticker_ = 0;

    this->has_run_ = true;
    read_all ();
    read_last ();
    read_one_all ();
    read_one_last ();
    test_exception ();
    //now test with collected handles
    read_one_all (true);
    read_one_last (true);
    test_exception_with_handles ();
  }

  void
  Receiver_exec_i::start_read ()
  {
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(1, 0),
                                          ACE_Time_Value(1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  // Port operations.
  ::Reader_Test::ReaderTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return ::Reader_Test::ReaderTestConnector::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
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
    this->reader_ = this->context_->get_connection_info_out_data();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (this->ticker_)
      {
        ACE_ERROR ((LM_ERROR, "Still checking DDS upon exit\n"));
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
      }

    if (!this->has_run_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
            ACE_TEXT ("Test did not run: Didn't receive ")
            ACE_TEXT ("the expected number of samples\n")));
      }
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

