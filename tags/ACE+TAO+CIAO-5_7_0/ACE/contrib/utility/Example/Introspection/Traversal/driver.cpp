// file      : Example/Introspection/Traversal/driver.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

/* FUZZ: disable check_for_improper_main_declaration */

#include <iostream>

#include "SyntaxTree.hpp"
#include "Traversal.hpp"

int
main ()
{
  using namespace SyntaxTree;

  /*
    Create a syntax tree that looks something like this:

    scope
    {
      interface declaration;

      scope
      {
        interface definition
        {
          decalartion;
        };
      };
    };

  */

  Scope s1;

  InterfaceDecl i1;
  s1.content_.push_back (&i1);

  Scope s2;
  s1.content_.push_back (&s2);

  InterfaceDef i2;
  s2.content_.push_back (&i2);

  Declaration d1;
  i2.content_.push_back (&d1);

  SyntaxTree::Node* root = &s1;

  // Now different ways of traversing this tree:

  {
    std::cout << "test #1" << std::endl;

    struct Generator : Traversal::Declaration, Traversal::Scope
    {
    };

    Generator g;
    g.dispatch (root);

    std::cout << std::endl;
  }

  {
    std::cout << "test #2" << std::endl;

    struct Generator : Traversal::Scope, Traversal::InterfaceDecl
    {
    };

    Generator g;
    g.dispatch (root);

    std::cout << std::endl;
  }

  {
    std::cout << "test #3" << std::endl;

    struct Generator : Traversal::Scope, Traversal::InterfaceDef
    {
    };

    Generator g;
    g.dispatch (root);

    std::cout << std::endl;
  }
}
//$Id$
