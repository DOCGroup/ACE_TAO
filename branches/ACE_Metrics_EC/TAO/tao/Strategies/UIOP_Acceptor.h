// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    UIOP_Acceptor.h
//
// = DESCRIPTION
//    Unix Domain Socket (UIOP) specific acceptor processing
//
// = AUTHOR
//    Fred Kuhns <fredk@cs.wustl.edu>
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_ACCEPTOR_H
#define TAO_UIOP_ACCEPTOR_H
#include "ace/pre.h"

#include "tao/Transport_Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "ace/Acceptor.h"
#include "ace/LSOCK_Acceptor.h"
#include "UIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "tao/GIOP_Message_Version.h"

/**
 * @class TAO_UIOP_Acceptor
 *
 * @brief The UIOP-specific bridge class for the concrete acceptor.
 */
class TAO_Strategies_Export TAO_UIOP_Acceptor : public TAO_Acceptor
{
public:
  // TAO_UIOP_Acceptor (ACE_UNIX_Addr &addr);
  // Create Acceptor object using addr.

  /// Create Acceptor object using addr.
  TAO_UIOP_Acceptor (CORBA::Boolean flag = 0);

  /// Destructor
  virtual ~TAO_UIOP_Acceptor (void);

  typedef ACE_Strategy_Acceptor<TAO_UIOP_Connection_Handler, ACE_LSOCK_ACCEPTOR> TAO_UIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_UIOP_Connection_Handler> TAO_UIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_UIOP_Connection_Handler> TAO_UIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_UIOP_Connection_Handler, ACE_LSOCK_ACCEPTOR> TAO_UIOP_ACCEPT_STRATEGY;

  /**
   * @name The TAO_Acceptor Methods
   *
   * Please check the documentation in Transport_Acceptor.h for details.
   */
  //@{
  virtual int open (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0);
  virtual int open_default (TAO_ORB_Core *orb_core,
                            ACE_Reactor *reactor,
                            int version_major,
                            int version_minor,
                            const char *options = 0);
  virtual int close (void);
  virtual int create_profile (const TAO::ObjectKey &object_key,
                              TAO_MProfile &mprofile,
                              CORBA::Short priority);

  virtual int is_collocated (const TAO_Endpoint* endpoint);
  virtual CORBA::ULong endpoint_count (void);

  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO::ObjectKey &key);
  //@}

private:
  /// Implement the common part of the open*() methods
  int open_i (const char *rendezvous,
              ACE_Reactor *reactor);

  /// Set the rendezvous point and verify that it is
  /// valid (e.g. wasn't truncated because it was too long).
  void rendezvous_point (ACE_UNIX_Addr &, const char *rendezvous);

  /// Parse protocol specific options.
  int parse_options (const char *options);

  /// Obtains uiop properties that must be used by this acceptor, i.e.,
  /// initializes <uiop_properties_>.
  int init_uiop_properties (void);

  /// Create a UIOP profile representing this acceptor.
  int create_new_profile (const TAO::ObjectKey &object_key,
                          TAO_MProfile &mprofile,
                          CORBA::Short priority);

  /// Add the endpoints on this acceptor to a shared profile.
  int create_shared_profile (const TAO::ObjectKey &object_key,
                             TAO_MProfile &mprofile,
                             CORBA::Short priority);

private:
  /// The concrete acceptor, as a pointer to its base class.
  TAO_UIOP_BASE_ACCEPTOR base_acceptor_;

  // Acceptor strategies.
  TAO_UIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_UIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_UIOP_ACCEPT_STRATEGY *accept_strategy_;

  /// The GIOP version for this endpoint
  TAO_GIOP_Message_Version version_;

  /// ORB Core.
  TAO_ORB_Core *orb_core_;

  /// Flag that determines whether or not the rendezvous point should
  /// be unlinked on close.  This is really only used when an error
  /// occurs.
  int unlink_on_close_;

  /// Should we use GIOP lite??
  CORBA::Boolean lite_flag_;

  /// Properties for all connections accepted by this acceptor.
  TAO_UIOP_Properties uiop_properties_;
};

# endif /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_ACCEPTOR_H */
