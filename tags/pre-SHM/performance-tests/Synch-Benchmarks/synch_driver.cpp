// $Id$

// Driver program that measures the performance of synchronization
// mechanisms provided by ACE and the underlying OS.

#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "Synch_Lib/Benchmark_Base.h"

ACE_RCSID(Synch_Benchmarks, synch_driver, "$Id$")

#if defined (ACE_HAS_THREADS)
int
main (int argc, char *argv[])
{
  ACE_Service_Config::open (argc, argv);
  ACE_Service_Repository_Iterator sri (*ACE_Service_Repository::instance ());

  // Iteratively execute each service loaded in from the svc.conf
  // file.

  for (const ACE_Service_Type *sr;
       sri.next (sr) != 0; )
    {
      // This would greatly benefit from RTTI typesafe downcasting...
      const ACE_Service_Type_Impl *type = sr->type ();
      const void *obj = type->object ();
      ACE_Service_Object *so = (ACE_Service_Object *) obj;
      Benchmark_Base *bb = (Benchmark_Base *) so;

      if (bb->benchmark_type () == Benchmark_Base::METHOD)
        {
          Benchmark_Method_Base *bm = (Benchmark_Method_Base *) bb;

          ACE_DEBUG ((LM_DEBUG, "\n\nExecuting %s\n", sr->name ()));

          bm->exec (&sri);
        }
      else
        sri.advance ();
    }
  return 0;
}
#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
		     "This test requires the platform to have threads\n"), -1);
}
#endif /* ACE_HAS_THREADS */
