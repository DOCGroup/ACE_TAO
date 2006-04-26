// $Id$

#include "ace/Obstack.h"

ACE_RCSID(ace, Obstack, "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Obstack_T<char>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Obstack_T<char>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_END_VERSIONED_NAMESPACE_DECL
