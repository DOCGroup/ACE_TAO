// $Id$

#include "Operation_Table_Factory.h"
#include "Operation_Table_Parameters.h"

ACE_RCSID(PortableServer,
          Operation_Table_Factory,
          "$Id$")

TAO_Operation_Table_Factory::TAO_Operation_Table_Factory (void)
{
}

TAO_Operation_Table_Factory::~TAO_Operation_Table_Factory (void)
{
}

TAO_Operation_Table *
TAO_Operation_Table_Factory::opname_lookup_strategy (void)
{
  TAO_Operation_Table_Parameters * const p =
    TAO_OP_TABLE_PARAMETERS::instance ();

  return p->concrete_strategy ();
}

