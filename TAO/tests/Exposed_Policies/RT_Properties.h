//=============================================================================
/**
 *  @file    RT_Properties.h
 *
 *   Defines a series of "real time" property that an Object
 *   or a POA created on a RT-ORB can have.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef RT_PROPERTIES_H_
#define RT_PROPERTIES_H_

#include "tao/RTCORBA/RTCORBA.h"
#include <memory>

class RT_Properties
{
public:
  // -- Ctor/Dtor --
  RT_Properties () = default;
  ~RT_Properties () = default;

  static std::unique_ptr<RT_Properties> create ();
  static std::unique_ptr<RT_Properties> read_from (ACE_TString const &file_name);

  // -- Accessor Methods --
  void priority (RTCORBA::Priority priority);
  RTCORBA::Priority priority () const;

  void priority_bands (RTCORBA::PriorityBands const &priority_bands);
  RTCORBA::PriorityBands const &priority_bands () const;

  void ior_source (ACE_TString const &source);
  ACE_TString const &ior_source () const;

private:
  RTCORBA::Priority priority_ {10};
  RTCORBA::PriorityBands priority_bands_;
  ACE_TString ior_source_ {ACE_TEXT ("poa_default.ior")};
};

#endif /* RT_PROPERTIES_H_ */
