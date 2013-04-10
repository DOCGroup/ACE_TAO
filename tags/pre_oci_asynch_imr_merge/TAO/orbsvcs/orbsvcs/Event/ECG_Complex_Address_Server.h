// -*- C++ -*-

/**
 *  @file   ECG_Complex_Address_Server.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@atdesk.com)
 *
 */

#ifndef TAO_ECG_COMPLEX_ADDRESS_SERVER_H
#define TAO_ECG_COMPLEX_ADDRESS_SERVER_H

#include /**/ "ace/pre.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

#include "orbsvcs/RtecUDPAdminS.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/INET_Addr.h"
#include "ace/Null_Mutex.h"

#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ECG_Complex_Address_Server
 *
 * @brief Implementation of RtecUDPAdmin idl interface, which returns
 * a different multicast address based on event source (or event type
 * - depending on how the server is configured).
 *
 * INITIALIZATION STRING FORMAT
 *
 * The string is a sequence of <key>@<value> pairs separated by
 * a single space, where <key> is event source (or type)
 * and <value> is the corresponding mcast address.  Example:
 * "34@230.100.0.2:2300 45@230.100.123.43:2300"
 * The string above represents two key-value pairs.
 *
 * A special key "*" is used to specify the default mcast address,
 * i.e., the one that will be returned for event sources that weren't
 * explicitly specified in the initialization string.  Example:
 * "*@230.100.0.2:2300 45@230.100.123.43:2300"
 *
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Complex_Address_Server :
  public virtual POA_RtecUDPAdmin::AddrServer
{
public:

  /// Create a new TAO_ECG_Complex_Address_Server object.
  /*
   * (Constructor access is restricted to insure that all
   * TAO_ECG_Complex_Address_Server objects are heap-allocated.)
   * <is_source_mapping> flag indicates whether this
   * server maps based on event header source or event header type.
   */
  static PortableServer::Servant_var<TAO_ECG_Complex_Address_Server>
         create (int is_source_mapping = 1);

  /// Destructor
  virtual ~TAO_ECG_Complex_Address_Server (void);

  /// Initializes the mapping from the <arg> string.  See class notes
  /// for the expected format.
  int init (const char *arg);

  // = The RtecUDPAdmin::AddrServer methods
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr);
  virtual void get_address (const RtecEventComm::EventHeader& header,
                            RtecUDPAdmin::UDP_Address_out addr6);

  /// Prints out complete content of the address server. Useful for
  /// debugging.
  void dump_content (void);

protected:

  /// Constructor (protected).  Clients can create new
  /// TAO_ECG_Complex_Address_Server objects using the static create()
  /// method.
  /// <is_source_mapping> flag indicates whether this
  /// server maps based on event header source or event header type.
  TAO_ECG_Complex_Address_Server (int is_source_mapping = 1);

private:

  /// Helper.  Given key and mcast address in string form, add them to
  /// the mapping.
  int add_entry (const char * key, const char * mcast_addr);

  /// Flag indicating whether this address server maps event source or
  /// event type to mcast groups.
  int is_source_mapping_;

  typedef ACE_Hash_Map_Manager_Ex<CORBA::Long, ACE_INET_Addr,
    ACE_Hash<CORBA::Long>, ACE_Equal_To<CORBA::Long>, ACE_Null_Mutex> MAP;

  MAP mcast_mapping_;

  /// Mcast group to be used for all sources (or types) not explicitly
  /// mapped.
  ACE_INET_Addr default_addr_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_Complex_Address_Server.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_ECG_COMPLEX_ADDRESS_SERVER_H */
