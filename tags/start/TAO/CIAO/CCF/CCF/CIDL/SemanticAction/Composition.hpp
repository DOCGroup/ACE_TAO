// file      : CCF/CIDL/SemanticAction/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

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
      //
      //
      //
      class Composition : public virtual Scope
      {
      public:
        struct Category
        {
          enum Value
          {
            ENTITY = 0,
            PROCESS,
            SERVICE,
            SESSION
          };

          friend std::ostream&
          operator<< (std::ostream& o, Value v);
        };

        virtual void
        begin (SimpleIdentifierPtr const& id, Category::Value category) = 0;

        virtual void
        end () = 0;
      };
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_ACTION_COMPOSITION_HPP
