// $Id$

// Test the capability of <ACE_Malloc> to handle a single malloc that
// can be rooted at different base addresses each time it's used.

#include "ace/Malloc.h"
#include "ace/Based_Pointer_T.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "test_position_independent_malloc.h"

ACE_RCSID(Shared_Malloc, test_multiple_mallocs, "$Id$")

typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex> MALLOC; 

// Default address for memory-mapped files.
static void *base_addr = ACE_DEFAULT_BASE_ADDR;

#if 0
// Some dummy data 
struct Dummy_Data
{
  int i1_;
  int i2_;
  int i3_;
  // ACE_Based_Pointer<Dummy_Data> next_;
};

struct Long_Test 
{
  ACE_Based_Pointer<long> bpl_;
  long array_[10];
};
#endif /* 0 */

static void
print (Dummy_Data *data)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "<<<<\ni1_ = %d, i2_ = %d, i3_ = %d\n",
              data->i1_, 
              data->i2_,
              data->i3_));

  ACE_DEBUG ((LM_DEBUG,
              "i1_ = %d, i2_ = %d, i3_ = %d\n>>>>\n",
              data->next_->i1_, 
              data->next_->i2_,
              data->next_->i3_));
#endif 
}

static void *
initialize (MALLOC *allocator)
{
  void *ptr;
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Dummy_Data)),
                        0);
  Dummy_Data *data1 = new (ptr) Dummy_Data;

  data1->i1_ = 111;
  data1->i2_ = 222;
  data1->i3_ = 333;

  void *gap = 0;
  ACE_ALLOCATOR_RETURN (gap,
                        allocator->malloc (sizeof (256)),
                        0);

  allocator->free (gap);

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Dummy_Data)),
                        0);
  Dummy_Data *data2 = new (ptr) Dummy_Data;

#if 0
  data1->next_ = data2;
  data1->next_->i1_ = 111;
  data1->next_->i2_ = 222;
  data1->next_->i3_ = 333;
  data2->next_ = data1;
  data2->next_->i1_ = -111;
  data2->next_->i2_ = -222;
  data2->next_->i3_ = -333;

  // Test in shared memory using long (array/pointer)
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Long_Test)),
                        0);
  Long_Test *lt = new (ptr) Long_Test;

  lt->array_[0] = 1000;
  lt->array_[1] = 1001;
  lt->array_[2] = 1002;
  lt->array_[3] = 1003;
  lt->array_[4] = 1004;
  lt->bpl_ = lt->array_;

  long longCont1 = *lt->bpl_;
  long longCont3 = lt->bpl_[3];

  // Test in local memory using long (array/pointer)
  ACE_NEW_RETURN (ptr,
                  long[5],
                  0);
  longTest *lt_lcl = new (ptr) Long_Test;

  lt_lcl->array_[0] = 2000;
  lt_lcl->array_[1] = 2001;
  lt_lcl->array_[2] = 2002;
  lt_lcl->array_[3] = 2003;
  lt_lcl->array_[4] = 2004;
  lt_lcl->bpl_ = lt_lcl->array_;

  long longCont_lcl1 = *lt_lcl->bpl_;
  long longCont_lcl4 = lt_lcl->bpl_[4];
#endif /* 0 */

  return data1;
}

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "a:T");

  for (int c;
       (c = get_opt ()) != -1;
       )
    {
      switch (c)
	{
	case 'a':
          // Override the default base address.
          base_addr = (void *) ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'T':
          ACE_Trace::start_tracing ();
	  break;
	}
    }
}

int 
main (int argc, char *argv[])
{
  parse_args (argc, argv);

  ACE_MMAP_Memory_Pool_Options options (base_addr);

  // Create an allocator.
  MALLOC *allocator;
  ACE_NEW_RETURN (allocator,
                  MALLOC ("dummy_file",
                          "dummy_lock",
                          &options),
                  1);
  void *data = 0; 

  // This is the first time in, so we allocate the memory and bind it
  // to the name "foo".
  if (allocator->find ("foo",
                       data) == -1)
    {
      // data = initialize (allocator);

      data = allocator->malloc (sizeof (long));
      *(long *) data = -36;

      if (allocator->bind ("foo",
                           data) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "bind"),
                          1);

      ACE_DEBUG ((LM_DEBUG,
                  "Run again to see results and release resources.\n"));
    }
  // If we find "foo" then we're running the "second" time, so we must
  // release the resources.
  else
    {
      // @@ Add a new print statement...
      // print ((Dummy_Data *) data);
      ACE_DEBUG ((LM_DEBUG, 
                  "data = %d\n",
                  *(long *) data));
      allocator->free (data);
      allocator->remove ();
      ACE_DEBUG ((LM_DEBUG,
                  "all resources released\n"));
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Based_Pointer<Dummy_Data>;
template class ACE_Based_Pointer_Basic<Dummy_Data>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Based_Pointer<Dummy_Data>
#pragma instantiate ACE_Based_Pointer_Basic<Dummy_Data>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
