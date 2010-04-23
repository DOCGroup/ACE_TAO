// file      : Example/Hetero/Container/container.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

/* FUZZ: disable check_for_improper_main_declaration */

#include "Utility/Hetero/Container.hpp"
#include "Utility/Hetero/TypedContainer.hpp"
#include "Utility/Hetero/Vector.hpp"
#include "Utility/Hetero/Shell.hpp"

#include <string>
#include <iostream>
#include <algorithm>

using std::string;

using std::cout;
using std::cerr;
using std::endl;

namespace Hetero = Utility::Hetero;

using Hetero::Container;
using Hetero::TypedContainer;
using Hetero::TypeList;
using Hetero::Shell;


struct PrintCore
{
  typedef void RetType;

  template <typename T>
  void
  operator() (T const& t)
  {
    cout << t << endl;
  }
};

typedef Shell<PrintCore> Print;

void
print (bool b)
{
  cout << (b ? "T" : "NIL") << endl;
}

int
main ()
{
  try
  {
    Container a (10L);
    Container b (true);
    Container c (string ("hello"));

    string s = c + string (" world");

    long l = a + 20L;

    cout << s << "; " << l << endl;

    print (b);

    //
    //
    //

    typedef
    TypedContainer <TypeList<long, bool, string> >
    MyContainer;

    MyContainer x (true);
    MyContainer y (10L);
    MyContainer z (string ("hey dude"));

    Print print;

    print (x);
    print (y);
    print (z);

    //
    //
    //

    typedef
    Hetero::Vector<long, bool, string>
    vector;

    vector v;
    v.push_back (10L);
    v.push_back (true);
    v.push_back (false);
    v.push_back (string ("hey"));

    for (vector::iterator i = v.begin (); i != v.end (); i++)
    {
      print (*i);
    }

    std::for_each (v.begin (), v.end (), print);
  }
  catch (Hetero::Typing const&)
  {
    cerr << "typing error" << endl;
  }
}
//$Id$
