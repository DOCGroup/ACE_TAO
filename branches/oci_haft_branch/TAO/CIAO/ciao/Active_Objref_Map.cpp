// $Id$

#include "ciao/Active_Objref_Map.h"



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Active_Map_Manager<CORBA::Object_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Active_Map_Manager<CORBA::Object_var>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
