// $Id$

#include "tao/TAO_Server_Request.h"
#include "tao/operation_details.h"
#include "tao/Transport.h"
#include "tao/Profile.h"
#include "tao/SystemException.h"
#include "tao/debug.h"
#include "tao/CDR.h"

#include "Codeset_Manager.h"

#include "ace/Dynamic_Service.h"
#include "ace/Codeset_Registry.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (tao,
           Codeset_Manager,
           "$Id$")


TAO_Codeset_Manager::~TAO_Codeset_Manager ()
{
}

TAO_Codeset_Factory::~TAO_Codeset_Factory ()
{
}
