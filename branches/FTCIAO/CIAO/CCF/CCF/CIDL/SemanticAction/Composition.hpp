// file      : CCF/CIDL/SemanticAction/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_ACTION_COMPOSITION_HPP
#define CCF_CIDL_SEMANTIC_ACTION_COMPOSITION_HPP

#include "CCF/CIDL/SemanticAction/Elements.hpp"

#include <iosfwd>

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      struct Composition : Scope
      {
        struct Category
        {
          enum Value
          {
            entity , process, service, session
          };
        };

        virtual void
        begin (SimpleIdentifierPtr const& id, Category::Value category) = 0;

        virtual void
        open_scope () = 0;

        virtual void
        close_scope () = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

std::ostream&
operator<< (std::ostream& o,
            CCF::CIDL::SemanticAction::Composition::Category::Value v);

#endif  // CCF_CIDL_SEMANTIC_ACTION_COMPOSITION_HPP
