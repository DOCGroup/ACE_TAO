// file      : CCF/IDL2/SyntaxTree/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_SYNTAX_TREE_OPERATION_HPP
#define CCF_IDL2_SYNTAX_TREE_OPERATION_HPP

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
      class AttributeDecl : public virtual Declaration
      {
      public:
        virtual
        ~AttributeDecl () throw () {}

        AttributeDecl (SimpleName const& name,
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
          return "attribute";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
      };

      typedef
      StrictPtr<AttributeDecl>
      AttributeDeclPtr;

      //
      //
      //
      class OperationParameter : public virtual Node
      {
      public:
        virtual
        ~OperationParameter () throw () {}

        struct Direction
        {
          enum Value
          {
            IN,
            OUT,
            INOUT
          };

          friend std::ostream&
          operator<< (std::ostream& o, Value d)
          {
            if (d == IN) o << "in";
            else if (d == OUT) o << "out";
            else o << "inout";
            return o;
          }
        };

        OperationParameter (Direction::Value direction,
                            ScopedName type,
                            SimpleName name,
                            DeclarationTable const& table)
            : direction_ (direction),
              type_ (table, type),
              name_ (name)
        {
          type_info (static_type_info ());
        }

      public:
        Direction::Value
        direction ()
        {
          return direction_;
        }

        TypeDeclPtr
        type ()
        {
          return type_.resolve ();
        }

        SimpleName
        name ()
        {
          return name_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        Direction::Value direction_;
        TypeDeclRef type_;
        SimpleName name_;
      };

      typedef
      StrictPtr<OperationParameter>
      OperationParameterPtr;

      typedef
      std::vector<OperationParameterPtr>
      OperationParameterList;


      //
      //
      //
      class OperationDecl : public virtual Declaration
      {
      public:
        virtual
        ~OperationDecl () throw () {}

        OperationDecl (SimpleName const& name,
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

      public:
        typedef
        OperationParameterList::const_iterator
        Iterator;

        void
        insert (OperationParameterPtr op)
        {
          //@@ would be nice to have some checking here
          //   list should probably be a set(by name)
          parameter_list_.push_back (op);
        }

        Iterator
        begin ()
        {
          return parameter_list_.begin ();
        }

        Iterator
        end ()
        {
          return parameter_list_.end ();
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "operation";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        TypeDeclRef type_;
        OperationParameterList parameter_list_;
      };

      typedef
      StrictPtr<OperationDecl>
      OperationDeclPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_OPERATION_HPP
