// file      : CCF/IDL3/SemanticAction/Factory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_ACTION_FACTORY_HPP
#define CCF_IDL3_SEMANTIC_ACTION_FACTORY_HPP

#include "CCF/IDL2/SemanticAction/Factory.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticAction
    {
      class Component;
      class Provides;
      class Uses;
      class Publishes;
      class Emits;
      class Consumes;
      class EventType;
      class Home;
      class HomeFactory;

      //
      //
      //
      class Factory : public virtual IDL2::SemanticAction::Factory
      {
      public:
        virtual Component&
        component () = 0;

        virtual Provides&
        provides () = 0;

        virtual Uses&
        uses () = 0;

        virtual Publishes&
        publishes () = 0;

        virtual Emits&
        emits () = 0;

        virtual Consumes&
        consumes () = 0;

        virtual EventType&
        event_type () = 0;

        virtual Home&
        home () = 0;

        virtual HomeFactory&
        home_factory () = 0;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_FACTORY_HPP
