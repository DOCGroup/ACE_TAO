// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class Target, class Object>
TAO_ESF_Connected_Command<Target,Object>::
      TAO_ESF_Connected_Command (Target *target,
                                Object *object)
  : target_ (target),
    object_ (object)
{
}

// ****************************************************************

template<class Target, class Object>
TAO_ESF_Reconnected_Command<Target,Object>::
      TAO_ESF_Reconnected_Command (Target *target,
                                  Object *object)
  : target_ (target),
    object_ (object)
{
}

// ****************************************************************

template<class Target, class Object>
TAO_ESF_Disconnected_Command<Target,Object>::
      TAO_ESF_Disconnected_Command (Target *target,
                                   Object *object)
  : target_ (target),
    object_ (object)
{
}

// ****************************************************************

template<class Target>
TAO_ESF_Shutdown_Command<Target>::
      TAO_ESF_Shutdown_Command (Target *target)
  : target_ (target)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
