// file      : CCF/IDL3/SemanticGraph/Component.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/SemanticGraph/EventType.hpp"
#include "CCF/IDL3/SemanticGraph/Component.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Provider
      //
      //
      namespace
      {
        TypeInfo
        provider_init_ ()
        {
          TypeInfo ti (typeid (Provider));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo provider_ (provider_init_ ());
      }

      TypeInfo const& Provider::
      static_type_info () { return provider_; }


      // User
      //
      //
      namespace
      {
        TypeInfo
        user_init_ ()
        {
          TypeInfo ti (typeid (User));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo user_ (user_init_ ());
      }

      TypeInfo const& User::
      static_type_info () { return user_; }

      User::
      ~User ()
      {
      }


      // MultiUser
      //
      //
      namespace
      {
        TypeInfo
        multi_user_init_ ()
        {
          TypeInfo ti (typeid (MultiUser));
          ti.add_base (Access::PUBLIC, true, User::static_type_info ());
          return ti;
        }

        TypeInfo multi_user_ (multi_user_init_ ());
      }

      TypeInfo const& MultiUser::
      static_type_info () { return multi_user_; }


      // SingleUser
      //
      //
      namespace
      {
        TypeInfo
        single_user_init_ ()
        {
          TypeInfo ti (typeid (SingleUser));
          ti.add_base (Access::PUBLIC, true, User::static_type_info ());
          return ti;
        }

        TypeInfo single_user_ (single_user_init_ ());
      }

      TypeInfo const& SingleUser::
      static_type_info () { return single_user_; }

      // Publisher
      //
      //
      namespace
      {
        TypeInfo
        publisher_init_ ()
        {
          TypeInfo ti (typeid (Publisher));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo publisher_ (publisher_init_ ());
      }

      TypeInfo const& Publisher::
      static_type_info () { return publisher_; }


      // Emitter
      //
      //
      namespace
      {
        TypeInfo
        emitter_init_ ()
        {
          TypeInfo ti (typeid (Emitter));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo emitter_ (emitter_init_ ());
      }

      TypeInfo const& Emitter::
      static_type_info () { return emitter_; }


      // Consumer
      //
      //
      namespace
      {
        TypeInfo
        consumer_init_ ()
        {
          TypeInfo ti (typeid (Consumer));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo consumer_ (consumer_init_ ());
      }

      TypeInfo const& Consumer::
      static_type_info () { return consumer_; }


      // Component
      //
      //
      namespace
      {
        TypeInfo
        component_init_ ()
        {
          TypeInfo ti (typeid (Component));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo component_ (component_init_ ());
      }

      TypeInfo const& Component::
      static_type_info () { return component_; }
    }
  }
}
