//$Id$

#include "RT_Properties.h"

ACE_RCSID (ExposedPolicies, RT_Properties, "$Id$")

RT_Properties::RT_Properties (void)
  : priority_ (10)
{
  ACE_OS_String::strcpy (ior_source_, "poa_default.ior");
}

RT_Properties::~RT_Properties (void)
{
  // No-Op.
}

RT_Properties *
RT_Properties::read_from (const char *file_name
                          TAO_ENV_ARG_DECL)
{
  FILE *fp = ACE_OS::fopen (file_name, "r");

  RT_Properties *rt_properties;

  ACE_NEW_THROW_EX (rt_properties,
                    RT_Properties,
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));

  // @@ Angelo: what if the length is more than 255?
  char string_field[256];
  int int_field;
  unsigned int i = 0;
  while (fscanf (fp, "%s", string_field) != EOF )
    {
      if (ACE_OS_String::strcmp (string_field, "Priority") == 0)
        {
          fscanf (fp, "%d", &int_field);
          rt_properties->priority (int_field);
        }
      else if (ACE_OS_String::strcmp (string_field, "Priority_Bands") == 0)
        {
          fscanf (fp, "%d", &int_field);
          rt_properties->priority_bands_.length (int_field);

        }
      else if (ACE_OS_String::strcmp (string_field, "Priority_Range") == 0)
        {
          fscanf (fp, "%d", &int_field);
          rt_properties->priority_bands_[i].low = int_field;

          fscanf (fp, "%d", &int_field);
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
RT_Properties::ior_source (const char *s)
{
  // @@ Angelo: please use strncpy() for strings like this, otherwise
  // you could blow the buffer limits!
  ACE_OS_String::strcpy (this->ior_source_, s);
}

const char *
RT_Properties::ior_source (void)
{
  return this->ior_source_;
}
