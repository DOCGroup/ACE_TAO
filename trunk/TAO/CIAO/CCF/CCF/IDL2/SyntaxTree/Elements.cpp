// file      : CCF/IDL2/SyntaxTree/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

using namespace Introspection;

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      // Node
      //
      //
      namespace
      {
        TypeInfo
        node_init_ ()
        {
          TypeInfo ti (typeid (Node));
	  // I don't really need this information
          // ti.add_base (Access::PUBLIC, true, Object::static_type_info ());
          return ti;
        }

        TypeInfo node_ (node_init_ ());
      }

      TypeInfo const& Node::
      static_type_info () { return node_; }


      // Comma
      //
      //
      namespace
      {
        TypeInfo
        comma_init_ ()
        {
          TypeInfo ti (typeid (Comma));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo comma_ (comma_init_ ());
      }

      TypeInfo const& Comma::
      static_type_info () { return comma_; }


      // Declaration
      //
      //
      namespace
      {
        TypeInfo
        declaration_init_ ()
        {
          TypeInfo ti (typeid (Declaration));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo declaration_ (declaration_init_ ());
      }

      TypeInfo const& Declaration::
      static_type_info () { return declaration_; }

      Declaration::
      Declaration (SimpleName const& name, ScopePtr const& scope)
          : order_ (scope->create_order ()),
            name_ (scope->name (), name),
            scope_ (scope->table (),
                    scope->name (),
                    scope->order ()) // Hint: scope->scope () may throw
      {
        type_info (static_type_info ());
      }

      Declaration::
      Declaration (ScopedName const& name,
                   Order const& order,
                   DeclarationTable const& table)
          : order_ (order),
            name_ (name),
            scope_ (table) // this stuff is faked
      {
        type_info (static_type_info ());
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


      // DeclarationTable
      //
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
          if ((*pair.first)->declaration_class () != d->declaration_class ())
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
                /*
                  std::cerr << std::endl
                  << "before condition failed : "
                  << (*pair.first)->order () << " ("
                  << (*pair.first)->name ()
                  << ") expected to be before "
                  << before << std::endl; */
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


      // Scope
      //
      //
      namespace
      {
        TypeInfo
        scope_init_ ()
        {
          TypeInfo ti (typeid (Scope));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo scope_ti_ (scope_init_ ());
      }

      TypeInfo const& Scope::
      static_type_info () { return scope_ti_; }

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


      //
      // TypeDecl
      //
      namespace
      {
        TypeInfo
        type_decl_init_ ()
        {
          TypeInfo ti (typeid (TypeDecl));
          ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
          return ti;
        }

        TypeInfo type_decl_ (type_decl_init_ ());
      }

      TypeInfo const& TypeDecl::
      static_type_info () { return type_decl_; }


      // TypeForwardDecl
      //
      //
      namespace
      {
        TypeInfo
        type_forward_decl_init_ ()
        {
          TypeInfo ti (typeid (TypeForwardDecl));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo type_forward_decl_ (type_forward_decl_init_ ());
      }

      TypeInfo const& TypeForwardDecl::
      static_type_info () { return type_forward_decl_; }


      // TypeDef
      //
      //
      namespace
      {
        TypeInfo
        type_def_init_ ()
        {
          TypeInfo ti (typeid (TypeDef));
          ti.add_base (Access::PUBLIC, true, TypeDecl::static_type_info ());
          return ti;
        }

        TypeInfo type_def_ (type_def_init_ ());
      }

      TypeInfo const& TypeDef::
      static_type_info () { return type_def_; }
    }
  }
}
