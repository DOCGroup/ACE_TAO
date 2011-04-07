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

namespace TAO
{
  /**
   * @class be_visitor_objref_typecode
   *
   * @brief TypeCode generation visitor for types with a TypeCode
   *        format similar to an object reference / interface
   *        TypeCode.
   *
   * TypeCode generation visitor for types with a TypeCode format
   * similar to an object reference / interface TypeCode.
   */
  class be_visitor_objref_typecode
    : public be_visitor_typecode_defn
  {
  public:
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

    /// Visit a native.
    /**
     * @see visit_interface
     */
    virtual int visit_native (be_native * node);

  private:
    /// Generate TypeCode instance.
    int visit_i (char const * kind,
                 char const * flat_name,
                 char const * repository_id,
                 char const * original_local_name,
                 be_type * node);
  };
}

#endif  /* TAO_BE_VISITOR_OBJREF_TYPECODE_H */
