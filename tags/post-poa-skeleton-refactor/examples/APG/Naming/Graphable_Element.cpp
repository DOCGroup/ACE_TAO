// $Id$

#include "Graphable_Element.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class _List_base<Graphable_Element, allocator<Graphable_Element> >;
template void list<Graphable_Element, allocator<Graphable_Element> >::sort(void);
template void list<Graphable_Element, allocator<Graphable_Element> >::merge(list<Graphable_Element, allocator<Graphable_Element> > &);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
pragma instantiate _List_base<Graphable_Element, allocator<Graphable_Element> >;
pragma instantiate list<Graphable_Element, allocator<Graphable_Element> >::sort(void);
pragma instantiate list<Graphable_Element, allocator<Graphable_Element> >::merge(list<Graphable_Element, allocator<Graphable_Element> > &);
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
