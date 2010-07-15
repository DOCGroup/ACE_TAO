// -*- C++ -*-

//=============================================================================
/**
 *  @file   Endpoint.h
 *
 *  $Id$
 *
 * Endpoint interface, part of TAO pluggable protocol framework.
 *
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ENDPOINT_H
#define TAO_ENDPOINT_H

#include /**/ "ace/pre.h"
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/*
 * Includes and forward decls for specializing TAO's
 * endpoint implementation.
 */
//@@ TAO_ENDPOINT_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

/**
 * @class TAO_Endpoint
 *
 * @brief Defines the Endpoint interface in the Pluggable Protocol
 * framework.
 *
 * Lightweight encapsulation of addressing information for a
 * single acceptor endpoint.  In other words, Endpoint represents
 * a single point of contact for the server, and is the smallest
 * unit of addressing information necessary for a client to connect
 * to a server.
 * A Profile contains one or more Endpoints, i.e., knows of
 * one or more ways to contact server(s).
 */
class TAO_Export TAO_Endpoint
{
public:
  /// Constructor.
  TAO_Endpoint (CORBA::ULong tag,
                CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Destructor.
  virtual ~TAO_Endpoint (void);

  /// IOP protocol tag accessor.
  CORBA::ULong tag (void) const;

  /// @c priority_ attribute setter.
  void priority (CORBA::Short priority);

  /// @c priority_ attribute getter.
  CORBA::Short priority (void) const;

  /**
   * @name TAO_Endpoint Template Methods
   *
   * Abstract methods to be implemented by concrete subclasses.
   */
  //@{
  /**
   * @return true if this endpoint is equivalent to @a other_endpoint.
   */
  virtual CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint) = 0;

  /// Endpoints can be linked in a list.
  /**
   * @return The next endpoint in the list, if any.
   */
  virtual TAO_Endpoint *next (void) = 0;

  /**
   * Return the next endpoint in the list, but use protocol-specific
   * filtering to constrain the value. The orb core is needed to supply
   * any sort of filter arguments, and the root endpoint is needed in case
   * the algorithm needs to rewind. If the supplied root is 0, then this
   * is assumed to be the candidate next endpoint.
   *
   * To use this, the caller starts off the change with root == 0. This
   * is a bit of a violation in logic, a more correct implementation would
   * accept this == 0 and a non-null root.
   * To do iteration using next_filtered, do:
   *   for (TAO_Endpoint *ep = root_endpoint->next_filtered (orb_core, 0);
   *        ep != 0;
   *        ep = ep->next_filtered(orb_core, root_endpoint)) { }
   */
  virtual TAO_Endpoint *next_filtered (TAO_ORB_Core *, TAO_Endpoint *root);

  /// Return a string representation for the address.
  /**
   * The purpose of this method is to provide a general interface to
   * the underlying address object's @c addr_to_string method.  This
   * allows the protocol implementor to select the appropriate string
   * format.
   *
   * @return -1 if buffer is too small.
   */
  virtual int addr_to_string (char *buffer, size_t length) = 0;

  /// This method returns a deep copy of the corresponding endpoints by
  /// allocating memory.
  virtual TAO_Endpoint *duplicate (void) = 0;

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void)  = 0;

  /*
   * Hook to add public methods from derived class onto base
   * Endpoint class.
   */
  //@@ TAO_ENDPOINT_SPL_PUBLIC_METHODS_ADD_HOOK

protected:

  /// Lock for the address lookup.
  /**
   * @todo This lock should be strategized so that we dont lock in
   *       single threaded configurations.  It is not possible to do
   *       this now as most of the information is available in the
   *       ORB_Core which is not available here.
   */
  mutable TAO_SYNCH_MUTEX addr_lookup_lock_;

  /// Cache the hash value
  CORBA::ULong hash_val_;

  /// IOP tag, identifying the protocol for which this endpoint
  /// contains addressing info.
  CORBA::ULong const tag_;

  /**
   * CORBA priority of the acceptor this Endpoint is representing.
   * This is part of TAO 'prioritized endpoints' architecture, and is
   * currently used for RTCORBA only.
   */
  CORBA::Short priority_;

private:

  /// Endpoints should not be copied.
  TAO_Endpoint (const TAO_Endpoint&);
  void operator= (const TAO_Endpoint&);

  /*
   * Addition of private members from derived class.
   */
  //@@ TAO_ENDPOINT_SPL_PRIVATE_DATA_ADD_HOOK

};

//@@ TAO_ENDPOINT_SPL_EXTERN_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Endpoint.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_PROFILE_H */
