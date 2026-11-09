ACEXML_INLINE int
ACEXML_Entity_Manager::add_entity (ACEXML_Char const *ref,
                                   ACEXML_Char const *v)
{
  if (this->entities_ == nullptr)
    {
      ACEXML_ENTITIES_MANAGER *entities {};
      ACE_NEW_RETURN (entities, ACEXML_ENTITIES_MANAGER, -1);
      this->entities_.reset (entities);
    }
  ACEXML_String const name (ref, nullptr, false);
  ACEXML_String const value (v, nullptr, false);
  return this->entities_->bind (name, value);
}

ACEXML_INLINE ACEXML_Char const *
ACEXML_Entity_Manager::resolve_entity (ACEXML_Char const *ref)
{
  if (this->entities_ == nullptr)
    return nullptr;

  ACEXML_ENTITY_ENTRY *entry {};

  if (this->entities_->find (ACEXML_String (ref, nullptr, false),
                            entry) == 0)
    return entry->int_id_.c_str();
  return nullptr;
}

ACEXML_INLINE int
ACEXML_Entity_Manager::resolve_entity (ACEXML_Char const *ref,
                                       ACEXML_Char*& systemId,
                                       ACEXML_Char*& publicId)
{
  if (this->entities_ == nullptr)
    return 0;

  publicId = systemId = nullptr;
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
ACEXML_Entity_Manager::reset ()
{
  this->entities_.reset ();
  return 0;
}

ACEXML_INLINE size_t
ACEXML_Entity_Manager::size () const
{
  if (this->entities_ == nullptr)
    return 0;
  return this->entities_->current_size();
}
