// ========================================================================
// $Id$
// 
// = FILENAME
//    Server_Discovery.cpp
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "Server_0005fDiscovery_0005fUtil.h"
#include "Trader_Client.h"

// Since we're doing C-like function calls here, let's throw in some
// global variables for good measure.

//static const char* LIBRARY_NAME = "libtclient.so";
//static const char* FACTORY_NAME = "create_trader_client";
static jclass _movie_class;
static jfieldID _name_id, _filename_id, _category_id, _description_id;

// Static function declarations:
//void print_dlerror (void);

/*
 * Class:     Server_0005fDiscovery_0005fUtil
 * Method:    evaluate_performance_property
 * Signature: (Ljava/lang/String;Ljava/lang/String;)F
 */
JNIEXPORT jfloat JNICALL
Java_Server_1Discovery_1Util_evaluate_1performance_1property
(JNIEnv *env,
 jclass this_class,
 jstring java_server_name,
 jstring java_prop_name)
{
  jfloat return_value = 0.0;

  Trader_Client* trader_client = Trader_Client::instance ();
  if (trader_client != 0)
    {
      const char* server_name = env->GetStringUTFChars (java_server_name, 0);
      const char* prop_name = env->GetStringUTFChars (java_prop_name, 0);

      return_value =
        trader_client->evaluate_performance_property (server_name, prop_name);
      
      env->ReleaseStringUTFChars (java_server_name, server_name);
      env->ReleaseStringUTFChars (java_prop_name, prop_name);
    }

  return return_value;
}

/*
 * Class:     Server_0005fDiscovery_0005fUtil
 * Method:    get_movie_info
 * Signature: (Ljava/lang/String;)[LTAO_VR/Movie;
 */
JNIEXPORT jobjectArray JNICALL
Java_Server_1Discovery_1Util_get_1movie_1info__Ljava_lang_String_2
(JNIEnv * env,
 jclass this_class,
 jstring java_server_name)
{
  fprintf (stderr, "Server_Discover: get movie info.\n");
  jobjectArray return_value = 0;

  Trader_Client* trader_client = Trader_Client::instance ();
  if (trader_client != 0)
    {
      // Get a Movie_Iterator that will iterate over the
      // movie_structures for all movies of the given server.
      const char* server_name = env->GetStringUTFChars (java_server_name, 0);
      Movie_Iterator* movie_iter = trader_client->get_movie_info (server_name);

      if (movie_iter != 0)
        {
          // Allocate a new array of structures. 
          int length = movie_iter->num_movies (), i = 0;
          jobject initial_object = env->AllocObject (_movie_class);
          return_value = env->NewObjectArray (length, _movie_class, initial_object);
          env->DeleteLocalRef (initial_object);
          
          for (; movie_iter->has_more_elements (); movie_iter->advance (), i++)
            {
              // For each movie structure, convert the C++ strings
              // into java strings, and set them in the structure.
              jobject movie_struct = env->AllocObject (_movie_class);
              jstring name = env->NewStringUTF (movie_iter->name ());
              jstring filename = env->NewStringUTF (movie_iter->filename ());
              jstring category = env->NewStringUTF (movie_iter->category ());
              jstring description = env->NewStringUTF (movie_iter->description ());
              env->SetObjectField (movie_struct, _name_id, name);
              env->SetObjectField (movie_struct, _filename_id, filename);
              env->SetObjectField (movie_struct, _category_id, category);
              env->SetObjectField (movie_struct, _description_id, description);
              env->SetObjectArrayElement (return_value, i, movie_struct);
              env->DeleteLocalRef (movie_struct);
            }
          
          delete movie_iter;
        }
      
      env->ReleaseStringUTFChars (java_server_name, server_name);
    }
  
  return return_value;
}

