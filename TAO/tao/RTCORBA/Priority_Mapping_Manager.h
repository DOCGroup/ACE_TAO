/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Priority_Mapping_Manager.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_PRIORITY_MAPPING_MANAGER_H
#define TAO_PRIORITY_MAPPING_MANAGER_H
#include "ace/pre.h"

#include "tao/orbconf.h"
#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#include "Priority_Mapping.h"
#include "tao/LocalObject.h"

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
  public virtual TAO_Local_RefCounted_Object
{

public:
  /// Constructor.
  TAO_Priority_Mapping_Manager (RTCORBA::PriorityMapping * mapping);

  ///
  void mapping (RTCORBA::PriorityMapping * mapping);

  ///
  RTCORBA::PriorityMapping *mapping (void);

public:
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_Priority_Mapping_Manager_ptr _ptr_type;
  typedef TAO_Priority_Mapping_Manager_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  // the static operations
  static TAO_Priority_Mapping_Manager_ptr _duplicate (TAO_Priority_Mapping_Manager_ptr obj);
  static TAO_Priority_Mapping_Manager_ptr _narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  static TAO_Priority_Mapping_Manager_ptr _unchecked_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  static TAO_Priority_Mapping_Manager_ptr _nil (void)
    {
      return (TAO_Priority_Mapping_Manager_ptr)0;
    }

  virtual void *_tao_QueryInterface (ptrdiff_t type);

  virtual const char* _interface_repository_id (void) const;

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  ~TAO_Priority_Mapping_Manager (void);

private:

  TAO_Priority_Mapping_Manager (const TAO_Priority_Mapping_Manager &);
  void operator= (const TAO_Priority_Mapping_Manager &);

private:

  TAO_Priority_Mapping *mapping_;
};

class TAO_RTCORBA_Export TAO_Priority_Mapping_Manager_var : public TAO_Base_var
{
public:
  TAO_Priority_Mapping_Manager_var (void); // default constructor
  TAO_Priority_Mapping_Manager_var (TAO_Priority_Mapping_Manager_ptr);
  TAO_Priority_Mapping_Manager_var (const TAO_Priority_Mapping_Manager_var &); // copy constructor
  ~TAO_Priority_Mapping_Manager_var (void); // destructor

  TAO_Priority_Mapping_Manager_var &operator= (TAO_Priority_Mapping_Manager_ptr);
  TAO_Priority_Mapping_Manager_var &operator= (const TAO_Priority_Mapping_Manager_var &);
  TAO_Priority_Mapping_Manager_ptr operator-> (void) const;

  /// in, inout, out, _retn
  operator const TAO_Priority_Mapping_Manager_ptr &() const;
  operator TAO_Priority_Mapping_Manager_ptr &();
  TAO_Priority_Mapping_Manager_ptr in (void) const;
  TAO_Priority_Mapping_Manager_ptr &inout (void);
  TAO_Priority_Mapping_Manager_ptr &out (void);
  TAO_Priority_Mapping_Manager_ptr _retn (void);
  TAO_Priority_Mapping_Manager_ptr ptr (void) const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_Priority_Mapping_Manager_ptr duplicate (TAO_Priority_Mapping_Manager_ptr);
  static void release (TAO_Priority_Mapping_Manager_ptr);
  static TAO_Priority_Mapping_Manager_ptr nil (void);
  static TAO_Priority_Mapping_Manager_ptr narrow (CORBA::Object * ACE_ENV_ARG_DECL_NOT_USED);
  static CORBA::Object * upcast (void *);

private:
  /// Unimplemented - prevents widening assignment.
  TAO_Priority_Mapping_Manager_ptr ptr_;
  TAO_Priority_Mapping_Manager_var (const TAO_Base_var &rhs);
  TAO_Priority_Mapping_Manager_var &operator= (const TAO_Base_var &rhs);
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
  TAO_Priority_Mapping_Manager_ptr &ptr (void);
  TAO_Priority_Mapping_Manager_ptr operator-> (void);

private:
  TAO_Priority_Mapping_Manager_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
#include "Priority_Mapping_Manager.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_PRIORITY_MAPPING_MANAGER_H */
