// -*- C++ -*-

/**
 *  @file   ECG_Mcast_Gateway.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@atdesk.com)
 */

#ifndef TAO_ECG_MCAST_GATEWAY_H
#define TAO_ECG_MCAST_GATEWAY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Event/ECG_UDP_Sender.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/ECG_Defaults.h"
#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/SString.h"
#include "ace/os_include/os_stdint.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Event_Handler;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ECG_Mcast_Gateway
 *
 * @brief Implement the builder for setting up Event Channel multicast
 *        gateway.
 *        NOT THREAD-SAFE.
 *
 * This class simplifies creation of federated Event Channels by
 * presenting a simple unified interface for creating and configuring
 * all components needed to federate an Event Channel.
 * Configuration options are described below.
 *
 * NOTE: This class does not own any of the components it creates and
 * its lifetime is independent of theirs.  This class acts purely as a
 * wrapper facade for creating and wiring appropriate components
 * together.
 *
 * @todo This class is an ACE_Service_Object, but the only reason for
 * it is the need for easy configuration using files.  Since
 * ACE_Service_Object provides much more than that, we should look
 * into replacing it with a more lightweight utility that would serve
 * our needs.
 *
 * CONFIGURATION OPTIONS
 * There are two ways to use this class:
 * 1) Use service config file to specify configuration options (which
 *    are described below), and use service configurator to obtain a
 *    an instance of configured TAO_ECG_Mcast_Gateway in your program.  (See
 *    TAO/orbsvcs/tests/Event/Mcast/Common and
 *    TAO/orbsvcs/tests/Event/Mcast/Simple for an example.)
 *
 * Service config file options:
 *
 * -ECGService <service>
 *  Valid values: sender, receiver, two_way
 *  Specifies whether this gateway should act as a multicast sender of
 *  the events or multicast receiver, or both.
 *
 * -ECGAddressServer <server_type>
 *  Valid values: basic, source, type
 *  Specifies what implementation of the address server should be used
 *  by the gateway.  basic - the same multicast address is returned for
 *  all event headers.  source - multicast addresses are returned based
 *  on the event source, according to the mapping provided at
 *  initialization.  type - multicast addresses are returned based
 *  on the event type, according to the mapping provided at
 *  initialization.
 *
 * -ECGAddressServerArg <arg>
 *  Valid value: arg is a string, whose format requirements are
 *  specific to the implementation of address server used.
 *  Arg is not interpreted by the gateway, but simply passed to the
 *  address server specified by -ECGAddressServer during initialization.
 *  THIS OPTION MUST ALWAYS BE SPECIFIED BY THE USER (there is no
 *  default value for it)
 *
 * -ECGHandler <handler_type>
 *  Valid values: basic, complex, udp
 *  Specifies what implementation of event handler should be used if
 *  gateway is acting as events receiver. basic - a simple event
 *  handler listening on a single mcast address.  complex -
 *  event handler listening on multiple mcast addresses based on
 *  events of interest to consumers.  udp - similar to basic handler,
 *  except listens on udp address as opposed to a multicast group.
 *
 * -ECGTTL <ttl>
 *  Valid values: a number > 0
 *  IP_Multicast time to live value that should be set on a sending socket.
 *  This option matters only if the gateway is acting as a sender of
 *  mcast messages.
 *
 * -ECGNIC <nic>
 *  Valid values: name of the network interface
 *  This interface is used for sending and/or receiving multicast messages.
 *
 * -ECGNonBlocking
 *  Boolean flag to configure if the socket is in blocking or
 *  non-blocking code.  The default is non-blocking.
 *  NOTE: Certain device drivers block the process if the physical
 *        link fails.
 *
 *
 * 2) Create an instance of TAO_ECG_Mcast_Gateway in your code, on the stack or
 *    dynamically, and use init () method to configure it.  No
 *    configuration files involved.  See service config options above for the
 *    description of configurable options, and init() method below for how
 *    to specify them.
 *
 *
 * Default configuration values (for either use case) can be found in
 * ECG_Defaults.h
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Mcast_Gateway
  : public ACE_Service_Object
{
public:

  /// The Service_Object entry points.
  //@{
  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);
  //@}

  /// Helper function to register the Gateway into the service
  /// configurator.
  static int init_svcs (void);

  /// Constructor.
  TAO_ECG_Mcast_Gateway (void);

  /// Values for some configuration parameters to init ().
  //@{
  enum Service_Type {ECG_MCAST_SENDER,
                     ECG_MCAST_RECEIVER,
                     ECG_MCAST_TWO_WAY};

  enum Address_Server_Type {ECG_ADDRESS_SERVER_BASIC,
                            ECG_ADDRESS_SERVER_SOURCE,
                            ECG_ADDRESS_SERVER_TYPE};

  enum Handler_Type {ECG_HANDLER_BASIC,
                     ECG_HANDLER_COMPLEX,
                     ECG_HANDLER_UDP};
  //@}

  /**
   * @struct Attributes
   *
   * @brief Helper class to initialize a TAO_ECG_Mcast_Gateway.
   *
   * The TAO_ECG_Mcast_Gateway class has several properties that can
   * be configured at initialization time, with default values.  The
   * canonical trick of using a constructor or init() method with
   * multiple arguments does not work because to change the last
   * argument the application developer needs to know all the other
   * defaults.
   *
   * Instead we define a helper class that initializes all the fields
   * to reasonable defaults.  The application only sets the fields
   * that it is interested into, if new fields are added applications
   * do not need to change.
   *
   * With a little trickery we can even eliminate named temporaries:
   *
   * TAO_ECG_Mcast_Gateway gw;<BR>
   * gw.init(TAO_ECG_Mcast_Gateway::Attributes().set_foo(x).set_bar(y));
   *
   * but we are not implementing that one (detecting errors is too
   * hard without exceptions and ACE+TAO are somewhat shy of
   * exceptions at this point.)
   */
  struct TAO_RTEvent_Serv_Export Attributes
  {
    Attributes (void);

    Address_Server_Type address_server_type;
    Handler_Type handler_type;
    Service_Type service_type;
    u_char ttl_value;
    ACE_CString nic;
    int ip_multicast_loop;
    int non_blocking;
  };

  /// Configure TAO_ECG_Mcast_Gateway programatically.  This method should
  /// be used when NOT using service configurator to obtain/configure
  /// TAO_ECG_Mcast_Gateway.  See class documentation above for more
  /// info.
  int init (const char * address_server_arg,
            const Attributes & attributes = Attributes());

  /// Same as the method above, but also gives the client an opportunity to
  /// specify consumer qos, i.e., which EC traffic should get multicasted.
  /*
   * By default, multicast sender subscribes to all events in the
   * Event Channel, i.e., all events pushed to the EC get multicasted
   * (as long as their ttl is > 0).  This method allows clients to
   * specify a more restrictive qos, hence limiting which EC traffic
   * gets multicasted.
   */
  int init (const RtecEventChannelAdmin::ConsumerQOS & consumer_qos,
            const char * address_server_arg,
            const Attributes & attributes = Attributes());

  /// The main method - create, configure and run federation
  /// components according to the specified configuration.
  void run (CORBA::ORB_ptr orb,
            RtecEventChannelAdmin::EventChannel_ptr ec);

