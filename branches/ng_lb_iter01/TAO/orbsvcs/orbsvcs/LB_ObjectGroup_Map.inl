// -*- C++ -*-

ACE_INLINE TAO_LB_ObjectGroup_Map::iterator
TAO_LB_ObjectGroup_Map::begin (void)
{
  return this->table_.begin ();
}

ACE_INLINE TAO_LB_ObjectGroup_Map::iterator
TAO_LB_ObjectGroup_Map::end (void)
{
  return this->table_.end ();
}
