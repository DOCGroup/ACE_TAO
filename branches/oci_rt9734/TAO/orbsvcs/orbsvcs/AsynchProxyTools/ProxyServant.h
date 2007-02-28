// -*- C++ -*-

//
//$Id$
//

#ifndef TAO_PROXY_SERVANT
#define TAO_PROXY_SERVANT

#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "orbsvcs/AsynchProxyTools/AsynchProxyTools_Export.h"

class TAO_AMH_DSI_Response_Handler;
typedef TAO_AMH_DSI_Response_Handler * TAO_AMH_DSI_Response_Handler_ptr;

class TAO_AsynchProxyTools_Export TAO_ProxyServant :
  public virtual TAO_DynamicImplementation
{
public:
  TAO_ProxyServant();
  void invoke (CORBA::ServerRequest_ptr request,
               TAO_AMH_DSI_Response_Handler_ptr rh);
  virtual void invoke_i () = 0;
};


#endif // TAO_PROXY_SERVANT
