// $Id$
#ifndef CCF_IDL2_SYNTAX_TREE_HPP
#define CCF_IDL2_SYNTAX_TREE_HPP

#include "MSVC_Pragmas.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>

#include <iostream>

#include "CCF/CompilerElements/FileSystem.hpp"
#include "CCF/CompilerElements/ReferenceCounting.hpp"

#include "CCF/CompilerElements/Traversal.hpp"

//@@ It doesn't seem that traversal mechanism has to be embeded
//   into SyntaxTree anymore.

namespace IDL2
{
  namespace SyntaxTree
  {
    using ReferenceCounting::StrictPtr;
    using ReferenceCounting::strict_cast;


    //
    // Can be scoped or simple.
    //
    class Name
    {
    public:
      explicit
      Name (std::string const& name)
          : name_ (name)
      {
      }

      bool
      operator< (Name const& other) const
      {
        return name_ < other.name_;
      }

      bool
      operator== (Name const& other) const
      {
        return name_ == other.name_;
      }

      bool
      operator!= (Name const& other) const
      {
        return name_ == other.name_;
      }



    protected:
      std::string name_;

      friend std::ostream&
      operator << (std::ostream& o, Name const& name)
      {
        return o << name.name_;
      }

      friend class ScopedName;
    };


    //
    //
    //
    class SimpleName : public Name
    {
    public:
      explicit
      SimpleName (std::string const& name)
          : Name (name)
      {
        //@@ need some checking
      }

      bool
      operator< (SimpleName const& other) const
      {
        return name_ < other.name_;
      }

      bool
      operator== (SimpleName const& other) const
      {
        return name_ == other.name_;
      }

      bool
      operator!= (SimpleName const& other) const
      {
        return name_ != other.name_;
      }

    private:
      std::string
      str () const
      {
        return name_;
      }

      friend SimpleName
      operator+ (SimpleName const& s1, std::string const& s2)
      {
        return SimpleName (s1.str () + s2);
      }

      friend SimpleName
      operator+ (std::string const& s1, SimpleName const& s2)
      {
        return SimpleName (s1 + s2.str ());
      }
    };

    //
    //
    //
    class ScopedName : public Name
    {
    public:
      class InconsistentState {};
      class InvalidArgument {};

    public:
      explicit
      ScopedName (std::string const& name) throw (InvalidArgument)
          : Name (name)
      {
        try
        {
          check_state ();
        }
        catch (InconsistentState const&)
        {
          throw InvalidArgument ();
        }
      }

      explicit
      ScopedName (Name const& name) throw (InvalidArgument)
          : Name (name)
      {
        try
        {
          check_state ();
        }
        catch (InconsistentState const&)
        {
          throw InvalidArgument ();
        }
      }

      ScopedName (ScopedName const& scope, Name const& name)
        throw (InvalidArgument)
          : Name (scope.name_ + "::" + name.name_)
      {
        try
        {
          check_state ();
        }
        catch (InconsistentState const&)
        {
          throw InvalidArgument ();
        }
      }

      bool
      operator< (ScopedName const& other) const
      {
        return name_ < other.name_;
      }

      bool
      operator== (ScopedName const& other) const
      {
        return name_ == other.name_;
      }

      bool
      operator!= (ScopedName const& other) const
      {
        return name_ != other.name_;
      }

    public:
      SimpleName
      simple () const
      {
        std::string::size_type pos =  name_.rfind ("::");
        if (pos == std::string::npos)
        {
          if (name_.empty ()) return SimpleName(""); //file scope name
          else throw InconsistentState ();
        }

        return SimpleName(std::string (name_.begin () + pos + 2,
                                       name_.end ()));
      }

      class AtRoot {};

      ScopedName
      scope () const throw (AtRoot)
      {
        std::string::size_type pos =  name_.rfind ("::");
        if (pos == std::string::npos)
        {
          throw AtRoot ();
        }

        return ScopedName(std::string (name_.begin (),
                                       name_.begin () + pos));
      }

      Name
      in_file_scope () const
      {
        return Name(std::string (name_.begin () + 2, name_.end ()));
      }


    private:
      void
      check_state () throw (InconsistentState)
      {
        if (name_.empty ()) return;

        if (name_.length () < 2) throw InconsistentState ();

        if (name_[0] != ':' || name_[1] != ':') throw InconsistentState ();

        //scan the string
        for (std::string::const_iterator i = name_.begin ();
             i != name_.end ();
             i++)
        {
          if (*i == ':')
          {
            if (i + 1 == name_.end () || *(i + 1) != ':')
            {
              throw InconsistentState ();
            }
            else
            {
              i++; //skip "::"
            }
          }
        }
      }

    };

