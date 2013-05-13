// -*- C++ -*-

/**
 *  @file   EC_Lifetime_Utils_T.h
 *
 *  $Id$
 *
 *  @author Jody Hagins (jody@atdesk.com)
 *  @author Marina Spivak (marina@atdesk.com)
 *
 *  This file is a temporary place for general CORBA application
 *  utility classes.  These classes will be moved out from the EC
 *  library and into TAO or will be replaced by other TAO classes with
 *  similar functionality.
 */

#ifndef TAO_EC_LIFETIME_UTILS_T_H
#define TAO_EC_LIFETIME_UTILS_T_H

#include "orbsvcs/Event/EC_Lifetime_Utils.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @brief Helper for activating objects.
 * Activates @a servant with @a poa and returns the object reference via
 * @a obj_ref.  If @a object_deactivator != 0, it is populated with info
 * necessary to deactivate the @a servant from @a poa.
 */
template <typename T>
void activate (T & obj_ref,
               PortableServer::POA_ptr poa,
               PortableServer::ServantBase * servant,
               TAO_EC_Object_Deactivator & object_deactivator);


//***************************************************************************

/**
 * @class TAO_EC_Auto_Command<COMMAND>
 *
 * @brief Utility class which executes COMMAND in its destructor.
 *
 * Template argument requirements:
 *
 * Has void execute (void); method which
 * can throw ONLY CORBA exceptions.
 * Has default and copy constructors.
 *
 */
template <class T>
class TAO_EC_Auto_Command
{
public:
  TAO_EC_Auto_Command (void);
  TAO_EC_Auto_Command (const T & command);
  ~TAO_EC_Auto_Command (void);
  void set_command (const T & command);
  void set_command (TAO_EC_Auto_Command<T> & auto_command);
  void execute (void);
  void allow_command (void);
  void disallow_command (void);

private:

  TAO_EC_Auto_Command (const TAO_EC_Auto_Command &);
  TAO_EC_Auto_Command & operator=  (const TAO_EC_Auto_Command &);

  T command_;
  int allow_command_;
};


//***************************************************************************

template <class T>
class TAO_EC_Shutdown_Command
{
public:
  TAO_EC_Shutdown_Command (void);
  TAO_EC_Shutdown_Command (T target);
  void execute (void);

private:

  T target_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Lifetime_Utils_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Event/EC_Lifetime_Utils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Lifetime_Utils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* EC_LIFETIME_UTILS_T_H */
