#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/PI/ORBInitInfo.h"

#include "testS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace
{
  constexpr PortableInterceptor::SlotId invalid_slot_id {2093843221};

  struct Test_State
  {
    Test_State () = default;
    Test_State (Test_State const &) = delete;
    Test_State (Test_State &&) = delete;
    Test_State &operator= (Test_State const &) = delete;
    Test_State &operator= (Test_State &&) = delete;

    int result {};
    CORBA::ORB_var orb {};
    PortableInterceptor::Current_var pi_current {};
    PortableInterceptor::SlotId slot_id {invalid_slot_id};
    testInterface_var server {};
  };

  void isCorrect (Test_State &state,
                  CORBA::Any const &data,
                  CORBA::Long const correct,
                  ACE_TCHAR const * const ID)
    {
      CORBA::Long actual {};
      if (data >>= actual)
        {
          if (correct == actual)
            ACE_DEBUG ((LM_DEBUG,
                        ". %s retrieved CORRECT long (%d) from the TSC.\n",
                        ID, actual));
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ". %s retrieved **INCORRECT** long (%d should be %d) from TSC.\n",
                          ID, actual, correct));
              state.result = 1;
            }
        }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        ". %s can't retrieved long from the TSC.\n",
                        ID));
            state.result = 1;
          }
    }

  //------------------------------------------------------------------
  // Need either a Client OR Server OR BOTH Request Interceptors
  // registered for the use of PICurrent to make sense in the application.
  struct ClientRequestInterceptor
    : public virtual PortableInterceptor::ClientRequestInterceptor,
      public virtual CORBA::LocalObject
  {
    /// Constructor.
    ClientRequestInterceptor () = default;
    ClientRequestInterceptor (ClientRequestInterceptor const &) = delete;
    ClientRequestInterceptor (ClientRequestInterceptor &&) = delete;
    ClientRequestInterceptor &operator= (ClientRequestInterceptor const &) = delete;
    ClientRequestInterceptor &operator= (ClientRequestInterceptor &&) = delete;

    /// Return the name of this ClientRequestInterceptor.
    char *name () override
      {
        return CORBA::string_dup ("ClientRequestInterceptor");
      }

    void destroy () override {}

    void send_request (PortableInterceptor::ClientRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, send_request\n"));
      }

    void send_poll (PortableInterceptor::ClientRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, send_poll\n"));
      }

    void receive_reply (PortableInterceptor::ClientRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, receive_reply\n"));
      }

    void receive_exception (PortableInterceptor::ClientRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, receive_exception\n"));
      }

    void receive_other (PortableInterceptor::ClientRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, receive_other\n"));
      }
  };

  //------------------------------------------------------------------
  // This is our server side request interceptor.
  // Need either a Client OR Server OR BOTH Request Interceptors
  // registered for the use of PICurrent to make sense in the application.
  struct ServerRequestInterceptor
    : public virtual PortableInterceptor::ServerRequestInterceptor,
      public virtual CORBA::LocalObject
    {
      /// Constructor.
      ServerRequestInterceptor () = default;
      ServerRequestInterceptor (ServerRequestInterceptor const &) = delete;
      ServerRequestInterceptor (ServerRequestInterceptor &&) = delete;
      ServerRequestInterceptor &operator= (ServerRequestInterceptor const &) = delete;
      ServerRequestInterceptor &operator= (ServerRequestInterceptor &&) = delete;

      /// Return the name of this ServerRequestinterceptor.
      char *name () override
      {
        return CORBA::string_dup ("ServerRequestInterceptor");
      }

      void destroy () override {}

    #if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
      void tao_ft_interception_point (
          PortableInterceptor::ServerRequestInfo_ptr,
          CORBA::OctetSeq_out) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, tao_ft_interception_point\n"));
      }
    #endif // TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1

      void receive_request_service_contexts (
          PortableInterceptor::ServerRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, receive_request_service_contexts\n"));
      }

      void receive_request (PortableInterceptor::ServerRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, receive_request\n"));
      }

      void send_reply (PortableInterceptor::ServerRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, send_reply\n"));
      }

      void send_exception (PortableInterceptor::ServerRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, send_exception\n"));
      }

      void send_other (PortableInterceptor::ServerRequestInfo_ptr) override
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, send_other\n"));
      }
    };

  //------------------------------------------------------------------
  // This is the test ORB initializer that registers the IORInterceptor
  // for this test.
  class MyORBInitializer :
    public virtual PortableInterceptor::ORBInitializer,
    public virtual CORBA::LocalObject
    {
    public:
      explicit MyORBInitializer (Test_State &state)
        : state_ (state)
      {
      }
      MyORBInitializer (MyORBInitializer const &) = delete;
      MyORBInitializer (MyORBInitializer &&) = delete;
      MyORBInitializer &operator= (MyORBInitializer const &) = delete;
      MyORBInitializer &operator= (MyORBInitializer &&) = delete;
      ~MyORBInitializer () override = default;
      /// The pre-initialization hook.
      void pre_init (PortableInterceptor::ORBInitInfo_ptr) override
      {}

      /// The post-initialization hook.
      void post_init (PortableInterceptor::ORBInitInfo_ptr info) override
      {
        CORBA::Object_var const obj = info->resolve_initial_references ("PICurrent");
        state_.pi_current = PortableInterceptor::Current::_narrow (obj.in ());
        if (CORBA::is_nil (state_.pi_current.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "ERROR: Could not resolve PICurrent object.\n"));
            throw ::CORBA::INTERNAL ();
          }
        state_.slot_id = info->allocate_slot_id ();

        // Need either a Client OR Server OR BOTH Request Interceptors
        // Registered for the use of PICurrent to make sense in the application.
        PortableInterceptor::ClientRequestInterceptor_ptr client_tmp {};
        ACE_NEW_THROW_EX (client_tmp,
                          ClientRequestInterceptor (),
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        // The _var takes ownership of the allocated interceptor.
        PortableInterceptor::ClientRequestInterceptor_var const client_interceptor = client_tmp;
        info->add_client_request_interceptor (client_interceptor.in ());

        // Need either a Client OR Server OR BOTH Request Interceptors
        // Registered for the use of PICurrent to make sense in the application.
        PortableInterceptor::ServerRequestInterceptor_ptr server_tmp {};
        ACE_NEW_THROW_EX (server_tmp,
                          ServerRequestInterceptor,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        // The _var takes ownership of the allocated interceptor.
        PortableInterceptor::ServerRequestInterceptor_var const server_interceptor = server_tmp;
        info->add_server_request_interceptor (server_interceptor.in ());

        // Disable collocation identification -- TAO-specific!!!
        // The fix needs to work even if collocation is not optomized for.
        TAO_ORBInitInfo_var const tao_info = TAO_ORBInitInfo::_narrow (info);
        tao_info->orb_core ()->optimize_collocation_objects (false);
      }

    private:
      Test_State &state_;
    };

  //------------------------------------------------------------------
  // This is the implimentation of our test server.
  class testInterface_i : public virtual POA_testInterface
    {
    public:
      /// Constructor & Destructor.
      explicit testInterface_i (Test_State &state)
        : state_ (state)
      {
      }
      testInterface_i (testInterface_i const &) = delete;
      testInterface_i (testInterface_i &&) = delete;
      testInterface_i &operator= (testInterface_i const &) = delete;
      testInterface_i &operator= (testInterface_i &&) = delete;
      ~testInterface_i () override = default;
      /// Main servant test methods.
      void firstCalling () override
        {
          // Insert some data into the allocated PICurrent slot BEFORE we start.
          CORBA::Any data {};
          CORBA::Long const number {2};  // Different from the client's data
          data <<= number;
          state_.pi_current.in ()->set_slot (state_.slot_id, data);
          ACE_DEBUG ((LM_DEBUG,
                      ". SERVER firstCalling() stored long (%d) to TSC.\n",
                      number));
          // Perform the next nested collocated call.
          state_.server->secondCalling ();
          // Check that the data in the PICurrent slot has not been corrupted.
          CORBA::Any_var const data2 = state_.pi_current.in ()->get_slot (state_.slot_id);
          isCorrect (state_, data2.in (), 2, ACE_TEXT ("SERVER firstCalling()"));
        }
      void secondCalling () override
        {
          // Insert some data into the allocated PICurrent slot.
          CORBA::Any data {};
          CORBA::Long const number {3};  // Different from other server's data
          data <<= number;
          state_.pi_current.in ()->set_slot (state_.slot_id, data);
          ACE_DEBUG ((LM_DEBUG,
                      ". SERVER secondCalling() stored long (%d) to TSC.\n",
                      number));
        }

    private:
      Test_State &state_;
    };

  struct ORB_Guard
  {
    ORB_Guard (Test_State &state, int argc, ACE_TCHAR *argv[])
      : state_ (state)
    {
      state_.orb = CORBA::ORB_init (argc, argv, "test_orb");
    }
    ORB_Guard (ORB_Guard const &) = delete;
    ORB_Guard (ORB_Guard &&) = delete;
    ORB_Guard &operator= (ORB_Guard const &) = delete;
    ORB_Guard &operator= (ORB_Guard &&) = delete;

    ~ORB_Guard ()
    {
      state_.server = testInterface::_nil ();
      state_.pi_current = PortableInterceptor::Current::_nil ();
      state_.orb = CORBA::ORB::_nil ();
    }

    Test_State &state_;
  };
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

//------------------------------------------------------------------
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Test_State state;
  try
    {
      // Initialise the ORBInitializer for server side interception points
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};
      ACE_NEW_RETURN (temp_initializer,
                      MyORBInitializer (state),
                      -1);  // No CORBA exceptions yet we haven't yet ORB_init!
      // The _var takes ownership of the allocated initializer.
      PortableInterceptor::ORBInitializer_var const orb_initializer = temp_initializer;
      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      // Start up the ORB and start the POAManager
      ORB_Guard const orb_guard (state, argc, argv);

      CORBA::Object_var const root_object = state.orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (root_object.in ());
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);
      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();
      if (CORBA::is_nil (poa_manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain POAManager reference.\n"),
                          -1);
      poa_manager->activate ();

      // OK now start our collocated client/server and activate it on the default POA
      testInterface_i *test_int {};
      ACE_NEW_THROW_EX (test_int,
                        testInterface_i (state),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      // ServantBase_var takes ownership of the allocated servant.
      PortableServer::ServantBase_var const server_impl = test_int;
      CORBA::Object_var const object = test_int->_this ();
      state.server = testInterface::_narrow (object.in ());
      if (CORBA::is_nil (state.server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain reference to testInterface object.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG, "Start of actual regression test...\n"));
      // Insert some data into the allocated PICurrent slot BEFORE we start.
      CORBA::Any data {};
      CORBA::Long const number {1};
      data <<= number;
      state.pi_current.in ()->set_slot (state.slot_id, data);
      ACE_DEBUG ((LM_DEBUG,
                  ". CLIENT stored long (%d) to TSC.\n",
                  number));
      // Perform the collocated call.
      state.server->firstCalling ();
      // Check that the data in the PICurrent slot has not been corrupted.
      CORBA::Any_var const data2 = state.pi_current.in ()->get_slot (state.slot_id);
      isCorrect (state, data2.in (), 1, ACE_TEXT ("CLIENT"));
      ACE_DEBUG ((LM_DEBUG, "End of actual regression test...\n"));

      state.server = testInterface::_nil ();
      root_poa->destroy (true, true);
      state.orb->destroy ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Corba exception");
      state.result = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "Unknown exception caught\n"));
      state.result = 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Test %s\n",
              (state.result ?
               ACE_TEXT ("***FAILURE***") :
               ACE_TEXT ("Success"))));
  return state.result;
}
