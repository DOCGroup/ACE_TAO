
// $Id$

#include "ace/INET_Addr.h"
#include "ace/os_include/os_netdb.h"

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
    ACE_ERROR_RETURN((LM_ERROR, "Connot open acceptor\n"), -1);

  if (acceptor_.acceptor().get_local_addr(listen_addr) !=0)
    ACE_ERROR_RETURN((LM_ERROR, "Connot get local addr\n"), -1);

  ACE_DEBUG((LM_DEBUG, "listening at %s:%d\n", listen_addr.get_host_name(),
                                               listen_addr.get_port_number()));

  ACE_TCHAR* buf = new ACE_TCHAR[ MAXHOSTNAMELEN + 1 ];
  listen_addr.addr_to_string(buf, MAXHOSTNAMELEN, 0);
  location_.length(1);
#if defined (ACE_USES_WCHAR)
  location_[0].id = ACE::String_Conversion::Convert_Out<char>( buf ).c_str();
  delete buf;
#else
  location_[0].id = buf;
#endif
  return 0;
}

template <class ACCEPTOR, class CONNECTOR, class DETECTION_HANDLER>
int
Fault_Detector_T<ACCEPTOR, CONNECTOR, DETECTION_HANDLER>::connect(const FTRT::Location& location)
{
    if (location.length() == 0)
      return -1;

    typename CONNECTOR::PEER_ADDR prev_addr(location[0].id);

    DETECTION_HANDLER* handler;
    ACE_NEW_RETURN(handler, DETECTION_HANDLER(listener_), 0) ;

    handler->reactor(&reactor_task_.reactor_);
    ACE_DEBUG((LM_DEBUG, "connecting to %s\n", location[0].id.in()));

    int result = connector_.connect(handler->peer(), prev_addr);

    if (result == 0)
      handler->open(this);
    else
      handler->close();

    return result ;
}
