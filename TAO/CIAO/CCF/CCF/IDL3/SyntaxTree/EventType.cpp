// file      : CCF/IDL3/SyntaxTree/EventType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL3/SyntaxTree/EventType.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL3
  {
    namespace SyntaxTree
    {

      // EventTypeDecl
      //
      //
      namespace
      {
        TypeInfo
        event_type_decl_init_ ()
        {
          TypeInfo ti (typeid (EventTypeDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo event_type_decl_ (event_type_decl_init_ ());
      }

      TypeInfo const& EventTypeDecl::
      static_type_info () { return event_type_decl_; }


      // EventTypeForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        event_type_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (EventTypeForwardDecl));

          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeForwardDecl::static_type_info ());

          ti.add_base (Access::PUBLIC,
                       true,
                       EventTypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo event_type_forward_decl_ (event_type_forward_decl_init_ ());
      }

      TypeInfo const& EventTypeForwardDecl::
      static_type_info () { return event_type_forward_decl_; }


      // EventTypeDef
      //
      //
      namespace
      {
        TypeInfo
        event_type_def_init_ ()
        {
          TypeInfo ti (typeid (EventTypeDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       EventTypeDecl::static_type_info ());

          ti.add_base (Access::PUBLIC,
                       true,
                       ValueTypeDef::static_type_info ());
          return ti;
        }

        TypeInfo event_type_def_ (event_type_def_init_ ());
      }

      TypeInfo const& EventTypeDef::
      static_type_info () { return event_type_def_; }


      // ConcreteEventTypeDecl
      //
      //
      namespace
      {
        TypeInfo
        concrete_event_type_decl_init_ ()
        {
          TypeInfo ti (typeid (ConcreteEventTypeDecl));
          ti.add_base (Access::PUBLIC,
                       true,
                       EventTypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo concrete_event_type_decl_ (concrete_event_type_decl_init_ ());
      }

      TypeInfo const& ConcreteEventTypeDecl::
      static_type_info () { return concrete_event_type_decl_; }


      // ConcreteEventTypeDef
      //
      //
      namespace
      {
        TypeInfo
        concrete_event_type_def_init_ ()
        {
          TypeInfo ti (typeid (ConcreteEventTypeDef));
          ti.add_base (Access::PUBLIC,
                       true,
                       ConcreteEventTypeDecl::static_type_info ());
          ti.add_base (Access::PUBLIC,
                       true,
                       EventTypeDef::static_type_info ());
          return ti;
        }

        TypeInfo concrete_event_type_def_ (concrete_event_type_def_init_ ());
      }

      TypeInfo const& ConcreteEventTypeDef::
      static_type_info () { return concrete_event_type_def_; }
    }
  }
}
