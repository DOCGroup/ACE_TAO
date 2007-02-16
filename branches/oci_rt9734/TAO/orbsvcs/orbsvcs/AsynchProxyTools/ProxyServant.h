//
//$Id$
//
class ProxyServant : public virtual TAO_DynamicImplementation
{
public:
  ProxyServant();
  void invoke (CORBA::ServerRequest_ptr request,
               TAO_AMH_DSI_Response_Handler_ptr rh);
  virtual void invoke_i () = 0;
};
