// file      : CCF/Example/ComponentDeploymentDescriptor/SyntaxTree.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CDD_SYNTAX_TREE_HPP
#define CDD_SYNTAX_TREE_HPP

#include <set>
#include <vector>
#include <string>


#include "CCF/CompilerElements/ReferenceCounting.hpp"
#include "CCF/CompilerElements/Introspection.hpp"

namespace CDD
{
  namespace SyntaxTree
  {
    using ReferenceCounting::StrictPtr;

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
      virtual
      ~Node () throw () {}

      Node ()
      {
        type_info (static_type_info ());
      }

    public:
      template <typename Type>
      StrictPtr<Type>
      dynamic_type ()
      {
        NodePtr self (ReferenceCounting::add_ref (this));
        return ReferenceCounting::strict_cast<Type>(self);
      }

    public:
      static Introspection::TypeInfo const&
      static_type_info ();
    };

    typedef
    std::vector<NodePtr>
    NodeList;


    //
    //
    //
    class PortInstance : public virtual Node
    {
    public:

      virtual
      ~PortInstance () throw () {}

      PortInstance (std::string name)
          : name_ (name)
      {
        type_info (static_type_info ());
      }

    public:
      std::string
      name () const
      {
        return name_;
      }

    public:
      static Introspection::TypeInfo const&
      static_type_info ();

    private:
      std::string name_;
    };

    typedef
    StrictPtr<PortInstance>
    PortInstancePtr;

    template <typename T>
    struct NameComparator
    {
      bool
      operator () (T const& x, T const& y) const
      {
        return x->name () < y->name ();
      }
    };

    typedef
    std::set<PortInstancePtr, NameComparator<PortInstancePtr> >
    PortInstanceSet;


    //
    //
    //
    class ComponentInstance : public virtual Node
    {
    public:
      typedef
      PortInstanceSet::const_iterator
      Iterator;

    public:

      virtual
      ~ComponentInstance () throw () {}

      ComponentInstance (std::string name,
                         std::string type)
          : name_ (name),
            type_ (type)
      {
        type_info (static_type_info ());
      }

    public:

      std::string
      name () const
      {
        return name_;
      }

      std::string
      type () const
      {
        return type_;
      }

    public:
      class AlreadyExist {};

      void
      insert (PortInstancePtr const& d)
      {
        if (!ports_.insert (d).second) throw AlreadyExist ();
      }

      Iterator
      begin () const
      {
        return ports_.begin ();
      }


      Iterator
      end () const
      {
        return ports_.end ();
      }

    public:
      static Introspection::TypeInfo const&
      static_type_info ();

    private:
      std::string name_;
      std::string type_;

      PortInstanceSet ports_;
    };

    typedef
    StrictPtr<ComponentInstance>
    ComponentInstancePtr;


    //
    //
    //
    class Connection : public virtual Node
    {
    public:
      virtual
      ~Connection () throw () {}

      Connection (PortInstancePtr const& left,
                  PortInstancePtr const& right)
          : left_ (left),
            right_ (right)
      {
        type_info (static_type_info ());
      }

    public:

      PortInstancePtr
      left () const
      {
        return left_;
      }

      PortInstancePtr
      right () const
      {
        return right_;
      }

    public:
      static Introspection::TypeInfo const&
      static_type_info ();

    private:
      PortInstancePtr left_;
      PortInstancePtr right_;
    };

    typedef
    StrictPtr<Connection>
    ConnectionPtr;


    //
    //
    //
    class Descriptor : public virtual Node
    {
    public:
      typedef
      NodeList::const_iterator
      Iterator;

    public:
      virtual
      ~Descriptor () throw () {}

      Descriptor ()
      {
        type_info (static_type_info ());
      }

    public:
      void
      insert (NodePtr const& n)
      {
        nodes_.push_back (n);
      }

      Iterator
      begin () const
      {
        return nodes_.begin ();
      }


      Iterator
      end () const
      {
        return nodes_.end ();
      }

    public:
      static Introspection::TypeInfo const&
      static_type_info ();

    private:
      NodeList nodes_;
    };

    typedef
    StrictPtr<Descriptor>
    DescriptorPtr;
  }
}


#endif  // CDD_SYNTAX_TREE_HPP
