// $Id$
#include "Options.h"
#include "new.h"
#include "Single_Lookup.h"

#ifndef __OPTIMIZE__
Single_Lookup::Single_Lookup (char *usr_name)
{
  this->frp = new (PRIVATE_POOL) Protocol_Record (ACE::strnew (usr_name));
}

Single_Lookup::~Single_Lookup (void)
{
  if (Options::get_opt (Options::DEBUG))
    fprintf (stderr, "disposing Single_Lookup\n");
}

Protocol_Record *
Single_Lookup::get_each_entry (void)
{
  return this->frp;
}

Protocol_Record *
Single_Lookup::get_next_entry (void)
{
  return this->get_each_entry ();
}
#endif /* __OPTIMIZE__ */