    typedef
    std::vector<ScopedName>
    ScopedNameList;

    typedef
    std::set<ScopedName>
    ScopedNameSet;

    //
    //
    //
    class Order
    {
    public:
      Order (Order const& parent, unsigned long index)
          : list_ (parent.list_)
      {
        list_.push_back (index);
      }

      explicit
      Order (unsigned long index)
          : list_ (1, index)
      {
      }

    public:
      bool
      operator == (Order const& other) const
      {
        return list_ == other.list_;
      }

      bool
      operator != (Order const& other) const
      {
        return list_ != other.list_;
      }

      bool
      operator < (Order const& other) const
      {
        Iterator i = list_.begin ();
        Iterator j = other.list_.begin ();
        for (; i != list_.end () && j != other.list_.end (); i++, j++)
        {
          if (*i > *j)
          {
            return false;
          }
          else if (*i < *j)
          {
            return true;
          }
        }

        if (j != other.list_.end ())
        {
          // we are equal so far but other is longer which means it is after
          return true;
        }
        else
        {
          return false;
        }
      }

      bool
      suborder (Order const& subj) const
      {
        if (list_.size () >= subj.list_.size ()) return false;

        for (Iterator i = list_.begin (), j = subj.list_.begin ();
             i != list_.end ();
             i++, j++)
        {
          if (*i != *j) return false;
        }

        return true;
      }


    private:
      typedef
      std::vector<unsigned long>
      OrderList_;

      typedef
      OrderList_::const_iterator
      Iterator;

      OrderList_ list_;

      friend std::ostream&
      operator << (std::ostream& o, Order const& order)
      {
        for (Order::OrderList_::const_iterator i = order.list_.begin ();
             i != order.list_.end ();
             i++)
        {
          if (i != order.list_.begin ()) o << ':';
          o << *i;
        }

        return o;
      }
    };


    //
    //
    //
    class Node;
    typedef
    StrictPtr<Node>
    NodePtr;

    class Node : public virtual ReferenceCounting::DefaultImpl <>
    {
    protected:
      virtual
      ~Node () throw () {}

      // Dynamic typing
    public:

      template <typename Type>
      bool
      is_a ()
      {
        NodePtr self (ReferenceCounting::add_ref (this));
        return strict_cast<Type>(self) != 0;
      }

      template <typename Type>
      StrictPtr<Type>
      dynamic_type ()
      {
        NodePtr self (ReferenceCounting::add_ref (this));
        return strict_cast<Type>(self);
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v) = 0;
    };

    //
    //
    //
    class Comma : public virtual Node
    {
    public:
      virtual
      ~Comma () throw () {}

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };
    typedef
    StrictPtr<Comma>
    CommaPtr;


    //
    // Contains scoped name & reference to DeclarationTable.
    // Purpose: provides controlled way of cyclic dependencies
    //
    // Issues:
    //
    //  @@ There is a need for 'uninitialized' state of refernce.
    //

    class Declaration;

    typedef
    StrictPtr<Declaration>
    DeclarationPtr;

    class DeclarationTable;

    //@@ Better name for DeclarationRef:
    //   - DeclarationLink
    //
    template <typename T, typename Ptr = StrictPtr<T> >
    class DeclarationRef
    {
    public:
      class NotInitialized {};
      class DeclarationNotFound {};
      class TypeMismatch {};
      class TableMismatch {};


    public:

      DeclarationRef (DeclarationPtr decl);
      DeclarationRef (DeclarationTable const& table, ScopedName const& name);
      DeclarationRef (DeclarationTable const& table,
                      ScopedName const& name,
                      Order const& order);

      //This c-tor construct an uninitialized reference.
      DeclarationRef (DeclarationTable const& table);

      // Note that you can only assign DeclarationRef's associated with
      // the same DeclarationTable.
      //
      DeclarationRef&
      operator= (DeclarationRef const& other) throw (TableMismatch)
      {
        if (&table_ != &(other.table_)) throw TableMismatch ();

        name_ = other.name_;
        order_ = other.order_;
        initialized_ = other.initialized_;
        with_order__ = other.with_order_;

        return *this;
      }

      operator bool () const
      {
        return initialized_;
      }

