//$Id$

#include "RT_Properties.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

RT_Properties::RT_Properties (void)
  : priority_ (10)
{
  ior_source_ = ACE_TEXT("poa_default.ior");
}

RT_Properties::~RT_Properties (void)
{
  // No-Op.
}

RT_Properties *
RT_Properties::read_from (const ACE_TCHAR *file_name)
{
  FILE *fp = ACE_OS::fopen (file_name, "r");

  RT_Properties *rt_properties = 0;

  ACE_NEW_THROW_EX (rt_properties,
                    RT_Properties,
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  // @@ Angelo: what if the length is more than 255?
  char string_field[256];
  int int_field;
  unsigned int i = 0;
  int n_matched;

  while (fscanf (fp, "%s", string_field) != EOF )
    {
      if (ACE_OS::strcmp (string_field, "Priority") == 0)
        {
          n_matched = fscanf (fp, "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority (int_field);
        }
      else if (ACE_OS::strcmp (string_field, "Priority_Bands") == 0)
        {
          n_matched = fscanf (fp, "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority_bands_.length (int_field);

        }
      else if (ACE_OS::strcmp (string_field, "Priority_Range") == 0)
        {
          n_matched = fscanf (fp, "%d", &int_field);
          ACE_UNUSED_ARG (n_matched);

          rt_properties->priority_bands_[i].low = int_field;

          n_matched = fscanf (fp, "%d", &int_field);
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
RT_Properties::priority (void)
{
  return this->priority_;
}

void
RT_Properties::priority_bands (const RTCORBA::PriorityBands& priority_bands)
{
  this->priority_bands_ = priority_bands;
}

const RTCORBA::PriorityBands&
RT_Properties::priority_bands (void)
{
  return this->priority_bands_;
}

void
RT_Properties::ior_source (const ACE_TCHAR *s)
{
  this->ior_source_ = s;
}

const ACE_TCHAR *
RT_Properties::ior_source (void)
{
  return this->ior_source_.c_str ();
}
