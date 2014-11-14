
//=============================================================================
/**
 *  @file   Loader.h
 *
 *  $Id$
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
 *
 */
class OLT_Export Loader : public TAO_Object_Loader
{
public:
  /// ctor
  Loader (void);

  /// Create a new instance
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv[]);
};

// ACE_STATIC_SVC_DECLARE (Loader)
ACE_FACTORY_DECLARE (OLT, Loader)

#endif /* TAO_OBJECT_LOADER_LOADER_H */
