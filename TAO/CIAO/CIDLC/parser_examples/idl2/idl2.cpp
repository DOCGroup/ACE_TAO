// $Id$

#if defined(_DEBUG)
//#define SPIRIT_DEBUG
#endif // defined(_DEBUG)

#include "idl2_grammar.h"

int
main (int argc, char* argv[])
{
  cerr << "IDL2 Grammar checker implemented with Spirit ..." << endl;

idl2_grammar g;

  if (2 == argc)
    {
      parse (g, 
             argv[1]);
    }
  else
    {
      cerr << "No filename given" << endl;
    }
    
  return 0;
}
