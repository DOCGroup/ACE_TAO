// file      : Example/Introspection/InheritanceTree/inheritance_tree.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

// Note: This example is by no means complete. In fact properly printing
// arbitrary C++ inheritance tree is a non-trivial task. If you would like
// to improve this example please feel free to send your results back ;-).

/* FUZZ: disable check_for_improper_main_declaration */

#include "Hierarchy.hpp"

#include <set>
#include <iostream>

using std::endl;

typedef
std::set<TypeId>
TypeIdSet;

void
print_inheritance_tree_core (std::ostream& os,
                             TypeInfo const& ti,
                             TypeIdSet& set)
{
  bool nl = false;

  for (TypeInfo::BaseIterator i = ti.begin_base ();
       i != ti.end_base ();
       i++)
  {
    TypeId tid (i->type_info ().type_id ());

    if (set.find (tid) != set.end ()) continue;

    nl = true;
    set.insert (tid);
    print_inheritance_tree_core (os, i->type_info (), set);
  }

  if (nl) os << endl;

  os << ti.type_id () << "  ";
}

void
print_inheritance_tree (std::ostream& os, TypeInfo const& ti)
{
  TypeIdSet set;
  print_inheritance_tree_core (os, ti, set);
  os << endl;
}

int
main ()
{
  B* b  = new D;

  print_inheritance_tree (std::cout, b->type_info ());

  delete b;
}
//$Id$
