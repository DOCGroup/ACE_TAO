// $Id$

#include "ace/Read_Buffer.h"
#include "ace/Array_Base.h"

typedef ACE_Array_Base<CORBA::Short> Short_Array;

int
get_priority_bands (const char *test_type,
                    const char *bands_file,
                    RTCORBA::RTORB_ptr rt_orb,
                    CORBA::PolicyList &policies
                    TAO_ENV_ARG_DECL)
{
  //
  // Read bands from a file.
  //
  FILE* file =
    ACE_OS::fopen (bands_file, "r");

  if (file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s\n",
                       bands_file),
                      -1);

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
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Parsing error in file %s\n",
                       bands_file),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "\n\n"));

  CORBA::Policy_var banded_connection_policy =
    rt_orb->create_priority_banded_connection_policy (bands
                                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    banded_connection_policy;

  return 0;
}

int
get_values (const char *test_type,
            const char *file_name,
            const char *name,
            Short_Array &values)
{
  //
  // Read lanes from a file.
  //
  FILE* file =
    ACE_OS::fopen (file_name, "r");

  if (file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open file %s\n",
                       file_name),
                      -1);

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

  values.size (length);

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
                         &values[i]);
      if (result == 0 || result == EOF)
        break;

      working_string += ACE_OS::strlen (working_string);
      working_string += 1;

      ACE_DEBUG ((LM_DEBUG,
                  "[%d] ",
                  values[i]));
    }

  reader.alloc ()->free (string);

  if (result == 0 || result == EOF)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Parsing error in file %s\n",
                       file_name),
                      -1);

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
                    CORBA::PolicyList &policies
                    TAO_ENV_ARG_DECL)
{
  Short_Array priorities;
  int result =
    get_values (test_type,
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
                                          max_request_buffer_size
                                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Policy_var threadpool_policy =
    rt_orb->create_threadpool_policy (threadpool_id
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    threadpool_policy;

  return 0;
}

void
get_auto_priority_lanes_and_bands (CORBA::ULong number_of_lanes,
                                   RTCORBA::RTORB_ptr rt_orb,
                                   CORBA::ULong stacksize,
                                   CORBA::ULong static_threads,
                                   CORBA::ULong dynamic_threads,
                                   CORBA::Boolean allow_request_buffering,
                                   CORBA::ULong max_buffered_requests,
                                   CORBA::ULong max_request_buffer_size,
                                   CORBA::Boolean allow_borrowing,
                                   CORBA::PolicyList &policies
                                   TAO_ENV_ARG_DECL)
{
  RTCORBA::ThreadpoolLanes lanes;
  lanes.length (number_of_lanes);

  RTCORBA::PriorityBands bands;
  bands.length (number_of_lanes);

  CORBA::Short priority_range =
    RTCORBA::maxPriority - RTCORBA::minPriority;

  ACE_DEBUG ((LM_DEBUG,
              "\nUsing %d lanes\n",
              number_of_lanes));

  for (CORBA::ULong i = 0;
       i < number_of_lanes;
       ++i)
    {
      CORBA::Short high_priority =
        CORBA::Short (
                      ACE_OS::floor ((priority_range /
                                      double (number_of_lanes)) *
                                     (i + 1)));

      CORBA::Short low_priority =
        CORBA::Short (
                      ACE_OS::ceil ((priority_range /
                                     double (number_of_lanes)) *
                                    i));

      lanes[i].lane_priority = high_priority;
      lanes[i].static_threads = static_threads;
      lanes[i].dynamic_threads = dynamic_threads;

      bands[i].high = high_priority;
      bands[i].low = low_priority;

      ACE_DEBUG ((LM_DEBUG,
                  "%d: [%d %d] ",
                  i + 1,
                  low_priority,
                  high_priority));
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n\n"));

  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool_with_lanes (stacksize,
                                          lanes,
                                          allow_borrowing,
                                          allow_request_buffering,
                                          max_buffered_requests,
                                          max_request_buffer_size
                                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    rt_orb->create_priority_banded_connection_policy (bands
                                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policies.length (policies.length () + 1);
  policies[policies.length () - 1] =
    rt_orb->create_threadpool_policy (threadpool_id
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<CORBA::Short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array_Base<CORBA::Short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
