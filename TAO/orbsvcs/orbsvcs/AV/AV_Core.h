/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//   ORBSVCS (AVStreams)
//
// = FILENAME
//   AV_Core.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_CORE_H
#define TAO_AV_CORE_H

#include "tao/TAO.h"
#include "orbsvcs/AVStreamsC.h"

class TAO_Base_StreamEndPoint;
class TAO_AV_Connector_Registry;
class TAO_AV_Acceptor_Registry;
class TAO_AV_Acceptor;
class TAO_AV_Connector;
class TAO_FlowSpec_Entry;

//forward declaration.
class TAO_AV_Transport_Factory;

class TAO_AV_Export TAO_AV_Transport_Item
{
public:
  TAO_AV_Transport_Item (const ACE_CString &name);
  // creator method, the Transport name can only be set when the
  // object is created.

  const ACE_CString &name (void);
  // return a reference to the character representation of the Transport
  // factories name.

  TAO_AV_Transport_Factory *factory (void);
  // return a pointer to the Transport factory.

  void factory (TAO_AV_Transport_Factory *factory);
  // set the factory pointer's valus.

private:
  ACE_CString name_;
  // Transport factory name.

  TAO_AV_Transport_Factory *factory_;
  // pointer to factory object.
};

class TAO_AV_Flow_Protocol_Factory;

class TAO_AV_Export TAO_AV_Flow_Protocol_Item
{
public:
  TAO_AV_Flow_Protocol_Item (const ACE_CString &name);
  // creator method, the Flow_Protocol name can only be set when the
  // object is created.

  const ACE_CString &name (void);
  // return a reference to the character representation of the Flow_Protocol
  // factories name.

  TAO_AV_Flow_Protocol_Factory *factory (void);
  // return a pointer to the Flow_Protocol factory.

  void factory (TAO_AV_Flow_Protocol_Factory *factory);
  // set the factory pointer's valus.

private:
  ACE_CString name_;
  // Flow_Protocol factory name.

  TAO_AV_Flow_Protocol_Factory *factory_;
  // pointer to factory object.
};

// typedefs for containers containing the list of loaded Flow_Protocol
// factories.
typedef ACE_Unbounded_Set<TAO_AV_Flow_Protocol_Item*>
        TAO_AV_Flow_ProtocolFactorySet;

typedef ACE_Unbounded_Set_Iterator<TAO_AV_Flow_Protocol_Item*>
        TAO_AV_Flow_ProtocolFactorySetItor;

typedef ACE_Unbounded_Set<TAO_AV_Transport_Item*>
        TAO_AV_TransportFactorySet;

typedef ACE_Unbounded_Set_Iterator<TAO_AV_Transport_Item*>
        TAO_AV_TransportFactorySetItor;

typedef ACE_Unbounded_Set <TAO_FlowSpec_Entry*> TAO_AV_FlowSpecSet;
typedef ACE_Unbounded_Set_Iterator <TAO_FlowSpec_Entry*> TAO_AV_FlowSpecSetItor;

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
    TAO_AV_SFP_UDP_MCAST = 11
  };

  TAO_AV_Core (void);
  // Default constructor.

  ~TAO_AV_Core (void);
  // Destructor.

  int init (int &argc,
            char *argv [],
            CORBA::Environment &env);
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

  TAO_AV_Acceptor *get_acceptor (const char *flowname);
  TAO_AV_Connector *get_connector (const char *flowname);
  TAO_AV_Connector_Registry *connector_registry (void);
  TAO_FlowSpec_Entry *get_flow_spec_entry (TAO_AV_FlowSpecSet &flow_spec_set,
                                           const char *flowname);
  // = Get the acceptor registry
  TAO_AV_Acceptor_Registry  *acceptor_registry  (void);

  // = Get the protocol factories
  TAO_AV_Flow_ProtocolFactorySet *flow_protocol_factories (void);
  TAO_AV_TransportFactorySet *transport_factories (void);
  // = Set/get the <ACE_Reactor>.
  void reactor (ACE_Reactor *r);
  ACE_Reactor *reactor (void);
  TAO_ORB_Manager *orb_manager (void);

protected:
  TAO_AV_Connector_Registry *connector_registry_;
  // The connector registry which all active connecters must register
  // themselves with.

  TAO_AV_Acceptor_Registry *acceptor_registry_;
  // The registry which maintains a list of acceptor factories for each
  // loaded protocol.

  TAO_AV_TransportFactorySet transport_factories_;
  // Pointer to the list of transports loaded into this AV_Core instance.

  TAO_AV_Flow_ProtocolFactorySet flow_protocol_factories_;
  // Pointer to the list of flow protocol loaded into this AV_Core instance.

  ACE_Reactor *reactor_;
  CORBA::ORB_var orb_;
  TAO_ORB_Manager orb_manager_;
  CORBA::Boolean stop_run_;
};

#endif /* TAO_AV_CORE_H */
