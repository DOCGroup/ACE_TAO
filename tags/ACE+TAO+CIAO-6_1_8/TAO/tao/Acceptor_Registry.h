// -*- C++ -*-

//=============================================================================
/**
 *  @file     Acceptor_Registry.h
 *
 *  $Id$
 *
 *  Interface for the TAO pluggable protocol framework.
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_ACCEPTOR_REGISTRY_H
#define TAO_ACCEPTOR_REGISTRY_H

#include /**/ "ace/pre.h"
#include "ace/Unbounded_Set.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/Exception.h"
#include "tao/params.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Addr;
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Acceptor;
class TAO_Acceptor_Filter;
class TAO_Stub;
class TAO_Profile;
class TAO_MProfile;
class TAO_Protocol_Factory;
class TAO_Protocol_Item;

typedef ACE_Unbounded_Set_Iterator<TAO_Protocol_Item *>
        TAO_ProtocolFactorySetItor;

typedef TAO_Acceptor** TAO_AcceptorSetIterator;

/**
 * @class TAO_Acceptor_Registry
 *
 * @brief Acceptor Registry and Generic Acceptor interface definitions.
 * All loaded ESIOP or GIOP acceptor bridges must register with
 * this object.
 *
 * This class maintains a list of acceptor factories for all loaded ORB
 * protocols. There is one Acceptor_Registry per ORB_Core.
 */
class TAO_Export TAO_Acceptor_Registry : private ACE_Copy_Disabled
{
public:
  // = Initialization and termination methods.
  ///  Default constructor.
  TAO_Acceptor_Registry (void);

  ///  Default destructor.
  ~TAO_Acceptor_Registry (void);

  /// Initialize all registered acceptors.  Return -1 on error.
  int open (TAO_ORB_Core *orb_core,
            ACE_Reactor *reactor,
            const TAO_EndpointSet &endpoint_set,
            bool ignore_address);

  /// Close all open acceptors.
  int close_all (void);

  /// Returns the total number of endpoints in all of its acceptors.
  size_t endpoint_count (void);

  /// Check if there is at least one profile in @a mprofile that
  /// corresponds to a collocated object.
  bool is_collocated (const TAO_MProfile& mprofile);

  /// Return the acceptor bridges
  TAO_Acceptor *get_acceptor (CORBA::ULong tag);

  // = Iterator.
  TAO_AcceptorSetIterator begin (void);
  TAO_AcceptorSetIterator end (void);

private:

  /// Create a default acceptor for all loaded protocols.
  int open_default (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    const char *options);

  /// Create a default acceptor using the specified protocol factory.
  int open_default (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    int major,
                    int minor,
                    TAO_ProtocolFactorySetItor &factory,
                    const char *options);

  /// Open a default acceptor.
  int open_default_i (TAO_ORB_Core *orb_core,
                      ACE_Reactor *reactor,
                      int major,
                      int minor,
                      TAO_ProtocolFactorySetItor &factory,
                      TAO_Acceptor* acceptor,
                      const char *options);

  /// Extract endpoint-specific options from the endpoint string.
  void extract_endpoint_options (ACE_CString &addrs,
                                 ACE_CString &options,
                                 TAO_Protocol_Factory *factory);

  /// Extract endpoint/address specific version from the endpoint
  /// string.
  void extract_endpoint_version (ACE_CString &address,
                                 int &major,
                                 int &minor);

  /// Iterator through addrs in the string <iop>, and create an
  /// acceptor for each one.
  int open_i (TAO_ORB_Core *orb_core,
              ACE_Reactor *reactor,
              ACE_CString &address,
              TAO_ProtocolFactorySetItor &factory,
              bool ignore_address);

private:
  /// List of acceptors that are currently open.
  TAO_Acceptor **acceptors_;

  /// Number of acceptors that are currently open.
  size_t size_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Registry.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ACCEPTOR_REGISTRY_H */