      // Returns first declaration that matches.
      Ptr
      resolve () const
        throw (NotInitialized, DeclarationNotFound, TypeMismatch);

      ScopedName
      name () const throw (NotInitialized)
      {
        if (!initialized_) throw NotInitialized ();
        return name_;
      }

    private:
      DeclarationTable const& table_;
      ScopedName name_;
      Order order_;
      bool initialized_;
      bool with_order_;
    };


    //
    // Declaration represents any IDL construct that has a scoped name
    //

    class Scope;

    typedef
    StrictPtr<Scope>
    ScopePtr;

    class Declaration : public virtual Node
    {
    protected:
      //@@ pure virtual d-tor with body doesn't work?
      virtual
      ~Declaration () throw () {}

      Declaration (SimpleName const& name, ScopePtr const& scope);

      // This c-tor is here for Declarations that are not in scope
      // e.g. FileScope
      Declaration (ScopedName const& name,
                   Order const& order,
                   DeclarationTable const& table);

    public:

      ScopedName
      name () const;

      Order
      order () const;

      class NotInScope {}; // don't cross the edge! ;-)

      virtual ScopePtr
      scope () const throw (NotInScope);

      // Runtime declaration type information. I could have used RTTI but
      // I will also need a human-readable representation (e.g.
      // "local interface" instead of "IDL2::SyntaxTree::LocalInterface")
      // for diagnistic.
    public:
      virtual std::string
      declaration_type ()
      {
        return "declaration";
      }

    private:
      Order order_;
      ScopedName name_;
      DeclarationRef<Scope> scope_;
    };

    typedef
    StrictPtr<Declaration>
    DeclarationPtr;


    //
    //
    //
    template <typename T = DeclarationPtr>
    struct DeclarationOrderComparator
    {
      bool operator () (T const& x, T const& y) const
      {
        return x->order () < y->order ();
      }
    };

    template <typename T = DeclarationPtr>
    struct DeclarationNameComparator
    {
      bool operator () (T const& x, T const& y) const
      {
        return x->name () < y->name ();
      }
    };


    //
    //
    //
    template <typename T = DeclarationRef<Declaration> >
    struct DeclarationRefOrderComparator
    {
      bool operator () (T const& x, T const& y) const
      {
        return x.resolve ()->order () < y.resolve ()->order ();
      }
    };

    template <typename T = DeclarationRef<Declaration> >
    struct DeclarationRefNameComparator
    {
      bool operator () (T const& x, T const& y) const
      {
        return x.resolve ()->name () < y.resolve ()->name ();
      }
    };


    //
    // Mapping of fq-name -> sequence<Declaration> and some other
    // useful stuff.
    //
    class DeclarationTable
    {
    private:
      typedef
      std::set<DeclarationPtr, DeclarationOrderComparator<> >
      DeclarationSet;

      typedef
      std::map<ScopedName, DeclarationSet>
      NameDeclarationMap;

    public:

      typedef
      DeclarationSet::const_iterator
      Iterator;

      typedef
      std::pair<Iterator, Iterator>
      IteratorPair;

      class InvalidArgument {};
      class AlreadyExist {};

      class TypeMismatch {};

    public:
      void
      insert (DeclarationPtr d) throw (InvalidArgument,
                                       AlreadyExist,
                                       TypeMismatch);
      Iterator
      begin () const;

      Iterator
      end () const;

      // Returns iterator pair. first points to the first element,
      // second points to the element after last.
      IteratorPair
      lookup (ScopedName const& n) const;

      bool
      exist (ScopedName const& n) const
      {
        IteratorPair pair = lookup (n);
        return pair.first != pair.second;
      }


      class DeclarationNotFound {};

      template <typename T>
      StrictPtr<T>
      lookup (ScopedName const& n) const
        throw (DeclarationNotFound, TypeMismatch);

      template <typename T>
      StrictPtr<T>
      lookup (ScopedName const& n, Order const& o) const
        throw (DeclarationNotFound, TypeMismatch);

      class ResolutionFailure {};
      class NameNotFound  : public ResolutionFailure {};
      class PredicateNotMet : public ResolutionFailure {};

      struct ResolvePredicate
      {
        virtual bool
        test (DeclarationPtr const& d) throw (ResolutionFailure) = 0;
      };

      ScopedName
      resolve (Name const& name,
               ScopedName const& from,
               Order const& before,
               ResolvePredicate& p) const throw (ResolutionFailure);

