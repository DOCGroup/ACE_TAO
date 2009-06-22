/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Naming_Client.h
 *
 *  $Id$
 *
 *    Implement wrappers useful to Naming Service clients.
 *
 *
 *  @author Nagarajan Surendran (naga@cs.wustl.edu)
 *  @author Matt Braun <mjb2@cs.wustl.edu>
 *  @author and Douglas C. Schmidt <schmidt@cs.wustl.edu>.
 */
//=============================================================================


#ifndef TAO_NAMING_CLIENT_H
#define TAO_NAMING_CLIENT_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/naming_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Naming_Client
 *
 * @brief This class is intended to simplify programs that want to play
 * the role of Naming Service clients.
 *
 * Wrapper class that locates the root Naming Context.  It also
 * defines the operator-> so that <NamingContext> functions like
 * <resolve>, etc. can be directly called on a
 * <TAO_Naming_Client> object, and will be forwarded to the root
 * Naming Context.
 */
class TAO_Naming_Export TAO_Naming_Client
{
public:
  // = Initialization and termination methods.

  /// Default constructor.
  TAO_Naming_Client (void);

  /**
   * Look for a Naming Service for a period of @a timeout using
   * @c resolve_initial_references.  Return 0 if Naming Service is
   * successfully located, and -1 on failure.
   */
  int init (CORBA::ORB_ptr orb, ACE_Time_Value *timeout = 0);

  /// Destructor.
  ~TAO_Naming_Client (void);

  /// Returns a pointer to the root Naming Context.
  CosNaming::NamingContext_ptr operator-> (void) const;

  /**
   * Returns a pointer to the root Naming Context.  This ptr is
   * duplicated via <_duplicate> so that it can be stored into a
   * <*_var>.
   */
  CosNaming::NamingContext_ptr get_context (void) const;

protected:
  /// Reference to the root Naming Context.
  CosNaming::NamingContext_var naming_context_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NAMING_CLIENT_H */
