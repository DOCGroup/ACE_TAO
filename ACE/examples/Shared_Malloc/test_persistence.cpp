// Test the persistence capabilities of <ACE_Malloc> when configured
// for mmap-based shared memory management.

// Read and update employee records stored in an ACE_Malloc persistent pool.

#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Lib_Find.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_stdlib.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <string>

using TEST_MALLOC = ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
using MALLOC_LIFO_ITERATOR = ACE_Malloc_LIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
using MALLOC_FIFO_ITERATOR = ACE_Malloc_FIFO_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;

// Shared memory manager.
static std::unique_ptr<TEST_MALLOC> shmem_allocator;

// Backing store name.
static std::basic_string<ACE_TCHAR> backing_store;

class Employee
{
public:
  Employee () = default;

  Employee (char const *name, u_long id) : id_ (id)
  {
    size_t const len = std::strlen (name) + 1;
    this->name_ = static_cast<char *> (shmem_allocator->malloc (len));
    std::memcpy (this->name_, name, len);
  }

  Employee (Employee const &) = delete;
  Employee (Employee &&) = delete;
  Employee &operator= (Employee const &) = delete;
  Employee &operator= (Employee &&) = delete;

  ~Employee () { shmem_allocator->free (this->name_); }

  char const *name () const { return this->name_; }

  void name (char const *name)
  {
    if (this->name_ != nullptr)
      shmem_allocator->free (this->name_);

    size_t const len = std::strlen (name) + 1;
    this->name_ = static_cast<char *> (shmem_allocator->malloc (len));
    std::memcpy (this->name_, name, len);
  }

  u_long id () const { return id_; }

  void id (u_long id) { id_ = id; }

  void *operator new (size_t)
  {
    return shmem_allocator->malloc (sizeof (Employee));
  }

  void *operator new (size_t, std::nothrow_t const &)
  {
    return shmem_allocator->malloc (sizeof (Employee));
  }
  void operator delete (void *p, std::nothrow_t const &) noexcept
  {
    shmem_allocator->free (p);
  }

  void operator delete (void *pointer) noexcept
  {
    shmem_allocator->free (pointer);
  }

private:
  // This raw pointer targets text allocated inside the persistent ACE_Malloc
  // pool; std::string would store a process-local heap pointer in the record.
  char *name_ {};

  u_long id_ {};
  // Employee ID.
};

class GUI_Handler
{
public:
  GUI_Handler () { menu (); }
  GUI_Handler (GUI_Handler const &) = delete;
  GUI_Handler (GUI_Handler &&) = delete;
  GUI_Handler &operator= (GUI_Handler const &) = delete;
  GUI_Handler &operator= (GUI_Handler &&) = delete;

  ~GUI_Handler ()
  {
    TEST_MALLOC::MEMORY_POOL &pool =
      shmem_allocator->memory_pool ();
    pool.sync ();
  }

  int service ()
  {
    std::string option;

    if (!(std::cin >> option))
      {
        ACE_ERROR ((LM_ERROR,
                    "try again\n"));
        return 0;
      }

    int result {};
    switch (option[0])
      {
      case 'I' :
      case 'i' :
        {
          std::string name;
          std::string id;
          if (!(std::cin >> name >> id))
            break;
          result = insert_employee (name,
                                    std::atoi (id.c_str ()));
        }
        break;
      case 'F' :
      case 'f' :
        {
          std::string name;
          if (!(std::cin >> name))
            break;
          result = find_employee (name);
        }
        break;
      case 'D' :
      case 'd' :
        {
          std::string name;
          if (!(std::cin >> name))
            break;
          result = delete_employee (name);
        }
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
        std::cout << "unrecognized command" << std::endl;
      }
    if (result == 0)
      std::cout << "Last operation was successful!!" << std::endl;
    else
      std::cout << "Last operation failed!! " << std::endl;

    menu ();

    return 0;
  }

