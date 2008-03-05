// file      : CCF/IDL3/SemanticAction/Impl/Factory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_IMPL_FACTORY_HPP
#define CCF_IDL3_SEMANTIC_ACTION_IMPL_FACTORY_HPP

#include "CCF/IDL3/SemanticAction/Factory.hpp"

#include "CCF/IDL2/SemanticAction/Impl/Factory.hpp"

#include "CCF/IDL3/SemanticAction/Impl/Component.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Provides.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Uses.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Publishes.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Emits.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Consumes.hpp"
#include "CCF/IDL3/SemanticAction/Impl/EventType.hpp"
#include "CCF/IDL3/SemanticAction/Impl/EventTypeFactory.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Home.hpp"
#include "CCF/IDL3/SemanticAction/Impl/HomeFactory.hpp"
#include "CCF/IDL3/SemanticAction/Impl/HomeFinder.hpp"
#include "CCF/IDL3/SemanticAction/Impl/Include.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        struct Factory : virtual IDL3::SemanticAction::Factory,
                         virtual IDL2::SemanticAction::Impl::Factory
        {
          Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SemanticGraph::TranslationUnit& tu);

          virtual SemanticAction::Consumes&
          consumes ()
          {
            return consumes_;
          }

          virtual SemanticAction::Component&
          component ()
          {
            return component_;
          }

          virtual SemanticAction::Emits&
          emits ()
          {
            return emits_;
          }


          virtual SemanticAction::EventType&
          event_type ()
          {
            return event_type_;
          }

          virtual EventTypeFactory&
          event_type_factory ()
          {
            return event_type_factory_;
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

          virtual SemanticAction::HomeFinder&
          home_finder ()
          {
            return home_finder_;
          }

          virtual IDL2::SemanticAction::Include&
          include ()
          {
            return include_;
          }

          virtual SemanticAction::Provides&
          provides ()
          {
            return provides_;
          }

          virtual SemanticAction::Publishes&
          publishes ()
          {
            return publishes_;
          }

          virtual SemanticAction::Uses&
          uses ()
          {
            return uses_;
          }

        private:

          Component component_;
          Consumes consumes_;
          Emits emits_;
          EventType event_type_;
          EventTypeFactory event_type_factory_;
          Home home_;
          HomeFactory home_factory_;
          HomeFinder home_finder_;
          Include include_;
          Provides provides_;
          Publishes publishes_;
          Uses uses_;
        };
      }
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_IMPL_FACTORY_HPP
