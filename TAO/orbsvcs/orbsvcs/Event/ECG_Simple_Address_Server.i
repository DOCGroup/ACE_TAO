// $Id$



ACE_INLINE TAO_EC_Servant_Var<TAO_ECG_Simple_Address_Server>
TAO_ECG_Simple_Address_Server::create (void)
{
  TAO_EC_Servant_Var<TAO_ECG_Simple_Address_Server> s;
  ACE_NEW_RETURN (s,
                  TAO_ECG_Simple_Address_Server,
                  s);
  return s;
}
