#include "SSLIOP_X509.h"


ACE_RCSID (SSLIOP,
           SSLIOP_X509,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::SSLIOP::OpenSSL_st_var< ::X509 >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO::SSLIOP::OpenSSL_st_var< ::X509 >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
