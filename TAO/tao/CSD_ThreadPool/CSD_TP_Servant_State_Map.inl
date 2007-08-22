// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Servant_State_Map::TP_Servant_State_Map()
{
}


ACE_INLINE
TAO::CSD::TP_Servant_State_Map::~TP_Servant_State_Map()
{
}


ACE_INLINE
TAO::CSD::TP_Servant_State*
TAO::CSD::TP_Servant_State_Map::find(PortableServer::Servant servant)
{
  void* key = servant;

  TP_Servant_State::HandleType value;

  if (this->map_.find(key, value) != 0)
    {
      throw PortableServer::POA::ServantNotActive ();
    }

  return value._retn();
}


ACE_INLINE
void
TAO::CSD::TP_Servant_State_Map::insert(PortableServer::Servant servant)
{
  void* key = servant;

  TP_Servant_State::HandleType value = new TP_Servant_State ();

  int const result = this->map_.bind(key, value);

  if (result == 1)
    {
      throw PortableServer::POA::ServantAlreadyActive ();
    }

  ACE_ASSERT (result == 0);
}


ACE_INLINE
void
TAO::CSD::TP_Servant_State_Map::remove(PortableServer::Servant servant)
{
  void* key = servant;

  if (this->map_.unbind(key) == -1)
    {
      throw PortableServer::POA::ServantNotActive ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