    private:
      DeclarationSet decl_set_;
      NameDeclarationMap name_decl_map_;
    };


    // @@ scope is not a declaration?
    // Scope
    //
    class Scope : public virtual Declaration
    {
    private:
      //@@ same type as in DeclarationTable
      typedef
      std::set<DeclarationPtr, DeclarationOrderComparator<> >
      DeclarationSet;

    public:
      typedef
      DeclarationSet::const_iterator
      Iterator;

      class InvalidArgument {};
      class AlreadyExist {};


    protected:
      virtual
      ~Scope () throw () {}

      // This c-tor is here for Declarations that are not in scope
      // e.g. FileScope.
      Scope (DeclarationTable& table,
             ScopedName const& name,
             Order const& order)
          : Declaration (name, order, table),
            table_ (table),
            next_order_ (0) {}

      Scope (SimpleName const& name, ScopePtr const& scope)
          : Declaration (name, scope),
            table_ (scope->table ()),
            next_order_ (0)
      {
      }

    public:

      // Inject new Declaration into a scope. Relative position of a new
      // entity will be determined based on order (order..! order..!).
      void
      insert (DeclarationPtr d) throw (InvalidArgument, AlreadyExist);

      Iterator
      begin () const;

      Iterator
      end () const;

      Order
      create_order ();

      // Returns order that would be returned by next call to
      // create_order ().
      Order
      peek_order ();


    public:

      DeclarationTable const& table () const
      {
        return table_;
      }

      DeclarationTable& table ()
      {
        return table_;
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "scope";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      DeclarationSet content_;
      DeclarationTable& table_;
      unsigned long next_order_;
    };


    //
    //
    //
    class TypeDecl : public virtual Declaration
    {
    protected:
      virtual
      ~TypeDecl () throw () {}

      TypeDecl (SimpleName const& name,
                ScopePtr const& scope)
          : Declaration (name, scope)
      {
      }

      // Type completeness
    public:
      virtual bool
      defined () const = 0;

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

    typedef
    StrictPtr<TypeDecl>
    TypeDeclPtr;

    typedef
    DeclarationRef<TypeDecl>
    TypeDeclRef;

    //
    //
    //
    class TypeForwardDecl : public virtual TypeDecl
    {
    protected:
      virtual
      ~TypeForwardDecl () throw () {}

      TypeForwardDecl (SimpleName const& name,
                       ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope)
      {
      }

      // Type completeness
    public:
      virtual bool
      defined () const
      {
        return false;
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "type";
      }
    };


    //
    //
    //
    class TypeDef : public virtual TypeDecl
    {
    protected:
      virtual
      ~TypeDef () throw () {}

      TypeDef (SimpleName const& name,
               ScopePtr const& scope)
          : Declaration (name, scope),
            TypeDecl (name, scope)
      {
      }

      // Type completeness
    public:
      virtual bool
      defined () const
      {
        return true;
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "type";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

    typedef
    StrictPtr<TypeDef>
    TypeDefPtr;

    typedef
    DeclarationRef<TypeDef>
    TypeDefRef;


    /*
    // Anonymous type
    class Anonymous : public virtual Type
    {
    };

    */


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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "interface";
      }


      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "interface";
      }
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

      class DuplicateName {};

      InterfaceDef (SimpleName const& name,
                    ScopePtr const& scope,
                    ScopedNameSet const& inherits) throw (DuplicateName)
          : Declaration (name, scope),
            TypeDecl (name, scope),
            InterfaceDecl (name, scope),
            TypeDef (name, scope),
            Scope (name, scope)
      {
        for (ScopedNameSet::const_iterator i = inherits.begin ();
             i != inherits.end ();
             i++)
        {
          if (!inherits_.insert (
                InterfaceDefRef (scope->table (), *i)).second)
          {
            //@@ this never happens
            throw DuplicateName ();
          }
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
      declaration_type ()
      {
        return "interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "abstract interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };


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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "abstract interface";
      }
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "abstract interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "local interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };


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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "local interface";
      }
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "local interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "unconstrained interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "unconstrained interface";
      }
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
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "unconstrained interface";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
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


    //
    //
    //
    class AttributeDecl : public virtual Declaration
    {
    public:
      virtual
      ~AttributeDecl () throw () {}

      AttributeDecl (SimpleName const& name,
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
        return "attribute";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      TypeDeclRef type_;
    };

    typedef
    StrictPtr<AttributeDecl>
    AttributeDeclPtr;

