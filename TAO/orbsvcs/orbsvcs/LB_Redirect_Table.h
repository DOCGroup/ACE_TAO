// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_Redirect_Table.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_REDIRECT_TABLE_H
#define TAO_LB_REDIRECT_TABLE_H

#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Functor.h"


/**
 * @class TAO_LB_Redirect_Table
 *
 * @brief Maintain a table of "redirects" corresponding to a target
 *        with the given RepositoryId.
 *
 * If a table entry for the given RepositoryId exists, then the load
 * monitor will redirect the client request to the target pointed to
 * by the object reference in that entry.
 *
 * It is necessary to maintain a redirect table since the load
 * balancer determines which object group to select a replica from by
 * examining the target's object ID.  As such a request cannot simply
 * be forwarded back to the load balancer without knowing what its
 * intended target (i.e. object group) is.  This table keeps track of
 * the object group reference to which a replica of a given type
 * belongs.
 */
class TAO_LB_Redirect_Table
{
public:

  typedef ACE_Hash_Map_Manager_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex> Table;

  /// Constructor
  TAO_LB_Redirect_Table (void);

  /// Destructor
  ~TAO_LB_Redirect_Table (void);

  /// Register an object reference with the table, and map the given
  /// ID to it.
  void register_redirect (const char * type_id,
                          CORBA::Object_ptr redirect_to,
                          CORBA::Environment &ACE_TRY_ENV);

  /// Remove the redirect associated with the given RepositoryId.
  void remove_redirect (const char *type_id,
                        CORBA::Environment &ACE_TRY_ENV);

  /// Check if a redirect exists for the current target.  If so, then
  /// perform the request redirection by throwing a
  /// PortableInterceptor::ForwardRequest exception.
  void find_redirect (PortableInterceptor::ServerRequestInfo_ptr ri,
                      CORBA::Environment &ACE_TRY_ENV);

  /// Reclaim the resources held by this table (deallocate strings,
  /// release object references, etc).
  void destroy (void);

private:

  /// Helper method that binds the redirect to the given RepositoryId.
  int bind (const char *type_id, CORBA::Object_ptr redirect_to);

private:

  /// Prevent copying
  ACE_UNIMPLEMENTED_FUNC (TAO_LB_Redirect_Table (const TAO_LB_Redirect_Table &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_LB_Redirect_Table &))

private:

  /// The implementation.
  Table table_;

};

#include "ace/post.h"

#endif  /* TAO_LB_REDIRECT_TABLE_H */
