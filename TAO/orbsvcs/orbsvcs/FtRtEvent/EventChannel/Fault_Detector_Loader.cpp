// $Id$

#include "Fault_Detector_Loader.h"
#include "FTEC_Fault_Listener.h"
#include "SCTP_Fault_Detector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ConnectionHandler_T.h"

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

    // Parse any service configurator parameters.
    if (argc > 0 && ACE_OS::strcasecmp (argv[0], ACE_LIB_TEXT("sctp")) == 0)
    {
      detector_.reset(new STCP_Fault_Detector);
      argc--; argv++;
    }
    else
      detector_.reset(new TCP_Fault_Detector);
    return detector_->init(argc, argv);
  }

  Fault_Detector*
    Fault_Detector_Loader::detector()
  {
    return detector_.get();
  }

  /////////////////////////////////////////////////////////////////////

  ACE_FACTORY_DEFINE (TAO_FTRTEC, Fault_Detector_Loader);

  ACE_STATIC_SVC_DEFINE (Fault_Detector_Loader,
    ACE_TEXT ("FTRTEC_Fault_Detector"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (Fault_Detector_Loader),
    ACE_Service_Type::DELETE_THIS
    | ACE_Service_Type::DELETE_OBJ,
    0);
}
