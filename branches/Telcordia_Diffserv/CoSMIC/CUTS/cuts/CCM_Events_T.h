// -*- C++ -*-

//=============================================================================
/**
 * @file    Events_T.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_CCM_EVENTS_T_H_
#define _CUTS_CCM_EVENTS_T_H_

#include "cuts/config.h"
#include "cuts/CUTS_exec_export.h"

//=============================================================================
/**
 * @class CUTS_EventInfo
 *
 * This class contains the traits for uniquely identifying event types. To
 * define events in CUTS, you must specialize this template with your event
 * event type and define all the traits in the class. If this class is
 * not specialized with the target event type, the it will take all the
 * default values. This will result in the event being considered not
 * register/defined.
 */
//=============================================================================

template <typename EVENTTYPE>
class CUTS_Event_Info
{
public:
  /// Defines the unique ID for the event.
  static const long event_id_ = -1;
};

//=============================================================================
/**
 * @class CUTS_Event_T
 *
 * This class contains the traits for uniquely identifying event types. To
 * define events in CUTS, you must specialize this template with your event
 * event type and define all the traits in the class. If this class is
 * not specialized with the target event type, the it will take all the
 * default values. This will result in the event being considered not
 * register/defined.
 */
//=============================================================================

template <typename EVENTTYPE>
class CUTS_Event_T
  : virtual public EVENTTYPE,
    virtual public ::CORBA::DefaultValueRefCountBase
{
public:
  /// Type definition for the EVENTTYPE
  typedef EVENTTYPE Event_Type;

  /// Default constructor.
  CUTS_Event_T (void);

  /// Destructor.
  virtual ~CUTS_Event_T (void);

  /// Get the <event_id_> for the <eventtype>
  virtual ::CORBA::Long event_id (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException));
};

//=============================================================================
/**
 * @class CUTS_Event_init_T
 */
//=============================================================================

template <typename EVENTTYPE>
class CUTS_Event_init_T
  : public virtual ::CORBA::ValueFactoryBase
{
public:
  /// Type definition for the <eventtype>.
  typedef typename EVENTTYPE::Event_Type Event_Type;

  /// Default constructor.
  CUTS_Event_init_T (void);

  /// Cast operation.
  static CUTS_Event_init_T * _downcast (CORBA::ValueFactoryBase *);

  /// Create an instance of the <eventtype>.
  virtual CORBA::ValueBase * create_for_unmarshal (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Repository ID of the <eventtype>.
  virtual const char * tao_repository_id (void);

protected:
  /// Destructor.
  virtual ~CUTS_Event_init_T (void);
};

#if defined (__CUTS_INLINE__)
# include "cuts/CCM_Events_T.inl"
#endif

#include "cuts/CCM_Events_T.cpp"

#endif  // !defined _CUTS_CCM_EVENTS_T_H_
