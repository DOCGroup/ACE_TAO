// $Id$
#ifndef CCF_IDL2_SYNTAX_TREE_BUILT_IN_HPP
#define CCF_IDL2_SYNTAX_TREE_BUILT_IN_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SyntaxTree.hpp"

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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "built-in type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "built-in type void";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "built-in type long";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "built-in type boolean";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "built-in type string";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

    typedef
    StrictPtr<String>
    StringPtr;
  }
}

#endif // CCF_IDL2_SYNTAX_TREE_BUILT_IN_HPP
