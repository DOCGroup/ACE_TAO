namespace IDL2
{
  namespace SyntaxTree
  {

    //
    // DeclarationRef
    //
    template <typename T, typename Ptr>
    DeclarationRef<T, Ptr>::
    DeclarationRef (DeclarationTable const& table, ScopedName const& name)
        : table_ (table),
          name_ (name),
          order_ (0),
          initialized_ (true),
          with_order_ (false)
    {
    }

    template <typename T, typename Ptr>
    DeclarationRef<T, Ptr>::
    DeclarationRef (DeclarationTable const& table,
                    ScopedName const& name,
                    Order const& order)
        : table_ (table),
          name_ (name),
          order_ (order),
          initialized_ (true),
          with_order_ (true)
    {
    }

    template <typename T, typename Ptr>
    DeclarationRef<T, Ptr>::
    DeclarationRef (DeclarationPtr decl)
        : table_ (decl->scope ()->table ()),
          name_ (decl->name ()),
          order_ (decl->order ()),
          initialized_ (true),
          with_order_ (true)
    {
    }

    template <typename T, typename Ptr>
    DeclarationRef<T, Ptr>::
    DeclarationRef (DeclarationTable const& table)
        : table_ (table),
          name_ (""),
          order_ (0),
          initialized_ (false),
          with_order_ (false)
    {
    }

    template <typename T, typename Ptr>
    Ptr DeclarationRef<T, Ptr>::
    resolve () const throw (NotInitialized, DeclarationNotFound, TypeMismatch)
    {
      if (!initialized_) throw NotInitialized ();

      try
      {
        //@@ gcc bug
        return with_order_
          ? table_.template lookup<T> (name_, order_)
          : table_.template lookup<T> (name_);
      }
      catch (DeclarationTable::TypeMismatch const&)
      {
        throw TypeMismatch ();
      }
      catch (DeclarationTable::DeclarationNotFound const&)
      {
        throw DeclarationNotFound ();
      }
    }

    //
    // DeclarationTable
    //
    template <typename T>
    StrictPtr<T> DeclarationTable::
    lookup (ScopedName const& n) const
      throw (DeclarationNotFound, TypeMismatch)
    {
      IteratorPair pair = lookup (n);

      if (pair.first == pair.second) throw DeclarationNotFound ();

      for (; pair.first != pair.second; pair.first++)
      {
        DeclarationPtr d = (*pair.first);

        //@@ gcc bug
        StrictPtr<T> p ((*pair.first)->template dynamic_type<T> ());

        if (p != 0) return p;
      }

      throw TypeMismatch ();
    }


    template <typename T>
    StrictPtr<T> DeclarationTable::
    lookup (ScopedName const& n, Order const& o) const
      throw (DeclarationNotFound, TypeMismatch)
    {
      IteratorPair pair = lookup (n);

      for (; pair.first != pair.second; pair.first++)
      {
        if ((*pair.first)->order () != o) continue;

        //@@ gcc bug
        StrictPtr<T> p ((*pair.first)->template dynamic_type<T> ());

        if (p != 0) return p;

        throw TypeMismatch ();
      }

      throw DeclarationNotFound ();
    }
  }
}
