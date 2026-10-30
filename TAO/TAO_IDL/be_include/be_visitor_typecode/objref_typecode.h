// -*- C++ -*-

//=============================================================================
/**
 *  @file  objref_typecode.h
 *
 *  Object reference TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_OBJREF_TYPECODE_H
#define TAO_BE_VISITOR_OBJREF_TYPECODE_H

#include <string>

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
    explicit be_visitor_objref_typecode (be_visitor_context * ctx);

    be_visitor_objref_typecode (be_visitor_objref_typecode const &) = delete;
    be_visitor_objref_typecode (be_visitor_objref_typecode &&) = delete;
    be_visitor_objref_typecode &operator= (be_visitor_objref_typecode const &) = delete;
    be_visitor_objref_typecode &operator= (be_visitor_objref_typecode &&) = delete;

    /// Visit an object reference / interface.
    /**
     * @todo The legacy method name "@c visit_interface()" is redundant
     *       since it is obvious from the argument what kind of
     *       TypeCode is being visited.  It will be changed once the
     *       rest of the legacy method names and their call sites are
     *       updated accordingly.
     */
    int visit_interface (be_interface * node) override;

    /// Visit a native.
    /**
     * @see visit_interface
     */
    int visit_native (be_native * node) override;

  private:
    /// Generate TypeCode instance.
    int visit_i (std::string const& kind,
                 std::string const& flat_name,
                 std::string const& repository_id,
                 std::string const& original_local_name,
                 be_type * node);
  };
}

#endif  /* TAO_BE_VISITOR_OBJREF_TYPECODE_H */
