// $Id$

#include "ACEXML/parser/parser/Entity_Manager.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Entity_Manager.i"
#endif /* __ACEXML_INLINE__ */

static const ACEXML_Char amp_name[] = {'a', 'm', 'p', 0 };
static const ACEXML_Char amp_value[] = {'&', 0};
static const ACEXML_Char lt_name[] = {'l', 't', 0};
static const ACEXML_Char lt_value[] = {'<', 0};
static const ACEXML_Char gt_name[] = {'g', 't', 0};
static const ACEXML_Char gt_value[] = {'>', 0};
static const ACEXML_Char apos_name[] = {'a', 'p', 'o', 's', 0};
static const ACEXML_Char apos_value[] = {'\'', 0};
static const ACEXML_Char quot_name[] = {'q', 'u', 'o', 't', 0};
static const ACEXML_Char quot_value[] = {'"', 0};

ACEXML_Entity_Manager::ACEXML_Entity_Manager (void)
  : entities_ ()
{
  // @@ No way to know if these bindings success or not.

  ACEXML_String ampname (amp_name, 0, 0);
  ACEXML_String ampvalue  (amp_value, 0, 0);
  this->entities_.bind (ampname, ampvalue);

  ACEXML_String ltname (lt_name, 0, 0);
  ACEXML_String ltvalue  (lt_value, 0, 0);
  this->entities_.bind (ltname, ltvalue);

  ACEXML_String gtname (gt_name, 0, 0);
  ACEXML_String gtvalue  (gt_value, 0, 0);
  this->entities_.bind (gtname, gtvalue);

  ACEXML_String aposname (apos_name, 0, 0);
  ACEXML_String aposvalue  (apos_value, 0, 0);
  this->entities_.bind (aposname, aposvalue);

  ACEXML_String quotname (quot_name, 0, 0);
  ACEXML_String quotvalue  (quot_value, 0, 0);
  this->entities_.bind (quotname, quotvalue);
}

ACEXML_Entity_Manager::~ACEXML_Entity_Manager (void)
{
}
