// -*- C++ -*-

//=============================================================================
/**
 *  @file    Operation_Table.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_OPTABLE_H
#define TAO_OPTABLE_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"
#include "tao/Collocation_Strategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServerRequest;
class TAO_Abstract_ServantBase;
class TAO_ServantBase;

namespace TAO
{
  namespace Portable_Server
  {
    class Servant_Upcall;
  }
}

typedef void (*TAO_Skeleton)(
    TAO_ServerRequest &,
    TAO::Portable_Server::Servant_Upcall *,
    TAO_ServantBase *);

typedef void (*TAO_Collocated_Skeleton)(
    TAO_Abstract_ServantBase *,
    TAO::Argument **);

/**
 * @struct TAO_operation_db_entry
 *
 * @brief Define a table entry that holds an operation name and its
 * corresponding skeleton.  A table of such entries is used to
 * initialize the different lookup strategies.
 */
struct TAO_operation_db_entry
{
  /// Operation name
  char const * opname;

  /// Remote/thru-POA skeleton pointer
  TAO_Skeleton skel_ptr;

  /// Collocated skeleton pointers.
  TAO_Collocated_Skeleton direct_skel_ptr;
};

// --------------------------

namespace TAO
{
  /**
   * @class Operation_Skeleton_Ptr
   *
   * @brief A logical aggregation of all the operation skeleton pointers
   * in use.
   *
   * This is not used by the IDL compiler. This is used internally
   * within different strategies.
   */
  struct Operation_Skeletons
  {
    Operation_Skeletons (void);

    /// Remote skeleton pointer
    TAO_Skeleton skel_ptr;

    /// Collocated skeleton pointers.
    TAO_Skeleton thruPOA_skel_ptr;
    TAO_Collocated_Skeleton direct_skel_ptr;
  };
}

/**
 * @class TAO_Operation_Table
 *
 * @brief Abstract class for maintaining and lookup of CORBA IDL
 * operation names.
 */
class TAO_PortableServer_Export TAO_Operation_Table
{
public:
  /**
   * Uses @a opname to look up the skeleton function and pass it back
   * in @a skelfunc.  Returns non-negative integer on success, or -1
   * on failure.
   */
  virtual int find (const char *opname,
                    TAO_Skeleton &skelfunc,
                    const unsigned int length = 0) = 0;

  /**
   * Uses @a opname to look up the collocated skeleton function and
   * pass it back in @a skelfunc.  Returns non-negative integer on
   * success, or -1 on failure.
   */
  virtual int find (const char *opname,
                    TAO_Collocated_Skeleton &skelfunc,
                    TAO::Collocation_Strategy s,
                    const unsigned int length = 0) = 0;

  /// Associate the skeleton @a skel_ptr with an operation named
  /// @a opname.  Returns -1 on failure, 0 on success, 1 on duplicate.
  virtual int bind (const char *opname,
                    const TAO::Operation_Skeletons skel_ptr) = 0;

  virtual ~TAO_Operation_Table (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_OPTABLE_H */
