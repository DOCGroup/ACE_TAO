// $Id$
#include "Options.h"
#include "new.h"
#include "PMC_All.h"
#include "PMC_Flo.h"
#include "PMC_Usr.h"
#include "PMC_Ruser.h"
#include "SMR_Client.h"

#ifndef __OPTIMIZE__
SMR_Client::SMR_Client (short port_number)
{
  if (CM_Client::open (port_number) < 0)
    perror (Options::program_name), exit (1);
}

SMR_Client::~SMR_Client (void)
{}
#endif /* __OPTIMIZE__ */
