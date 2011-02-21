// $Id$

// Test the persistence capabilities of <ACE_Malloc> when configured
// for mmap-based shared memory management.

// This examples uses scanf to read user inputs from stdin into fixed
// sized buffers.  This may cause buffers to overrun.

#include "ace/OS_NS_string.h"
#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Null_Mutex.h"

typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> TEST_MALLOC;
typedef ACE_Malloc_LIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC_LIFO_ITERATOR;
typedef ACE_Malloc_FIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC_FIFO_ITERATOR;

// Shared memory manager.
static TEST_MALLOC *shmem_allocator = 0;

// Backing store name.
static ACE_TCHAR backing_store[MAXPATHLEN + 1] = ACE_TEXT ("");

class Employee
{
public:
  Employee (void): name_ (0), id_ (0) {}

  Employee (const char *name, u_long id) : id_ (id)
  {
    size_t len = ACE_OS::strlen (name) + 1;
    this->name_ = reinterpret_cast<char *> (shmem_allocator->malloc (len));
    ACE_OS::strcpy (this->name_, name);
  }

  ~Employee (void) { shmem_allocator->free (this->name_); }

  const char *name (void) const { return this->name_; }

  void name (const char *name)
  {
    if (this->name_)
      shmem_allocator->free (this->name_);

    size_t len = ACE_OS::strlen (name) + 1;
    this->name_ = reinterpret_cast<char *> (shmem_allocator->malloc (len));
    ACE_OS::strcpy (this->name_, name);
  }

  u_long id (void) const { return id_; }

  void id (u_long id) { id_ = id; }

  void *operator new (size_t)
  {
    return shmem_allocator->malloc (sizeof (Employee));
  }

#if defined (ACE_HAS_NEW_NOTHROW)
  void *operator new (size_t, const ACE_nothrow_t&)
  {
    return shmem_allocator->malloc (sizeof (Employee));
  }
#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
  void operator delete (void *p, const ACE_nothrow_t&) throw ()
  {
    shmem_allocator->free (p);
  }
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */
#endif

  void operator delete (void *pointer)
  {
    shmem_allocator->free (pointer);
  }

private:
  char *name_;
  // Employee name.

  u_long id_;
  // Employee ID.
};

class GUI_Handler
{
public:
  GUI_Handler (void) { menu (); }

  ~GUI_Handler (void)
  {
    TEST_MALLOC::MEMORY_POOL &pool =
      shmem_allocator->memory_pool ();
    pool.sync ();
  }

  int service(void)
  {
    char option[BUFSIZ];
    char buf1[BUFSIZ];
    char buf2[BUFSIZ];

    if (::scanf ("%s", option) <= 0)
      {
        ACE_ERROR ((LM_ERROR,
                    "try again\n"));
        return 0;
      }

    int result = 0;
    switch (option[0])
      {
      case 'I' :
      case 'i' :
        if (::scanf ("%s %s", buf1, buf2) <= 0)
          break;
        result = insert_employee (buf1,
                                  ACE_OS::atoi (buf2));
        break;
      case 'F' :
      case 'f' :
        if (::scanf ("%s", buf1) <= 0)
          break;
        result = find_employee (buf1);
        break;
      case 'D' :
      case 'd' :
        if (::scanf ("%s", buf1) <= 0)
          break;
        result = delete_employee (buf1);
        break;
      case 'L' :
      case 'l' :
        result = list_employees ();
        break;
      case 'Q' :
      case 'q' :
        return -1;
        ACE_NOTREACHED(break);
      default :
        cout << "unrecognized command" << endl;
      }
    if (result == 0)
      cout << "Last operation was successful!!" << endl;
    else
      cout << "Last operation failed!! " << endl;

    menu ();

    return 0;
  }

  void menu(void)
  {
    cout << endl;
    cout << "\t**************************                                  " << endl;
    cout << "\tThe employee database menu                                  " << endl;
    cout << endl;
    cout << "\t<I> Insert    <name>   <id>                                 " << endl;
    cout << "\t<D> Delete    <name>                                        " << endl;
    cout << "\t<F> Find      <name>                                        " << endl;
    cout << endl;
    cout << "\t<L> List all employees                                      " << endl;
    cout << endl;
    cout << "\t<Q> Quit " << endl;
    cout << "\t**************************                                  " << endl;
  }

private:
  int insert_employee (const char *name,
                       u_long id);
  int find_employee (const char *name);
  int list_employees (void);
  int delete_employee (const char *name);
};

