// file      : CCF/IDL3/SyntaxTree/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

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
      public:
        virtual
        ~EventTypeDecl () throw () {}

        EventTypeDecl (SimpleName const& name,
                       ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              ValueTypeDecl (name, scope)
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
      public:
        virtual
        ~EventTypeForwardDecl () throw () {}

        EventTypeForwardDecl (SimpleName const& name,
                              ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              ValueTypeDecl (name, scope),
              TypeForwardDecl (name, scope),
              ValueTypeForwardDecl (name, scope),
              EventTypeDecl (name, scope)
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
      public:
        virtual
        ~EventTypeDef () throw () {}

        EventTypeDef (SimpleName const& name,
                      ScopePtr const& scope,
                      ScopedNameSet const& inherits)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              ValueTypeDecl (name, scope),
              EventTypeDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              ValueTypeDef (name, scope, inherits)
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
      public:
        virtual
        ~ConcreteEventTypeDecl () throw () {}

        ConcreteEventTypeDecl (SimpleName const& name,
                               ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              ValueTypeDecl (name, scope),
              EventTypeDecl (name, scope)
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
              TypeDecl (name, scope),
              ValueTypeDecl (name, scope),
              EventTypeDecl (name, scope),
              ConcreteEventTypeDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              ValueTypeDef (name, scope, inherits),
              EventTypeDef (name, scope, inherits)
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
