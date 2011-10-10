// $Id$

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
  //------------------------------------------------------------------
  // For simplicity most of the main variables are available GLOBALLY.
  int Result= 0;
  CORBA::ORB_var orb;
  PortableInterceptor::Current_var PICurrent;
  PortableInterceptor::SlotId slot_id= 2093843221;
  testInterface_var server;

  void isCorrect (const CORBA::Any &data, const CORBA::Long correct, const ACE_TCHAR *const ID)
    {
      CORBA::Long actual= 0;
      if (data >>= actual)
        if (correct == actual)
          ACE_DEBUG ((LM_DEBUG,
                      ". %s retrieved CORRECT long (%d) from the TSC.\n",
                      ID, actual));
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        ". %s retrieved **INCORRECT** long (%d should be %d) from TSC.\n",
                        ID, actual, correct));
            Result= 1;
          }
      else
        {
           ACE_DEBUG ((LM_DEBUG,
                       ". %s can't retrieved long from the TSC.\n",
                       ID));
            Result= 1;
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
    ClientRequestInterceptor () {}

    /// Return the name of this ClientRequestInterceptor.
    virtual char * name ()
      {
        return CORBA::string_dup ("ClientRequestInterceptor");
      }

    virtual void destroy () {}

    virtual void send_request (
      PortableInterceptor::ClientRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, send_request\n"));
      }

    virtual void send_poll (
      PortableInterceptor::ClientRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, send_poll\n"));
      }

    virtual void receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, receive_reply\n"));
      }

    virtual void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". CLIENT Interception, receive_exception\n"));
      }

    virtual void receive_other (
      PortableInterceptor::ClientRequestInfo_ptr)
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
      ServerRequestInterceptor () {}

      /// Return the name of this ServerRequestinterceptor.
      virtual char * name ()
      {
        return CORBA::string_dup ("ServerRequestInterceptor");
      }

      virtual void destroy () {}

    #if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
      virtual void tao_ft_interception_point (
          PortableInterceptor::ServerRequestInfo_ptr,
          CORBA::OctetSeq_out)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, tao_ft_interception_point\n"));
      }
    #endif // TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1

      virtual void receive_request_service_contexts (
          PortableInterceptor::ServerRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, receive_request_service_contexts\n"));
      }

      virtual void receive_request (
          PortableInterceptor::ServerRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, receive_request\n"));
      }

      virtual void send_reply (
          PortableInterceptor::ServerRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, send_reply\n"));
      }

      virtual void send_exception (
          PortableInterceptor::ServerRequestInfo_ptr)
      {
        ACE_DEBUG ((LM_DEBUG,
                   ". SERVER Interception, send_exception\n"));
      }

      virtual void send_other (
          PortableInterceptor::ServerRequestInfo_ptr)
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
      ~MyORBInitializer() {slot_id= 2093843221;} // Invalidate slot
      /// The pre-initialization hook.
      virtual void pre_init (
        PortableInterceptor::ORBInitInfo_ptr)
      {}

      /// The post-initialization hook.
      virtual void post_init (
        PortableInterceptor::ORBInitInfo_ptr info)
      {
        CORBA::Object_var
          obj= info->resolve_initial_references ("PICurrent");
        PICurrent= PortableInterceptor::Current::_narrow (obj.in ());
        if (CORBA::is_nil (PICurrent.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "ERROR: Could not resolve PICurrent object.\n"));
            throw ::CORBA::INTERNAL ();
          }
        slot_id= info->allocate_slot_id ();

        // Need either a Client OR Server OR BOTH Request Interceptors
        // Registered for the use of PICurrent to make sense in the application.
        PortableInterceptor::ClientRequestInterceptor_ptr client_tmp;
        ACE_NEW_THROW_EX (client_tmp,
                          ClientRequestInterceptor (),
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        PortableInterceptor::ClientRequestInterceptor_var
          client_interceptor= client_tmp;
        info->add_client_request_interceptor (client_interceptor.in ());

        // Need either a Client OR Server OR BOTH Request Interceptors
        // Registered for the use of PICurrent to make sense in the application.
        PortableInterceptor::ServerRequestInterceptor_ptr server_tmp;
        ACE_NEW_THROW_EX (server_tmp,
                          ServerRequestInterceptor,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        PortableInterceptor::ServerRequestInterceptor_var
          server_interceptor= server_tmp;
        info->add_server_request_interceptor (server_interceptor.in ());

        // Disable collocation identification -- TAO-specific!!!
        // The fix needs to work even if collocation is not optomized for.
        TAO_ORBInitInfo_var
          tao_info= TAO_ORBInitInfo::_narrow (info);
        tao_info->orb_core ()->optimize_collocation_objects (0);
      }
    };

  //------------------------------------------------------------------
  // This is the implimentation of our test server.
  class testInterface_i : public virtual POA_testInterface
    {
    public:
      /// Constructor & Destructor.
      testInterface_i () {}
      ~testInterface_i () {}
      /// Main servant test methods.
      virtual void firstCalling ()
        {
          // Insert some data into the allocated PICurrent slot BEFORE we start.
          CORBA::Any data;
          CORBA::Long number = 2;  // Different from the client's data
          data <<= number;
          PICurrent.in()->set_slot (slot_id, data);
          ACE_DEBUG ((LM_DEBUG,
                      ". SERVER firstCalling() stored long (%d) to TSC.\n",
                      number));
          // Perform the next nested collocated call.
          server->secondCalling();
          // Check that the data in the PICurrent slot has not been corrupted.
          CORBA::Any_var data2= PICurrent.in()->get_slot (slot_id);
          isCorrect (data2.in (), 2, ACE_TEXT ("SERVER firstCalling()"));
        }
      virtual void secondCalling ()
        {
          // Insert some data into the allocated PICurrent slot.
          CORBA::Any data;
          CORBA::Long number = 3;  // Different from other server's data
          data <<= number;
          PICurrent.in()->set_slot (slot_id, data);
          ACE_DEBUG ((LM_DEBUG,
                      ". SERVER secondCalling() stored long (%d) to TSC.\n",
                      number));
        }
    };
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

