// file      : CCF/IDL2/SemanticAction/Impl/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPE_ID_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPE_ID_HPP

#include "CCF/IDL2/SemanticAction/TypeId.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        struct TypeId : SemanticAction::TypeId
        {
          TypeId (Context& c);

          virtual void
          begin (IdentifierPtr const& d, StringLiteralPtr const& id);

          virtual void
          end ();

        private:
          Context& ctx;
        };


        //
        //
        //
        struct TypePrefix : SemanticAction::TypePrefix
        {
          TypePrefix (Context& c);

          virtual void
          begin (IdentifierPtr const& d, StringLiteralPtr const& prefix);

          virtual void
          end ();

        private:
          Context& ctx;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPE_ID_HPP
