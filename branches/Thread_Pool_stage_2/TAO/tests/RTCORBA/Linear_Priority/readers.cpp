// $Id$

#include "ace/Read_Buffer.h"
#include "ace/Array_Base.h"

typedef ACE_Array_Base<CORBA::Short> Priorities;

int
get_priority_bands (const char *test_type,
                    const char *bands_file,
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
                  "\n%s: No bands set!\n\n",
                  test_type));
      return 0;
    }

  CORBA::ULong bands_length =
    (reader.replaced () + 1) / 2;

  RTCORBA::PriorityBands bands;
  bands.length (bands_length);

  ACE_DEBUG ((LM_DEBUG,
              "\n%s: There are %d bands: ",
              test_type,
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
  ACE_CHECK_RETURN (-1);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    banded_connection_policy;

  return 0;
}

int
get_priorities (const char *test_type,
                const char *file_name,
                const char *name,
                Priorities &priorities)
{
  //
  // Read lanes from a file.
  //
  FILE* file =
    ACE_OS::fopen (file_name, "r");

  if (file == 0)
    return -1;

  ACE_Read_Buffer reader (file, 1);

  char *string =
    reader.read (EOF, ' ', '\0');

  // Check for empty lanes file.
  if (string == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n%s: No %s set!\n\n",
                  test_type,
                  name));
      return 0;
    }

  size_t length =
    reader.replaced () + 1;

  priorities.size (length);

  ACE_DEBUG ((LM_DEBUG,
              "\n%s: There are %d %s: ",
              test_type,
              length,
              name));

  int result = 1;
  char* working_string = string;
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      result = ::sscanf (working_string,
                         "%hd",
                         &priorities[i]);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      ACE_DEBUG ((LM_DEBUG,
                  "[%d] ",
                  priorities[i]));
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    return -1;

  ACE_DEBUG ((LM_DEBUG,
              "\n\n"));

  return 0;
}

int
get_priority_lanes (const char *test_type,
                    const char *lanes_file,
                    RTCORBA::RTORB_ptr rt_orb,
                    CORBA::ULong stacksize,
                    CORBA::ULong static_threads,
                    CORBA::ULong dynamic_threads,
                    CORBA::Boolean allow_request_buffering,
                    CORBA::ULong max_buffered_requests,
                    CORBA::ULong max_request_buffer_size,
                    CORBA::Boolean allow_borrowing,
                    CORBA::PolicyList &policies,
                    CORBA::Environment &ACE_TRY_ENV)
{
  Priorities priorities;
  int result =
    get_priorities (test_type,
                    lanes_file,
                    "lanes",
                    priorities);
  if (result != 0 ||
      priorities.size () == 0)
    return result;

  RTCORBA::ThreadpoolLanes lanes;
  lanes.length (priorities.size ());

  for (CORBA::ULong i = 0;
       i < priorities.size ();
       ++i)
    {
      lanes[i].lane_priority = priorities[i];
      lanes[i].static_threads = static_threads;
      lanes[i].dynamic_threads = dynamic_threads;
    }

  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool_with_lanes (stacksize,
                                          lanes,
                                          allow_borrowing,
                                          allow_request_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size,
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id,
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<CORBA::Short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<CORBA::Short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
