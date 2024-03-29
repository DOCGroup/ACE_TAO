// -*- C++ -*-

//=============================================================================
/**
 *  @file    Operation_Table_Linear_Search.h
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_OPERATION_TABLE_LINEAR_SEARCH_H
#define TAO_OPERATION_TABLE_LINEAR_SEARCH_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Operation_Table.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Linear_Search_OpTable
 *
 * @brief Operation table lookup strategy based on
 * linear search.  Not efficient, but it works.
 */
class TAO_PortableServer_Export TAO_Linear_Search_OpTable
  : public TAO_Operation_Table
{
public:
  /// Destructor.
  ~TAO_Linear_Search_OpTable () override = default;

  /// See the documentation in the base class for details.
  int find (const char *opname,
            TAO_Skeleton &skel_ptr,
            const unsigned int length = 0) override;

  int find (const char *opname,
            TAO_Collocated_Skeleton &skelfunc,
            TAO::Collocation_Strategy s,
            const unsigned int length = 0) override;

  int bind (const char *opname,
            const TAO::Operation_Skeletons skelptr) override;

private:
  // = Method that should defined by the subclasses. GPERF program
  //   will generate this routine routines.
  virtual const TAO_operation_db_entry* lookup (const char *str) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_OPERATION_TABLE_LINEAR_SEARCH_H */
