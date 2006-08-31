// -*- C++ -*-

//=============================================================================
/**
 * @file      Auto_Release_Ptr.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_AUTO_RELEASE_PTR_H_
#define _CUTS_AUTO_RELEASE_PTR_H_

#include "cuts/config.h"

//=============================================================================
/**
 * @class CUTS_Auto_Release_Ptr
 *
 * @brief Auto pointer class that calls the release () method on
 * the object when destroyed.
 *
 * In order to use this class, the \a TYPE element must have a release ()
 * method defined that can be used for releasing resources as opposed to
 * using the destructor.
 */
//=============================================================================

template <typename TYPE>
class CUTS_Auto_Release_Ptr
{
public:
  /// Type definition for the pointer type.
  typedef TYPE Ptr_Type;

  /**
   * Initializing constructor.
   *
   * @param[in]     ptr     Pointer to the object to manage.
   */
  explicit CUTS_Auto_Release_Ptr (TYPE * ptr = 0);

  /// Destructor.
  ~CUTS_Auto_Release_Ptr (void);

  /**
   * Detach from the current object. This will prevent the contained
   * element from being released when this object is destroyed. The
   * contained object is returned to the client.
   *
   * @return      Pointer to the contained object.
   */
  TYPE * detach (void);

  /**
   * Force the object to manager a new element. This will cause
   * the current element to be released. To prevent the current
   * element from being released, the detach () method should
   * be invoked prior to this method.
   *
   * @param[in]     ptr     Pointer to the new element.
   */
  void reset (TYPE * ptr = 0);

  /**
   * Get an unmanaged pointer to the element.
   *
   * @return    Pointer to the element.
   */
  TYPE * get (void);

  /**
   * Pointer operator. This allows clients to use this class
   * as if it was a \a TYPE pointer itself.
   *
   * @return    Pointer to the element.
   */
  TYPE * operator -> (void);

  /**
   * Deference operator. This method returns a reference to the
   *contained element. If the \a ptr_ == 0, this a segmentation
   * fault will occur.
   *
   * @return      Reference to the element.
   */
  TYPE & operator * (void);

private:
  /// Pointer to the element.
  TYPE * ptr_;

  // Disallow the following operations.
  CUTS_Auto_Release_Ptr (const CUTS_Auto_Release_Ptr &);
  const CUTS_Auto_Release_Ptr & operator = (const CUTS_Auto_Release_Ptr &);
};

#if defined (__CUTS_INLINE__)
#include "cuts/Auto_Release_Ptr.inl"
#endif

#include "cuts/Auto_Release_Ptr.cpp"

#endif  // !defined _CUTS_AUTO_RELEASE_PTR_H_

