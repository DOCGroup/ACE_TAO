// file      : CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FACTORY_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FACTORY_HPP

#include "CCF/IDL2/SemanticGraph/Operation.hpp"
#include "CCF/IDL3/SemanticAction/HomeFactory.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class HomeFactory : public virtual SemanticAction::HomeFactory,
                            public Base
        {
        public:
          virtual
          ~HomeFactory () throw ();

          HomeFactory (Context& c);

          virtual void
          name (SimpleIdentifierPtr const& id);

          virtual void
          parameter (IdentifierPtr const& type_id,
                     SimpleIdentifierPtr const& name_id);

          virtual void
          raises (IdentifierPtr const& id);

        private:
          SemanticGraph::HomeFactory* hf_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_HOME_FACTORY_HPP
