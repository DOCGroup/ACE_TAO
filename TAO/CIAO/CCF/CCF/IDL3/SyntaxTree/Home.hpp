// file      : CCF/IDL3/SyntaxTree/Home.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_SYNTAX_TREE_HOME_HPP
#define CCF_IDL3_SYNTAX_TREE_HOME_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Interface.hpp"
#include "CCF/IDL3/SyntaxTree/Component.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SyntaxTree
    {
      // Import nodes from IDL2
      using namespace IDL2::SyntaxTree;

      // According to the spec Home cannot be forward-declared. Did they
      // goof again? - you would probably think...
      //

      class HomeDef;

      typedef
      StrictPtr<HomeDef>
      HomeDefPtr;

      typedef
      DeclarationRef<HomeDef>
      HomeDefRef;

      class HomeDef : public virtual TypeDef, public virtual Scope
      {
      public:
        virtual
        ~HomeDef () throw () {}

        HomeDef (SimpleName const& name,
                 ScopePtr const& scope,
                 ScopedName const& inherits,
                 ScopedNameSet const& supports,
                 ScopedName const& manages)

            : Declaration (name, scope),
              TypeDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              inherits_ (scope->table (), inherits),
              manages_ (scope->table (), manages)
        {
          type_info (static_type_info ());
          copy_supports_list (supports);
        }

        HomeDef (SimpleName const& name,
                 ScopePtr const& scope,
                 ScopedNameSet const& supports,
                 ScopedName const& manages)

            : Declaration (name, scope),
              TypeDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              inherits_ (scope->table ()),
              manages_ (scope->table (), manages)
        {
          type_info (static_type_info ());
          copy_supports_list (supports);
        }

      private:
        void
        copy_supports_list (ScopedNameSet const& supports)
        {
          for (ScopedNameSet::const_iterator i = supports.begin ();
               i != supports.end ();
               i++)
          {
            supports_.insert (InterfaceDefRef (scope ()->table (), *i));
          }
        }

      public:
        HomeDefRef
        inherits ()
        {
          return inherits_;
        }

      public:
        typedef
        InterfaceDefRefSetName::const_iterator
        Iterator;

        Iterator
        supports_begin ()
        {
          return supports_.begin ();
        }

        Iterator
        supports_end ()
        {
          return supports_.end ();
        }

      public:
        ComponentDefPtr
        manages () const
        {
          return manages_.resolve ();
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "home";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        HomeDefRef inherits_;
        InterfaceDefRefSetName supports_;
        ComponentDefRef manages_;
      };

      typedef
      DeclarationOrderComparator<HomeDefPtr>
      HomeDefOrderComparator;

      typedef
      std::set<HomeDefPtr, HomeDefOrderComparator>
      HomeDefSet;
    }
  }
}

#endif  // CCF_IDL3_SYNTAX_TREE_HOME_HPP
