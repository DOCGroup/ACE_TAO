// $Id$

#include "ace/Read_Buffer.h"

int
get_priority_bands (const char *bands_file,
                    RTCORBA::RTORB_ptr rt_orb,
                    CORBA::PolicyList &policies,
                    CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Read bands from a file.
  //
  FILE* file =
    ACE_OS::fopen (bands_file, "r");

  if (file == 0)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  // Check for empty bands file.
  if (string == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nNo bands set!\n\n"));
      return 0;
    }

  CORBA::ULong bands_length =
    (reader.replaced () + 1) / 2;

  RTCORBA::PriorityBands bands;
  bands.length (bands_length);

  ACE_DEBUG ((LM_DEBUG,
              "\nThere are %d bands: ",
              bands_length));

  int result = 1;
  char* working_string = string;
  for (CORBA::ULong i = 0; i < bands_length; ++i)
    {
      result = ::sscanf (working_string,
                         "%hd",
                         &bands[i].low);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      result = ::sscanf (working_string,
                         "%hd",
                         &bands[i].high);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      ACE_DEBUG ((LM_DEBUG,
                  "[%d %d] ",
                  bands[i].low,
                  bands[i].high));
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              "\n\n"));

  CORBA::Policy_var banded_connection_policy =
    rt_orb->create_priority_banded_connection_policy (bands,
                                                      ACE_TRY_ENV);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    banded_connection_policy;

  return 0;
}

int
get_priority_lanes (const char *lanes_file,
                    RTCORBA::RTORB_ptr rt_orb,
                    CORBA::PolicyList &policies,
                    CORBA::Environment &ACE_TRY_ENV)
{
  //
  // Read lanes from a file.
  //
  FILE* file =
    ACE_OS::fopen (lanes_file, "r");

  if (file == 0)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  // Check for empty lanes file.
  if (string == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nNo lanes set!\n\n"));
      return 0;
    }

  CORBA::ULong lanes_length =
    reader.replaced () + 1;

  RTCORBA::ThreadpoolLanes lanes;
  lanes.length (lanes_length);

  ACE_DEBUG ((LM_DEBUG,
              "\nThere are %d lanes: ",
              lanes_length));

  int result = 1;
  char* working_string = string;
  for (CORBA::ULong i = 0; i < lanes_length; ++i)
    {
      result = ::sscanf (working_string,
                         "%hd",
                         &lanes[i].lane_priority);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      lanes[i].static_threads = static_threads;
      lanes[i].dynamic_threads = dynamic_threads;

      ACE_DEBUG ((LM_DEBUG,
                  "[%d] ",
                  lanes[i].lane_priority));
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              "\n\n"));

  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool_with_lanes (stacksize,
                                          lanes,
                                          allow_borrowing,
                                          allow_request_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size,
                                          ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  return 0;
}