//------------------------------------------------------------------
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialise the ORBInitializer for server side interception points
      PortableInterceptor::ORBInitializer_ptr
        temp_initializer= PortableInterceptor::ORBInitializer::_nil ();
      ACE_NEW_RETURN (temp_initializer,
                      MyORBInitializer,
                      -1);  // No CORBA exceptions yet we haven't yet ORB_init!
      PortableInterceptor::ORBInitializer_var
        orb_initializer= temp_initializer;
      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      // Start up the ORB and start the POAManager
      struct guardORB
        {
          guardORB(int argc, ACE_TCHAR *argv[])
            {orb= CORBA::ORB_init (argc, argv, "test_orb");}
          ~guardORB() {PICurrent= 0; orb= 0;}
        } guardORB_(argc, argv);

      CORBA::Object_var
        obj= orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var
        RootPOA= PortableServer::POA::_narrow (obj.in ());
      if (CORBA::is_nil (RootPOA.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain RootPOA reference.\n"),
                          -1);
      PortableServer::POAManager_var
        POAManager= RootPOA->the_POAManager ();
      if (CORBA::is_nil (POAManager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain POAManager reference.\n"),
                          -1);
      POAManager->activate ();

      // OK now start our collocated client/server and activate it on the default POA
      testInterface_i *testInt;
      ACE_NEW_THROW_EX (testInt,
                         testInterface_i,
                         CORBA::NO_MEMORY (
                           CORBA::SystemException::_tao_minor_code (
                             TAO::VMCID,
                             ENOMEM),
                         CORBA::COMPLETED_NO));
      PortableServer::ServantBase_var
        server_impl= testInt;
      obj= testInt->_this ();
      struct serverGuard
        {
          serverGuard(CORBA::Object_ptr obj)
            {server= testInterface::_narrow (obj);}
          ~serverGuard() {server= 0;}
        } serverGuard_ (obj.in ());
      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to obtain reference to testInterface object.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG, "Start of actual regression test...\n"));
      // Insert some data into the allocated PICurrent slot BEFORE we start.
      CORBA::Any data;
      CORBA::Long number = 1;
      data <<= number;
      PICurrent.in ()->set_slot (slot_id, data);
      ACE_DEBUG ((LM_DEBUG,
                  ". CLIENT stored long (%d) to TSC.\n",
                  number));
      // Perform the collocated call.
      server->firstCalling();
      // Check that the data in the PICurrent slot has not been corrupted.
      CORBA::Any_var data2= PICurrent.in ()->get_slot (slot_id);
      isCorrect (data2.in (), 1, ACE_TEXT ("CLIENT"));
      ACE_DEBUG ((LM_DEBUG, "End of actual regression test...\n"));

      RootPOA->destroy (1, 1);
      orb->destroy ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Corba exception");
      Result= 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "Unknown exception caught\n"));
      Result= 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Test %s\n",
              (Result ?
               ACE_TEXT ("***FAILURE***") :
               ACE_TEXT ("Success"))));
  return Result;
}
