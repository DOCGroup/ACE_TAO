// $Id$

#include "FT_Naming_Service.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Server.h"
#include "orbsvcs/Naming/Persistent_Naming_Context_Factory.h"

// Default Constructor.
TAO_FT_Naming_Service::TAO_FT_Naming_Service (void)
{
}

// Constructor taking command-line arguments.
TAO_FT_Naming_Service::TAO_FT_Naming_Service (int argc, ACE_TCHAR* argv[])
  : TAO_Naming_Service (argc, argv)
{
}



/// Create a server object for the naming service
TAO_Naming_Server*
TAO_FT_Naming_Service::create_naming_server ()
{
  return new (ACE_nothrow) TAO_FT_Naming_Server ();
}


// Destructor.
TAO_FT_Naming_Service::~TAO_FT_Naming_Service (void)
{
}
