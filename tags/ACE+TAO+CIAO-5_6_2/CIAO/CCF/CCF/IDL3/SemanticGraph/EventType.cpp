// file      : CCF/IDL3/SemanticGraph/EventType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/EventType.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // EventType
      //
      //
      namespace
      {
        TypeInfo
        event_type_init_ ()
        {
          TypeInfo ti (typeid (EventType));
          ti.add_base (Access::PUBLIC, true, ValueType::static_type_info ());
          return ti;
        }

        TypeInfo event_type_ (event_type_init_ ());
      }

      TypeInfo const& EventType::
      static_type_info () { return event_type_; }

      EventType::
      ~EventType ()
      {
      }


      // AbstractEventType
      //
      //
      namespace
      {
        TypeInfo
        abstract_event_type_init_ ()
        {
          TypeInfo ti (typeid (AbstractEventType));
          ti.add_base (Access::PUBLIC, true, EventType::static_type_info ());
          ti.add_base (
            Access::PUBLIC, true, AbstractValueType::static_type_info ());
          return ti;
        }

        TypeInfo abstract_event_type_ (abstract_event_type_init_ ());
      }

      TypeInfo const& AbstractEventType::
      static_type_info () { return abstract_event_type_; }


      // ConcreteEventType
      //
      //
      namespace
      {
        TypeInfo
        concrete_event_type_init_ ()
        {
          TypeInfo ti (typeid (ConcreteEventType));
          ti.add_base (Access::PUBLIC, true, EventType::static_type_info ());
          ti.add_base (
            Access::PUBLIC, true, ConcreteValueType::static_type_info ());
          return ti;
        }

        TypeInfo concrete_event_type_ (concrete_event_type_init_ ());
      }

      TypeInfo const& ConcreteEventType::
      static_type_info () { return concrete_event_type_; }


      // EventTypeFactory
      //
      //
      namespace
      {
        TypeInfo
        event_type_factory_init_ ()
        {
          TypeInfo ti (typeid (EventTypeFactory));
          ti.add_base (
            Access::PUBLIC, true, ValueTypeFactory::static_type_info ());
          return ti;
        }

        TypeInfo event_type_factory_ (event_type_factory_init_ ());
      }

      TypeInfo const& EventTypeFactory::
      static_type_info () { return event_type_factory_; }
    }
  }
}
