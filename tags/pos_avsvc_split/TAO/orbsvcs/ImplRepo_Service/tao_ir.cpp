/* -*- C++ -*- */
// $Id$

#include "tao_ir_i.h"

ACE_RCSID(ImplRepo_Service, tao_ir, "$Id$")

int
main (int argc, ASYS_TCHAR *argv[])
{
  TAO_IR_i tao_ir_i;

  if (tao_ir_i.init (argc, argv) == -1)
    return 1;
  else
    return tao_ir_i.run ();
}
