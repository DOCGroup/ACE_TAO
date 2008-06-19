+// $Id$
+
+#include "TestC.h"
+#include "ace/Get_Opt.h"
+#include "tao/ZIOP/ZIOP.h"
+#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
+
+ACE_RCSID(Hello, client, "$Id$")
+
+const char *ior = "file://test.ior";
+
+int
+parse_args (int argc, char *argv[])
+{
+  ACE_Get_Opt get_opts (argc, argv, "k:");
+  int c;
+
+  while ((c = get_opts ()) != -1)
+    switch (c)
+      {
+      case 'k':
+        ior = get_opts.opt_arg ();
+        break;
+
+      case '?':
+      default:
+        ACE_ERROR_RETURN ((LM_ERROR,
+                           "usage:  %s "
+                           "-k <ior> "
+                           "\n",
+                           argv [0]),
+                          -1);
+      }
+  // Indicates sucessful parsing of the command line
+  return 0;
+}
+
+int
+ACE_TMAIN(int argc, ACE_TCHAR *argv[])
+{
+  try
+    {
+      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
+
+      if (parse_args (argc, argv) != 0)
+        return 1;
+
+      CORBA::Object_var compression_manager =
+        orb->resolve_initial_references("CompressionManager");
+
+      Compression::CompressionManager_var manager =
+        Compression::CompressionManager::_narrow (compression_manager.in ());
+
+      if (CORBA::is_nil(manager.in ()))
+        ACE_ERROR_RETURN ((LM_ERROR,
+                           " (%P|%t) Panic: nil compression manager\n"),
+                          1);
+
+      Compression::CompressorFactory_ptr compressor_factory;
+
+      ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);
+
+      Compression::CompressorFactory_var compr_fact = compressor_factory;
+      manager->register_factory(compr_fact.in ());
+
+      CORBA::Object_var tmp = orb->string_to_object(ior);
+
+      Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());
+
+      if (CORBA::is_nil (hello.in ()))
+        {
+          ACE_ERROR_RETURN ((LM_DEBUG,
+                             "Nil Test::Hello reference <%s>\n",
+                             ior),
+                            1);
+        }
+
+      CORBA::String_var the_string = hello->get_string ("This is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test string\n");
+
+      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
+                  the_string.in ()));
+
+      hello->shutdown ();
+
+      orb->destroy ();
+    }
+  catch (const CORBA::Exception& ex)
+    {
+      ex._tao_print_exception ("Exception caught:");
+      return 1;
+    }
+
+  return 0;
+}

