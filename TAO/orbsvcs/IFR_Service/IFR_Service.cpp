/* -*- C++ -*- */
// $Id$

#include "IFR_Service.h"
#include "orbsvcs/orbsvcs/IFRService/ComponentRepository_i.h"
#include "orbsvcs/orbsvcs/IFRService/Options.h"
#include "orbsvcs/orbsvcs/IFRService/IFR_ComponentsS.h"
#include "orbsvcs/orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IOR_Multicast.h"
#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (IFR_Service,
           IFR_Service,
           "$Id$")

IFR_Service::IFR_Service (void)
{
}

IFR_Service::~IFR_Service (void)
{
}

int
IFR_Service::init (int argc,
                   char *argv[]
                   ACE_ENV_ARG_DECL)
{
  int result;
  ACE_TRY
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    0
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result = this->my_ifr_server_.init_with_orb (argc,
                                                   argv,
                                                   this->orb_.in ());
      if (result == -1)
        return result;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "IFR_Service::init");

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
IFR_Service::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->orb_->run (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
IFR_Service::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      this->my_ifr_server_.fini ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "IFR_Service::fini");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

