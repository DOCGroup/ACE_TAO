// $Id$

#include "common/Element_Def_Builder.h"

ACEXML_Element_Def_Builder::~ACEXML_Element_Def_Builder ()
{

}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<ACEXML_Element_Def_Builder>;
template class ACE_Auto_Basic_Ptr<ACEXML_Element_Def_Builder>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<ACEXML_Element_Def_Builder>
#pragma instantiate ACE_Auto_Basic_Ptr<ACEXML_Element_Def_Builder>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
