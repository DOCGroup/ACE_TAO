// -*- C++ -*-

//=============================================================================
/**
 *  @file  union_typecode.h
 *
 *  $Id$
 *
 *  Union TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_UNION_TYPECODE_H
#define TAO_BE_VISITOR_UNION_TYPECODE_H

#include "ace/pre.h"

namespace TAO
{

  /**
   * @class be_visitor_union_typecode
   *
   * @brief Union TypeCode generation visitor.
   *
   * Union TypeCode generation visitor.
   */
  class be_visitor_union_typecode
    : public be_visitor_typecode_defn
  {
  public:

    /// Constructor.
    be_visitor_union_typecode (be_visitor_context * ctx);

    /// Visit a union.
    /**
     * @todo The legacy method name "@c visit_union()" is
     *       redundant since it is obvious from the argument what kind
     *       of TypeCode is being visited.  It will be changed once
     *       the rest of the legacy method names and their call sites
     *       are updated accordingly.
     */
    virtual int visit_union (be_union * node);

  private:

    /// Generate union branch related TypeCode code.
    int visit_cases (be_union * node, be_union_branch *& default_case);

  };

}

#include "ace/post.h"

#endif  /* TAO_BE_VISITOR_UNION_TYPECODE_H */
