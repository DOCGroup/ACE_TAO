#ifndef IDL_VERSION_HEADER
#define IDL_VERSION_HEADER

#include "TAO_IDL_FE_Export.h"

enum TAO_IDL_FE_Export SpecificIdlVersion
{
  IDL_VERSION_INVALID,
  IDL_VERSION_3,
  IDL_VERSION_4_0,
  IDL_VERSION_COUNT
};

const SpecificIdlVersion DEFAULT_IDL_VERSION = IDL_VERSION_4_0;

class TAO_IDL_FE_Export IdlVersion
{
public:
  IdlVersion ();
  IdlVersion (SpecificIdlVersion version);

  SpecificIdlVersion version () const;
  void version (SpecificIdlVersion version);

  bool is_valid () const;
  
  const char * to_string () const;
  void from_string (const char * version);

  bool operator== (SpecificIdlVersion version) const;
  bool operator!= (SpecificIdlVersion version) const;
  bool operator> (SpecificIdlVersion version) const;
  bool operator>= (SpecificIdlVersion version) const;
  bool operator< (SpecificIdlVersion version) const;
  bool operator<= (SpecificIdlVersion version) const;

private:
  SpecificIdlVersion version_;
};

#endif
