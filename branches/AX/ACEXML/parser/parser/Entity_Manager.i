// $Id$

ACEXML_INLINE int
ACEXML_Entity_Manager::add_entity (const ACEXML_Char *ref,
                                   const ACEXML_Char *v)
{
  ACEXML_String name (ref, 0, 0);
  ACEXML_String value  (v, 0, 0);
  return this->entities_.bind (name, value);
}

ACEXML_INLINE const ACEXML_String *
ACEXML_Entity_Manager::resolve_entity (const ACEXML_Char *ref)
{
  ACEXML_ENTITY_ENTRY *entry;

  if (this->entities_.find (ACEXML_String (ref, 0, 0),
                            entry) == 0)
    return &entry->int_id_;
  return 0;
}
