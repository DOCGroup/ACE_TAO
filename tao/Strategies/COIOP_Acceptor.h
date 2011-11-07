// -*- C++ -*-

//=============================================================================
/**
 *  @file    COIOP_Acceptor.h
 *
 *  $Id$
 *
 *  COIOP specific acceptor processing
 *
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_COIOP_ACCEPTOR_H
#define TAO_COIOP_ACCEPTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Transport_Acceptor.h"
#include "tao/Acceptor_Impl.h"
#include "tao/GIOP_Message_Version.h"
#include "ace/UUID.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO COIOP_Acceptor concrete call definition

/**
 * @class TAO_COIOP_Acceptor
 *
 * @brief TAO_COIOP_Acceptor
 *
 * The COIOP-specific bridge class for the concrete acceptor.
 */
class TAO_Strategies_Export TAO_COIOP_Acceptor : public TAO_Acceptor
{
public:
  /// Constructor.
  TAO_COIOP_Acceptor (void);

  /// Destructor.
  ~TAO_COIOP_Acceptor (void);

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

  virtual int is_collocated (const TAO_Endpoint *endpoint);
  virtual CORBA::ULong endpoint_count (void);

  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO::ObjectKey &key);
  //@}

protected:

  /// Parse protocol specific options.
  virtual int parse_options (const char *options);

  /// Helper method to add a new profile to the mprofile for
  /// each endpoint.
  int create_new_profile (const TAO::ObjectKey &object_key,
                          TAO_MProfile &mprofile,
                          CORBA::Short priority);

  /// Helper method to create a profile that contains all of
  /// our endpoints.
  int create_shared_profile (const TAO::ObjectKey &object_key,
                             TAO_MProfile &mprofile,
                             CORBA::Short priority);
protected:

  /**
   * Cache the information about the endpoints serviced by this
   * acceptor.
   */
  ACE_Utils::UUID uuid_;

  /**
   * The GIOP version for this endpoint
   * @@ Theoretically they shouldn't be here!! We need to look at a
   * way to move this out
   */
  TAO_GIOP_Message_Version version_;

  /// ORB Core.
  TAO_ORB_Core *orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_COIOP_ACCEPTOR_H */
