// file      : CCF/IDL2/SyntaxTree/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

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
      class InterfaceDecl : public virtual TypeDecl
      {
      public:
        virtual
        ~InterfaceDecl () throw () {}

        InterfaceDecl (SimpleName const& name,
                       ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "interface";
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
      public:
        virtual
        ~InterfaceForwardDecl () throw () {}

        InterfaceForwardDecl (SimpleName const& name,
                              ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              TypeForwardDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "interface";
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
      public:
        virtual
        ~InterfaceDef () throw () {}

        InterfaceDef (SimpleName const& name,
                      ScopePtr const& scope,
                      ScopedNameSet const& inherits)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope)
        {
          type_info (static_type_info ());

          for (ScopedNameSet::const_iterator i = inherits.begin ();
               i != inherits.end ();
               i++)
          {
            inherits_.insert (InterfaceDefRef (scope->table (), *i));
          }
        }

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

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "interface";
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
      public:
        virtual
        ~AbstractInterfaceDecl () throw () {}

        // forward-declared unconstrained interface c-tor
        AbstractInterfaceDecl (SimpleName const& name,
                               ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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

        // forward-declared unconstrained interface c-tor
        AbstractInterfaceForwardDecl (SimpleName const& name,
                                      ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              TypeForwardDecl (name, scope),
              InterfaceForwardDecl (name, scope),
              AbstractInterfaceDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              AbstractInterfaceDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              InterfaceDef (name, scope, inherits)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
      public:
        virtual
        ~LocalInterfaceDecl () throw () {}

        LocalInterfaceDecl (SimpleName const& name,
                            ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              TypeForwardDecl (name, scope),
              InterfaceForwardDecl (name, scope),
              LocalInterfaceDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              LocalInterfaceDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              InterfaceDef (name, scope, inherits)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
      public:
        virtual
        ~UnconstrainedInterfaceDecl () throw () {}

        UnconstrainedInterfaceDecl (SimpleName const& name,
                                    ScopePtr const& scope)
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
            : Declaration (name, scope),
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              TypeForwardDecl (name, scope),
              InterfaceForwardDecl (name, scope),
              UnconstrainedInterfaceDecl (name, scope)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
              TypeDecl (name, scope),
              InterfaceDecl (name, scope),
              UnconstrainedInterfaceDecl (name, scope),
              TypeDef (name, scope),
              Scope (name, scope),
              InterfaceDef (name, scope, inherits)
        {
          type_info (static_type_info ());
        }

        // Runtime declaration type information
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
