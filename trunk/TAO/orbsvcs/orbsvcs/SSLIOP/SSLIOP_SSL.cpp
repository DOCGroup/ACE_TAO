// $Id$

#include "SSLIOP_SSL.h"


ACE_RCSID (SSLIOP,
           SSLIOP_SSL,
           "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO::SSLIOP::OpenSSL_st_var< ::SSL >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate TAO::SSLIOP::OpenSSL_st_var< ::SSL >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
