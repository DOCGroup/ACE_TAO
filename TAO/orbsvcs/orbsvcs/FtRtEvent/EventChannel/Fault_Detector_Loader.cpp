// $Id$

#include "Fault_Detector_Loader.h"
#include "Fault_Detector_T.h"
#include "FTEC_Fault_Listener.h"
#include "SCTP_Fault_Detector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ConnectionHandler_T.h"

#include "ace/Acceptor.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (EventChannel,
           Fault_Detector_Loader,
           "$Id$")

namespace FTRTEC {
  typedef Fault_Detector_T<
    ACE_Acceptor<ConnectionAcceptHandler<ACE_SOCK_STREAM> , ACE_SOCK_ACCEPTOR>,
    ACE_SOCK_Connector, ConnectionDetectHandler<ACE_SOCK_STREAM> >
    TCP_Fault_Detector;

  Fault_Detector_Loader::Fault_Detector_Loader (void)
  {
  }

  Fault_Detector_Loader::~Fault_Detector_Loader (void)
  {
  }

  int
  Fault_Detector_Loader::init (int argc,
    ACE_TCHAR* argv[])
  {
    static int initialized = 0;

    // Only allow initialization once.
    if (initialized)
      return 0;

    initialized = 1;

    Fault_Detector* detector = 0;

    // Parse any service configurator parameters.
    if (argc > 0 && ACE_OS::strcasecmp (argv[0], ACE_LIB_TEXT("sctp")) == 0)
    {
#if (TAO_HAS_SCIOP == 1)
      ACE_AUTO_PTR_RESET(detector_, detector, Fault_Detector);
#else
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) SCTP not enabled. ",
                  " Enable SCTP and rebuild ACE+TAO \n"));
#endif /* TAO_HAS_SCIOP */
      argc--; argv++;
    }
    else {
      ACE_NEW_RETURN(detector, TCP_Fault_Detector, -1);
      ACE_AUTO_PTR_RESET(detector_, detector, Fault_Detector);
    }
    return detector_->init(argc, argv);
  }

  Fault_Detector*
    Fault_Detector_Loader::detector()
  {
    return detector_.get();
  }

  /////////////////////////////////////////////////////////////////////

  ACE_FACTORY_DEFINE (TAO_FTRTEC, Fault_Detector_Loader)

  ACE_STATIC_SVC_DEFINE (Fault_Detector_Loader,
    ACE_TEXT ("FTRTEC_Fault_Detector"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (Fault_Detector_Loader),
    ACE_Service_Type::DELETE_THIS
    | ACE_Service_Type::DELETE_OBJ,
    0)
}
