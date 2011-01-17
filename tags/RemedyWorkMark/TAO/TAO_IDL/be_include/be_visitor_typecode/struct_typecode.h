// -*- C++ -*-

//=============================================================================
/**
 *  @file  struct_typecode.h
 *
 *  $Id$
 *
 *  Structure TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_STRUCT_TYPECODE_H
#define TAO_BE_VISITOR_STRUCT_TYPECODE_H

namespace TAO
{

  /**
   * @class be_visitor_struct_typecode
   *
   * @brief Structure TypeCode generation visitor.
   *
   * Structure TypeCode generation visitor.
   */
  class be_visitor_struct_typecode
    : public be_visitor_typecode_defn
  {
  public:

    /// Constructor.
    be_visitor_struct_typecode (be_visitor_context * ctx);

    /// Visit a structure.
    /**
     * @todo The legacy method name "@c visit_structure()" is
     *       redundant since it is obvious from the argument what kind
     *       of TypeCode is being visited.  It will be changed once
     *       the rest of the legacy method names and their call sites
     *       are updated accordingly.
     */
    virtual int visit_structure (be_structure * node);

    /// Visit an exception.
    /**
     * @todo The legacy method name "@c visit_exception()" is
     *       redundant since it is obvious from the argument what kind
     *       of TypeCode is being visited.  It will be changed once
     *       the rest of the legacy method names and their call sites
     *       are updated accordingly.
     */
    virtual int visit_exception (be_exception * node);

  private:

    /// Underlying structure/exception visitor code.
    /**
     * @note Structure/exception TypeCode generation really starts in
     *       this method.
     */
    int visit (AST_Structure * node, bool is_exception);

    /// Generate structure field TypeCode declarations, if necessary.
    int gen_member_typecodes (AST_Structure * node);

    /// Generate structure field related TypeCode code.
    int visit_members (AST_Structure * node);

  private:

    /// @c true if the struct is recursive.
    /**
     * @note Only applies for struct TypeCodes.  Exceptions cannot be
     *       recursive.
     */
    bool is_recursive_;

    /// @c true if this visitor is already visiting an outer scope
    /// struct or exception
    bool is_nested_;

  };

}

#endif  /* TAO_BE_VISITOR_STRUCT_TYPECODE_H */
