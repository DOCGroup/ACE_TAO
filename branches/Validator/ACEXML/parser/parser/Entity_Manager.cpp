// $Id$

#include "ACEXML/parser/parser/Entity_Manager.h"

static const ACEXML_Char empty_string[] = { 0 };

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Entity_Manager.i"
#endif /* __ACEXML_INLINE__ */


ACEXML_Entity_Manager::ACEXML_Entity_Manager (void)
  : entities_ ()
{

}

ACEXML_Entity_Manager::~ACEXML_Entity_Manager (void)
{
}
