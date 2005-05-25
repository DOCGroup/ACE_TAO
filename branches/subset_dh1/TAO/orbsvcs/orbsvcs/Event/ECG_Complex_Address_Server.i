// $Id$


ACE_INLINE TAO_EC_Servant_Var<TAO_ECG_Complex_Address_Server>
TAO_ECG_Complex_Address_Server::create (int is_source_mapping)
{
  TAO_EC_Servant_Var<TAO_ECG_Complex_Address_Server> s;
  ACE_NEW_RETURN (s,
                  TAO_ECG_Complex_Address_Server (is_source_mapping),
                  s);
  return s;
}
