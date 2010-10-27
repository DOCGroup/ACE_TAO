// $Id$

// MFC style projects on MicroSoft compilers require CPP files
// They can't cope with c files (as MFC projects are CPP by
// there very nature). Some other platforms incorrectly can't
// cope with the openSSL DSAParams_dup/DHparams_dup when compiled
// under cpp files (which is why this file duplicates the
// params_dup.c file).

#include "params_dup.h"
#include <openssl/asn1.h>

extern "C" DSA *
DSAPARAMS_DUP_WRAPPER_NAME (DSA * dsa)
{
  return DSAparams_dup (dsa);
}

extern "C" DH *
DHPARAMS_DUP_WRAPPER_NAME (DH * dh)
{
  return DHparams_dup (dh);
}
