// $Id$

#include "DIOP.h"
#include "DIOP_Factory.h"

ACE_RCSID(TAO_DIOP, DIOP_Factory, "$Id$")

TAO_DIOP_Protocol_Factory_Adder::TAO_DIOP_Protocol_Factory_Adder (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_DIOP_Protocol_Factory);
}

