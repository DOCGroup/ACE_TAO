// $Id$
#ifndef CCF_IDL3_SYNTAX_TREE_VALUE_HPP
#define CCF_IDL3_SYNTAX_TREE_VALUE_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SyntaxTree_Value.hpp"

namespace IDL3
{
  namespace SyntaxTree
  {
    // Import nodes from IDL2
    using namespace IDL2::SyntaxTree;

    //
    //
    //
    class EventTypeDecl : public virtual ValueDecl
    {
    public:
      virtual
      ~EventTypeDecl () throw () {}

      EventTypeDecl (SimpleName const& name,
                     ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            ValueDecl (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "event type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v)
      {
      }

    };

    typedef
    StrictPtr<EventTypeDecl>
    EventTypeDeclPtr;


    //
    //
    //
    class EventTypeForwardDecl : public virtual ValueForwardDecl,
                                 public virtual EventTypeDecl
    {
    public:
      virtual
      ~EventTypeForwardDecl () throw () {}

      EventTypeForwardDecl (SimpleName const& name,
                            ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            ValueDecl (name, scope),
            TypeForwardDecl (name, scope),
            ValueForwardDecl (name, scope),
            EventTypeDecl (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "event type";
      }
    };

    //
    //
    //
    class EventTypeDef : public virtual EventTypeDecl,
                         public virtual ValueDef
    {
    public:
      virtual
      ~EventTypeDef () throw () {}

      EventTypeDef (SimpleName const& name,
                    ScopePtr const& scope,
                    ScopedNameSet const& inherits)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            ValueDecl (name, scope),
            EventTypeDecl (name, scope),
            TypeDef (name, scope),
            Scope (name, scope),
            ValueDef (name, scope, inherits)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "event type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v)
      {
      }

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
            ValueDecl (name, scope),
            EventTypeDecl (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "concrete event type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v)
      {
      }

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
            ValueDecl (name, scope),
            EventTypeDecl (name, scope),
            ConcreteEventTypeDecl (name, scope),
            TypeDef (name, scope),
            Scope (name, scope),
            ValueDef (name, scope, inherits),
            EventTypeDef (name, scope, inherits)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "concrete event type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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

#endif // CCF_IDL3_SYNTAX_TREE_VALUE_HPP
