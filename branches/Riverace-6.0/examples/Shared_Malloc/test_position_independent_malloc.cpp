// $Id$

// Test the capability of the "position-independent" <ACE_Malloc> to
// handle a single malloc that can be rooted at different base
// addresses each time it's used.  The actual backing store used by
// <ACE_Malloc> is located in a memory-mapped file.

#include "test_position_independent_malloc.h"
#include "ace/PI_Malloc.h"
#include "ace/Based_Pointer_T.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Process_Mutex.h"
#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"



#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
typedef ACE_PI_Control_Block CONTROL_BLOCK;
#else
typedef ACE_Control_Block CONTROL_BLOCK;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

typedef ACE_Malloc_T <ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, CONTROL_BLOCK> TEST_MALLOC;

// Default address for memory-mapped files.
static void *base_addr = ACE_DEFAULT_BASE_ADDR;

static void
print (Test_Data *data)
{
  for (Test_Data *t = data; t != 0; t = t->next_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<<<<\ni1_ = %d, i2_ = %d, i3_ = %d\n",
                  t->i1_,
                  t->i2_,
                  t->i3_));
      ACE_DEBUG ((LM_DEBUG,
                  "*t->bpl_ = %d, t->long_test_->array_[0] = %d\n>>>>\n",
                  *t->long_test_->bpl_,
                  t->long_test_->array_[0]));
    }
}

static void *
initialize (TEST_MALLOC *allocator)
{
  void *ptr;
  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Test_Data)),
                        0);
  Test_Data *data1 = new (ptr) Test_Data;

  data1->i1_ = 111;
  data1->i2_ = 222;
  data1->i3_ = 333;

  void *gap = 0;
  ACE_ALLOCATOR_RETURN (gap,
                        allocator->malloc (sizeof (256)),
                        0);

  allocator->free (gap);

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Test_Data)),
                        0);
  Test_Data *data2 = new (ptr) Test_Data;

  data1->next_ = 0;
  data1->i1_ = 111;
  data1->i2_ = 222;
  data1->i3_ = 333;
  data2->next_ = data1;
  data2->i1_ = -111;
  data2->i2_ = -222;
  data2->i3_ = -333;

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

  data1->long_test_= lt;

  ACE_ASSERT (*lt->bpl_ == 1000);
  ACE_ASSERT (lt->bpl_[3] == 1003);

  ACE_ALLOCATOR_RETURN (ptr,
                        allocator->malloc (sizeof (Long_Test)),
                        0);
  lt = new (ptr) Long_Test;

  lt->array_[0] = 2000;
  lt->array_[1] = 2001;
  lt->array_[2] = 2002;
  lt->array_[3] = 2003;
  lt->array_[4] = 2004;
  lt->bpl_ = lt->array_;

  data2->long_test_= lt;

  ACE_ASSERT (*lt->bpl_ == 2000);
  ACE_ASSERT (lt->bpl_[4] == 2004);

  return data2;
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("a:T"));

  for (int c;
       (c = get_opt ()) != -1;
       )
    {
      switch (c)
        {
        case 'a':
          // Override the default base address.
          base_addr = reinterpret_cast<void *> (static_cast<intptr_t> (ACE_OS::atoi (get_opt.opt_arg ())));
          break;
        case 'T':
#if defined (ACE_HAS_TRACE)
          ACE_Trace::start_tracing ();
#endif /* ACE_HAS_TRACE */
          break;
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  ACE_MMAP_Memory_Pool_Options options (base_addr);

  // Create an allocator.
  TEST_MALLOC *ptr = 0;
  ACE_NEW_RETURN (ptr,
                  TEST_MALLOC (ACE_TEXT("test_file"),
                               ACE_TEXT("test_lock"),
                               &options),
                  1);
  auto_ptr <TEST_MALLOC> allocator (ptr);
  void *data = 0;

  // This is the first time in, so we allocate the memory and bind it
  // to the name "foo".
  if (allocator->find ("foo",
                       data) == -1)
    {
      data = initialize (allocator.get ());

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
      print ((Test_Data *) data);
      allocator->free (data);
      allocator->remove ();
      ACE_DEBUG ((LM_DEBUG,
                  "all resources released\n"));
    }

  return 0;
}

