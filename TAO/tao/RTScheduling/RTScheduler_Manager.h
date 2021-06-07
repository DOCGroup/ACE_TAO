// -*- C++ -*-

//=============================================================================
/**
 *  @file   RTScheduler_Manager.h
 *
 *  @author Yamuna Krishnmaurthy <yamuna@oomworks.com>
 */
//=============================================================================

#ifndef TAO_RTSCHEDULER_MANAGER_H
#define TAO_RTSCHEDULER_MANAGER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTScheduler_Manager;
class TAO_RTScheduler_Manager_var;
typedef TAO_RTScheduler_Manager *TAO_RTScheduler_Manager_ptr;

/**
 * @class TAO_RTScheduler_Manager
 *
 * @brief RTScheduler_Manager pseudo-objref.
 *
 * Allows setting of user-defined RTScheduler at run-time.
 */
class TAO_RTScheduler_Export TAO_RTScheduler_Manager :
  public virtual ::CORBA::LocalObject
{
public:
  TAO_RTScheduler_Manager () = default;

  /// Constructor.
  TAO_RTScheduler_Manager (TAO_ORB_Core* orb);

  void rtscheduler (RTScheduling::Scheduler_ptr rtscheduler);

  RTScheduling::Scheduler_ptr rtscheduler ();

public:
  typedef TAO_RTScheduler_Manager_ptr _ptr_type;
  typedef TAO_RTScheduler_Manager_var _var_type;

  // the static operations
  static TAO_RTScheduler_Manager_ptr _duplicate (TAO_RTScheduler_Manager_ptr obj);
  static TAO_RTScheduler_Manager_ptr _narrow (CORBA::Object_ptr obj);
  static TAO_RTScheduler_Manager_ptr _nil ()
    {
      return nullptr;
    }

  virtual const char* _interface_repository_id () const;

protected:
  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  ~TAO_RTScheduler_Manager () = default;

private:
  TAO_RTScheduler_Manager (const TAO_RTScheduler_Manager &) = delete;
  void operator= (const TAO_RTScheduler_Manager &) = delete;
  TAO_ORB_Core* orb_ {};

private:
  RTScheduling::Scheduler_var rtscheduler_;
};

class TAO_RTScheduler_Export TAO_RTScheduler_Manager_var
  : private TAO_Base_var
{
public:
  TAO_RTScheduler_Manager_var (void); // default constructor
  TAO_RTScheduler_Manager_var (TAO_RTScheduler_Manager_ptr);
  TAO_RTScheduler_Manager_var (const TAO_RTScheduler_Manager_var &); // copy constructor
  ~TAO_RTScheduler_Manager_var (void); // destructor

  TAO_RTScheduler_Manager_var &operator= (TAO_RTScheduler_Manager_ptr);
  TAO_RTScheduler_Manager_var &operator= (const TAO_RTScheduler_Manager_var &);
  TAO_RTScheduler_Manager_ptr operator-> () const;

  /// in, inout, out, _retn
  operator const TAO_RTScheduler_Manager_ptr &() const;
  operator TAO_RTScheduler_Manager_ptr &();
  TAO_RTScheduler_Manager_ptr in () const;
  TAO_RTScheduler_Manager_ptr &inout (void);
  TAO_RTScheduler_Manager_ptr &out (void);
  TAO_RTScheduler_Manager_ptr _retn (void);
  TAO_RTScheduler_Manager_ptr ptr () const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_RTScheduler_Manager_ptr duplicate (TAO_RTScheduler_Manager_ptr);
  static void release (TAO_RTScheduler_Manager_ptr);
  static TAO_RTScheduler_Manager_ptr nil (void);
  static TAO_RTScheduler_Manager_ptr narrow (CORBA::Object * );
  static CORBA::Object * upcast (void *);

private:
  TAO_RTScheduler_Manager_ptr ptr_;
  TAO_RTScheduler_Manager_var (const TAO_Base_var &rhs) = delete;
  TAO_RTScheduler_Manager_var &operator= (const TAO_Base_var &rhs) = delete;
};

class TAO_RTScheduler_Export TAO_RTScheduler_Manager_out
{
public:
  TAO_RTScheduler_Manager_out (TAO_RTScheduler_Manager_ptr &);
  TAO_RTScheduler_Manager_out (TAO_RTScheduler_Manager_var &);
  TAO_RTScheduler_Manager_out (const TAO_RTScheduler_Manager_out &);
  TAO_RTScheduler_Manager_out &operator= (const TAO_RTScheduler_Manager_out &);
  TAO_RTScheduler_Manager_out &operator= (const TAO_RTScheduler_Manager_var &);
  TAO_RTScheduler_Manager_out &operator= (TAO_RTScheduler_Manager_ptr);
  operator TAO_RTScheduler_Manager_ptr &();
  TAO_RTScheduler_Manager_ptr &ptr ();
  TAO_RTScheduler_Manager_ptr operator-> ();

private:
  TAO_RTScheduler_Manager_ptr &ptr_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/RTScheduling/RTScheduler_Manager.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_RTSCHEDULER_MANAGER_H */