private:

  /// Helpers.
  //@{
  /// Check that arguments to run() are not nil.
  void verify_args (CORBA::ORB_ptr orb,
                    RtecEventChannelAdmin::EventChannel_ptr ec);

  /// Verifies configuration values specified through init() make sense.
  int validate_configuration (void);
  //@}

  /// Allocate and initialize appropriate objects.
  //@{
  PortableServer::ServantBase *
        init_address_server (void);

  PortableServer::Servant_var<TAO_ECG_UDP_Sender>
        init_sender (RtecEventChannelAdmin::EventChannel_ptr ec,
                     RtecUDPAdmin::AddrServer_ptr address_server,
                     TAO_ECG_Refcounted_Endpoint endpoint_rptr);

  PortableServer::Servant_var<TAO_ECG_UDP_Receiver>
        init_receiver (RtecEventChannelAdmin::EventChannel_ptr ec,
                       RtecUDPAdmin::AddrServer_ptr address_server,
                       TAO_ECG_Refcounted_Endpoint endpoint_rptr);

  TAO_ECG_Refcounted_Endpoint init_endpoint (void);

  TAO_ECG_Refcounted_Handler
        init_handler (TAO_ECG_Dgram_Handler *recv,
                      RtecEventChannelAdmin::EventChannel_ptr ec,
                      ACE_Reactor * reactor);
  //@}

  /// Flags controlling configuration.
  //@{
  Service_Type service_type_;
  Handler_Type handler_type_;
  Address_Server_Type address_server_type_;
  ACE_CString address_server_arg_;
  u_char ttl_value_;
  ACE_TString nic_;
  int ip_multicast_loop_;
  int non_blocking_;

  RtecEventChannelAdmin::ConsumerQOS consumer_qos_;
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/ECG_Mcast_Gateway.inl"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_ECG_Mcast_Gateway)
ACE_FACTORY_DECLARE (TAO_RTEvent_Serv, TAO_ECG_Mcast_Gateway)

#include /**/ "ace/post.h"
#endif /* TAO_ECG_MCAST_GATEWAY_H */
