// file      : CCF/IDL3/SyntaxTree/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SYNTAX_TREE_OPERATION_HPP
#define CCF_IDL3_SYNTAX_TREE_OPERATION_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Operation.hpp"
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
      class ProvidesDecl : public virtual Declaration
      {
      public:
        virtual
        ~ProvidesDecl () throw () {}

        ProvidesDecl (SimpleName const& name,
                      ScopedName const& type,
                      ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (scope->table (), type)
        {
          type_info (static_type_info ());
        }

      public:
        InterfaceDeclPtr
        type ()
        {
          return type_.resolve ();
        }


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "provides";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        InterfaceDeclRef type_;
      };

      typedef
      StrictPtr<ProvidesDecl>
      ProvidesDeclPtr;


      //
      //
      //
      class UsesDecl : public virtual Declaration
      {
      public:
        virtual
        ~UsesDecl () throw () {}

        UsesDecl (SimpleName const& name,
                  ScopedName const& type,
                  ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (scope->table (), type)

        {
          type_info (static_type_info ());
        }

      public:
        TypeDeclPtr
        type ()
        {
          return type_.resolve ();
        }


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "uses";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<UsesDecl>
      UsesDeclPtr;

      //
      //
      //
      class PublishesDecl : public virtual Declaration
      {
      public:
        virtual
        ~PublishesDecl () throw () {}

        PublishesDecl (SimpleName const& name,
                       ScopedName const& type,
                       ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (scope->table (), type)
        {
          type_info (static_type_info ());
        }

      public:
        TypeDeclPtr
        type ()
        {
          return type_.resolve ();
        }


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "publishes";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<PublishesDecl>
      PublishesDeclPtr;


      //
      //
      //
      class EmitsDecl : public virtual Declaration
      {
      public:
        virtual
        ~EmitsDecl () throw () {}

        EmitsDecl (SimpleName const& name,
                   ScopedName const& type,
                   ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (scope->table (), type)
        {
          type_info (static_type_info ());
        }

      public:
        TypeDeclPtr
        type ()
        {
          return type_.resolve ();
        }


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "emits";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<EmitsDecl>
      EmitsDeclPtr;


      //
      //
      //
      class ConsumesDecl : public virtual Declaration
      {
      public:
        virtual
        ~ConsumesDecl () throw () {}

        ConsumesDecl (SimpleName const& name,
                      ScopedName const& type,
                      ScopePtr const& scope)
            : Declaration (name, scope),
              type_ (scope->table (), type)
        {
          type_info (static_type_info ());
        }

      public:
        TypeDeclPtr
        type ()
        {
          return type_.resolve ();
        }


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "consumes";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<ConsumesDecl>
      ConsumesDeclPtr;


      //
      //
      //
      class HomeFactoryParameter : public virtual OperationParameter
      {
      public:
        virtual
        ~HomeFactoryParameter () throw ()
        {
        }

        HomeFactoryParameter (ScopedName type,
                              SimpleName name,
                              DeclarationTable const& table)
            : OperationParameter (Direction::IN, type, name, table)
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<HomeFactoryParameter>
      HomeFactoryParameterPtr;

      //
      //
      //
      class HomeFactoryDecl : public virtual OperationDecl
      {
      public:
        virtual
        ~HomeFactoryDecl () throw () {}

        HomeFactoryDecl (SimpleName const& name,
                         ScopedName const& type,
                         ScopePtr const& scope)
            : Declaration (name, scope),
              OperationDecl (name, type, scope)
        {
          type_info (static_type_info ());
        }

        //@@ It would be nice to be able to kind of virtual override
        //   type () mf from OperationDecl with covariant return type
        //   technique.


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "home factory";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<HomeFactoryDecl>
      HomeFactoryDeclPtr;
    }
  }
}

#endif  // CCF_IDL3_SYNTAX_TREE_OPERATION_HPP
