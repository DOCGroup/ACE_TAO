// $Id$
#include "IDL2_Traversal.hpp"
#include "IDL2_SyntaxTree.hpp"

//@@tmp
#include <iostream>
using std::cerr;
using std::endl;

namespace IDL2
{
  namespace SyntaxTree
  {
    using ReferenceCounting::add_ref;

    using CCF::Traversal::try_visit;

    namespace
    {
      bool
      try_visit_scope (CCF::Traversal::Visitor* v,
                       ScopePtr const& node)
      {
        return try_visit<Traversal::Scope> (&Traversal::Scope::visit_scope, v, node);
      }

      bool
      try_visit_type_decl (CCF::Traversal::Visitor* v,
                          TypeDeclPtr const& node)
      {
        return try_visit<Traversal::TypeDecl> (&Traversal::TypeDecl::visit_type_decl, v, node);
      }

      bool
      try_visit_type_def (CCF::Traversal::Visitor* v,
                          TypeDefPtr const& node)
      {
        return try_visit<Traversal::TypeDef> (&Traversal::TypeDef::visit_type_def, v, node);
      }

      bool
      try_visit_interface_decl (CCF::Traversal::Visitor* v,
                                InterfaceDeclPtr const& node)
      {
        return try_visit<Traversal::InterfaceDecl> (&Traversal::InterfaceDecl::visit_interface_decl,
                          v,
                          node);
      }

      bool
      try_visit_interface_def (CCF::Traversal::Visitor* v,
                               InterfaceDefPtr const& node)
      {
        return try_visit<Traversal::InterfaceDef> (
          &Traversal::InterfaceDef::visit_interface_def,
                          v,
                          node);
      }

      bool
      try_visit_abstract_interface_decl (
        CCF::Traversal::Visitor* v,
        AbstractInterfaceDeclPtr const& node)
      {
        return try_visit<Traversal::AbstractInterfaceDecl> (
          &Traversal::AbstractInterfaceDecl::visit_abstract_interface_decl,
          v,
          node);
      }

      bool
      try_visit_local_interface_decl (
        CCF::Traversal::Visitor* v,
        LocalInterfaceDeclPtr const& node)
      {
        return try_visit<Traversal::LocalInterfaceDecl> (
          &Traversal::LocalInterfaceDecl::visit_local_interface_decl,
          v,
          node);
      }

      bool
      try_visit_unconstrained_interface_decl (
        CCF::Traversal::Visitor* v,
        UnconstrainedInterfaceDeclPtr const& node)
      {
        return try_visit<Traversal::UnconstrainedInterfaceDecl> (
          &Traversal::UnconstrainedInterfaceDecl::
            visit_unconstrained_interface_decl,
          v,
          node);
      }
    }

    //
    // Comma
    //
    void Comma::
    accept (CCF::Traversal::Visitor* v)
    {
      CommaPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::Comma> (&Traversal::Comma::visit_comma, v, self);
    }


    //
    // Declaration
    //
    Declaration::
    Declaration (SimpleName const& name, ScopePtr const& scope)
        : order_ (scope->create_order ()),
          name_ (scope->name (), name),
          scope_ (scope->table (),
                  scope->name (),
                  scope->order ()) // Hint: scope->scope () may throw
    {
    }

    Declaration::
    Declaration (ScopedName const& name,
                 Order const& order,
                 DeclarationTable const& table)
        : order_ (order),
          name_ (name),
          scope_ (table) // this stuff is faked
    {
    }

    ScopedName Declaration::
    name () const
    {
      return name_;
    }

    Order Declaration::
    order () const
    {
      return order_;
    }

    ScopePtr Declaration::
    scope () const throw (NotInScope)
    {
      return scope_.resolve ();
    }

    //
    // DeclarationTable
    //

    void DeclarationTable::
    insert (DeclarationPtr d) throw (InvalidArgument,
                                     AlreadyExist,
                                     TypeMismatch)
    {
      if (d == 0) throw InvalidArgument ();

      IteratorPair pair = lookup (d->name ());

      if (pair.first != pair.second)
      {
        // I have some declarations with this name already
        if ((*pair.first)->declaration_type () != d->declaration_type ())
        {
          throw TypeMismatch ();
        }
      }

      std::pair<DeclarationSet::iterator, bool> result =
        decl_set_.insert (d);

      if (result.second == false) throw AlreadyExist ();

      try
      {
        if(name_decl_map_[d->name ()].insert (d).second == false)
        {
          throw AlreadyExist ();
        }
      }
      catch (...)
      {
        // Leaving table in consistent state
        decl_set_.erase (result.first);
        throw;
      }
    }

