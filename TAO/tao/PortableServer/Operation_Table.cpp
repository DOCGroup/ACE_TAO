// $Id$

#include "Operation_Table.h"
#include "tao/ORB.h"

ACE_RCSID(PortableServer,
          Operation_Table,
          "$Id$")

// destructor
TAO_Operation_Table::~TAO_Operation_Table (void)
{
}

/**************************************************************/
TAO::Operation_Skeletons::Operation_Skeletons (void)
  : skel_ptr (0)
    , thruPOA_skel_ptr (0)
    , direct_skel_ptr (0)
{
}

