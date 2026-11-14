#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Loader.h"

#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Initializer.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/OS_NS_strings.h"

#include <mutex>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FTRT {
  FTRT_ClientORB_Loader::FTRT_ClientORB_Loader () = default;

  FTRT_ClientORB_Loader::~FTRT_ClientORB_Loader () = default;

  int FTRT_ClientORB_Loader::init (int argc,
    ACE_TCHAR* argv[])
  {
    static std::once_flag initialized;
    static int init_result {};

    std::call_once (initialized, [argc, argv]()
      {
        // Set defaults and parse service configurator parameters.
        int transaction_depth {1};
        for (int curarg {}; curarg < argc; ++curarg)
          {
            if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT ("-ORBTransactionDepth")) == 0)
              {
                ++curarg;
                if (curarg < argc)
                  transaction_depth = ACE_OS::atoi (argv[curarg]);
              }
          }

        // Register the ORB initializer.
        try
          {
            PortableInterceptor::ORBInitializer_ptr temp_orb_initializer {};
            PortableInterceptor::ORBInitializer_var orb_initializer;

            ACE_NEW_THROW_EX (temp_orb_initializer,
                              FTRT_ClientORB_Initializer (transaction_depth),
                              CORBA::NO_MEMORY (
                                CORBA::SystemException::_tao_minor_code (
                                  TAO::VMCID,
                                  ENOMEM),
                                CORBA::COMPLETED_NO));
            orb_initializer = temp_orb_initializer;

            PortableInterceptor::register_orb_initializer (orb_initializer.in ());
          }
        catch (CORBA::Exception const &ex)
          {
            ex._tao_print_exception (
              "Unexpected exception caught while initializing the TransactionDepth");
            init_result = 1;
          }
      });

    return init_result;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

  /////////////////////////////////////////////////////////////////////

ACE_FACTORY_NAMESPACE_DEFINE (
  TAO_FTRT,
  FTRT_ClientORB_Loader,
  TAO_FTRT::FTRT_ClientORB_Loader)

ACE_STATIC_SVC_DEFINE (
  FTRT_ClientORB_Loader,
  ACE_TEXT ("FTRT_ClientORB_Service"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (FTRT_ClientORB_Loader),
  ACE_Service_Type::DELETE_THIS
  | ACE_Service_Type::DELETE_OBJ,
  0)

