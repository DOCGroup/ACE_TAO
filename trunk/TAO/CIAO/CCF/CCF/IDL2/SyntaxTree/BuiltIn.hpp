// file      : CCF/IDL2/SyntaxTree/BuiltIn.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_SYNTAX_TREE_BUILT_IN_HPP
#define CCF_IDL2_SYNTAX_TREE_BUILT_IN_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      //@@ I have a potential problem with two-word built-in types
      //   e.g. unsigned long. Though I don't see anything bad in
      //   allowing (internally) for names to contain spaces e.g.
      //   '::unsigned long'. I will also have to teach LexicalAnalyzer
      //   to parse this stuff properly.
      //

      //
      //
      //
      class BuiltInTypeDef : public virtual TypeDef
      {
      protected:
        virtual
        ~BuiltInTypeDef () throw () {}

        BuiltInTypeDef (SimpleName const& name,
                        ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              TypeDef (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "built-in type";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<BuiltInTypeDef>
      BuiltInTypeDefPtr;


      //
      //
      //
      class Void : public virtual BuiltInTypeDef
      {
      public:
        virtual
        ~Void () throw () {}

        Void (ScopePtr const& scope)
            : Declaration (SimpleName ("void"), scope),
              TypeDecl (SimpleName ("void"), scope),
              TypeDef (SimpleName ("void"), scope),
              BuiltInTypeDef (SimpleName ("void"), scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "built-in type void";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<Void>
      VoidPtr;

      //
      //
      //
      class Long : public virtual BuiltInTypeDef
      {
      public:
        virtual
        ~Long () throw () {}

        Long (ScopePtr const& scope)
            : Declaration (SimpleName ("long"), scope),
              TypeDecl (SimpleName ("long"), scope),
              TypeDef (SimpleName ("long"), scope),
              BuiltInTypeDef (SimpleName ("long"), scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "built-in type long";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<Long>
      LongPtr;

      //
      //
      //
      class Boolean : public virtual BuiltInTypeDef
      {
      public:
        virtual
        ~Boolean () throw () {}

        Boolean (ScopePtr const& scope)
            : Declaration (SimpleName ("boolean"), scope),
              TypeDecl (SimpleName ("boolean"), scope),
              TypeDef (SimpleName ("boolean"), scope),
              BuiltInTypeDef (SimpleName ("boolean"), scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "built-in type boolean";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<Boolean>
      BooleanPtr;


      //
      //
      //
      class String : public virtual BuiltInTypeDef
      {
      public:
        virtual
        ~String () throw () {}

        String (ScopePtr const& scope)
            : Declaration (SimpleName ("string"), scope),
              TypeDecl (SimpleName ("string"), scope),
              TypeDef (SimpleName ("string"), scope),
              BuiltInTypeDef (SimpleName ("string"), scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "built-in type string";
        }

      public:
        static Utility::Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<String>
      StringPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_BUILT_IN_HPP
