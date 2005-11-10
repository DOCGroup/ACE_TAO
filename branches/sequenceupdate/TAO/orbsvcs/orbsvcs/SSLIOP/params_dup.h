/* $Id$ */

#ifndef TAO_SSLIOP_PARAMS_DUP_H
#define TAO_SSLIOP_PARAMS_DUP_H

// As of 0.9.7e, OpenSSL's DSAparams_dup() and DHparams_dup() macros
// contain casts that are invalid in C++.  These C wrapper functions
// allows them to be called from C++.

#include <openssl/dsa.h>
#include <openssl/dh.h>

#ifdef __cplusplus
extern "C" {
#endif

extern DSA *DSAparams_dup_wrapper (DSA *dsa);
extern DH  *DHparams_dup_wrapper (DH *dh);

#ifdef __cplusplus
}
#endif

#endif
