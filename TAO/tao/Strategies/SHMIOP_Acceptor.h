// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    SHMIOP_Acceptor.h
//
// = DESCRIPTION
//    SHMIOP specific acceptor processing
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#ifndef TAO_SHMIOP_ACCEPTOR_H
#define TAO_SHMIOP_ACCEPTOR_H
#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Transport_Acceptor.h"
#include "SHMIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "ace/Acceptor.h"
#include "ace/MEM_Acceptor.h"
#include "tao/GIOP_Message_Version.h"
// TAO SHMIOP_Acceptor concrete call defination

/**
 * @class TAO_SHMIOP_Acceptor
 *
 * @brief The SHMIOP-specific bridge class for the concrete acceptor.
 *
 */
class TAO_Strategies_Export TAO_SHMIOP_Acceptor : public TAO_Acceptor
{
public:
  /// Constructor.
  TAO_SHMIOP_Acceptor (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_SHMIOP_Acceptor (void);

  typedef ACE_Strategy_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR> TAO_SHMIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_SHMIOP_Connection_Handler> TAO_SHMIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler> TAO_SHMIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR> TAO_SHMIOP_ACCEPT_STRATEGY;

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
                    const char *port,
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

  /// Set the MMAP options the MEM_Stream this acceptor creates will
  /// use.
  int set_mmap_options (const ACE_TCHAR *prefix,
                        off_t size);

private:
  /// Implement the common part of the open*() methods.
  int open_i (TAO_ORB_Core* orb_core,
              ACE_Reactor *reactor);

  /// Parse protocol specific options.
  virtual int parse_options (const char *options);

  /// Create a SHMIOP profile representing this acceptor.
  int create_new_profile (const TAO::ObjectKey &object_key,
                          TAO_MProfile &mprofile,
                          CORBA::Short priority);

  /// Add the endpoints on this acceptor to a shared profile.
  int create_shared_profile (const TAO::ObjectKey &object_key,
                             TAO_MProfile &mprofile,
                             CORBA::Short priority);

protected:
  /// Cache the information about the endpoint serviced by this
  /// acceptor.
  /// @todo There may in fact be multiple hostnames for this endpoint.
  ///    For example it the IP address is INADDR_ANY
  ///    (0.0.0.0) then there will be possibly a different hostname for
  ///    each interface.
  ACE_CString host_;

  /// A local endpoint.
  ACE_MEM_Addr address_;

  /// The GIOP version for this endpoint
  TAO_GIOP_Message_Version version_;

  /// ORB Core.
  TAO_ORB_Core *orb_core_;

private:
  /// The concrete acceptor, as a pointer to it's base class.
  TAO_SHMIOP_BASE_ACCEPTOR base_acceptor_;

  /// Acceptor strategies.
  TAO_SHMIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_SHMIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_SHMIOP_ACCEPT_STRATEGY *accept_strategy_;

  /// Determine the prefix (include path name) of the mmap file.
  const ACE_TCHAR *mmap_file_prefix_;

  /// Determine the minimum size of mmap file.  This dictate the
  /// maximum size of a CORBA method invocation.
  off_t mmap_size_;

  /// Should we use GIOP lite??
  CORBA::Boolean lite_flag_;
};

#if defined(__ACE_INLINE__)
#include "SHMIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_SHMIOP_ACCEPTOR_H */
