// $Id$

// Test the persistence capabilities of the ACE shared memory manager.

#include "ace/Malloc.h"

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# include /**/ <iostream.h>
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC;
typedef ACE_Malloc_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC_ITERATOR;

// Shared memory manager.
static MALLOC *shmem_manager = 0;

// Backing store name.
static char *backing_store = ACE_DEFAULT_BACKING_STORE;

class Employee
{
public:
  Employee (void): name_ (0), id_ (0) {}

  Employee (char* name, u_long id) : id_ (id)
    {
      this->name_ = (char*) shmem_manager->malloc (ACE_OS::strlen (name) + 1);
      ACE_OS::strcpy (this->name_, name );
    }

  ~Employee (void) { shmem_manager->free (this->name_); }

  char *name (void) const    { return this->name_; }

  void name (char* name)
    {
      if (this->name_)
	shmem_manager->free (this->name_);
      this->name_ = (char*) shmem_manager->malloc (ACE_OS::strlen (name) + 1);
      ACE_OS::strcpy (this->name_, name);
    }

  u_long id (void) const { return id_; }

  void id (u_long id) { id_ = id; }

  void *operator new (size_t)
    {
      return shmem_manager->malloc (sizeof (Employee));
    }

  void operator delete (void *pointer) { shmem_manager->free (pointer); }

private:
  char *name_;
  // Employee name.

  u_long id_;
  // Employee ID.
};

class GUI_Handler
{
public:
  GUI_Handler (void) { menu(); }

  ~GUI_Handler (void)
    {
      MALLOC::MEMORY_POOL &pool = shmem_manager->memory_pool();
      pool.sync ();
    }

  int service(void)
    {
      char option[BUFSIZ];
      char buf1[BUFSIZ];
      char buf2[BUFSIZ];

      if (::scanf ("%s", option) <= 0)
	{
	  ACE_ERROR ((LM_ERROR, "try again\n"));
	  return 0;
	}

      int result = 0;
      switch (option[0])
	{
	case 'I' :
	case 'i' :
	  if (::scanf ("%s %s", buf1, buf2) <= 0)
	    break;
	  result = insert_employee (buf1, ACE_OS::atoi (buf2));
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
  int insert_employee (char* name, u_long id)
    {
      if (find_employee (name) == 0)
	ACE_ERROR_RETURN ((LM_ERROR, "Employee already exists\n"), -1);

      Employee* new_employee = new Employee (name, id);
      shmem_manager->bind (name, new_employee);
      return 0;
    }

  int find_employee (char* name)
    {
      void *temp;
      if (shmem_manager->find (name, temp) == 0)
	{
	  Employee *employee = (Employee *) temp;

	  ACE_DEBUG ((LM_DEBUG, "The following employee was found.......\n\n"));
	  ACE_DEBUG ((LM_DEBUG, "Employee name: %s\nEmployee id:   %d\n",
		      employee->name (), employee->id ()));
	  return 0;
	}

      return -1;
    }

  int list_employees (void)
    {
      MALLOC_ITERATOR iterator (*shmem_manager);

      ACE_DEBUG ((LM_DEBUG, "The following employees were found.......\n\n"));

      for (void* temp = 0;
	  iterator.next (temp) != 0;
	  iterator.advance ())
	{
	  Employee *employee = (Employee *) temp;
	  ACE_DEBUG ((LM_DEBUG, "Employee name: %s\nEmployee id:   %d\n",
		      employee->name (), employee->id ()));
	}
      return 0;
    }

  int delete_employee (char* name)
    {
      void *temp;

      if (shmem_manager->unbind (name, temp) == 0)
	{
	  Employee *employee = (Employee *) temp;

	  ACE_DEBUG ((LM_DEBUG,
		      "The following employee was found and deleted.......\n\n"));

	  ACE_DEBUG ((LM_DEBUG, "Employee name: %s\nEmployee id:   %d\n",
		      employee->name (), employee->id ()));

	  delete employee;
	  return 0;
	}

      ACE_DEBUG ((LM_DEBUG, "There is no employee with name %s", name));
      return -1;
    }
};

void
parse_args (int argc, char *argv[])
{
  if (argc > 1);
    backing_store = argv[1];
}

int
main (int argc, char *argv[])
{
  parse_args (argc, argv);

  shmem_manager = new MALLOC (backing_store);

  GUI_Handler handler;

  for(;;)
    if (handler.service() == -1)
      {
	ACE_DEBUG ((LM_DEBUG, "closing down ....\n"));
	break;
      }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Malloc_Iterator <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
