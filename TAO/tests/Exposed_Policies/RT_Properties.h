//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    RT_Properties.h
//
// = DESCRIPTION
//     Defines a series of "real time" property that an Object
//     or a POA created on a RT-ORB can have.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef RT_PROPERTIES_H_
#define RT_PROPERTIES_H_

#include "tao/RTCORBA/RTCORBA.h"

class RT_Properties
{
public:
  // -- Ctor/Dtor --
  RT_Properties (void);
  ~RT_Properties (void);

  static RT_Properties * read_from (const char *file_name
                                    ACE_ENV_ARG_DECL);

  // -- Accessor Methods --
  void priority (RTCORBA::Priority priority);
  RTCORBA::Priority priority (void);

  void priority_bands (const RTCORBA::PriorityBands& priority_bands);
  const RTCORBA::PriorityBands& priority_bands (void);

  void ior_source (const char *s);
  const char* ior_source (void);

private:

  RTCORBA::Priority priority_;
  RTCORBA::PriorityBands priority_bands_;
  char ior_source_[256];
};

#endif /* RT_PROPERTIES_H_ */
