// file      : CCF/IDL2/SyntaxTree/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_TYPE_ID_HPP
#define CCF_IDL2_SYNTAX_TREE_TYPE_ID_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      //
      //
      //
      class TypeId : public virtual Declaration
      {
      public:
        virtual
        ~TypeId () throw () {}

        TypeId (SimpleName const& name,
                Order const& order,
                ScopePtr const& scope,
                ScopedName const& declaration,
                StringLiteral const& id)
            : Declaration (name, order, scope),
              declaration_ (declaration),
              id_ (id)
        {
          type_info (static_type_info ());
        }

      public:
        ScopedName const&
        declaration () const
        {
          return declaration_;
        }

        StringLiteral const&
        id () const
        {
          return id_;
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "typeid";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        ScopedName declaration_;
        StringLiteral id_;
      };

      typedef
      StrictPtr<TypeId>
      TypeIdPtr;


      //
      //
      //
      class TypePrefix : public virtual Declaration
      {
      public:
        virtual
        ~TypePrefix () throw () {}

        TypePrefix (SimpleName const& name,
                    Order const& order,
                    ScopePtr const& scope,
                    ScopedName const& declaration,
                    StringLiteral const& prefix)
            : Declaration (name, order, scope),
              declaration_ (declaration),
              prefix_ (prefix)
        {
          type_info (static_type_info ());
        }

      public:
        ScopedName const&
        declaration () const
        {
          return declaration_;
        }

        StringLiteral const&
        prefix () const
        {
          return prefix_;
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "typeprefix";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        ScopedName declaration_;
        StringLiteral prefix_;
      };

      typedef
      StrictPtr<TypePrefix>
      TypePrefixPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_TYPE_ID_HPP