    DeclarationTable::IteratorPair DeclarationTable::
    lookup (ScopedName const& n) const
    {
      NameDeclarationMap::const_iterator i = name_decl_map_.find (n);
      if (i == name_decl_map_.end ())
      {
        return IteratorPair (decl_set_.end (), decl_set_.end ());
      }
      else
      {
        return IteratorPair (i->second.begin (), i->second.end ());
      }
    }

    DeclarationTable::Iterator DeclarationTable::
    begin () const
    {
      return decl_set_.begin ();
    }

    DeclarationTable::Iterator DeclarationTable::
    end () const
    {
      return decl_set_.end ();
    }

    ScopedName DeclarationTable::
    resolve (Name const& name,
             ScopedName const& from,
             Order const& before,
             ResolvePredicate& p) const throw (ResolutionFailure)
    {
      ScopedName result ("");

      // Check if name is already scoped
      try
      {
        result = ScopedName (name);
      }
      catch (ScopedName::InvalidArgument const&)
      {
        //seems not
        try
        {
          for (ScopedName base = from;;base = base.scope ())
          {
            result = ScopedName (base, name);

            //cerr << "*** resolve: considering: " << result << endl;

            IteratorPair pair = lookup (result);

            if (pair.first != pair.second)
            {
              // Found something. Now checking the order. Note that the
              // first element has the smallest order so it's sufficient
              // to check just the first element for now.
              if ((*pair.first)->order () < before)
              {
                break;
              }
              std::cerr << std::endl
                        << "before condition failed : "
                        << (*pair.first)->order () << " ("
                        << (*pair.first)->name ()
                        << ") expected to be before "
                        << before << std::endl;
            }
          }
        }
        catch (ScopedName::AtRoot const&)
        {
          // didn't find anything
          throw NameNotFound ();
        }
      }

      // Now check predicat
      IteratorPair pair = lookup (result);

      if (pair.first == pair.second) throw NameNotFound ();

      for (;pair.first != pair.second; pair.first++)
      {
        // we should stay before
        if (!((*pair.first)->order () < before)) break;

        if (p.test (*pair.first)) return result;
      }

      throw PredicateNotMet ();
    }

    //
    // Scope
    //

    void Scope::
    insert (DeclarationPtr d) throw (InvalidArgument, AlreadyExist)
    {
      if (d == 0) throw InvalidArgument ();

      std::pair<DeclarationSet::iterator, bool> result =
        content_.insert (d);

      if (result.second == false) throw AlreadyExist ();

      try
      {
        try
        {
          table_.insert (d);
        }
        catch (...)
        {
          //Keep consistent state
          content_.erase (result.first);
          throw;
        }
      }
      catch (DeclarationTable::InvalidArgument const&)
      {
        throw InvalidArgument ();
      }
      catch (DeclarationTable::AlreadyExist const&)
      {
        throw AlreadyExist ();
      }
    }

    Scope::Iterator Scope::
    begin () const
    {
      return content_.begin ();
    }

    Scope::Iterator Scope::
    end () const
    {
      return content_.end ();
    }

    Order Scope::
    create_order ()
    {
      return Order (order (), next_order_++);
    }

    Order Scope::
    peek_order ()
    {
      return Order (order (), next_order_);
    }

    void Scope::
    accept (CCF::Traversal::Visitor* v)
    {
      ScopePtr self (ReferenceCounting::add_ref (this));
      try_visit_scope (v, self);
    }

    //
    // TypeDecl
    //

    void TypeDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      TypeDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::TypeDecl> (&Traversal::TypeDecl::visit_type_decl, v, self);
    }


    //
    // TypeDef
    //

