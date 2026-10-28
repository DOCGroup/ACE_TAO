// -*- C++ -*-

//=============================================================================
/**
 *  @file  alias_typecode.h
 *
 *  Alias (typedef) TypeCode generation visitor.
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_ALIAS_TYPECODE_H
#define TAO_BE_VISITOR_ALIAS_TYPECODE_H

#include <string_view>

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
    explicit be_visitor_alias_typecode (be_visitor_context * ctx);

    be_visitor_alias_typecode (be_visitor_alias_typecode const &) = delete;
    be_visitor_alias_typecode (be_visitor_alias_typecode &&) = delete;
    be_visitor_alias_typecode &operator= (be_visitor_alias_typecode const &) = delete;
    be_visitor_alias_typecode &operator= (be_visitor_alias_typecode &&) = delete;

    /// Visit a typedef.
    /**
     * @todo The legacy method name "@c visit_typedef()" is redundant
     *       since it is obvious from the argument what kind of
     *       TypeCode is being visited.  It will be changed once the
     *       rest of the legacy method names and their call sites are
     *       updated accordingly.
     */
    int visit_typedef (be_typedef * node) override;

    /// Visit a valuebox.
    int visit_valuebox (be_valuebox * node) override;

  private:
    int common (be_type * node,
                be_type * base,
                std::string_view tctype);
  };
}

#endif  /* TAO_BE_VISITOR_ALIAS_TYPECODE_H */
