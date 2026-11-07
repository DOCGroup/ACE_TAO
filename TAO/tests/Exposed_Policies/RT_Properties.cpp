#include "RT_Properties.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_stdio.h"
#include <cstdio>
#include <cstring>
#include <new>

std::unique_ptr<RT_Properties>
RT_Properties::create ()
{
  try
    {
      return std::make_unique<RT_Properties> ();
    }
  catch (std::bad_alloc const &)
    {
      throw CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO);
    }
}

std::unique_ptr<RT_Properties>
RT_Properties::read_from (ACE_TString const &file_name)
{
  std::unique_ptr<FILE, int (*) (FILE *)> file (
    ACE_OS::fopen (file_name.c_str (), ACE_TEXT ("r")), ACE_OS::fclose);
  if (!file)
    throw CORBA::BAD_PARAM (TAO::VMCID, CORBA::COMPLETED_NO);

  std::unique_ptr<RT_Properties> rt_properties = RT_Properties::create ();

  // @@ Angelo: what if the length is more than 255?
  char string_field[256] {};
  int int_field {};
  CORBA::ULong i {};
  int n_matched {};

  while (std::fscanf (file.get (), "%255s", string_field) != EOF)
    {
      if (std::strcmp (string_field, "Priority") == 0)
        {
          n_matched = std::fscanf (file.get (), "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority (int_field);
        }
      else if (std::strcmp (string_field, "Priority_Bands") == 0)
        {
          n_matched = std::fscanf (file.get (), "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority_bands_.length (int_field);
        }
      else if (std::strcmp (string_field, "Priority_Range") == 0)
        {
          n_matched = std::fscanf (file.get (), "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority_bands_[i].low = int_field;

          n_matched = std::fscanf (file.get (), "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority_bands_[i].high = int_field;

          ++i;
        }
    }

  return rt_properties;
}

void
RT_Properties::priority (RTCORBA::Priority priority)
{
  this->priority_ = priority;
}

RTCORBA::Priority
RT_Properties::priority () const
{
  return this->priority_;
}

void
RT_Properties::priority_bands (RTCORBA::PriorityBands const &priority_bands)
{
  this->priority_bands_ = priority_bands;
}

RTCORBA::PriorityBands const &
RT_Properties::priority_bands () const
{
  return this->priority_bands_;
}

void
RT_Properties::ior_source (ACE_TString const &source)
{
  this->ior_source_ = source;
}

ACE_TString const &
RT_Properties::ior_source () const
{
  return this->ior_source_;
}
