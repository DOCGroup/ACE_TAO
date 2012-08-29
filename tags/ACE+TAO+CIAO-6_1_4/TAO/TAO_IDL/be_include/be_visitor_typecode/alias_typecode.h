// -*- C++ -*-

//=============================================================================
/**
 *  @file  alias_typecode.h
 *
 *  $Id$
 *
 *  Alias (typedef) TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_ALIAS_TYPECODE_H
#define TAO_BE_VISITOR_ALIAS_TYPECODE_H

namespace TAO
{

  /**
   * @class be_visitor_alias_typecode
   *
   * @brief Alias (typedef) TypeCode generation visitor.
   *
   * Alias (typedef) TypeCode generation visitor.
   */
  class be_visitor_alias_typecode
    : public be_visitor_typecode_defn
  {
  public:

    /// Constructor.
    be_visitor_alias_typecode (be_visitor_context * ctx);

    /// Visit a typedef.
    /**
     * @todo The legacy method name "@c visit_typedef()" is redundant
     *       since it is obvious from the argument what kind of
     *       TypeCode is being visited.  It will be changed once the
     *       rest of the legacy method names and their call sites are
     *       updated accordingly.
     */
    virtual int visit_typedef (be_typedef * node);

    /// Visit a valuebox.
    virtual int visit_valuebox (be_valuebox * node);

  private:
    int common (be_type * node,
                be_type * base,
                const char * tctype);

  };

}

#endif  /* TAO_BE_VISITOR_ALIAS_TYPECODE_H */
