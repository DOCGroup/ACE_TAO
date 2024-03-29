// -*- C++ -*-

//=============================================================================
/**
 *  @file   Priority_Mapping_Manager.h
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_PRIORITY_MAPPING_MANAGER_H
#define TAO_PRIORITY_MAPPING_MANAGER_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"
#include "tao/RTCORBA/Priority_Mapping.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Priority_Mapping_Manager;
class TAO_Priority_Mapping_Manager_var;
typedef TAO_Priority_Mapping_Manager *TAO_Priority_Mapping_Manager_ptr;


/**
 * @class TAO_Priority_Mapping_Manager
 *
 * @brief Priority_Mapping_Manager pseudo-objref.
 *
 * Allows setting of user-defined Priority_Mapping at run-time.
 */
class TAO_RTCORBA_Export TAO_Priority_Mapping_Manager :
  public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_Priority_Mapping_Manager (RTCORBA::PriorityMapping * mapping);

  ///
  void mapping (RTCORBA::PriorityMapping * mapping);

  ///
  RTCORBA::PriorityMapping *mapping ();

public:
  typedef TAO_Priority_Mapping_Manager_ptr _ptr_type;
  typedef TAO_Priority_Mapping_Manager_var _var_type;

  // the static operations
  static TAO_Priority_Mapping_Manager_ptr _duplicate (TAO_Priority_Mapping_Manager_ptr obj);
  static TAO_Priority_Mapping_Manager_ptr _narrow (CORBA::Object_ptr obj);

  static TAO_Priority_Mapping_Manager_ptr _nil ()
    {
      return nullptr;
    }

  virtual const char* _interface_repository_id () const;

protected:
  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  ~TAO_Priority_Mapping_Manager ();

private:
  TAO_Priority_Mapping_Manager (const TAO_Priority_Mapping_Manager &);
  void operator= (const TAO_Priority_Mapping_Manager &);

private:
  TAO_Priority_Mapping *mapping_;
};

class TAO_RTCORBA_Export TAO_Priority_Mapping_Manager_var
  : private TAO_Base_var
{
public:
  TAO_Priority_Mapping_Manager_var (); // default constructor
  TAO_Priority_Mapping_Manager_var (TAO_Priority_Mapping_Manager_ptr);
  TAO_Priority_Mapping_Manager_var (const TAO_Priority_Mapping_Manager_var &); // copy constructor
  ~TAO_Priority_Mapping_Manager_var (); // destructor

  TAO_Priority_Mapping_Manager_var &operator= (TAO_Priority_Mapping_Manager_ptr);
  TAO_Priority_Mapping_Manager_var &operator= (const TAO_Priority_Mapping_Manager_var &);
  TAO_Priority_Mapping_Manager_ptr operator-> () const;

  /// in, inout, out, _retn
  operator const TAO_Priority_Mapping_Manager_ptr &() const;
  operator TAO_Priority_Mapping_Manager_ptr &();
  TAO_Priority_Mapping_Manager_ptr in () const;
  TAO_Priority_Mapping_Manager_ptr &inout ();
  TAO_Priority_Mapping_Manager_ptr &out ();
  TAO_Priority_Mapping_Manager_ptr _retn ();
  TAO_Priority_Mapping_Manager_ptr ptr () const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_Priority_Mapping_Manager_ptr duplicate (TAO_Priority_Mapping_Manager_ptr);
  static void release (TAO_Priority_Mapping_Manager_ptr);
  static TAO_Priority_Mapping_Manager_ptr nil ();
  static TAO_Priority_Mapping_Manager_ptr narrow (CORBA::Object * );
  static CORBA::Object * upcast (void *);

private:
  TAO_Priority_Mapping_Manager_ptr ptr_;
  TAO_Priority_Mapping_Manager_var (const TAO_Base_var &rhs) = delete;
  TAO_Priority_Mapping_Manager_var &operator= (const TAO_Base_var &rhs) = delete;
};

class TAO_RTCORBA_Export TAO_Priority_Mapping_Manager_out
{
public:
  TAO_Priority_Mapping_Manager_out (TAO_Priority_Mapping_Manager_ptr &);
  TAO_Priority_Mapping_Manager_out (TAO_Priority_Mapping_Manager_var &);
  TAO_Priority_Mapping_Manager_out (const TAO_Priority_Mapping_Manager_out &);
  TAO_Priority_Mapping_Manager_out &operator= (const TAO_Priority_Mapping_Manager_out &);
  TAO_Priority_Mapping_Manager_out &operator= (const TAO_Priority_Mapping_Manager_var &);
  TAO_Priority_Mapping_Manager_out &operator= (TAO_Priority_Mapping_Manager_ptr);
  operator TAO_Priority_Mapping_Manager_ptr &();
  TAO_Priority_Mapping_Manager_ptr &ptr ();
  TAO_Priority_Mapping_Manager_ptr operator-> ();

private:
  TAO_Priority_Mapping_Manager_ptr &ptr_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/RTCORBA/Priority_Mapping_Manager.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_PRIORITY_MAPPING_MANAGER_H */
