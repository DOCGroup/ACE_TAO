// $Id$
#ifndef CCF_IDL2_SYNTAX_TREE_VALUE_HPP
#define CCF_IDL2_SYNTAX_TREE_VALUE_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SyntaxTree.hpp"

namespace IDL2
{
  namespace SyntaxTree
  {
    //
    //
    //
    class ValueDecl : public virtual TypeDecl
    {
    public:
      virtual
      ~ValueDecl () throw () {}

      ValueDecl (SimpleName const& name,
                 ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "value";
      }


      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v)
      {
      }

    };

    typedef
    StrictPtr<ValueDecl>
    ValueDeclPtr;

    typedef
    DeclarationOrderComparator<ValueDeclPtr>
    ValueDeclOrderComparator;

    typedef
    std::set<ValueDeclPtr, ValueDeclOrderComparator>
    ValueDeclSet;

    typedef
    DeclarationRef<ValueDecl>
    ValueDeclRef;

    typedef
    DeclarationRefOrderComparator<ValueDeclRef>
    ValueDeclRefOrderComparator;

    typedef
    std::set<ValueDeclRef, ValueDeclRefOrderComparator>
    ValueDeclRefSet;


    //
    //
    //
    class ValueForwardDecl : public virtual ValueDecl,
                             public virtual TypeForwardDecl
    {
    public:
      virtual
      ~ValueForwardDecl () throw () {}

      ValueForwardDecl (SimpleName const& name,
                            ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            ValueDecl (name, scope),
            TypeForwardDecl (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "value";
      }
    };

    //
    //
    //
    class ValueDef;

    typedef
    StrictPtr<ValueDef>
    ValueDefPtr;

    typedef
    DeclarationRef<ValueDef>
    ValueDefRef;

    typedef
    DeclarationRefNameComparator<ValueDefRef>
    ValueDefRefNameComparator;

    typedef
    std::set<ValueDefRef, ValueDefRefNameComparator>
    ValueDefRefSetName;

    class ValueDef : public virtual ValueDecl,
                     public virtual TypeDef,
                     public virtual Scope
    {
    public:
      virtual
      ~ValueDef () throw () {}

      class DuplicateName {};

      ValueDef (SimpleName const& name,
                    ScopePtr const& scope,
                    ScopedNameSet const& inherits) throw (DuplicateName)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            ValueDecl (name, scope),
            TypeDef (name, scope),
            Scope (name, scope)
      {
        for (ScopedNameSet::const_iterator i = inherits.begin ();
             i != inherits.end ();
             i++)
        {
          if (!inherits_.insert (ValueDefRef (scope->table (), *i)).second)
          {
            //@@ this never happens
            throw DuplicateName ();
          }
        }
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "value";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v)
      {
      }


    private:
      ValueDefRefSetName inherits_;
    };

    typedef
    DeclarationOrderComparator<ValueDefPtr>
    ValueDefOrderComparator;

    typedef
    std::set<ValueDefPtr, ValueDefOrderComparator>
    ValueDefSet;

    typedef
    DeclarationRefOrderComparator<ValueDefRef>
    ValueDefRefOrderComparator;

    typedef
    std::set<ValueDefRef, ValueDefRefOrderComparator>
    ValueDefRefSet;
  }
}

#endif // CCF_IDL2_SYNTAX_TREE_VALUE_HPP
