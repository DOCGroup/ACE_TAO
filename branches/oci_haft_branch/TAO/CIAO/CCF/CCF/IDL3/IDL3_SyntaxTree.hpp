// $Id$
#ifndef CCF_IDL3_SYNTAX_TREE_HPP
#define CCF_IDL3_SYNTAX_TREE_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_SyntaxTree.hpp"

namespace IDL3
{
  namespace SyntaxTree
  {
    // Import nodes from IDL2
    using namespace IDL2::SyntaxTree;


    //
    //
    //
    class ComponentDecl : public virtual TypeDecl
    {
    public:
      virtual
      ~ComponentDecl () throw () {}

      ComponentDecl (SimpleName const& name,
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
        return "component";
      }


      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

    typedef
    StrictPtr<ComponentDecl>
    ComponentDeclPtr;

    typedef
    DeclarationRef<ComponentDecl>
    ComponentDeclRef;


    //
    //
    //
    class ComponentForwardDecl : public virtual ComponentDecl,
                                 public virtual TypeForwardDecl
    {
    public:
      virtual
      ~ComponentForwardDecl () throw () {}

      ComponentForwardDecl (SimpleName const& name,
                            ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            ComponentDecl (name, scope),
            TypeForwardDecl (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "component";
      }


      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };



    //
    //
    //
    class ComponentDef;

    typedef
    StrictPtr<ComponentDef>
    ComponentDefPtr;

    typedef
    DeclarationRef<ComponentDef>
    ComponentDefRef;

    class ComponentDef : public virtual TypeDef,
                         public virtual ComponentDecl,
                         public virtual Scope
    {
    public:
      virtual
      ~ComponentDef () throw () {}

      class DuplicateName {};

      ComponentDef (SimpleName const& name,
                    ScopePtr const& scope,
                    ScopedName const& inherits,
                    ScopedNameSet const& supports) throw (DuplicateName)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            TypeDef (name, scope),
            ComponentDecl (name, scope),
            Scope (name, scope),
            inherits_ (scope->table (), inherits)
      {
        copy_supports_list (supports);
      }

      ComponentDef (SimpleName const& name,
                    ScopePtr const& scope,
                    ScopedNameSet const& supports) throw (DuplicateName)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            TypeDef (name, scope),
            ComponentDecl (name, scope),
            Scope (name, scope),
            inherits_ (scope->table ())
      {
        copy_supports_list (supports);
      }

    private:
      void
      copy_supports_list (ScopedNameSet const& supports)
      {
        for (ScopedNameSet::const_iterator i = supports.begin ();
             i != supports.end ();
             i++)
        {
          if (!supports_.insert (
                InterfaceDefRef (scope ()->table (), *i)).second)
          {
            //@@ this never happens
            throw DuplicateName ();
          }
        }
      }


    public:
      ComponentDefRef
      inherits ()
      {
        return inherits_;
      }


    public:

      typedef
      InterfaceDefRefSetName::const_iterator
      Iterator;

      Iterator
      supports_begin () const
      {
        return supports_.begin ();
      }

      Iterator
      supports_end () const
      {
        return supports_.end ();
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "component";
      }

    private:
      ComponentDefRef inherits_;
      InterfaceDefRefSetName supports_;
    };

    typedef
    DeclarationOrderComparator<ComponentDefPtr>
    ComponentDefOrderComparator;

    typedef
    std::set<ComponentDefPtr, ComponentDefOrderComparator>
    ComponentDefSet;

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
      declaration_type ()
      {
        return "provides";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      InterfaceDeclRef type_;
    };


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
      declaration_type ()
      {
        return "uses";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      TypeDeclRef type_;
    };


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
      declaration_type ()
      {
        return "publishes";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      TypeDeclRef type_;
    };


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
      declaration_type ()
      {
        return "emits";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      TypeDeclRef type_;
    };


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
      declaration_type ()
      {
        return "consumes";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      TypeDeclRef type_;
    };



    // According to the spec Home cannot be forward-declared. Did they
    // goof again? - you would probably think...
    //

    class HomeDef;

    typedef
    StrictPtr<HomeDef>
    HomeDefPtr;

    typedef
    DeclarationRef<HomeDef>
    HomeDefRef;

    class HomeDef : public virtual TypeDef,
                    public virtual Scope
    {
    public:
      virtual
      ~HomeDef () throw () {}

      class DuplicateName {};

      HomeDef (SimpleName const& name,
               ScopePtr const& scope,
               ScopedName const& inherits,
               ScopedNameSet const& supports,
               ScopedName const& manages) throw (DuplicateName)

          : Declaration (name, scope),
            TypeDecl (name, scope),
            TypeDef (name, scope),
            Scope (name, scope),
            inherits_ (scope->table (), inherits),
            manages_ (scope->table (), manages)
      {
        copy_supports_list (supports);
      }

      HomeDef (SimpleName const& name,
               ScopePtr const& scope,
               ScopedNameSet const& supports,
               ScopedName const& manages) throw (DuplicateName)

          : Declaration (name, scope),
            TypeDecl (name, scope),
            TypeDef (name, scope),
            Scope (name, scope),
            inherits_ (scope->table ()),
            manages_ (scope->table (), manages)
      {
        copy_supports_list (supports);
      }

    private:
      void
      copy_supports_list (ScopedNameSet const& supports)
      {
        for (ScopedNameSet::const_iterator i = supports.begin ();
             i != supports.end ();
             i++)
        {
          if (!supports_.insert (
                InterfaceDefRef (scope ()->table (), *i)).second)
          {
            //@@ this never happens
            throw DuplicateName ();
          }
        }
      }

    public:
      HomeDefRef
      inherits ()
      {
        return inherits_;
      }

    public:
      typedef
      InterfaceDefRefSetName::const_iterator
      Iterator;

      Iterator
      supports_begin ()
      {
        return supports_.begin ();
      }

      Iterator
      supports_end ()
      {
        return supports_.end ();
      }

    public:
      ComponentDefPtr
      manages () const
      {
        return manages_.resolve ();
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "home";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      HomeDefRef inherits_;
      InterfaceDefRefSetName supports_;
      ComponentDefRef manages_;
    };

    typedef
    DeclarationOrderComparator<HomeDefPtr>
    HomeDefOrderComparator;

    typedef
    std::set<HomeDefPtr, HomeDefOrderComparator>
    HomeDefSet;


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
      }
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
      }

      //@@ It would be nice to be able to kind of virtual override
      //   type () mf from OperationDecl with covariant return type
      //   technique.


      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "home factory";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };
    typedef
    StrictPtr<HomeFactoryDecl>
    HomeFactoryDeclPtr;
  }
}

//@@ tmp
#include "CCF/IDL3/IDL3_SyntaxTree_EventType.hpp"

#endif // CCF_IDL3_SYNTAX_TREE_HPP
