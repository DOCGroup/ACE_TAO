/* -*- C++ -*- */
/**
 *  @file EC_Lifetime_Utils.h
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

#ifndef TAO_EC_LIFETIME_UTILS_H
#define TAO_EC_LIFETIME_UTILS_H
#include /**/ "ace/pre.h"

#include /**/ "event_export.h"
#include "orbsvcs/orbsvcs/RtecEventChannelAdminC.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_Object_Deactivator
 *
 * @brief Utility for deactivating servants from POA.
 *
 * Maintains state necessary to deactivate a servant from POA.
 * Can be told to deactivate a servant explicitly or can do so
 * automagically, in its destructor.
 */
class TAO_RTEvent_Export TAO_EC_Object_Deactivator
{
public:
  /// Default constructor.  Deactivation info can be supplied later
  /// through set_values ().
  TAO_EC_Object_Deactivator (void);

  /// Constructor.  Set <id> which will be deactivated from <poa> in
  /// the deactivator's destructor, unless deactivate () or
  /// disallow_deactivation () are invoked before the destruction.
  TAO_EC_Object_Deactivator (PortableServer::POA_ptr poa,
                             PortableServer::ObjectId const & id);

  /// Destructor.  Deactivates <id_> from <poa_> if those values have
  /// been set, and neither deactivate() nor disallow_deactivation()
  /// have been invoked.
  ~TAO_EC_Object_Deactivator (void);

  /// Set <id> which will be deactivated from <poa> in
  /// the deactivator's destructor, unless deactivate () or
  /// disallow_deactivation () are invoked before the destruction.
  void set_values (PortableServer::POA_ptr poa,
                   PortableServer::ObjectId const & id);

  /// Take on the state of <deactivator>. <deactivator> loses its state.
  void set_values (TAO_EC_Object_Deactivator & deactivator);

  /// Explicitly enable deactivation to happen in destructor or when
  /// deactivate() is called.
  void allow_deactivation (void);

  /// Explicitly disable deactivation from happening in destructor or
  /// when deactivate() is called.
  void disallow_deactivation (void);

  /// Perform deactivation now if <poa_> and <id_> values have been set, and
  /// deactivation hasn't happened yet nor has it been explicitly
  /// disallowed.  CORBA exceptions occurring during deactivation are
  /// not propagated.   Deactivation will NOT happen in the destructor.
  void deactivate (void);

  /// Accessor for the POA used in deactivation.
  PortableServer::POA_var poa (void) const;

private:

  /// Disallow.
  //@{
  TAO_EC_Object_Deactivator (const TAO_EC_Object_Deactivator &rhs);
  TAO_EC_Object_Deactivator& operator= (const TAO_EC_Object_Deactivator &rhs);
  //@}

  /// POA from which the object will be deactivated.
  PortableServer::POA_var poa_;

  /// ObjectId of the object to be deactivated.
  PortableServer::ObjectId id_;

  /// Flag indicating whether deactivation will be attempted.
  /// The flag is set to false if <poa_> and <id_> haven't been set
  /// yet, or if deactivation already happened, or if
  /// disallow_deactivation () method is invoked.
  int deactivate_;
};

//***************************************************************************

/**
 * @class TAO_EC_Deactivated_Object
 *
 * @brief Object deactivation utility (mix-in) class.
 *
 * Maintains state necessary to deactivate object inheriting from this
 * class from POA.  The state can be set using set_deactivator()
 * method.  Then, the object can deactivate itself by doing
 * this->deactivator_.deactivate ()
 *
 * NOTE: deactivation does NOT happen automatically, and must be
 * explicitly initiated as described above.
 */
class TAO_RTEvent_Export TAO_EC_Deactivated_Object
{
public:

  /// Set deactivation state to that specified by the <deactivator>
  /// argument.
  void set_deactivator (TAO_EC_Object_Deactivator & deactivator);

protected:

  TAO_EC_Deactivated_Object (void);
  ~TAO_EC_Deactivated_Object (void);

  /// Utility for deactivating ourselves from POA.
  TAO_EC_Object_Deactivator deactivator_;
};

//***************************************************************************

/**
 * @class TAO_EC_ORB_Holder
 *
 * @brief Utility for automatically destroying the ORB.
 *
 * Holds a reference to an ORB, and calls destroy() on it in the
 * destructor.
 */
class TAO_RTEvent_Export TAO_EC_ORB_Holder
{
public:
  /// Constructor. No-op.
  TAO_EC_ORB_Holder (void);

  /// Destructor.  If holding an ORB, destroy it.
  ~TAO_EC_ORB_Holder (void);

  /// Set the ORB to be destroyed in destructor to <orb_var>.  If
  /// TAO_EC_ORB_Holder already held an orb prior to invocation of
  /// this method, that orb is NOT destroyed.
  void init (CORBA::ORB_var orb_var);

private:

  /// Disallow.
  //@{
  TAO_EC_ORB_Holder & operator= (const TAO_EC_ORB_Holder &rhs);
  TAO_EC_ORB_Holder(const TAO_EC_ORB_Holder &rhs);
  //@}

  /// ORB to be destroyed.
  CORBA::ORB_var orb_;
};

//***************************************************************************

/**
 * @class TAO_EC_Event_Channel_Holder
 *
 * @brief Utility for automatically destroying the Event Channel.
 *
 * Holds a reference to an Event Channel, and calls destroy() on it in the
 * destructor.
 */
class TAO_RTEvent_Export TAO_EC_Event_Channel_Holder
{
public:
  /// Constructor. No-op.
  TAO_EC_Event_Channel_Holder (void);

  /// Destructor.  If holding an Event Channel, destroy it.
  ~TAO_EC_Event_Channel_Holder (void);

  /// Set the Event Channel to be destroyed in destructor to <ec_var>.  If
  /// TAO_EC_Event_Channel_Holder already held an Event Channel prior
  /// to invocation of this method, that Event Channel is NOT destroyed.
  void init (RtecEventChannelAdmin::EventChannel_var ec_var);

private:

  /// Disallow.
  //@{
  TAO_EC_Event_Channel_Holder & operator= (const TAO_EC_Event_Channel_Holder &rhs);
  TAO_EC_Event_Channel_Holder(const TAO_EC_Event_Channel_Holder &rhs);
  //@}

  /// EC to be destroyed.
  RtecEventChannelAdmin::EventChannel_var ec_;
};


#if defined (__ACE_INLINE__)
#include "EC_Lifetime_Utils.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_LIFETIME_UTILS_H */