    void TypeDef::
    accept (CCF::Traversal::Visitor* v)
    {
      TypeDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::TypeDef> (
           &Traversal::TypeDef::visit_type_def,
           v,
           self))
      {
        TypeDecl::accept (v);
      }
    }

    //
    // InterfaceDecl
    //

    void InterfaceDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      InterfaceDeclPtr self (ReferenceCounting::add_ref (this));

      if (!try_visit_interface_decl (v, self))
      {
        TypeDecl::accept (v);
      }
    }

    //
    // InterfaceDef
    //

    void InterfaceDef::
    accept (CCF::Traversal::Visitor* v)
    {
      InterfaceDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit_interface_def (v, self))
      {
        InterfaceDecl::accept (v);
        Scope::accept (v);
      }
    }


    //
    // AbstractInterfaceDecl
    //

    void AbstractInterfaceDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      AbstractInterfaceDeclPtr self (ReferenceCounting::add_ref (this));

      if (!try_visit_abstract_interface_decl (v, self))
      {
        InterfaceDecl::accept (v);
      }
    }

    //
    // AbstractInterfaceDef
    //

    void AbstractInterfaceDef::
    accept (CCF::Traversal::Visitor* v)
    {
      AbstractInterfaceDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::AbstractInterfaceDef> (
           &Traversal::AbstractInterfaceDef::visit_abstract_interface_def,
           v,
           self))
      {
        AbstractInterfaceDecl::accept (v);
        InterfaceDef::accept (v);
      }
    }

    //
    // LocalInterfaceDecl
    //

    void LocalInterfaceDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      LocalInterfaceDeclPtr self (ReferenceCounting::add_ref (this));

      if (!try_visit_local_interface_decl (v, self))
      {
        InterfaceDecl::accept (v);
      }
    }


    //
    // LocalInterfaceDef
    //

    void LocalInterfaceDef::
    accept (CCF::Traversal::Visitor* v)
    {
      LocalInterfaceDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::LocalInterfaceDef> (
           &Traversal::LocalInterfaceDef::visit_local_interface_def,
           v,
           self))
      {
        bool b = try_visit_interface_def (v, self);
        bool a = try_visit_local_interface_decl (v, self);

        // if (a && b) || a
        if (a) return;

        if (b)
        {
          try_visit_type_def (v, self);
          try_visit_scope (v, self);
        }
        else
        {
          try_visit_scope (v, self);
          a = try_visit_type_def (v, self);
          b = try_visit_interface_decl (v, self);

          if (a || b) return;

          try_visit_type_decl (v, self);
        }
      }
    }

    //
    // UnconstrainedInterfaceDecl
    //

    void UnconstrainedInterfaceDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      UnconstrainedInterfaceDeclPtr self (ReferenceCounting::add_ref (this));

      if (!try_visit_unconstrained_interface_decl (v, self))
      {
        InterfaceDecl::accept (v);
      }
    }


    //
    // UnconstrainedInterfaceDef
    //

    void UnconstrainedInterfaceDef::
    accept (CCF::Traversal::Visitor* v)
    {
      UnconstrainedInterfaceDefPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::UnconstrainedInterfaceDef> (
           &Traversal::UnconstrainedInterfaceDef::
             visit_unconstrained_interface_def,
           v,
           self))
      {
        bool b = try_visit_interface_def (v, self);
        bool a = try_visit_unconstrained_interface_decl (v, self);

        // if (a && b) || a
        if (a) return;

        if (b)
        {
          try_visit_type_def (v, self);
          try_visit_scope (v, self);
        }
        else
        {
          try_visit_scope (v, self);
          a = try_visit_type_def (v, self);
          b = try_visit_interface_decl (v, self);

          if (a || b) return;

          try_visit_type_decl (v, self);
        }
      }
    }

    //
    // AttributeDecl
    //

    void AttributeDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      AttributeDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::AttributeDecl> (
        &Traversal::AttributeDecl::visit_attribute_decl,
        v,
        self);
    }

    //
    // OperationParameter
    //
    void OperationParameter::
    accept (CCF::Traversal::Visitor* v)
    {
      OperationParameterPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::OperationParameter> (&Traversal::OperationParameter::visit_operation_parameter,
                 v,
                 self);
    }


    //
    // OperationDecl
    //
    void OperationDecl::
    accept (CCF::Traversal::Visitor* v)
    {
      OperationDeclPtr self (ReferenceCounting::add_ref (this));
      try_visit<Traversal::OperationDecl> (
        &Traversal::OperationDecl::visit_operation_decl,
        v,
        self);
    }


    //
    // Module
    //

    void Module::
    accept (CCF::Traversal::Visitor* v)
    {
      ModulePtr self (ReferenceCounting::add_ref (this));

      if (!try_visit<Traversal::Module> (&Traversal::Module::visit_module,
                                         v,
                                         self))
      {
        Scope::accept (v);
      }
    }

    //
    // FileScope
    //

    FileScope::
    FileScope (DeclarationTable& table, Order const& order)
        : Declaration (ScopedName (""), order, table),
          Scope (table, ScopedName (""), order)
    {
    }

    void FileScope::
    accept (CCF::Traversal::Visitor* v)
    {
      FileScopePtr self (ReferenceCounting::add_ref (this));

      if (!try_visit<Traversal::FileScope> (
            &Traversal::FileScope::visit_file_scope,
            v,
            self))
      {
        Scope::accept (v);
      }
    }

    //
    // TranslationRegion
    //

    void TranslationRegion::
    insert (TranslationRegionPtr const& tr) throw (InvalidArgument)
    {
      if (tr == 0) throw InvalidArgument ();

      region_list_.push_back (tr);
    }

    TranslationRegion::Iterator TranslationRegion::
    begin () const
    {
      return region_list_.begin ();
    }

    TranslationRegion::Iterator TranslationRegion::
    end () const
    {
      return region_list_.end ();
    }

    void TranslationRegion::
    accept (CCF::Traversal::Visitor* v)
    {
      TranslationRegionPtr self (ReferenceCounting::add_ref (this));

      try_visit<Traversal::TranslationRegion> (
        &Traversal::TranslationRegion::visit_translation_region,
                 v,
                 self);
    }

    //
    // IncludeTranslationRegion
    //

    void IncludeTranslationRegion::
    accept (CCF::Traversal::Visitor* v)
    {
      IncludeTranslationRegionPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::IncludeTranslationRegion>(
           &Traversal::IncludeTranslationRegion::
             visit_include_translation_region,
           v,
           self)) TranslationRegion::accept (v);
    }

    //
    // SysIncludeTranslationRegion
    //

    void SysIncludeTranslationRegion::
    accept (CCF::Traversal::Visitor* v)
    {
      SysIncludeTranslationRegionPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::SysIncludeTranslationRegion>(
           &Traversal::SysIncludeTranslationRegion::
             visit_sys_include_translation_region,
           v,
           self)) TranslationRegion::accept (v);
    }

    //
    // ImpliedIncludeTranslationRegion
    //

    void ImpliedIncludeTranslationRegion::
    accept (CCF::Traversal::Visitor* v)
    {
      ImpliedIncludeTranslationRegionPtr self (
        ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::ImpliedIncludeTranslationRegion>(
           &Traversal::ImpliedIncludeTranslationRegion::
             visit_implied_include_translation_region,
           v,
           self)) TranslationRegion::accept (v);
    }

    //
    // PrincipalTranslationRegion
    //

    void PrincipalTranslationRegion::
    accept (CCF::Traversal::Visitor* v)
    {
      PrincipalTranslationRegionPtr self (ReferenceCounting::add_ref (this));

      if(!try_visit<Traversal::PrincipalTranslationRegion>(
           &Traversal::PrincipalTranslationRegion::
             visit_principal_translation_region,
           v,
           self)) TranslationRegion::accept (v);
    }

    //
    // TranslationUnit
    //

    void TranslationUnit::
    insert (TranslationRegionPtr const& tr) throw (InvalidArgument)
    {
      if (tr == 0) throw InvalidArgument ();

      region_list_.push_back (tr);
    }

    TranslationUnit::Iterator TranslationUnit::
    begin () const
    {
      return region_list_.begin ();
    }

    TranslationUnit::Iterator TranslationUnit::
    end () const
    {
      return region_list_.end ();
    }

    void TranslationUnit::
    accept (CCF::Traversal::Visitor* v)
    {
      TranslationUnitPtr self (ReferenceCounting::add_ref (this));

      try_visit<Traversal::TranslationUnit> (
        &Traversal::TranslationUnit::visit_translation_unit,
                 v,
                 self);
    }
  }
}
