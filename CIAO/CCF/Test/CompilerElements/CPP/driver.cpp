// file      : CCF/Test/CompilerElements/CPP/driver.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <iostream>

#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/Preprocessor.hpp"

using std::cout;
using std::endl;

using CCF::CompilerElements::InputStreamAdapter;

using namespace CCF::CompilerElements::CPP;

//FUZZ: disable check_for_improper_main_declaration

int
main ()
{
  InputStreamAdapter isa (std::cin);
  Preprocessor pp (isa);

  try
  {
    for (;;)
    {
      Token t (pp.next ());

      if (t == Token::eos) break;

      std::cout << t;
    }
  }
  catch (std::exception const& e)
  {
    cout << e.what () << endl;
  }

  return 0;
}