/*
 * Class:     Server_0005fDiscovery_0005fUtil
 * Method:    get_servers
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL
Java_Server_1Discovery_1Util_get_1servers (JNIEnv *env, jclass this_class)
{
  fprintf (stderr, "Server Discovery: get servers.\n");
  jobjectArray server_names = 0;

  Trader_Client* trader_client = Trader_Client::instance ();
  if (trader_client != 0)
    {
      int length = 0;
      char** server_names_str = 0;
      trader_client->get_servers (length, server_names_str);

      if (length > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Converting to java strings.\n"));
          // Create an array of Java strings, suitable for returning
          // to the Java invoker. 
          jstring initial_string = env->NewStringUTF ("");
          jclass string_class = env->FindClass ("java/lang/String");
          jobjectArray server_names =
            env->NewObjectArray (length, string_class, initial_string);

          // Convert those C++ strings into Java Strings.
          for (int i = 0; i < length; i++)
            {
              const char* server_name = server_names_str[i];
              ACE_DEBUG ((LM_DEBUG, "Converting %s to a java string.\n", server_name));
              jstring java_server_name = env->NewStringUTF (server_name);

              ACE_DEBUG ((LM_DEBUG, "Placing %s in the return array.\n", server_name));
              env->SetObjectArrayElement (server_names, i, java_server_name);

              ACE_DEBUG ((LM_DEBUG, "Deleting a reference to the java string.\n"));
              env->DeleteLocalRef (java_server_name);
            }
        }

      ACE_DEBUG ((LM_DEBUG, "Deleting the array of C++ server name strings.\n"));
      delete [] server_names_str;
    }

  return server_names;
}

/*
 * Class:     Server_0005fDiscovery_0005fUtil
 * Method:    load_movie
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_Server_1Discovery_1Util_load_1movie (JNIEnv * env,
                                          jclass this_class,
                                          jstring java_server_name,
                                          jstring java_movie_name)
{
  Trader_Client* trader_client = Trader_Client::instance ();
  if (trader_client != 0)
    {
      const char* server_name = env->GetStringUTFChars (java_server_name, 0);
      const char* movie_name = env->GetStringUTFChars (java_movie_name, 0);
      trader_client->load_movie (server_name, movie_name);
      env->ReleaseStringUTFChars (java_server_name, server_name);
      env->ReleaseStringUTFChars (java_movie_name, movie_name);
    }
}

/*
 * Class:     Server_0005fDiscovery_0005fUtil
 * Method:    query_trader
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_Server_1Discovery_1Util_query_1trader (JNIEnv *env, jclass this_class)
{
  Trader_Client* trader_client = Trader_Client::instance ();
  if (trader_client != 0)
    trader_client->query_trader ();
}

/*
 * Class:     Server_0005fDiscovery_0005fUtil
 * Method:    init
 * Signature: ()V
 */
/*
void print_dlerror (void)
{
  const char* error_msg = ::dlerror ();
  if (error_msg != 0)
    {
      ::fprintf (stderr, "Horrors of horrors! Dynamic linking failed "
                 "for the following reason: %s\n", error_msg);
    }
  else
    ::fprintf (stderr, "Horrors of horrors! Dynamic linking failed.\n");
}

JNIEXPORT jint JNICALL
Java_Server_1Discovery_1Util_init (JNIEnv * env,
                                   jclass this_class,
                                   jobjectArray args)
{
  // Explicitly dynamically link to the Trader_Client library and
  // obtain a pointer to its factory global function.
  ::fprintf (stderr, "Attempting to open trader client library.\n");
  void* handle = ::dlopen (LIBRARY_NAME, RTLD_NOW);

  if (handle != 0)
    {
      ::fprintf (stderr, "Attempting to resolve factory function.\n");
      Trader_Client* (*factory) (void) =
        (Trader_Client* (*) (void)) ::dlsym (handle, FACTORY_NAME);

      if (factory != 0)
        {
          ::fprintf (stderr, "Invoking factory method.\n");
          _trader_client = factory ();
        }
      else
        ::print_dlerror ();
    }
  else
    ::print_dlerror ();

  int return_value = 0;
  if (_trader_client != 0)
    {
      int argc = env->GetArrayLength (args);
      char** argv = new char*[argc];
      
      // Pull the command line args into C++ format from the java format.
      for (int i = 0; i < argc; i++)
        {
          jstring arg = static_cast<jstring> (env->GetObjectArrayElement (args, i));
          argv[i] = const_cast<char*> (env->GetStringUTFChars (arg, 0));
          env->DeleteLocalRef (arg);
        }

      _trader_client->init (argc, argv);
      
      // Release the allocated command line arguments.
      for (i = 0; i < argc; i++)
        {
          jstring arg = static_cast<jstring> (env->GetObjectArrayElement (args, i));
          env->ReleaseStringUTFChars (arg, argv[i]);
          env->DeleteLocalRef (arg);
        }

      // Init some global variables
      jclass movie_class = env->FindClass ("TAO_VR/Movie");        
      _movie_class = static_cast<jclass> (env->NewGlobalRef (movie_class));
      _name_id = env->GetFieldID (movie_class, "name_", "Ljava/lang/String;");
      _filename_id = env->GetFieldID (movie_class, "filename_", "Ljava/lang/String;");
      _category_id = env->GetFieldID (movie_class, "category_", "Ljava/lang/String;");
      _description_id = env->GetFieldID (movie_class, "description_", "Ljava/lang/String;");
      
      delete [] argv;            
      return_value = 0;
    }

  return return_value = -1;
}
*/
