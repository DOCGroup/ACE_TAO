// file      : CCF/IDL3/SemanticAction/Impl/Factory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_FACTORY_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_FACTORY_HPP

#include "CCF/IDL3/SemanticAction/Factory.hpp"

#include "CCF/IDL2/SemanticAction/Impl/Factory.hpp"

#include "CCF/IDL3/SemanticAction/Impl/Include.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Component.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Provides.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Uses.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Publishes.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Emits.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Consumes.hpp"
#include "CCF/IDL3/SemanticAction/Impl/EventType.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Home.hpp"
#include "CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        class Factory : public virtual IDL3::SemanticAction::Factory,
                        public virtual IDL2::SemanticAction::Impl::Factory
        {
        public:
          virtual
          ~Factory () throw () {}

          Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SyntaxTree::TranslationRegionPtr const& r)
              : IDL2::SemanticAction::Impl::Factory (context, dout, r),
                trace_ (context.get ("idl3::semantic-action::trace", false)),

                include_ (trace_, context, dout, *this, r, scope_),
                component_ (trace_, scope_),
                provides_ (trace_, scope_),
                uses_ (trace_, scope_),
                publishes_ (trace_, scope_),
                emits_ (trace_, scope_),
                consumes_ (trace_, scope_),
                event_type_ (trace_, scope_),
                home_ (trace_, scope_),
                home_factory_ (trace_, scope_)
          {
          }

        public:

          virtual IDL2::SemanticAction::Include&
          include ()
          {
            return include_;
          }

          virtual SemanticAction::Component&
          component ()
          {
            return component_;
          }

          virtual SemanticAction::Provides&
          provides ()
          {
            return provides_;
          }

          virtual SemanticAction::Uses&
          uses ()
          {
            return uses_;
          }

          virtual SemanticAction::Publishes&
          publishes ()
          {
            return publishes_;
          }

          virtual SemanticAction::Emits&
          emits ()
          {
            return emits_;
          }

          virtual SemanticAction::Consumes&
          consumes ()
          {
            return consumes_;
          }


          virtual SemanticAction::EventType&
          event_type ()
          {
            return event_type_;
          }

          virtual SemanticAction::Home&
          home ()
          {
            return home_;
          }

          virtual SemanticAction::HomeFactory&
          home_factory ()
          {
            return home_factory_;
          }


        private:

          bool trace_;

          Include include_;

          Component component_;
          Provides provides_;
          Uses uses_;
          Publishes publishes_;
          Emits emits_;
          Consumes consumes_;
          EventType event_type_;
          Home home_;
          HomeFactory home_factory_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_FACTORY_HPP
