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

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_T.h"


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

  /// <priority_> attribute setter.
  void priority (CORBA::Short priority);

  /// <priority_> attribute getter.
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

  /// Endpoints can be stringed in a list.
  /**
   * @return The next endpoint in the list, if any.
   */
  virtual TAO_Endpoint *next (void) = 0;

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
  virtual CORBA::ULong hash (void) = 0;

protected:

  /// Lock for the address lookup.
  /**
   * @todo This lock should be strategized so that we dont lock in
   *       single threaded configurations.  It is not possible to do
   *       this now as most of the information is available in the
   *       ORB_Core which is not available here.
   */
  mutable TAO_SYNCH_MUTEX addr_lookup_lock_;

private:

  /// Endpoints should not be copied.
  ACE_UNIMPLEMENTED_FUNC (TAO_Endpoint (const TAO_Endpoint&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Endpoint&))

private:

  /// IOP tag, identifying the protocol for which this endpoint
  /// contains addressing info.
  CORBA::ULong tag_;

  /**
   * CORBA priority of the acceptor this Endpoint is representing.
   * This is part of TAO 'prioritized endpoints' architecture, and is
   * currently used for RTCORBA only.
   */
  CORBA::Short priority_;

};


#if defined (__ACE_INLINE__)
# include "tao/Endpoint.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_PROFILE_H */
