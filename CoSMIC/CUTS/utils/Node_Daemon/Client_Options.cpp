// $Id$

#include "Client_Options.h"

//
// Client_Options
//
Client_Options::Client_Options (void)
: verbose_ (false),
  localhost_ (false),
  count_ (1),
  action_ (CA_NONE),
  shutdown_ (false),
  port_ (10000),
  details_ (false)
{

}
