// -*- C++ -*-

//=============================================================================
/**
 *  @file   AV_Core.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_AV_CORE_H
#define TAO_AV_CORE_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/AVStreamsC.h"
#include "orbsvcs/AV/AV_export.h"

#include "ace/Singleton.h"
#include "ace/Unbounded_Set.h"

class TAO_Base_StreamEndPoint;
class TAO_AV_Connector_Registry;
class TAO_AV_Acceptor_Registry;
class TAO_AV_Acceptor;
class TAO_AV_Connector;
class TAO_FlowSpec_Entry;
class TAO_AV_Transport_Item;
class TAO_AV_Transport_Factory;
class TAO_AV_Flow_Protocol_Item;
class TAO_AV_Flow_Protocol_Factory;
class ACE_Reactor;

typedef ACE_Unbounded_Set <TAO_FlowSpec_Entry*> TAO_AV_FlowSpecSet;
typedef ACE_Unbounded_Set_Iterator <TAO_FlowSpec_Entry*> TAO_AV_FlowSpecSetItor;

typedef ACE_Unbounded_Set<TAO_AV_Transport_Item*> TAO_AV_TransportFactorySet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Transport_Item*> TAO_AV_TransportFactorySetItor;

// typedefs for containers containing the list of loaded Flow_Protocol factories.
typedef ACE_Unbounded_Set<TAO_AV_Flow_Protocol_Item*> TAO_AV_Flow_ProtocolFactorySet;
typedef ACE_Unbounded_Set_Iterator<TAO_AV_Flow_Protocol_Item*> TAO_AV_Flow_ProtocolFactorySetItor;

/**
 * @class TAO_AV_Core
 * @brief This class encapsulates access to the TAO AV Core's resources
 *        and its state.
 */
class TAO_AV_Export TAO_AV_Core
{
public:
  enum EndPoint {TAO_AV_ENDPOINT_A,TAO_AV_ENDPOINT_B};

  enum Protocol
  {
    TAO_AV_NOPROTOCOL = -1,
    TAO_AV_TCP        =  0,
    TAO_AV_UDP        =  1,
    TAO_AV_AAL5       =  2,
    TAO_AV_AAL3_4     =  3,
    TAO_AV_AAL1       =  4,
    TAO_AV_RTP_UDP    =  5,
    TAO_AV_RTP_AAL5   =  6,
    TAO_AV_IPX        =  7,
    TAO_AV_SFP_UDP    =  8,
    TAO_AV_UDP_MCAST  =  9,
    TAO_AV_RTP_UDP_MCAST = 10,
    TAO_AV_SFP_UDP_MCAST = 11,
    TAO_AV_QOS_UDP       = 12,
    TAO_AV_USERDEFINED_UDP       = 13,
    TAO_AV_USERDEFINED_UDP_MCAST       = 14
  };

  enum Flow_Component
  {
    TAO_AV_DATA = 1,
    TAO_AV_CONTROL = 2,
    TAO_AV_BOTH = 3
  };

  /// Default constructor.
  TAO_AV_Core (void);

  /// Destructor.
  ~TAO_AV_Core (void);

  int init (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa
            ACE_ENV_ARG_DECL);
  int run (void);
  int stop_run (void);
  int init_forward_flows (TAO_Base_StreamEndPoint *endpoint,
                          TAO_AV_FlowSpecSet &flow_spec_set,
                          EndPoint direction,
                          AVStreams::flowSpec &flow_spec);
  int init_reverse_flows (TAO_Base_StreamEndPoint *endpoint,
                          TAO_AV_FlowSpecSet &forward_flow_spec_set,
                          TAO_AV_FlowSpecSet &reverse_flow_spec_set,
                          EndPoint direction);
  int init_transport_factories (void);
  int init_flow_protocol_factories (void);

  int load_default_transport_factories (void);
  int load_default_flow_protocol_factories (void);

  /// = Get the acceptor registry
  TAO_AV_Acceptor *get_acceptor (const char *flowname);
  TAO_AV_Connector *get_connector (const char *flowname);
  int remove_acceptor (const char *flowname);
  int remove_connector (const char *flowname);
  TAO_AV_Connector_Registry *connector_registry (void);
  TAO_FlowSpec_Entry *get_flow_spec_entry (TAO_AV_FlowSpecSet &flow_spec_set,
                                           const char *flowname);
  TAO_AV_Acceptor_Registry  *acceptor_registry  (void);

  // = Get the protocol factories
  /// = Set/get the <ACE_Reactor>.
  TAO_AV_Flow_Protocol_Factory *get_flow_protocol_factory(const char *flow_protocol);
  TAO_AV_Transport_Factory *get_transport_factory(const char *transport_protocol);
  TAO_AV_Flow_ProtocolFactorySet *flow_protocol_factories (void);
  TAO_AV_TransportFactorySet *transport_factories (void);
  void reactor (ACE_Reactor *r);
  ACE_Reactor *reactor (void);
  CORBA::ORB_ptr orb (void);
  void orb (CORBA::ORB_ptr orb_);
  PortableServer::POA_ptr poa (void);
  void poa (PortableServer::POA_ptr poa_);

  static int deactivate_servant (PortableServer::Servant servant);
  static char *get_flowname (const char *flow_spec_entry_str);
  static ACE_CString get_control_flowname(const char *flowname);

protected:
  /// The connector registry which all active connecters must register
  /// themselves with.
  TAO_AV_Connector_Registry *connector_registry_;

  /// The registry which maintains a list of acceptor factories for each
  /// loaded protocol.
  TAO_AV_Acceptor_Registry *acceptor_registry_;

  /// Pointer to the list of transports loaded into this AV_Core instance.
  TAO_AV_TransportFactorySet transport_factories_;

  /// Pointer to the list of flow protocol loaded into this AV_Core instance.
  TAO_AV_Flow_ProtocolFactorySet flow_protocol_factories_;

  ACE_Reactor *reactor_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  CORBA::Boolean stop_run_;
};

typedef ACE_Singleton<TAO_AV_Core, ACE_Null_Mutex> TAO_AV_CORE;

#if defined (__BORLANDC__)
# if !defined (TAO_AV_BUILD_DLL)
#   pragma option push -Jgx
# endif
#endif
TAO_AV_SINGLETON_DECLARE (ACE_Singleton, TAO_AV_Core, ACE_Null_Mutex)
#if defined (__BORLANDC__)
# if !defined(TAO_AV_BUILD_DLL)
#   pragma option pop
# endif
#endif

#include /**/ "ace/post.h"
#endif /* TAO_AV_CORE_H */
