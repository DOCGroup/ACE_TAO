// $Id$

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

