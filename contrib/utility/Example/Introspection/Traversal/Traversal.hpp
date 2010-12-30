// file      : Example/Introspection/Traversal/Traversal.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef TRAVERSAL_HPP
#define TRAVERSAL_HPP

#include <map>
#include <iostream>

#include "Utility/Introspection/Introspection.hpp"

#include "SyntaxTree.hpp"

namespace Traversal
{
  class Traverser;

  //
  //
  //
  class Dispatcher
  {
  public:
    virtual
    ~Dispatcher ()
    {
    }

    virtual void
    dispatch (SyntaxTree::Node* n);

  protected:
    void
    map (Utility::Introspection::TypeId id, Traverser* t)
    {
      traversal_map_[id] = t;
    }

  private:
    typedef
    std::map<Utility::Introspection::TypeId, Traverser*>
    TraversalMap;

    TraversalMap traversal_map_;
  };


  //
  //
  //
  class Traverser : public virtual Dispatcher
  {
  public:
    virtual void
    traverse (SyntaxTree::Node* n) = 0;
  };

  //
  //
  //
  struct Node : Traverser
  {
    Node ()
    {
      map (typeid (SyntaxTree::Node), this);
    }

    virtual void
    traverse (SyntaxTree::Node*)
    {
      std::cerr << "node" << std::endl;
    }
  };


  //
  //
  //
  struct Declaration : Traverser
  {
    Declaration ()
    {
      map (typeid (SyntaxTree::Declaration), this);
    }

    virtual void
    traverse (SyntaxTree::Node*)
    {
      std::cerr << "declaration" << std::endl;
    }
  };

  //
  //
  //
  struct Scope : Traverser
  {
    Scope ()
    {
      map (typeid (SyntaxTree::Scope), this);
    }

    virtual void
    traverse (SyntaxTree::Node* n)
    {
      std::cerr << "scope" << std::endl;

      SyntaxTree::Scope* s = dynamic_cast<SyntaxTree::Scope*> (n);

      for (SyntaxTree::DeclarationList::iterator i = s->content_.begin ();
           i != s->content_.end ();
           i++)
      {
        dispatch (*i);
      }
    }
  };

  //
  //
  //
  struct InterfaceDecl : Traverser
  {
    InterfaceDecl ()
    {
      map (typeid (SyntaxTree::InterfaceDecl), this);
    }

    virtual void
    traverse (SyntaxTree::Node*)
    {
      std::cerr << "interface declaration" << std::endl;
    }
  };

  //
  //
  //
  struct InterfaceDef : Traverser
  {
    InterfaceDef ()
    {
      map (typeid (SyntaxTree::InterfaceDef), this);
    }

    virtual void
    traverse (SyntaxTree::Node*)
    {
      std::cerr << "interface definition" << std::endl;
    }
  };
}

#endif  // TRAVERSAL_HPP
//$Id$
