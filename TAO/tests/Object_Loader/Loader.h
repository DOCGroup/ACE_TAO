
//=============================================================================
/**
 *  @file   Loader.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_OBJECT_LOADER_LOADER_H
#define TAO_OBJECT_LOADER_LOADER_H

#include "tao/Object_Loader.h"
#include "OLT_Export.h"

/**
 * @class Loader
 *
 * @brief Implement the Test interface
 */
class OLT_Export Loader : public TAO_Object_Loader
{
public:
  /// ctor
  Loader () = default;

  Loader (Loader const &) = delete;
  Loader (Loader &&) = delete;
  Loader & operator= (Loader const &) = delete;
  Loader & operator= (Loader &&) = delete;

  /// Create a new instance
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc,
                                   ACE_TCHAR *argv[]) override;
};

// ACE_STATIC_SVC_DECLARE (Loader)
ACE_FACTORY_DECLARE (OLT, Loader)

#endif /* TAO_OBJECT_LOADER_LOADER_H */
