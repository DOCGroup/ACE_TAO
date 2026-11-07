// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_Service.h
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_DYNAMIC_SERVICE_H
#define ACE_DYNAMIC_SERVICE_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"
#include "ace/Global_Macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Dynamic_Service_Base.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Service_Object;

/**
 * @class ACE_Dynamic_Service
 *
 * @brief Provides a general interface to retrieve arbitrary objects
 * from the ACE service repository.
 *
 * Uses "name" for lookup in the ACE service repository. Obtains
 * the object and returns it as the appropriate type.
 */
template <class TYPE>
class ACE_Dynamic_Service : public ACE_Dynamic_Service_Base
{
public:
  /// Return instance using @a name to search the Service_Repository.
  static TYPE *instance (ACE_TCHAR const *name);
  static TYPE *instance (ACE_TCHAR const *name, bool no_global);

  static TYPE *instance (ACE_Service_Gestalt const *repo,
                         ACE_TCHAR const *name);
  static TYPE *instance (ACE_Service_Gestalt const *repo,
                         ACE_TCHAR const *name, bool no_global);

#if defined (ACE_USES_WCHAR)

  /// Return instance using @a name to search the Service_Repository.
  static TYPE *instance (ACE_ANTI_TCHAR const *name);

  static TYPE *instance (ACE_ANTI_TCHAR const *name, bool no_global);

  static TYPE *instance (ACE_Service_Gestalt const *repo,
                         ACE_ANTI_TCHAR const *name);
  static TYPE *instance (ACE_Service_Gestalt const *repo,
                         ACE_ANTI_TCHAR const *name, bool no_global);
#endif  // ACE_USES_WCHAR

private:
  ACE_Dynamic_Service () = delete;
  ACE_Dynamic_Service (ACE_Dynamic_Service const &) = delete;
  ACE_Dynamic_Service (ACE_Dynamic_Service &&) = delete;
  ACE_Dynamic_Service &operator= (ACE_Dynamic_Service const &) = delete;
  ACE_Dynamic_Service &operator= (ACE_Dynamic_Service &&) = delete;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Dynamic_Service.inl"
#endif /* __ACE_INLINE__ */

# include "ace/Dynamic_Service.cpp"

#include /**/ "ace/post.h"

#endif /* ACE_DYNAMIC_SERVICE_H */
