/* -*- C++ -*- */
// $Id$

#include "tao_imr_i.h"

ACE_RCSID(ImplRepo_Service, tao_imr, "$Id$")

int
main (int argc, ASYS_TCHAR *argv[])
{
  TAO_IMR_i tao_imr_i;

  if (tao_imr_i.init (argc, argv) == -1)
    return 1;
  else
    return tao_imr_i.run ();
}
