// file      : CCF/IDL2/SyntaxTree/ValueType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_VALUE_TYPE_HPP
#define CCF_IDL2_SYNTAX_TREE_VALUE_TYPE_HPP

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
      class ValueTypeDecl : public virtual TypeDecl
      {
      protected:
        virtual
        ~ValueTypeDecl () throw () {}

        ValueTypeDecl ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "value";
        }


      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<ValueTypeDecl>
      ValueTypeDeclPtr;

      typedef
      DeclarationOrderComparator<ValueTypeDeclPtr>
      ValueTypeDeclOrderComparator;

      typedef
      std::set<ValueTypeDeclPtr, ValueTypeDeclOrderComparator>
      ValueTypeDeclSet;

      typedef
      DeclarationRef<ValueTypeDecl>
      ValueTypeDeclRef;

      typedef
      DeclarationRefOrderComparator<ValueTypeDeclRef>
      ValueTypeDeclRefOrderComparator;

      typedef
      std::set<ValueTypeDeclRef, ValueTypeDeclRefOrderComparator>
      ValueTypeDeclRefSet;


      //
      //
      //
      class ValueTypeForwardDecl : public virtual ValueTypeDecl,
                                   public virtual TypeForwardDecl
      {
      public:
        virtual
        ~ValueTypeForwardDecl () throw () {}

        ValueTypeForwardDecl (SimpleName const& name,
                              ScopePtr const& scope)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

      protected:
        ValueTypeForwardDecl ()
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "value";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class ValueTypeDef;

      typedef
      StrictPtr<ValueTypeDef>
      ValueTypeDefPtr;

      typedef
      DeclarationRef<ValueTypeDef>
      ValueTypeDefRef;

      typedef
      DeclarationRefNameComparator<ValueTypeDefRef>
      ValueTypeDefRefNameComparator;

      typedef
      std::set<ValueTypeDefRef, ValueTypeDefRefNameComparator>
      ValueTypeDefRefSetName;

      class ValueTypeDef : public virtual ValueTypeDecl,
                           public virtual TypeDef,
                           public virtual Scope
      {
      public:
        virtual
        ~ValueTypeDef () throw () {}

        ValueTypeDef (SimpleName const& name,
                      ScopePtr const& scope,
                      ScopedNameSet const& inherits)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());

          for (ScopedNameSet::const_iterator i = inherits.begin ();
               i != inherits.end ();
               i++)
          {
            inherits_.insert (ValueTypeDefRef (table (), *i));
          }
        }

      protected:
        ValueTypeDef (ScopedNameSet const& inherits)
        {
          type_info (static_type_info ());

          for (ScopedNameSet::const_iterator i = inherits.begin ();
               i != inherits.end ();
               i++)
          {
            inherits_.insert (ValueTypeDefRef (table (), *i));
          }
        }

        // This c-tor is never called.
        //
        ValueTypeDef ();


        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "value";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();

      private:
        ValueTypeDefRefSetName inherits_;
      };

      typedef
      DeclarationOrderComparator<ValueTypeDefPtr>
      ValueTypeDefOrderComparator;

      typedef
      std::set<ValueTypeDefPtr, ValueTypeDefOrderComparator>
      ValueTypeDefSet;

      typedef
      DeclarationRefOrderComparator<ValueTypeDefRef>
      ValueTypeDefRefOrderComparator;

      typedef
      std::set<ValueTypeDefRef, ValueTypeDefRefOrderComparator>
      ValueTypeDefRefSet;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_VALUE_TYPE_HPP
