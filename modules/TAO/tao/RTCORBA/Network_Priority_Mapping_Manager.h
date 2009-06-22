// -*- C++ -*-

//=============================================================================
/**
 *  @file   Network_Priority_Mapping_Manager.h
 *
 *  $Id$
 *
 *  @author Yamuna Krishnamurthy (yamuna@oomworks.com)
 */
//=============================================================================


#ifndef TAO_NETWORK_PRIORITY_MAPPING_MANAGER_H
#define TAO_NETWORK_PRIORITY_MAPPING_MANAGER_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Network_Priority_Mapping_Manager;
class TAO_Network_Priority_Mapping_Manager_var;
typedef TAO_Network_Priority_Mapping_Manager *TAO_Network_Priority_Mapping_Manager_ptr;


namespace RTCORBA
{
}
/**
 * @class TAO_Network_Priority_Mapping_Manager
 *
 * @brief Network_Priority_Mapping_Manager pseudo-objref.
 *
 * Allows setting of user-defined Network_Priority_Mapping at run-time.
 */
class TAO_RTCORBA_Export TAO_Network_Priority_Mapping_Manager :
  public virtual ::CORBA::LocalObject
{

public:
  /// Constructor.
  TAO_Network_Priority_Mapping_Manager (RTCORBA::NetworkPriorityMapping * mapping);

  ///
  void mapping (RTCORBA::NetworkPriorityMapping * mapping);

  ///
  RTCORBA::NetworkPriorityMapping *mapping (void);

public:
  typedef TAO_Network_Priority_Mapping_Manager_ptr _ptr_type;
  typedef TAO_Network_Priority_Mapping_Manager_var _var_type;

  // the static operations
  static TAO_Network_Priority_Mapping_Manager_ptr _duplicate (
      TAO_Network_Priority_Mapping_Manager_ptr obj
    );
  static TAO_Network_Priority_Mapping_Manager_ptr _narrow (
      CORBA::Object_ptr obj
      );

  static TAO_Network_Priority_Mapping_Manager_ptr _nil (void)
    {
      return (TAO_Network_Priority_Mapping_Manager_ptr)0;
    }

  virtual const char* _interface_repository_id (void) const;

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  ~TAO_Network_Priority_Mapping_Manager (void);

private:

  TAO_Network_Priority_Mapping_Manager (const TAO_Network_Priority_Mapping_Manager &);
  void operator= (const TAO_Network_Priority_Mapping_Manager &);

private:

  TAO_Network_Priority_Mapping *mapping_;
};

class TAO_RTCORBA_Export TAO_Network_Priority_Mapping_Manager_var
  : private TAO_Base_var
{
public:
  TAO_Network_Priority_Mapping_Manager_var (void); // default constructor
  TAO_Network_Priority_Mapping_Manager_var (TAO_Network_Priority_Mapping_Manager_ptr);
  TAO_Network_Priority_Mapping_Manager_var (const TAO_Network_Priority_Mapping_Manager_var &); // copy constructor
  ~TAO_Network_Priority_Mapping_Manager_var (void); // destructor

  TAO_Network_Priority_Mapping_Manager_var &operator= (TAO_Network_Priority_Mapping_Manager_ptr);
  TAO_Network_Priority_Mapping_Manager_var &operator= (const TAO_Network_Priority_Mapping_Manager_var &);
  TAO_Network_Priority_Mapping_Manager_ptr operator-> (void) const;

  /// in, inout, out, _retn
  operator const TAO_Network_Priority_Mapping_Manager_ptr &() const;
  operator TAO_Network_Priority_Mapping_Manager_ptr &();
  TAO_Network_Priority_Mapping_Manager_ptr in (void) const;
  TAO_Network_Priority_Mapping_Manager_ptr &inout (void);
  TAO_Network_Priority_Mapping_Manager_ptr &out (void);
  TAO_Network_Priority_Mapping_Manager_ptr _retn (void);
  TAO_Network_Priority_Mapping_Manager_ptr ptr (void) const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_Network_Priority_Mapping_Manager_ptr duplicate (TAO_Network_Priority_Mapping_Manager_ptr);
  static void release (TAO_Network_Priority_Mapping_Manager_ptr);
  static TAO_Network_Priority_Mapping_Manager_ptr nil (void);
  static TAO_Network_Priority_Mapping_Manager_ptr narrow (CORBA::Object * );
  static CORBA::Object * upcast (void *);

private:
  /// Unimplemented - prevents widening assignment.
  TAO_Network_Priority_Mapping_Manager_ptr ptr_;
  TAO_Network_Priority_Mapping_Manager_var (const TAO_Base_var &rhs);
  TAO_Network_Priority_Mapping_Manager_var &operator= (const TAO_Base_var &rhs);
};

class TAO_RTCORBA_Export TAO_Network_Priority_Mapping_Manager_out
{
public:
  TAO_Network_Priority_Mapping_Manager_out (TAO_Network_Priority_Mapping_Manager_ptr &);
  TAO_Network_Priority_Mapping_Manager_out (TAO_Network_Priority_Mapping_Manager_var &);
  TAO_Network_Priority_Mapping_Manager_out (const TAO_Network_Priority_Mapping_Manager_out &);
  TAO_Network_Priority_Mapping_Manager_out &operator= (const TAO_Network_Priority_Mapping_Manager_out &);
  TAO_Network_Priority_Mapping_Manager_out &operator= (const TAO_Network_Priority_Mapping_Manager_var &);
  TAO_Network_Priority_Mapping_Manager_out &operator= (TAO_Network_Priority_Mapping_Manager_ptr);
  operator TAO_Network_Priority_Mapping_Manager_ptr &();
  TAO_Network_Priority_Mapping_Manager_ptr &ptr (void);
  TAO_Network_Priority_Mapping_Manager_ptr operator-> (void);

private:
  TAO_Network_Priority_Mapping_Manager_ptr &ptr_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_NETWORK_PRIORITY_MAPPING_MANAGER_H */
