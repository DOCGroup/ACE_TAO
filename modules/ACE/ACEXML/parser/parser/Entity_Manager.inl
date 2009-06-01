// $Id$

ACEXML_INLINE int
ACEXML_Entity_Manager::add_entity (const ACEXML_Char *ref,
                                   const ACEXML_Char *v)
{
  if (!this->entities_ )
    ACE_NEW_RETURN (this->entities_, ACEXML_ENTITIES_MANAGER, -1);
  ACEXML_String name (ref, 0, false);
  ACEXML_String value  (v, 0, false);
  return this->entities_->bind (name, value);
}

ACEXML_INLINE const ACEXML_Char*
ACEXML_Entity_Manager::resolve_entity (const ACEXML_Char *ref)
{
  if (!this->entities_)
    return 0;

  ACEXML_ENTITY_ENTRY *entry = 0;

  if (this->entities_->find (ACEXML_String (ref, 0, false),
                            entry) == 0)
    return entry->int_id_.c_str();
  return 0;
}

ACEXML_INLINE int
ACEXML_Entity_Manager::resolve_entity (const ACEXML_Char* ref,
                                       ACEXML_Char*& systemId,
                                       ACEXML_Char*& publicId)
{
  if (!this->entities_)
    return 0;

  publicId = systemId = 0;
  ACEXML_ENTITY_ENTRY_ITERATOR iter (*this->entities_, ref);
  ACEXML_ENTITY_ENTRY_ITERATOR end (*this->entities_, ref, 1);

  if (iter != end)
    {
      systemId = const_cast<ACEXML_Char*> ((*iter).int_id_.c_str());
      ++iter;
      if (iter != end)
        publicId = const_cast<ACEXML_Char*> ((*iter).int_id_.c_str());
      return 0;
    }
  return -1;
}

ACEXML_INLINE int
ACEXML_Entity_Manager::reset (void)
{
  delete this->entities_;
  this->entities_ = 0;
  return 0;
}


ACEXML_INLINE size_t
ACEXML_Entity_Manager::size (void) const
{
  if (!this->entities_)
    return 0;
  return this->entities_->current_size();
}
