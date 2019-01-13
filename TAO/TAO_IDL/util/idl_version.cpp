/**
 * Source File for Tracking IDL Versions
 *
 * Header File Counterpart is include/idl_version.h
 */

#include "ace/OS_NS_string.h"
#include "idl_version.h"

static const char * idlVersionNames[IDL_VERSION_COUNT] = {
  "Invalid",
  "3",
  "4"
};

IdlVersion::IdlVersion () : version_(DEFAULT_DEFAULT_IDL_VERSION)
{
}

IdlVersion::IdlVersion (SpecificIdlVersion version)
{
  this->version (version);
}

SpecificIdlVersion IdlVersion::version () const
{
  return version_;
}

void IdlVersion::version (SpecificIdlVersion version)
{
  version_ = version < IDL_VERSION_COUNT ? version : IDL_VERSION_INVALID;
}

bool IdlVersion::is_valid () const
{
  return version_ != IDL_VERSION_INVALID;
}

const char * IdlVersion::to_string () const
{
  return idlVersionNames[version_];
}

void IdlVersion::from_string (const char * version)
{
  if (version)
    {
      for (int i = 0; i < IDL_VERSION_COUNT; i++)
        {
          if (!ACE_OS::strcmp (version, idlVersionNames[i]))
            {
              version_ = static_cast<SpecificIdlVersion> (i);
              return;
            }
        }
    }
  version_ = IDL_VERSION_INVALID;
}

IdlVersion::operator SpecificIdlVersion () const
{
  return version_;
}

bool IdlVersion::operator== (SpecificIdlVersion other) const
{
  return version_ == other;
}

bool IdlVersion::operator!= (SpecificIdlVersion other) const
{
  return version_ != other;
}

bool IdlVersion::operator> (SpecificIdlVersion other) const
{
  return version_ > other;
}

bool IdlVersion::operator>= (SpecificIdlVersion other) const
{
  return version_ >= other;
}

bool IdlVersion::operator< (SpecificIdlVersion other) const
{
  return version_ < other;
}

bool IdlVersion::operator<= (SpecificIdlVersion other) const
{
  return version_ <= other;
}