    //
    //
    //
    class OperationParameter : public virtual Node
    {
    public:
      virtual
      ~OperationParameter () throw () {}

      struct Direction
      {
        enum Value
        {
          IN,
          OUT,
          INOUT
        };

        friend std::ostream&
        operator<< (std::ostream& o, Value d)
        {
          if (d == IN) o << "in";
          else if (d == OUT) o << "out";
          else o << "inout";
          return o;
        }
      };

      OperationParameter (Direction::Value direction,
                 ScopedName type,
                 SimpleName name,
                 DeclarationTable const& table)
          : direction_ (direction),
            type_ (table, type),
            name_ (name)
      {
      }

    public:
      Direction::Value
      direction ()
      {
        return direction_;
      }

      TypeDeclPtr
      type ()
      {
        return type_.resolve ();
      }

      SimpleName
      name ()
      {
        return name_;
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      Direction::Value direction_;
      TypeDeclRef type_;
      SimpleName name_;
    };

    typedef
    StrictPtr<OperationParameter>
    OperationParameterPtr;

    //@@ Strip Ptr from *List, *Set, etc?
    typedef
    std::vector<OperationParameterPtr>
    OperationParameterList;


    //
    //
    //
    class OperationDecl : public virtual Declaration
    {
    public:
      virtual
      ~OperationDecl () throw () {}

