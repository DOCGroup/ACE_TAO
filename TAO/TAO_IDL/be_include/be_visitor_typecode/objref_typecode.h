// -*- C++ -*-

//=============================================================================
/**
 *  @file  objref_typecode.h
 *
 *  $Id$
 *
 *  Object reference TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_OBJREF_TYPECODE_H
#define TAO_BE_VISITOR_OBJREF_TYPECODE_H

#include "ace/pre.h"

namespace TAO
{

  /**
   * @class be_visitor_objref_typecode
   *
   * @brief Object reference TypeCode generation visitor.
   *
   * Object reference TypeCode generation visitor.
   */
  class be_visitor_objref_typecode
    : public be_visitor_typecode_defn
  {
  public:

    /// Constructor.
    be_visitor_objref_typecode (be_visitor_context * ctx);

    /// Visit an object reference / interface.
    /**
     * @todo The legacy method name "@c visit_interface()" is redundant
     *       since it is obvious from the argument what kind of
     *       TypeCode is being visited.  It will be changed once the
     *       rest of the legacy method names and their call sites are
     *       updated accordingly.
     */
    virtual int visit_interface (be_interface * node);

  };

}

#include "ace/post.h"

#endif  /* TAO_BE_VISITOR_OBJREF_TYPECODE_H */
