// $Id$

ACE_INLINE u_long 
TAO_Servant_Hash::operator () (PortableServer::Servant servant) const
{
  return u_long (servant);
}

