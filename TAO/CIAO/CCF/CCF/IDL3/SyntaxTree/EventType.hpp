// file      : CCF/IDL3/SyntaxTree/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SYNTAX_TREE_EVENT_TYPE_HPP
#define CCF_IDL3_SYNTAX_TREE_EVENT_TYPE_HPP

#include "CCF/IDL2/SyntaxTree/ValueType.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SyntaxTree
    {
      // Import nodes from IDL2
      using namespace IDL2::SyntaxTree;


      //
      //
      //
      class EventTypeDecl : public virtual ValueTypeDecl
      {
      protected:
        virtual
        ~EventTypeDecl () throw () {}

        EventTypeDecl ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "event type";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<EventTypeDecl>
      EventTypeDeclPtr;


      //
      //
      //
      class EventTypeForwardDecl : public virtual ValueTypeForwardDecl,
                                   public virtual EventTypeDecl
      {
      protected:
        virtual
        ~EventTypeForwardDecl () throw () {}

        EventTypeForwardDecl ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "event type";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class EventTypeDef : public virtual EventTypeDecl,
                           public virtual ValueTypeDef
      {
      protected:
        virtual
        ~EventTypeDef () throw () {}

        EventTypeDef ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "event type";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class ConcreteEventTypeDecl : public virtual EventTypeDecl
      {
      protected:
        virtual
        ~ConcreteEventTypeDecl () throw () {}

        ConcreteEventTypeDecl ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "concrete event type";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      // ConcreteEventTypeForwardDecl is not implemented yet.
      //
      //


      //
      //
      //
      class ConcreteEventTypeDef : public virtual ConcreteEventTypeDecl,
                                   public virtual EventTypeDef
      {
      public:
        virtual
        ~ConcreteEventTypeDef () throw () {}

        ConcreteEventTypeDef (SimpleName const& name,
                              ScopePtr const& scope,
                              ScopedNameSet const& inherits)
            : Declaration (name, scope),
              ValueTypeDef (inherits)
        {
          type_info (static_type_info ());
        }

      protected:
        ConcreteEventTypeDef (SimpleName const& name,
                              Order const& order,
                              ScopePtr const& scope,
                              ScopedNameSet const& inherits)
            : Declaration (name, order, scope),
              ValueTypeDef (inherits)
        {
          type_info (static_type_info ());
        }

      public:
        virtual TypeDeclPtr
        clone_typedef_temporary (SimpleName const& name,
                                 Order const& order,
                                 ScopePtr const& scope)
        {
          return TypeDeclPtr (
            new ConcreteEventTypeDef (name, order, scope, inherits ()));
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "concrete event type";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<ConcreteEventTypeDef>
      ConcreteEventTypeDefPtr;

      typedef
      DeclarationOrderComparator<ConcreteEventTypeDefPtr>
      ConcreteEventTypeDefOrderComparator;

      typedef
      std::set<ConcreteEventTypeDefPtr, ConcreteEventTypeDefOrderComparator>
      ConcreteEventTypeDefSet;
    }
  }
}

#endif  // CCF_IDL3_SYNTAX_TREE_EVENT_TYPE_HPP
