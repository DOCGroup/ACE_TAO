// $Id$

// MFC style projects on MicroSoft compilers require CPP files
// They can't cope with c files (as MFC projects are CPP by
// there very nature).

extern "C" {
#include "params_dup.c"
}
