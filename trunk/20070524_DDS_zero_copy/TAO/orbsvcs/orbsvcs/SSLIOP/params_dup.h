/* $Id$ */

#ifndef TAO_SSLIOP_PARAMS_DUP_H
#define TAO_SSLIOP_PARAMS_DUP_H

#include /**/ "ace/pre.h"

#include "ace/config-macros.h"
#include "tao/Versioned_Namespace.h"

// As of 0.9.7e, OpenSSL's DSAparams_dup() and DHparams_dup() macros
// contain casts that are invalid in C++.  These C wrapper functions
// allows them to be called from C++.

#include <openssl/dsa.h>
#include <openssl/dh.h>

#if (defined (TAO_HAS_VERSIONED_NAMESPACE) && TAO_HAS_VERSIONED_NAMESPACE == 1)

# define DSAPARAMS_DUP_WRAPPER_NAME ACE_PREPROC_CONCATENATE(TAO_VERSIONED_NAMESPACE_NAME, _DSAparams_dup_wrapper)
# define DHPARAMS_DUP_WRAPPER_NAME ACE_PREPROC_CONCATENATE(TAO_VERSIONED_NAMESPACE_NAME, _DHparams_dup_wrapper)

#else

# define DSAPARAMS_DUP_WRAPPER_NAME DSAparams_dup_wrapper
# define DHPARAMS_DUP_WRAPPER_NAME  DHparams_dup_wrapper

#endif  /* ACE_HAS_VERSIONED_NAMESPACE == 1 */


#ifdef __cplusplus
extern "C" {
#endif

extern DSA *DSAPARAMS_DUP_WRAPPER_NAME (DSA *dsa);
extern DH  *DHPARAMS_DUP_WRAPPER_NAME (DH *dh);

#ifdef __cplusplus
}
#endif

#include /**/ "ace/post.h"

#endif
