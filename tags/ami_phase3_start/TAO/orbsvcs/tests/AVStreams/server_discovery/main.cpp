// ========================================================================
// $Id$
// 
// = FILENAME
//    main.cpp
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================
#include <jni.h>
#include "ace/OS.h"

ACE_RCSID(server_discovery, main, "$Id$")

#define PATH_SEPARATOR ':'
#define USER_CLASSPATH "." /* where Prog.class is */

#include "Trader_Client.h"

int 
main (int argc, char** argv)
{
  JNIEnv *env;
  JavaVM *jvm;
  JDK1_1InitArgs vm_args;
  jint res;
  jclass cls;
  jmethodID mid;
  jstring jstr;
  jobjectArray args;
  char classpath[1024];

  /* IMPORTANT: specify vm_args version # if you use JDK1.1.2 and beyond */
  vm_args.version = 0x00010001;
  
  JNI_GetDefaultJavaVMInitArgs (&vm_args);
  
  /* Append USER_CLASSPATH to the end of default system class path */
  sprintf(classpath, "%s%c%s",
          vm_args.classpath, PATH_SEPARATOR, USER_CLASSPATH);
  vm_args.classpath = classpath;

  ACE_DEBUG ((LM_DEBUG, "Classpath: %s\n", classpath));
  
  /* Create the Java VM */
  res = JNI_CreateJavaVM (&jvm, (void**) &env, (void*) &vm_args);
  if (res < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Can't create Java VM\n"), -1);
  
  cls = env->FindClass ("Server_Discovery");
  if (cls == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Can't find Server Discovery class\n"), -1);
  
  mid = env->GetStaticMethodID(cls, "main", "([Ljava/lang/String;)V");
  if (mid == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Can't find Server_Discovery.main\n"), -1);
  
  jstr = env->NewStringUTF (" from C!");
  if (jstr == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Out of memory\n"), -1);

  args = env->NewObjectArray(1, env->FindClass ("java/lang/String"), jstr);
  if (args == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Out of memory\n"), -1);

  Trader_Client* trader_client = Trader_Client::instance ();
  trader_client->init (argc, argv);
  trader_client->query_trader ();

  ACE_DEBUG ((LM_DEBUG, "Launching the java GUI stuff.\n"));
  env->CallStaticVoidMethod (cls, mid, args);
  
  jvm->DestroyJavaVM ();
}