      OperationDecl (SimpleName const& name,
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

    public:
      typedef
      OperationParameterList::const_iterator
      Iterator;

      void
      insert (OperationParameterPtr op)
      {
        //@@ would be nice to have some checking here
        //   list should probably be a set(by name)
        parameter_list_.push_back (op);
      }

      //@@ It's probably a good idea to stript prefixes from
      //   functions like inherits_begin, etc where there is
      //   no ambiguity. Ambiguity can come from Scope member function.
      Iterator
      begin ()
      {
        return parameter_list_.begin ();
      }

      Iterator
      end ()
      {
        return parameter_list_.end ();
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "operation";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      TypeDeclRef type_;
      OperationParameterList parameter_list_;
    };

    typedef
    StrictPtr<OperationDecl>
    OperationDeclPtr;


    //
    // Module
    //
    class Module : public virtual Scope
    {
    public:
      virtual
      ~Module () throw () {}

      Module (SimpleName const& name, ScopePtr const& scope)
          : Declaration (name, scope),
            Scope (name, scope)
      {
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "module";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };
    typedef
    StrictPtr<Module>
    ModulePtr;

    //
    //
    //
    class FileScope : public virtual Scope
    {
    public:
      virtual
      ~FileScope () throw () {}

      FileScope (DeclarationTable& table, Order const& order);

      virtual ScopePtr
      scope () const throw (NotInScope)
      {
        throw NotInScope ();
      }

      // Runtime declaration type information
    public:
      virtual std::string
      declaration_type ()
      {
        return "file scope";
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

    typedef
    StrictPtr<FileScope>
    FileScopePtr;

    typedef
    DeclarationOrderComparator<FileScopePtr>
    FileScopeOrderComparator;

    typedef
    std::set<FileScopePtr, FileScopeOrderComparator>
    FileScopeSet;


    //
    //
    //
    class TranslationRegion;

    typedef
    StrictPtr<TranslationRegion>
    TranslationRegionPtr;

    typedef
    std::vector<TranslationRegionPtr>
    TranslationRegionList;

    class TranslationRegion : public virtual Node
    {
    public:
      virtual
      ~TranslationRegion () throw () {}

       TranslationRegion (DeclarationTable& table,
                          Order const& order)
          : order_ (order),
            table_ (table),
            next_order_ (0)
      {
        scope_ = FileScopePtr (new FileScope (table_, Order (order_, 1)));

        //@@ eh!
        table.insert (scope_);
      }

    public:
      DeclarationTable const&
      table () const
      {
        return table_;
      }

      DeclarationTable&
      table ()
      {
        return table_;
      }

    public:
      Order
      create_order ()
      {
        return Order (Order (order_, 0), next_order_++);
      }

    public:
      FileScopePtr
      scope ()
      {
        return scope_;
      }

    public:
      class InvalidArgument {};

      void
      insert (TranslationRegionPtr const& tr) throw (InvalidArgument);

      typedef
      TranslationRegionList::const_iterator
      Iterator;

      Iterator
      begin () const;

      Iterator
      end () const;

    private:
      Order order_;
      FileScopePtr scope_;
      DeclarationTable& table_;
      unsigned long next_order_;
      TranslationRegionList region_list_;

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };


    //
    //
    //
    class IncludeTranslationRegion : public virtual TranslationRegion
    {
    public:
      virtual
      ~IncludeTranslationRegion () throw () {}

      IncludeTranslationRegion (fs::path const& file_path,
                                DeclarationTable& table,
                                Order const& order)
          : TranslationRegion (table, order),
            file_path_ (file_path)
      {
      }

    public:
      fs::path file_path () const
      {
        return file_path_;
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      fs::path file_path_;
    };
    typedef
    StrictPtr<IncludeTranslationRegion>
    IncludeTranslationRegionPtr;

    //
    //
    //
    class SysIncludeTranslationRegion : public virtual TranslationRegion
    {
    public:
      virtual
      ~SysIncludeTranslationRegion () throw () {}

       SysIncludeTranslationRegion (fs::path const& file_path,
                                    DeclarationTable& table,
                                    Order const& order)
           : TranslationRegion (table, order),
             file_path_ (file_path)
      {
      }

    public:
      fs::path file_path () const
      {
        return file_path_;
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      fs::path file_path_;
    };
    typedef
    StrictPtr<SysIncludeTranslationRegion>
    SysIncludeTranslationRegionPtr;

    //
    //
    //
    class ImpliedIncludeTranslationRegion : public virtual TranslationRegion
    {
    public:
      virtual
      ~ImpliedIncludeTranslationRegion () throw () {}

       ImpliedIncludeTranslationRegion (fs::path const& file_path,
                                        DeclarationTable& table,
                                        Order const& order)
           : TranslationRegion (table, order),
             file_path_ (file_path)
      {
      }

    public:
      fs::path file_path () const
      {
        return file_path_;
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      fs::path file_path_;
    };
    typedef
    StrictPtr<ImpliedIncludeTranslationRegion>
    ImpliedIncludeTranslationRegionPtr;


    //
    //
    //
    class PrincipalTranslationRegion : public virtual TranslationRegion
    {
    public:
      virtual
      ~PrincipalTranslationRegion () throw () {}

       PrincipalTranslationRegion (fs::path const& file_path,
                                   DeclarationTable& table,
                                   Order const& order)
           : TranslationRegion (table, order),
             file_path_ (file_path)
      {
      }

    public:
      fs::path file_path () const
      {
        return file_path_;
      }

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);

    private:
      fs::path file_path_;
    };
    typedef
    StrictPtr<PrincipalTranslationRegion>
    PrincipalTranslationRegionPtr;


    //@@ TranslationUnit should not contain FileScope or should it?
    //@@ Seems not
    //
    //
    class TranslationUnit : public virtual Node
    {
    public:
      virtual
      ~TranslationUnit () throw () {}

      TranslationUnit ()
          : order_ (0),
            next_order_ (0)
      {
      }

    public:

      DeclarationTable const&
      table () const
      {
        return table_;
      }

      DeclarationTable&
      table ()
      {
        return table_;
      }

    public:
      Order
      create_order ()
      {
        return Order (order_, next_order_++);
      }

    public:
      class InvalidArgument {};

      void
      insert (TranslationRegionPtr const& tr) throw (InvalidArgument);

      typedef
      TranslationRegionList::const_iterator
      Iterator;

      Iterator
      begin () const;

      Iterator
      end () const;


      class NotFound {};

      PrincipalTranslationRegionPtr
      principal_translation_region () const throw (NotFound)
      {
        for (Iterator i = begin (); i != end (); i++)
        {
          if ((*i)->is_a<PrincipalTranslationRegion> ())
          {
            return (*i)->dynamic_type<PrincipalTranslationRegion> ();
          }
        }

        throw NotFound ();
      }

    private:
      Order order_;
      DeclarationTable table_;
      unsigned long next_order_;
      TranslationRegionList region_list_;

      // Traversal
    public:
      virtual void
      accept (CCF::Traversal::Visitor* v);
    };

    typedef
    StrictPtr<TranslationUnit>
    TranslationUnitPtr;
  }
}

//@@ tmp
#include "CCF/IDL2/IDL2_SyntaxTree_BuiltIn.hpp"
#include "CCF/IDL2/IDL2_SyntaxTree_Value.hpp"

#include "CCF/IDL2/IDL2_SyntaxTree.tpp"

#endif // CCF_IDL2_SYNTAX_TREE_HPP
