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
      struct Component;
      struct Provides;
      struct Uses;
      struct Publishes;
      struct Emits;
      struct Consumes;
      struct EventType;
      struct EventTypeFactory;
      struct Home;
      struct HomeFactory;
      struct HomeFinder;


      struct Factory : virtual IDL2::SemanticAction::Factory
      {
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

        virtual EventTypeFactory&
        event_type_factory () = 0;

        virtual Home&
        home () = 0;

        virtual HomeFactory&
        home_factory () = 0;

        virtual HomeFinder&
        home_finder () = 0;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_ACTION_FACTORY_HPP
