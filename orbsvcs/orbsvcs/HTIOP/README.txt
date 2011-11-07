$Id$

      HTTP Tunneling Inter-ORB Protocol

Introduction
Components
HTTP Tunneling Protocol
HTTP Tunneling Inter-ORB Protocol

Introduction



1.Terminology

Inside - This refers to any peer that is inside a firewall protected
enclave, and must forward requests to services through a proxy. It is
assumed with this protocol that the proxy restricts traffic to HTTP
requests/replies.

Outside - This refers to any peer is outside of the firewall
protecting inside peers. The outside peer advertises an endpoint that
is reachable from the inside peer's proxy.

Proxy - The gateway through which HTTP requests may flow. It is
assumed that no configuration changes to the proxy are permitted. For
now, Squid is the only HTTP proxy that is supported by HTIOP, although
others may also work.

HTID - A unique identifier for peers inside the firewall. This may be
a self-generated UUID, or it may be an externally generated
domain-specific identity. This identity is important to allow outside
peers to identify many different clients, which may reside on locally
similar or non-routable networks, such as 10.*.*.* or 192.168.*.*.

HTIOP_Factory - This is the service configuration class used to load
the pluggable protocol via the ACE Service Configuratior. The
HTIOP_Factory takes the following initialization arguments:

-config <filename> Specifies the text file containing HTBP specific
                   configuration. This value is optional.
-env_persist <filename> Specifies the name of a flat file containing
                   a previously persisted configuration, or to which
                   new configuration will be persisted. If both
                   -config and -env_persist are specified, the memory
                   mapped file will be loaded first, then the text
                   file interpreted. The new configuration will then
                   be persisted in the flat file.
-win32_reg         If set, then the windows' registry will be used
                   rather than the memory mapped file. This feature
                   is available only on windows.
-inside [-1|0|1]   Explicitly declare that the endpoint is inside or
                   outside the firewall. When set to the default -1,
                   the HTIOP_Factory uses the ACE_HTBP_Environment
                   proxy_host setting to determine if it is inside or
                   outside the firewall. It is only important to be
                   explicit about inside/outside definition when
                   testing without a real proxy. HTIOP is an
                   asymmetric protocol, peers outside the firewall
                   cannot initiate connections to inside peers, and
                   only an inside peer can talk to an outside peer.

2. Components

The HTTP tunneling protocol support consists of two libraries.

. libACE_HTBP is the library that implements the low level HTBP
Protocol. This allows the the communication between the inside and the
outside peers through a firewall proxy. It supports direct connections
as well though the default is the use of a proxy. This library should
be linked into code that wishes to support such communication.

. libTAO_HTIOP implements an Inter-ORB Protocol, HTIOP, that allows
TAO to utilize the HTBP Protocol.

3.  Directory Structure

. $ACE_ROOT/ace/HTBP
	Files used for implementing the low level HTBP Protocol.

. $TAO_ROOT/orbsvcs/orbsvcs/HTIOP
	Files that implements HTIOP protocol.

. $ACE_ROOT/tests/HTBP
	Tests for the low level HTBP Protocol.

. $TAO_ROOT/orbsvcs/tests/HTIOP
	Tests for the HTIOP protocol.

4. Requirements for using HTBP Protocol
  Refer to ace/HTBP/README for details on configuring HTBP clients

5. Using HTIOP in applications

  HTIOP is an Asymmetric protocol. This means that applications inside
  the firewall must be configured differently than applications on the
  outside. Inside peers are the only ones which may initiate
  connections. If a peer to peer relationship is desired, where CORBA
  invocations flow in either direction, then Bidirectional GIOP must
  be used.

  One of the challenges imposed by the use of a proxy is that if a
  connection is idle for long enough the connection may be
  closed. This means the inside peer must reopen it by sending another
  request. Since the outside peer cannot reopen the connection, it
  will queue messages until the inside peer re-establishes a
  connection, at which time all queued messages are sent.

  A second challenge is that while a proxy will open multiple TCP/IP
  connections to a server (the outside peer), when faced with multiple
  simultanious HTTP requests, it will reuse those connections at will
  to forward any subsequent requests. This means that a socket is
  associated with a particular HTIOP session for only one HTTP
  request/reply.

  Apart from loading the protocol, and declaring an endpoint, no
  modifications to application code are required in order to use
  HTIOP.

5a. Loading the protocol.

  HTIOP is loaded via svc.conf. See the list of initialization options
  shown above for details. Here is a sample svc.conf file for loading
  HTIOP, as it would be used in a server.

dynamic HTIOP_Factory Service_Object *
        TAO_HTIOP:_make_TAO_HTIOP_Protocol_Factory () ""

static Resource_Factory "-ORBProtocolFactory HTIOP_Factory"

  This example is specifically for a server since no configuration
  file is specified meaning no proxy host value is set. The following
  examples could be used to configure a client.

#used on the inside, but no proxy configured
dynamic HTIOP_Factory Service_Object *
        TAO_HTIOP:_make_TAO_HTIOP_Protocol_Factory ()
        "-inside 1"

#used on the inside, with proxy configuration supplied
dynamic HTIOP_Factory Service_Object *
        TAO_HTIOP:_make_TAO_HTIOP_Protocol_Factory ()
        "-config HTBP_Config.txt"

static Resource_Factory "-ORBProtocolFactory HTIOP_Factory"

5b. Declaring an endpoint.

  Since most HTTP proxies restrict outgoing connections only to port
  80, it is a requirement that an explicit endpoint be set for
  HTIOP. The signature is very similar to that of IIOP:

  -ORBEndpoint HTIOP://hostname:80[/hostname_in_ior=www.mybiz.com]

  The optional hostname_in_ior allows HTIOP to be used behind a NAT
  firewall.
