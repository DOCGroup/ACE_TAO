// file      : CCF/IDL2/SyntaxTree/Typedef.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_TYPEDEF_HPP
#define CCF_IDL2_SYNTAX_TREE_TYPEDEF_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      class TypedefDecl : public virtual TypeDecl
      {
      public:
        virtual
        ~TypedefDecl () throw () {}

        TypedefDecl (SimpleName const& name,
                     ScopedName const& type,
                     ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (type)
        {
          type_info (static_type_info ());
        }

      public:
        virtual TypeDeclPtr
        clone_typedef_temporary (SimpleName const& name,
                                 Order const& order,
                                 ScopePtr const& scope);
      public:
        virtual bool
        is_a (Introspection::TypeInfo const& ti) const;

        virtual NodePtr
        dynamic_type (Introspection::TypeInfo const& ti);

        virtual NodePtr
        virtual_type ();

      public:
        virtual bool
        complete () const
        {
          return underlying_type ()->complete ();
        }

      public:
        virtual std::string
        declaration_class ()
        {
          return "typedef";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclPtr
        underlying_type () const;

      private:
        ScopedName type_;
      };

      typedef
      StrictPtr<TypedefDecl>
      TypedefDeclPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_TYPEDEF_HPP
