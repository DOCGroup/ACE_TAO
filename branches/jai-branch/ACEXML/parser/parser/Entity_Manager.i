// $Id$

ACEXML_INLINE int
ACEXML_Entity_Manager::add_entity (const ACEXML_Char *ref,
                                   const ACEXML_Char *v)
{
  ACEXML_String name (ref, 0, 0);
  ACEXML_String value  (v, 0, 0);
  return this->entities_.bind (name, value);
}

ACEXML_INLINE const ACEXML_Char*
ACEXML_Entity_Manager::resolve_entity (const ACEXML_Char *ref)
{
  ACEXML_ENTITY_ENTRY *entry;

  if (this->entities_.find (ACEXML_String (ref, 0, 0),
                            entry) == 0)
    return entry->int_id_.c_str();
  return 0;
}

ACEXML_INLINE int
ACEXML_Entity_Manager::resolve_entity (const ACEXML_Char* ref,
                                       ACEXML_Char*& systemId,
                                       ACEXML_Char*& publicId)
{
  publicId = systemId = 0;
  ACEXML_ENTITY_ENTRY_ITERATOR iter (this->entities_, ref);
  ACEXML_ENTITY_ENTRY_ITERATOR end (this->entities_, ref, 1);

  if (iter != end)
    {
      systemId = ACE_const_cast (ACEXML_Char*, (*iter).int_id_.c_str());
      ++iter;
      if (iter != end)
        publicId = ACE_const_cast (ACEXML_Char*, (*iter).int_id_.c_str());
      return 0;
    }
  return -1;
}

ACEXML_INLINE int
ACEXML_Entity_Manager::reset (void)
{
  if (this->entities_.close() < 0 || this->entities_.open() < 0)
    return -1;
  return 0;
}


ACEXML_INLINE size_t
ACEXML_Entity_Manager::size (void) const
{
  return this->entities_.current_size();
}
