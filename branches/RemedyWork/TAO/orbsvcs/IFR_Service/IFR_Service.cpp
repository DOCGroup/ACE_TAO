/* -*- C++ -*- */
// $Id$

#include "IFR_Service.h"
#include "orbsvcs/IFRService/ComponentRepository_i.h"
#include "orbsvcs/IFRService/Options.h"
#include "orbsvcs/IFRService/IFR_ComponentsS.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"
#include "orbsvcs/IOR_Multicast.h"
#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"
#include "ace/Auto_Ptr.h"

IFR_Service::IFR_Service (void)
{
}

IFR_Service::~IFR_Service (void)
{
}

int
IFR_Service::init (int argc,
                   ACE_TCHAR *argv[])
{
  int result;
  try
    {
      this->orb_ = CORBA::ORB_init (argc, argv);

      result = this->my_ifr_server_.init_with_orb (argc,
                                                   argv,
                                                   this->orb_.in ());
      if (result != 0)
        {
          return result;
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("IFR_Service::init");

      throw;
    }
  return 0;
}

int
IFR_Service::run (void)
{
  this->orb_->run (0);

  return 0;
}

int
IFR_Service::fini (void)
{
  try
    {
      this->my_ifr_server_.fini ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("IFR_Service::fini");
      throw;
    }
  return 0;
}

void
IFR_Service::shutdown (void)
{
  this->orb_->shutdown ();
}
