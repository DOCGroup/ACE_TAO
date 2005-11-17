/* $Id$ */

#ifndef TAO_SSLIOP_PARAMS_DUP_H
#define TAO_SSLIOP_PARAMS_DUP_H

// As of 0.9.7e, OpenSSL's DSAparams_dup() and DHparams_dup() macros
// contain casts that are invalid in C++.  These C wrapper functions
// allows them to be called from C++.

#include <openssl/dsa.h>
#include <openssl/dh.h>

#if (defined (TAO_HAS_VERSIONED_NAMESPACE) \
     && TAO_HAS_VERSIONED_NAMESPACE == 1) \
  && !(defined (_MSC_VER) && _MSC_VER <= 1200)
// MSVC++ 6's preprocessor can't handle macro expansions required by
// the versioned namespace support.  *sigh*

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

#endif
