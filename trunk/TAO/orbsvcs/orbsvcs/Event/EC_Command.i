// $Id$

template<class Target, class Object>
TAO_EC_Connected_Command<Target,Object>::
      TAO_EC_Connected_Command (Target *target,
                                Object *object)
  : target_ (target),
    object_ (object)
{
}

// ****************************************************************

template<class Target, class Object>
TAO_EC_Disconnected_Command<Target,Object>::
      TAO_EC_Disconnected_Command (Target *target,
                                   Object *object)
  : target_ (target),
    object_ (object)
{
}

// ****************************************************************

template<class Target>
TAO_EC_Shutdown_Command<Target>::
      TAO_EC_Shutdown_Command (Target *target)
  : target_ (target)
{
}
