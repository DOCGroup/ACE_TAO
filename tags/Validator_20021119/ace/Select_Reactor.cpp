// $Id$

#include "ace/Select_Reactor.h"

ACE_RCSID(ace, Select_Reactor, "$Id$")


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
template class ACE_Select_Reactor_Token_T<ACE_Token>;
template class ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Token> >;
template class ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Token> >;
template class ACE_Guard< ACE_Select_Reactor_Token_T<ACE_Token> >;
# else
template class ACE_Select_Reactor_Token_T<ACE_Noop_Token>;
template class ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >;
template class ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >;
# endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   pragma instantiate ACE_Select_Reactor_Token_T<ACE_Token>
#   pragma instantiate ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Token> >
#   pragma instantiate ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Token> >
#   pragma instantiate ACE_Guard< ACE_Select_Reactor_Token_T<ACE_Token> >
# else
#   pragma instantiate ACE_Select_Reactor_Token_T<ACE_Noop_Token>
#   pragma instantiate ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
#   pragma instantiate ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
# endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
