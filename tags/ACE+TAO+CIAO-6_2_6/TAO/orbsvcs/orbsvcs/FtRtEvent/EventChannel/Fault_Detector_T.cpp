// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/INET_Addr.h"
#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ACCEPTOR, class CONNECTOR, class DETECTION_HANDLER>
Fault_Detector_T<ACCEPTOR, CONNECTOR, DETECTION_HANDLER>::Fault_Detector_T()
{
}

template <class ACCEPTOR, class CONNECTOR, class DETECTION_HANDLER>
Fault_Detector_T<ACCEPTOR, CONNECTOR, DETECTION_HANDLER>::~Fault_Detector_T()
{
}


template <class ACCEPTOR, class CONNECTOR, class DETECTION_HANDLER>
int
Fault_Detector_T<ACCEPTOR, CONNECTOR, DETECTION_HANDLER>::init_acceptor()
{
  typename CONNECTOR::PEER_ADDR listen_addr;
  if (acceptor_.open(listen_addr, &reactor_task_.reactor_) != 0)
    ORBSVCS_ERROR_RETURN((LM_ERROR, "Connot open acceptor\n"), -1);

  if (acceptor_.acceptor().get_local_addr(listen_addr) !=0)
    ORBSVCS_ERROR_RETURN((LM_ERROR, "Connot get local addr\n"), -1);

  ORBSVCS_DEBUG((LM_DEBUG, "listening at %s:%d\n", listen_addr.get_host_name(),
                                               listen_addr.get_port_number()));
  ACE_TCHAR tcharbuf [MAXHOSTNAMELEN];
  listen_addr.addr_to_string(tcharbuf, MAXHOSTNAMELEN, 0);
  char* buf = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(tcharbuf));
  location_.length(1);
  location_[0].id = buf;
  return 0;
}

template <class ACCEPTOR, class CONNECTOR, class DETECTION_HANDLER>
int
Fault_Detector_T<ACCEPTOR, CONNECTOR, DETECTION_HANDLER>::connect(const FTRT::Location& location)
{
    if (location.length() == 0)
      return -1;

    typename CONNECTOR::PEER_ADDR prev_addr(location[0].id);

    DETECTION_HANDLER* handler = 0;
    ACE_NEW_RETURN(handler, DETECTION_HANDLER(listener_), 0) ;

    handler->reactor(&reactor_task_.reactor_);
    ORBSVCS_DEBUG((LM_DEBUG, "connecting to %s\n", location[0].id.in()));

    int result = connector_.connect(handler->peer(), prev_addr);

    if (result == 0)
      handler->open(this);
    else
      handler->close();

    return result ;
}

TAO_END_VERSIONED_NAMESPACE_DECL
