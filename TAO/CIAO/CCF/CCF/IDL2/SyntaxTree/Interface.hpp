// file      : CCF/IDL2/SyntaxTree/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_INTERFACE_HPP
#define CCF_IDL2_SYNTAX_TREE_INTERFACE_HPP

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
      class InterfaceDecl : public virtual ForwardDeclarableTypeDecl
      {
      protected:
        virtual
        ~InterfaceDecl () throw () {}

        InterfaceDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        virtual bool
        complete () const
        {
          // Interface is considered complete even if it's
          // not defined.
          //
          return true;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<InterfaceDecl>
      InterfaceDeclPtr;

      typedef
      DeclarationOrderComparator<InterfaceDeclPtr>
      InterfaceDeclOrderComparator;

      typedef
      std::set<InterfaceDeclPtr, InterfaceDeclOrderComparator>
      InterfaceDeclSet;

      typedef
      DeclarationRef<InterfaceDecl>
      InterfaceDeclRef;

      typedef
      DeclarationRefOrderComparator<InterfaceDeclRef>
      InterfaceDeclRefOrderComparator;

      typedef
      std::set<InterfaceDeclRef, InterfaceDeclRefOrderComparator>
      InterfaceDeclRefSet;


      //
      //
      //
      class InterfaceForwardDecl : public virtual InterfaceDecl,
                                   public virtual TypeForwardDecl
      {
      protected:
        virtual
        ~InterfaceForwardDecl () throw () {}

        InterfaceForwardDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class InterfaceDef;

      typedef
      StrictPtr<InterfaceDef>
      InterfaceDefPtr;

      typedef
      DeclarationRef<InterfaceDef>
      InterfaceDefRef;

      typedef
      DeclarationRefNameComparator<InterfaceDefRef>
      InterfaceDefRefNameComparator;

      typedef
      std::set<InterfaceDefRef, InterfaceDefRefNameComparator>
      InterfaceDefRefSetName;

      class InterfaceDef : public virtual InterfaceDecl,
                           public virtual TypeDef,
                           public virtual Scope
      {
      protected:
        virtual
        ~InterfaceDef () throw () {}

        InterfaceDef (ScopedNameSet const& inherits)
        {
          type_info (static_type_info ());

          for (ScopedNameSet::const_iterator i = inherits.begin ();
               i != inherits.end ();
               i++)
          {
            inherits_.insert (InterfaceDefRef (table (), *i));
          }
        }

        // This c-tor is never called.
        //
        InterfaceDef ();


      public:
        typedef
        InterfaceDefRefSetName::const_iterator
        Iterator;

        Iterator
        inherits_begin () const
        {
          return inherits_.begin ();
        }

        Iterator
        inherits_end () const
        {
          return inherits_.end ();
        }

      protected:
        ScopedNameSet
        inherits () const
        {
          ScopedNameSet s;

          for (Iterator i (inherits_begin ()), end (inherits_end ());
               i != end;
               ++i) s.insert (i->name ());

          return s;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        InterfaceDefRefSetName inherits_;
      };

      typedef
      DeclarationOrderComparator<InterfaceDefPtr>
      InterfaceDefOrderComparator;

      typedef
      std::set<InterfaceDefPtr, InterfaceDefOrderComparator>
      InterfaceDefSet;

      typedef
      DeclarationRefOrderComparator<InterfaceDefRef>
      InterfaceDefRefOrderComparator;

      typedef
      std::set<InterfaceDefRef, InterfaceDefRefOrderComparator>
      InterfaceDefRefSet;


      //
      //
      //
      class AbstractInterfaceDecl : public virtual InterfaceDecl
      {
      protected:
        virtual
        ~AbstractInterfaceDecl () throw () {}

        AbstractInterfaceDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<AbstractInterfaceDecl>
      AbstractInterfaceDeclPtr;

      //
      //
      //
      class AbstractInterfaceForwardDecl : public virtual InterfaceForwardDecl,
                                           public virtual AbstractInterfaceDecl
      {
      public:
        virtual
        ~AbstractInterfaceForwardDecl () throw () {}

        AbstractInterfaceForwardDecl (SimpleName const& name,
                                      ScopePtr const& scope)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

      protected:
        AbstractInterfaceForwardDecl (SimpleName const& name,
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
          return TypeDeclPtr (
            new AbstractInterfaceForwardDecl (name, order, scope, ch));
        }

      public:
        virtual std::string
        declaration_class ()
        {
          return "abstract interface";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class AbstractInterfaceDef : public virtual AbstractInterfaceDecl,
                                   public virtual InterfaceDef
      {
      public:
        virtual
        ~AbstractInterfaceDef () throw () {}

        AbstractInterfaceDef (SimpleName const& name,
                              ScopePtr const& scope,
                              ScopedNameSet const& inherits)
            : Declaration (name, scope),
              InterfaceDef (inherits)
        {
          type_info (static_type_info ());
        }

      protected:
        AbstractInterfaceDef (SimpleName const& name,
                              Order const& order,
                              ScopePtr const& scope,
                              ContextHolderPtr const& ch,
                              ScopedNameSet const& inherits)
            : Node (ch),
              Declaration (name, order, scope),
              InterfaceDef (inherits)
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
          return TypeDeclPtr (
            new AbstractInterfaceDef (name, order, scope, ch, inherits ()));
        }


      public:
        virtual std::string
        declaration_class ()
        {
          return "abstract interface";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<AbstractInterfaceDef>
      AbstractInterfaceDefPtr;

      //
      //
      //
      class LocalInterfaceDecl : public virtual InterfaceDecl
      {
      protected:
        virtual
        ~LocalInterfaceDecl () throw () {}

        LocalInterfaceDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<LocalInterfaceDecl>
      LocalInterfaceDeclPtr;


      //
      //
      //
      class LocalInterfaceForwardDecl : public virtual InterfaceForwardDecl,
                                        public virtual LocalInterfaceDecl
      {
      public:
        virtual
        ~LocalInterfaceForwardDecl () throw () {}

        LocalInterfaceForwardDecl (SimpleName const& name,
                                   ScopePtr const& scope)
            : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

      protected:
        LocalInterfaceForwardDecl (SimpleName const& name,
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
          return TypeDeclPtr (
            new LocalInterfaceForwardDecl (name, order, scope, ch));
        }

      public:
        virtual std::string
        declaration_class ()
        {
          return "local interface";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class LocalInterfaceDef : public virtual LocalInterfaceDecl,
                                public virtual InterfaceDef
      {
      public:
        virtual
        ~LocalInterfaceDef () throw () {}

        LocalInterfaceDef (SimpleName const& name,
                           ScopePtr const& scope,
                           ScopedNameSet const& inherits)
            : Declaration (name, scope),
              InterfaceDef (inherits)
        {
          type_info (static_type_info ());
        }

      protected:
        LocalInterfaceDef (SimpleName const& name,
                           Order const& order,
                           ScopePtr const& scope,
                           ContextHolderPtr const& ch,
                           ScopedNameSet const& inherits)
            : Node (ch),
              Declaration (name, order, scope),
              InterfaceDef (inherits)
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
          return TypeDeclPtr (
            new LocalInterfaceDef (name, order, scope, ch, inherits ()));
        }


      public:
        virtual std::string
        declaration_class ()
        {
          return "local interface";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<LocalInterfaceDef>
      LocalInterfaceDefPtr;


      //
      //
      //
      class UnconstrainedInterfaceDecl : public virtual InterfaceDecl
      {
      protected:
        virtual
        ~UnconstrainedInterfaceDecl () throw () {}

        UnconstrainedInterfaceDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<UnconstrainedInterfaceDecl>
      UnconstrainedInterfaceDeclPtr;


      typedef
      DeclarationOrderComparator<UnconstrainedInterfaceDeclPtr>
      UnconstrainedInterfaceDeclOrderComparator;

      typedef
      std::set<UnconstrainedInterfaceDeclPtr,
               UnconstrainedInterfaceDeclOrderComparator>
      UnconstrainedInterfaceDeclSet;


      //
      //
      //
      class UnconstrainedInterfaceForwardDecl :
        public virtual InterfaceForwardDecl,
        public virtual UnconstrainedInterfaceDecl
      {
      public:
        virtual
        ~UnconstrainedInterfaceForwardDecl () throw () {}

         UnconstrainedInterfaceForwardDecl (SimpleName const& name,
                                            ScopePtr const& scope)
             : Declaration (name, scope)
        {
          type_info (static_type_info ());
        }

        UnconstrainedInterfaceForwardDecl (SimpleName const& name,
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
          return TypeDeclPtr (
            new UnconstrainedInterfaceForwardDecl (name, order, scope, ch));
        }

      public:
        virtual std::string
        declaration_class ()
        {
          return "unconstrained interface";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };


      //
      //
      //
      class UnconstrainedInterfaceDef :
        public virtual UnconstrainedInterfaceDecl,
        public virtual InterfaceDef
      {
      public:
        virtual
        ~UnconstrainedInterfaceDef () throw () {}

        UnconstrainedInterfaceDef (SimpleName const& name,
                                   ScopePtr const& scope,
                                   ScopedNameSet const& inherits)
            : Declaration (name, scope),
              InterfaceDef (inherits)
        {
          type_info (static_type_info ());
        }

      protected:
        UnconstrainedInterfaceDef (SimpleName const& name,
                                   Order const& order,
                                   ScopePtr const& scope,
                                   ContextHolderPtr const& ch,
                                   ScopedNameSet const& inherits)
            : Node (ch),
              Declaration (name, order, scope),
              InterfaceDef (inherits)
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
          return TypeDeclPtr ( new UnconstrainedInterfaceDef (
                                 name, order, scope, ch, inherits ()));
        }


      public:
        virtual std::string
        declaration_class ()
        {
          return "unconstrained interface";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<UnconstrainedInterfaceDef>
      UnconstrainedInterfaceDefPtr;


      typedef
      DeclarationOrderComparator<UnconstrainedInterfaceDefPtr>
      UnconstrainedInterfaceDefOrderComparator;

      typedef
      std::set<UnconstrainedInterfaceDefPtr,
               UnconstrainedInterfaceDefOrderComparator>
      UnconstrainedInterfaceDefSet;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_INTERFACE_HPP