  void menu ()
  {
    std::cout << std::endl
              << "\t**************************                                  " << std::endl
              << "\tThe employee database menu                                  " << std::endl
              << std::endl
              << "\t<I> Insert    <name>   <id>                                 " << std::endl
              << "\t<D> Delete    <name>                                        " << std::endl
              << "\t<F> Find      <name>                                        " << std::endl
              << std::endl
              << "\t<L> List all employees                                      " << std::endl
              << std::endl
              << "\t<Q> Quit " << std::endl
              << "\t**************************                                  " << std::endl;
  }

private:
  int insert_employee (std::string const &name,
                       u_long id);
  int find_employee (std::string const &name);
  int list_employees ();
  int delete_employee (std::string const &name);
};

int
GUI_Handler::insert_employee (std::string const &name,
                              u_long id)
{
  if (find_employee (name) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Employee already exists\n"),
                      -1);

  Employee *new_employee {};

  ACE_NEW_RETURN (new_employee,
                  Employee (name.c_str (), id),
                  -1);
  std::unique_ptr<Employee> employee (new_employee);

  if (shmem_allocator->bind (name.c_str (),
                             employee.get ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "bind failed\n"),
                      -1);
  employee.release ();
  return 0;
}

int
GUI_Handler::find_employee (std::string const &name)
{
  void *temp {};

  if (shmem_allocator->find (name.c_str (),
                             temp) == 0)
    {
      // find () leaves the allocator-owned record bound in the persistent pool.
      Employee const *const employee = static_cast<Employee const *> (temp);

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
GUI_Handler::list_employees ()
{
  ACE_DEBUG ((LM_DEBUG,
              "The following employees were found.......\n\n"));

  {
    ACE_DEBUG ((LM_DEBUG,
                "LIFO order:\n"));
    MALLOC_LIFO_ITERATOR iterator (*shmem_allocator);

    for (void *temp {};
         iterator.next (temp) != 0;
         iterator.advance ())
      {
        Employee *const employee = static_cast<Employee *> (temp);
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

    for (void *temp {};
         iterator.next (temp) != 0;
         iterator.advance ())
      {
        Employee *const employee = static_cast<Employee *> (temp);
        ACE_DEBUG ((LM_DEBUG,
                    "Employee name: %s\nEmployee id:   %d\n",
                    employee->name (),
                    employee->id ()));
      }
  }
  return 0;
}

int
GUI_Handler::delete_employee (std::string const &name)
{
  void *temp {};

  if (shmem_allocator->unbind (name.c_str (),
                               temp) == 0)
    {
      // unbind () transfers this allocator-owned record to this local owner.
      std::unique_ptr<Employee> employee (static_cast<Employee *> (temp));

      ACE_DEBUG ((LM_DEBUG,
                  "The following employee was found and deleted.......\n\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Employee name: %s\nEmployee id:   %d\n",
                  employee->name (),
                  employee->id ()));

      return 0;
    }

  ACE_DEBUG ((LM_DEBUG,
              "There is no employee with name %s",
              name.c_str ()));
  return -1;
}

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)
    backing_store = argv[1];
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  if (backing_store.empty ())
    {
#if defined (ACE_DEFAULT_BACKING_STORE)
      // Create a temporary file.
      backing_store = ACE_DEFAULT_BACKING_STORE;
#else /* ACE_DEFAULT_BACKING_STORE */
      ACE_TCHAR temp_dir[MAXPATHLEN + 1] {};
      if (ACE::get_temp_dir (temp_dir,
                             MAXPATHLEN - 17) == -1) // -17 for ace-malloc-XXXXXX
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Temporary path too long, ")
                      ACE_TEXT ("defaulting to current directory\n")));
        }
      else
        backing_store = temp_dir;

      // Add the filename to the end
      backing_store.append (ACE_TEXT ("ace-malloc-XXXXXX"));

#endif /* ACE_DEFAULT_BACKING_STORE */
    }

  TEST_MALLOC *raw_allocator {};
  ACE_NEW_RETURN (raw_allocator,
                  TEST_MALLOC (backing_store.c_str ()),
                  -1);
  shmem_allocator.reset (raw_allocator);

  GUI_Handler handler {};

  for (;;)
    if (handler.service () == -1)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("closing down ....\n")));
        break;
      }

  return 0;
}