int
GUI_Handler::insert_employee (const char *name,
                              u_long id)
{
  if (find_employee (name) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Employee already exists\n"),
                      -1);

  Employee *new_employee = 0;

  ACE_NEW_RETURN (new_employee,
                  Employee (name, id),
                  -1);

  if (shmem_allocator->bind (name,
                             new_employee) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "bind failed\n"),
                      -1);
  return 0;
}

int
GUI_Handler::find_employee (const char *name)
{
  void *temp = 0;

  if (shmem_allocator->find (name,
                             temp) == 0)
    {
      Employee *employee = reinterpret_cast<Employee *> (temp);

      ACE_DEBUG ((LM_DEBUG,
                  "The following employee was found.......\n\n"));
      ACE_DEBUG ((LM_DEBUG,
                  "Employee name: %s\nEmployee id:   %d\n",
                  employee->name (),
                  employee->id ()));
      return 0;
    }

  return -1;
}

int
GUI_Handler::list_employees (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "The following employees were found.......\n\n"));

  {
    ACE_DEBUG ((LM_DEBUG,
                "LIFO order:\n"));
    MALLOC_LIFO_ITERATOR iterator (*shmem_allocator);

    for (void *temp = 0;
         iterator.next (temp) != 0;
         iterator.advance ())
      {
        Employee *employee = reinterpret_cast<Employee *> (temp);
        ACE_DEBUG ((LM_DEBUG,
                    "Employee name: %s\nEmployee id:   %d\n",
                    employee->name (),
                    employee->id ()));
      }
  }

  {
    ACE_DEBUG ((LM_DEBUG,
                "FIFO order:\n"));
    MALLOC_FIFO_ITERATOR iterator (*shmem_allocator);

    for (void *temp = 0;
         iterator.next (temp) != 0;
         iterator.advance ())
      {
        Employee *employee = reinterpret_cast<Employee *> (temp);
        ACE_DEBUG ((LM_DEBUG,
                    "Employee name: %s\nEmployee id:   %d\n",
                    employee->name (),
                    employee->id ()));
      }
  }
  return 0;
}

int
GUI_Handler::delete_employee (const char *name)
{
  void *temp = 0;

  if (shmem_allocator->unbind (name,
                               temp) == 0)
    {
      Employee *employee = reinterpret_cast<Employee *> (temp);

      ACE_DEBUG ((LM_DEBUG,
                  "The following employee was found and deleted.......\n\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Employee name: %s\nEmployee id:   %d\n",
                  employee->name (),
                  employee->id ()));

      delete employee;
      return 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              "There is no employee with name %s",
              name));
  return -1;
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)
    ACE_OS::strcpy (backing_store, argv[1]);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  if (ACE_OS::strcmp (backing_store, ACE_TEXT ("")) == 0)
    {
#if defined (ACE_DEFAULT_BACKING_STORE)
      // Create a temporary file.
      ACE_OS::strcpy (backing_store,
                      ACE_DEFAULT_BACKING_STORE);
#else /* ACE_DEFAULT_BACKING_STORE */
      if (ACE::get_temp_dir (backing_store,
                             MAXPATHLEN - 17) == -1) // -17 for ace-malloc-XXXXXX
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Temporary path too long, ")
                      ACE_TEXT ("defaulting to current directory\n")));
          backing_store[0] = 0;
        }

      // Add the filename to the end
      ACE_OS::strcat (backing_store, ACE_TEXT ("ace-malloc-XXXXXX"));

#endif /* ACE_DEFAULT_BACKING_STORE */
    }

  ACE_NEW_RETURN (shmem_allocator,
                  TEST_MALLOC (backing_store),
                  -1);

  GUI_Handler handler;

  for (;;)
    if (handler.service () == -1)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("closing down ....\n")));
        break;
      }

  return 0;
}

