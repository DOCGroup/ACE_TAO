// $Id$
#include "Options.h"
#include "Search_Struct.h"

#ifndef __OPTIMIZE__
Search_Struct::~Search_Struct (void)
{
  if (Options::get_opt (Options::DEBUG))
    fprintf (stderr, "disposing Search_Struct\n");
}

Search_Struct::Search_Struct (void): count (0)
{}

int
Search_Struct::n_elems (void)
{
  return this->count;
}
#endif /* __OPTIMIZE__ */
