// file      : CCF/IDL2/SyntaxTree/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_ELEMENTS_HPP
#define CCF_IDL2_SYNTAX_TREE_ELEMENTS_HPP

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>

//@@ this should go to .cpp
#include <iostream>

#include "CCF/CompilerElements/FileSystem.hpp"
#include "CCF/CompilerElements/ReferenceCounting.hpp"
#include "CCF/CompilerElements/Introspection.hpp"
#include "CCF/CompilerElements/Context.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {
      using ReferenceCounting::StrictPtr;


      // Can be scoped or simple. "::" refers to file-scope.
      //
      //
      class Name
      {
      public:
        class InvalidArgument {};

      public:
        virtual
        ~Name ()
        {
        }

        explicit
        Name (std::string const& name) throw (InvalidArgument)
            : name_ (name)
        {
          if (name.empty ())
          {
            throw InvalidArgument ();
          }
          else if (name == "::")
          {
            name_ = "";
          }
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
          return name_ != other.name_;
        }

      public:
        virtual std::ostream&
        print (std::ostream& o) const
        {
          return o << name_;
        }

      protected:
        std::string name_;

        friend std::ostream&
        operator << (std::ostream& o, Name const& name)
        {
          return name.print (o);
        }

        friend class ScopedName;
      };


      // Simple name (i.e. without "::") or just "::"
      // (which denotes a file-scope).
      //
      //
      class SimpleName : public Name
      {
      public:
        explicit
        SimpleName (std::string const& name) throw (InvalidArgument)
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

      protected:

        using Name::name_;

        friend SimpleName
        operator+ (SimpleName const& s1, std::string const& s2)
        {
          return SimpleName (s1.name_ + s2);
        }

        friend SimpleName
        operator+ (std::string const& s1, SimpleName const& s2)
        {

          return SimpleName (s1 + s2.name_);
        }
      };


      // Should always start with "::". Can be just "::" which
      // means it's a file-scope.
      //
      //
      class ScopedName : public Name
      {
      private:
        class InconsistentState {};

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
            if (name_.empty ()) return SimpleName("::"); //file scope name
            else throw InconsistentState (); //@@ this should never happen
          }

          return SimpleName(std::string (name_.begin () + pos + 2,
                                         name_.end ()));
        }

        class FileScope {};

        ScopedName
        scope () const throw (FileScope)
        {
          std::string::size_type pos =  name_.rfind ("::");
          if (pos == std::string::npos)
          {
            throw FileScope ();
          }

          std::string tmp (name_.begin (), name_.begin () + pos);

          return ScopedName(tmp.empty () ? std::string ("::") : tmp);
        }

        Name
        in_file_scope () const throw (FileScope)
        {
          if (name_.empty ()) throw FileScope ();

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
      class StringLiteral
      {
      public:
        StringLiteral (std::string literal)
            : literal_ (literal)
        {
        }

        std::string
        str () const
        {
          return literal_;
        }

        friend std::ostream&
        operator << (std::ostream& o, StringLiteral const& lit)
        {
          return o << '\"' << lit.str () << '\"';
        }

      private:
        std::string literal_;
      };

      inline bool
      operator== (StringLiteral const& a, StringLiteral const& b)
      {
        return a.str () == b.str ();
      }

      inline bool
      operator!= (StringLiteral const& a, StringLiteral const& b)
      {
        return a.str () != b.str ();
      }


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
            if (i != order.list_.begin ()) o << '.';
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

      class Node : public virtual Introspection::Object,
                   public virtual ReferenceCounting::DefaultImpl <>
      {
      protected:
        struct ContextHolder : virtual ReferenceCounting::DefaultImpl <>
        {
          virtual ~ContextHolder () throw () {}

          CompilerElements::Context context;
        };

        typedef StrictPtr<ContextHolder> ContextHolderPtr;

      protected:
        virtual
        ~Node () throw () {}

        Node ()
        {
          type_info (static_type_info ());
        }

        Node (ContextHolderPtr const& ch)
            : context_holder_ (ch)
        {
          type_info (static_type_info ());
        }

        // Virtual typing.
        //
      public:

        //@@ should virtual_type be constant (since it is temporary)?
        //
        virtual NodePtr
        virtual_type ()
        {
          return NodePtr ();
        }

        // Dynamic typing.
        //
      public:

        virtual bool
        is_a (Introspection::TypeInfo const& ti) const;

        template <typename Type>
        bool
        is_a () const
        {
          return is_a (Type::static_type_info ());
        }

        virtual NodePtr
        dynamic_type (Introspection::TypeInfo const& ti);

        template <typename Type>
        StrictPtr<Type>
        dynamic_type ()
        {
          return ReferenceCounting::strict_cast<Type>(
            dynamic_type (Type::static_type_info ()));
        }

        // Context
      public:

        CompilerElements::Context&
        context ()
        {
          return context_holder ()->context;
        }

        CompilerElements::Context const&
        context () const
        {
          return context_holder ()->context;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();


      protected:
        ContextHolderPtr
        context_holder () const
        {
          if (context_holder_.in () == 0)
          {
            context_holder_ = ContextHolderPtr (new ContextHolder);
          }

          return context_holder_;
        }

      private:
        mutable ContextHolderPtr context_holder_;
      };


      //
      //
      //
      class Comma : public virtual Node
      {
      public:
        virtual
        ~Comma () throw () {}

        Comma ()
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
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
      //   @@ Better name for DeclarationRef:
      //      - DeclarationLink
      //

      class Declaration;

      typedef
      StrictPtr<Declaration>
      DeclarationPtr;

      class DeclarationTable;

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

        //This c-tor constructs an uninitialized reference.
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

      class DeclarationTable;

      class Declaration : public virtual Node
      {
      protected:
        virtual
        ~Declaration () throw () {}

        Declaration (SimpleName const& name, ScopePtr const& scope);

        Declaration (SimpleName const& name,
                     Order const& order,
                     ScopePtr const& scope);

        // This c-tor is here for Declarations that are not in scope
        // e.g. FileScope
        Declaration (ScopedName const& name,
                     Order const& order,
                     DeclarationTable& table);


        // This c-tor is declared but never defined. The trick is that
        // it should never be called since all inheritance in SyntaxTree
        // is virtual.
        //
        Declaration ();

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

        ScopedName
        name () const;

        Order
        order () const;

        class NotInScope {}; // don't cross the edge! ;-)

        virtual ScopePtr
        scope () const throw (NotInScope);

        // @@Runtime declaration type information. I could have used RTTI but
        // I will also need a human-readable representation (e.g.
        // "local interface" instead of "IDL2::SyntaxTree::LocalInterface")
        // for diagnistic.
      public:
        virtual std::string
        declaration_class () = 0;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        Order order_;
        ScopedName name_;
        DeclarationTable& table_;
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

      typedef
      std::set<DeclarationPtr, DeclarationOrderComparator<> >
      DeclarationSet;


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

        class DeclarationNotFound {};

        DeclarationPtr
        lookup (ScopedName const& n, Introspection::TypeInfo const& ti) const
          throw (DeclarationNotFound, TypeMismatch);

        template <typename T>
        StrictPtr<T>
        lookup (ScopedName const& n) const
          throw (DeclarationNotFound, TypeMismatch);

        template <typename T>
        StrictPtr<T>
        lookup (ScopedName const& n, Order const& o) const
          throw (DeclarationNotFound, TypeMismatch);

        bool
        exist (ScopedName const& n) const
        {
          IteratorPair pair = lookup (n);
          return pair.first != pair.second;
        }

        class ResolutionFailure {};
        class NameNotFound  : public ResolutionFailure {};
        class PredicateNotMet : public ResolutionFailure {};

        struct ResolvePredicate
        {
          virtual bool
          test (DeclarationPtr const&) const throw (ResolutionFailure)
          {
            return true;
          }
        };

        ScopedName
        resolve (Name const& name,
                 ScopedName const& from,
                 Order const& before,
                 ResolvePredicate const& p = ResolvePredicate ())
          const throw (ResolutionFailure);

      private:
        DeclarationSet decl_set_;
        NameDeclarationMap name_decl_map_;
      };


      //
      // Scope
      //
      class Scope : public virtual Declaration
      {
      public:
        typedef
        DeclarationSet::const_iterator
        Iterator;

        class InvalidArgument {};
        class AlreadyExist {};

      protected:
        virtual
        ~Scope () throw () {}

        Scope ()
            : next_order_ (0)
        {
          type_info (static_type_info ());
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
        static Introspection::TypeInfo const&
        static_type_info ();

      private:
        DeclarationSet content_;
        unsigned long next_order_;
      };


      //
      //
      //

      class TypeDecl;

      typedef
      StrictPtr<TypeDecl>
      TypeDeclPtr;

      class TypeDecl : public virtual Declaration
      {
      protected:
        virtual
        ~TypeDecl () throw () {}

        TypeDecl ()
        {
          type_info (static_type_info ());
        }

        // Typedef support.
        //
      public:
        //@@ returned object should be constant
        //
        //@@ it should probably take ScopedName
        //
        //@@ should it be constant?
        //
        virtual TypeDeclPtr
        clone_typedef_temporary (SimpleName const& name,
                                 Order const& order,
                                 ScopePtr const& scope,
                                 ContextHolderPtr const& ch) = 0;

        // Type completeness.
        //
      public:
        virtual bool
        complete () const = 0;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      DeclarationRef<TypeDecl>
      TypeDeclRef;



      //
      //
      //
      class ForwardDeclarableTypeDecl : public virtual TypeDecl
      {
      protected:
        virtual
        ~ForwardDeclarableTypeDecl () throw () {}

        ForwardDeclarableTypeDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        virtual bool
        defined () const = 0;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<ForwardDeclarableTypeDecl>
      ForwardDeclarableTypeDeclPtr;


      //
      //
      //
      class TypeForwardDecl : public virtual ForwardDeclarableTypeDecl
      {
      protected:
        virtual
        ~TypeForwardDecl () throw () {}

        TypeForwardDecl ()
        {
          type_info (static_type_info ());
        }

      public:
        virtual bool
        defined () const
        {
          return false;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<TypeForwardDecl>
      TypeForwardDeclPtr;


      //
      //
      //
      class TypeDef : public virtual ForwardDeclarableTypeDecl
      {
      protected:
        virtual
        ~TypeDef () throw () {}

        TypeDef ()
        {
          type_info (static_type_info ());
        }

      public:
        virtual bool
        defined () const
        {
          return true;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<TypeDef>
      TypeDefPtr;

      typedef
      DeclarationRef<TypeDef>
      TypeDefRef;
    }
  }
}

#include "CCF/IDL2/SyntaxTree/Elements.tpp"

#endif  // CCF_IDL2_SYNTAX_TREE_ELEMENTS_HPP
