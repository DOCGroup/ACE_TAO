// file      : CCF/IDL3/SyntaxTree/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SYNTAX_TREE_COMPONENT_HPP
#define CCF_IDL3_SYNTAX_TREE_COMPONENT_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Interface.hpp"

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
      class ComponentDecl : public virtual TypeDecl
      {
      protected:
        virtual
        ~ComponentDecl () throw () {}

        ComponentDecl ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "component";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<ComponentDecl>
      ComponentDeclPtr;

      typedef
      DeclarationRef<ComponentDecl>
      ComponentDeclRef;


      //
      //
      //
      class ComponentForwardDecl : public virtual ComponentDecl,
                                   public virtual TypeForwardDecl
      {
      public:
        virtual
        ~ComponentForwardDecl () throw () {}

        ComponentForwardDecl (SimpleName const& name,
                              ScopePtr const& scope)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

      protected:
        ComponentForwardDecl (SimpleName const& name,
                              Order const& order,
                              ScopePtr const& scope)
            : Declaration (name, order, scope)
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
            new ComponentForwardDecl (name, order, scope));
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "component";
        }


      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };



      //
      //
      //
      class ComponentDef;

      typedef
      StrictPtr<ComponentDef>
      ComponentDefPtr;

      typedef
      DeclarationRef<ComponentDef>
      ComponentDefRef;

      class ComponentDef : public virtual TypeDef,
                           public virtual ComponentDecl,
                           public virtual Scope
      {
      public:
        virtual
        ~ComponentDef () throw () {}

        ComponentDef (SimpleName const& name,
                      ScopePtr const& scope,
                      ScopedName const& inherits,
                      ScopedNameSet const& supports)
            : Declaration (name, scope),
              inherits_ (table (), inherits)
        {
          type_info (static_type_info ());
          copy_supports_list (supports);
        }

        ComponentDef (SimpleName const& name,
                      ScopePtr const& scope,
                      ScopedNameSet const& supports)
            : Declaration (name, scope),
              inherits_ (table ())
        {
          type_info (static_type_info ());
          copy_supports_list (supports);
        }

      protected:
        ComponentDef (SimpleName const& name,
                      Order const& order,
                      ScopePtr const& scope,
                      ComponentDefRef const& inherits,
                      InterfaceDefRefSetName const& supports)
            : Declaration (name, order, scope),
              inherits_ (inherits),
              supports_ (supports)
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
            new ComponentDef (name, order, scope, inherits_, supports_));
        }

      private:
        void
        copy_supports_list (ScopedNameSet const& supports)
        {
          for (ScopedNameSet::const_iterator i = supports.begin ();
               i != supports.end ();
               i++)
          {
            supports_.insert (InterfaceDefRef (table (), *i));
          }
        }


      public:
        ComponentDefRef
        inherits ()
        {
          return inherits_;
        }


      public:

        typedef
        InterfaceDefRefSetName::const_iterator
        Iterator;

        Iterator
        supports_begin () const
        {
          return supports_.begin ();
        }

        Iterator
        supports_end () const
        {
          return supports_.end ();
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "component";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        ComponentDefRef inherits_;
        InterfaceDefRefSetName supports_;
      };

      typedef
      DeclarationOrderComparator<ComponentDefPtr>
      ComponentDefOrderComparator;

      typedef
      std::set<ComponentDefPtr, ComponentDefOrderComparator>
      ComponentDefSet;
    }
  }
}

#endif  // CCF_IDL3_SYNTAX_TREE_COMPONENT_HPP
