// -*- C++ -*-

//=============================================================================
/**
 *  @file  enum_typecode.h
 *
 *  $Id$
 *
 *  Enumeration TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_ENUM_TYPECODE_H
#define TAO_BE_VISITOR_ENUM_TYPECODE_H

#include "typecode_defn.h"

namespace TAO
{

  /**
   * @class be_visitor_enum_typecode
   *
   * @brief Enumeration TypeCode generation visitor.
   *
   * Enumeration TypeCode generation visitor.
   */
  class be_visitor_enum_typecode
    : public be_visitor_typecode_defn
  {
  public:

    /// Constructor.
    be_visitor_enum_typecode (be_visitor_context * ctx);

    /// Visit an enumeration.
    /**
     * @todo The legacy method name "@c visit_enum()" is
     *       redundant since it is obvious from the argument what kind
     *       of TypeCode is being visited.  It will be changed once
     *       the rest of the legacy method names and their call sites
     *       are updated accordingly.
     */
    virtual int visit_enum (be_enum * node);

  private:

    /// Generate structure field related TypeCode code.
    int visit_members (be_enum * node);

  };

}

#endif  /* TAO_BE_VISITOR_ENUM_TYPECODE_H */
