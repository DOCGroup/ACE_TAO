// $Id$

#include "helloC.h"
#include "ace/Read_Buffer.h"

char *ior = 0;

int
read_ior (const char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename,
                       "invalid handle"),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);

  ior = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  if (ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "failed to read ior from file\n",
                       ""),
                      -1);
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Check arguments
      if  (argc != 2)
        {
          cerr << "Usage: client IOR_file" << endl;
          return -1;
        }

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

//       if (read_ior (argv[1]) != 0)
//         return -1;

      // Destringify argv[1]
      CORBA::Object_var obj = orb->string_to_object (argv[1] ACE_ENV_ARG_PARAMETER);
      if  (CORBA::is_nil (obj.in ()))
        {
          cerr << "Nil simple reference" << endl;
          return -1;
        }

      // Narrow
      HelloHome_var hh = HelloHome::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if  (CORBA::is_nil (hh.in ()))
        {
          cerr << "Argument is not a HelloHome reference" << endl;
          return -1;
        }

      HelloWorld_var hw = hh->create ();

      if  (CORBA::is_nil (hw.in ()))
        {
          cerr << "Fail to create a HelloWorld reference" << endl;
          return -1;
        }

      CORBA::String_var name = CORBA::string_dup ("Frodo");
      CORBA::String_var hi = hw->sayhello (name
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      cout << hi.in () << endl;

      hh->remove_component (hw
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#if 0
      HelloWorld_var hw2 = hh->create ();

      if  (CORBA::is_nil (hw2.in ()))
        {
          cerr << "Fail to create a HelloWorld reference" << endl;
          return -1;
        }

      hi = hw->sayhello (name
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
