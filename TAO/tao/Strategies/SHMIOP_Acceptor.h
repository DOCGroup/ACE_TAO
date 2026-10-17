// -*- C++ -*-
// ===================================================================
/**
 *  @file   SHMIOP_Acceptor.h
 *
 *  @brief  SHMIOP specific acceptor processing
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_SHMIOP_ACCEPTOR_H
#define TAO_SHMIOP_ACCEPTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Transport_Acceptor.h"
#include "tao/Strategies/SHMIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "ace/Acceptor.h"
#include "ace/MEM_Acceptor.h"
#include "tao/GIOP_Message_Version.h"
// TAO SHMIOP_Acceptor concrete call definitions

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_SHMIOP_Acceptor
 *
 * @brief The SHMIOP-specific bridge class for the concrete acceptor.
 */
class TAO_Strategies_Export TAO_SHMIOP_Acceptor : public TAO_Acceptor
{
public:
  /// Constructor.
  TAO_SHMIOP_Acceptor ();

  /// Destructor.
  ~TAO_SHMIOP_Acceptor ();

  using TAO_SHMIOP_BASE_ACCEPTOR = TAO_Strategy_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>;
  using TAO_SHMIOP_CREATION_STRATEGY = TAO_Creation_Strategy<TAO_SHMIOP_Connection_Handler>;
  using TAO_SHMIOP_CONCURRENCY_STRATEGY = TAO_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler>;
  using TAO_SHMIOP_ACCEPT_STRATEGY = TAO_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR>;

  /**
   * @name The TAO_Acceptor Methods
   *
   * Please check the documentation in Transport_Acceptor.h for details.
   */
  //@{
  int open (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    int version_major,
                    int version_minor,
                    char const *port,
                    char const *options = nullptr) override;
  int open_default (TAO_ORB_Core *orb_core,
                            ACE_Reactor *reactor,
                            int version_major,
                            int version_minor,
                            char const *options = nullptr) override;
  int close () override;
  int create_profile (TAO::ObjectKey const &object_key,
                              TAO_MProfile &mprofile,
                      CORBA::Short priority) override;

  int is_collocated (TAO_Endpoint const *endpoint) override;
  CORBA::ULong endpoint_count () override;

  int object_key (IOP::TaggedProfile &profile,
                  TAO::ObjectKey &key) override;
  //@}

  /// Set the MMAP options the MEM_Stream this acceptor creates will
  /// use.
  int set_mmap_options (ACE_TCHAR const *prefix,
                        ACE_OFF_T size);

private:
  /// Implement the common part of the open*() methods.
  int open_i (TAO_ORB_Core *orb_core,
              ACE_Reactor *reactor);

  /// Parse protocol specific options.
  virtual int parse_options (char const *options);

  /// Create a SHMIOP profile representing this acceptor.
  int create_new_profile (TAO::ObjectKey const &object_key,
                          TAO_MProfile &mprofile,
                          CORBA::Short priority);

  /// Add the endpoints on this acceptor to a shared profile.
  int create_shared_profile (TAO::ObjectKey const &object_key,
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
  TAO_SHMIOP_Acceptor (TAO_SHMIOP_Acceptor const &) = delete;
  TAO_SHMIOP_Acceptor &operator= (TAO_SHMIOP_Acceptor const &) = delete;
  TAO_SHMIOP_Acceptor (TAO_SHMIOP_Acceptor &&) = delete;
  TAO_SHMIOP_Acceptor &operator= (TAO_SHMIOP_Acceptor &&) = delete;

  /// The concrete acceptor, as a pointer to it's base class.
  TAO_SHMIOP_BASE_ACCEPTOR base_acceptor_;

  /// Acceptor strategies.
  TAO_SHMIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_SHMIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_SHMIOP_ACCEPT_STRATEGY *accept_strategy_;

  /// Determine the prefix (include path name) of the mmap file.
  ACE_TCHAR const *mmap_file_prefix_;

  /// Determine the minimum size of mmap file.  This dictate the
  /// maximum size of a CORBA method invocation.
  ACE_OFF_T mmap_size_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_SHMIOP_ACCEPTOR_H */
