// file      : CCF/IDL2/SyntaxTree/Struct.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_STRUCT_HPP
#define CCF_IDL2_SYNTAX_TREE_STRUCT_HPP

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
      class StructDecl : public virtual ForwardDeclarableTypeDecl
      {
      protected:
        virtual
        ~StructDecl () throw () {}

        StructDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        virtual bool
        complete () const;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<StructDecl>
      StructDeclPtr;


      //
      //
      //
      class StructForwardDecl : public virtual StructDecl,
                                public virtual TypeForwardDecl
      {
      public:
        virtual
        ~StructForwardDecl () throw () {}

        StructForwardDecl (SimpleName const& name, ScopePtr const& scope)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

      protected:
        StructForwardDecl (SimpleName const& name,
                           Order const& order,
                           ScopePtr const& scope,
                           ContextHolderPtr const& ch)
            : Node (ch),
              Declaration (name, order, scope)
        {
          type_info (static_type_info ());
        }

      public:
        virtual TypeDeclPtr
        clone_typedef_temporary (SimpleName const& name,
                                 Order const& order,
                                 ScopePtr const& scope,
                                 ContextHolderPtr const& ch)
        {
          return TypeDeclPtr (new StructForwardDecl (name, order, scope, ch));
        }

      public:
        virtual std::string
        declaration_class ()
        {
          return "struct";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<StructForwardDecl>
      StructForwardDeclPtr;

      //
      //
      //
      class StructDef : public virtual StructDecl,
                        public virtual TypeDef,
                        public virtual Scope
      {
      public:
        virtual
        ~StructDef () throw () {}

        StructDef (SimpleName const& name, ScopePtr const& scope)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

      protected:
        StructDef (SimpleName const& name,
                   Order const& order,
                   ScopePtr const& scope,
                   ContextHolderPtr const& ch)
            : Node (ch),
              Declaration (name, order, scope)
        {
          type_info (static_type_info ());
        }

      public:
        virtual TypeDeclPtr
        clone_typedef_temporary (SimpleName const& name,
                                 Order const& order,
                                 ScopePtr const& scope,
                                 ContextHolderPtr const& ch)
        {
          return TypeDeclPtr (new StructDef (name, order, scope, ch));
        }

      public:
        virtual std::string
        declaration_class ()
        {
          return "struct";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<StructDef>
      StructDefPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_STRUCT_HPP
