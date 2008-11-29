// -*- C++ -*-
// $Id$

#include "tao/PortableServer/PortableServer.h"

#include "LWFT_Client_Init.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

LWFT_Client_Init::LWFT_Client_Init (void)
{
}

LWFT_Client_Init::~LWFT_Client_Init (void)
{
}

CORBA::ORB_ptr
LWFT_Client_Init::init (int &argc, char *argv[])
{
  CORBA::ORB_var the_orb;

  try
    {
      fa_thread_.register_orb_initializer ();
      
      // fa_thread_ and calling app both own ORB reference.

      the_orb = CORBA::ORB_init (argc, argv);
      fa_thread_.orb (the_orb.in ());

      int result = fa_thread_.activate ();

      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("LWFT_Client_Init::init: - ")
                      ACE_TEXT ("Cannot activate forwarding ")
                      ACE_TEXT ("agent thread\n")));
          return CORBA::ORB::_nil ();
        }
      else
        {
          ACE_DEBUG ((LM_TRACE,
                      ACE_TEXT ("LWFT_Client_Init::init: - ")
                      ACE_TEXT ("forwarding agent activated.\n")));

        }
    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "LWFT_Client_Init::init: ");
    }
    
  return the_orb._retn ();
}
