// file      : CCF/IDL3/SemanticAction/Impl/HomeFinder.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FINDER_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FINDER_HPP

#include "CCF/IDL3/SemanticGraph/Home.hpp"
#include "CCF/IDL3/SemanticAction/HomeFinder.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {

        struct HomeFinder : SemanticAction::HomeFinder, Base
        {
          HomeFinder (Context& c);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          parameter (IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id);

          virtual void
          raises (IdentifierPtr const& id);

        private:
          SemanticGraph::HomeFinder* hf_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FINDER_HPP
