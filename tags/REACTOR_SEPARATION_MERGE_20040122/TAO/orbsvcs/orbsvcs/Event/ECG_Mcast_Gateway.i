// $Id$

ACE_INLINE
TAO_ECG_Mcast_Gateway::TAO_ECG_Mcast_Gateway (void)
  : service_type_ (TAO_ECG_DEFAULT_SERVICE)
  , handler_type_ (TAO_ECG_DEFAULT_HANDLER)
  , address_server_type_ (TAO_ECG_DEFAULT_ADDRESS_SERVER)
  , address_server_arg_ ((const char *) TAO_ECG_DEFAULT_ADDRESS_SERVER_ARG)
  , ttl_value_ (TAO_ECG_DEFAULT_TTL)
  , nic_ (ACE_static_cast(const char *,TAO_ECG_DEFAULT_NIC))
  , ip_multicast_loop_ (TAO_ECG_DEFAULT_IP_MULTICAST_LOOP)
  , non_blocking_ (TAO_ECG_DEFAULT_NON_BLOCKING)
  , consumer_qos_ ()
{
  this->consumer_qos_.dependencies.length (0);
}

ACE_INLINE
TAO_ECG_Mcast_Gateway::Attributes::Attributes (void)
  : address_server_type (TAO_ECG_DEFAULT_ADDRESS_SERVER)
  , handler_type (TAO_ECG_DEFAULT_HANDLER)
  , service_type (TAO_ECG_DEFAULT_SERVICE)
  , ttl_value (TAO_ECG_DEFAULT_TTL)
  , nic (ACE_static_cast(const char *,TAO_ECG_DEFAULT_NIC))
  , ip_multicast_loop (TAO_ECG_DEFAULT_IP_MULTICAST_LOOP)
  , non_blocking (TAO_ECG_DEFAULT_NON_BLOCKING)
{
}
