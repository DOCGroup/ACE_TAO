/* -*- C++ -*- */
// $Id$

#include "tao_imr_i.h"

int
main (int argc, ACE_TCHAR *argv[])
{
  TAO_IMR_i tao_imr_i;

  if (tao_imr_i.init (argc, argv) == -1)
    return 1;
  else
    return tao_imr_i.run ();
}
