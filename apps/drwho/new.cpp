// $Id$
/* Provide a very fast dynamic memory management abstraction tuned for
   drwho server characteristics. */

#include "assert.h"
#include "ace/OS.h"
#include "Options.h"
#include "new.h"

#if 0
/* Determine default alignment.  If your C++ compiler does not
   like this then try something like #define ALIGNMENT 8. */

struct fooalign {char x; double d;};

static int ALIGNMENT = ((char *)&((struct fooalign *) 0)->d - (char *)0);
static int PAGE_SIZE = getpagesize ();
#endif

static const int ALIGNMENT = 8;
static const int PAGE_SIZE = 4096;
static const int debug	   = 1;
static int   	 use_pool  = 0;

static inline int 
max (int a, int b) 
{ 
  return a >= b ? a : b; 
}

/* The ALIGNMENT argument must be a power of two... */

static inline int 
round (size_t size, int alignment)
{
  return size + alignment - 1 & ~(alignment - 1);
}

class Chunk_Manager
{
private:
  struct Chunk
    {
      Chunk        *next;
      char	   *buf_cur;
      char	   *buf_end;
      int	   buf_size;
      char	   buf_start[1];

      int	   chunk_must_grow (size_t size);
      void	   *get_n_bytes (size_t size);
      int	   chunk_size (void);
      void	   print_stats (int);
    };

  static void  insert_free_chunk (Chunk *ptr);
  static Chunk *check_free_list (size_t size);
  static Chunk *make_new_chunk (size_t size, Chunk *next = 0);
  static Chunk *free_list;
  static Chunk *head;
  static size_t default_chunk_size;

public:
  static void	*alloc_bytes (size_t size);
  static void   release_chunks (void);
};

size_t               Chunk_Manager::default_chunk_size = 4 * PAGE_SIZE;
Chunk_Manager::Chunk *Chunk_Manager::free_list	       = 0;
Chunk_Manager::Chunk *Chunk_Manager::head	       = 0;

void
#if defined (__GNUG__) || defined (SunOS5)
Chunk_Manager::Chunk::print_stats (int size)
#else
Chunk::print_stats (int size)
#endif
{
  fprintf (stderr, "buf_size = %d, bytes left = %d, bytes used = %d, bytes requested = %d\n",
	   this->buf_size, this->buf_end - this->buf_cur, this->buf_cur - this->buf_start, size);
}

int
#if defined (__GNUG__) || defined (SunOS5)
Chunk_Manager::Chunk::chunk_must_grow (size_t size)
#else
Chunk::chunk_must_grow (size_t size)
#endif
{
  return this->buf_end - this->buf_cur < size;
}

void *
#if defined (__GNUG__) || defined (SunOS5)
Chunk_Manager::Chunk::get_n_bytes (size_t size)
#else
Chunk::get_n_bytes (size_t size)
#endif
{
  void *temp     = this->buf_cur;
  this->buf_cur += size;
  return temp;
}

int
#if defined (__GNUG__) || defined (SunOS5)
Chunk_Manager::Chunk::chunk_size (void)
#else
Chunk::chunk_size (void)
#endif
{
  return this->buf_size;
}

void
Chunk_Manager::insert_free_chunk (Chunk *ptr)
{
  for (Chunk **flpp = &Chunk_Manager::free_list; (*flpp) != 0 && (*flpp)->buf_size < ptr->buf_size; flpp = &(*flpp)->next)
    ;

  ptr->next = *flpp;
  *flpp	     = ptr;
}

Chunk_Manager::Chunk *
Chunk_Manager::check_free_list (size_t size)
{

  for (Chunk **flpp = &Chunk_Manager::free_list; (*flpp) != 0 && (*flpp)->buf_size < size; flpp = &(*flpp)->next) 
    ;

  if (*flpp == 0)
    return 0;
  else
    {
      Chunk *flp = *flpp;
      *flpp 	 = (*flpp)->next;
      return flp;
    }    
}	

void
Chunk_Manager::release_chunks (void)
{
  Chunk *tmp = Chunk_Manager::head;

  while (tmp != 0)
    {
      Chunk *t = tmp;
      tmp      = tmp->next;
      insert_free_chunk (t);
    }

  Chunk_Manager::head = 0;
}

Chunk_Manager::Chunk *
Chunk_Manager::make_new_chunk (size_t size, Chunk *n)
{
  Chunk *free_chunk = Chunk_Manager::check_free_list (size = round (size, Chunk_Manager::default_chunk_size));

  if (free_chunk == 0)
    {
      if ((free_chunk = (Chunk *) malloc (sizeof *free_chunk + size)) == 0)
	return 0;

      free_chunk->buf_size = size;
      free_chunk->buf_end  = free_chunk->buf_start + free_chunk->buf_size;
    }
  free_chunk->buf_cur  = free_chunk->buf_start;
  free_chunk->next     = n;
  return free_chunk;
}

void *
Chunk_Manager::alloc_bytes (size_t size)
{
  size = round (size, ALIGNMENT);

  if (Chunk_Manager::head == 0)
    Chunk_Manager::head = Chunk_Manager::make_new_chunk (size);
  else if (Chunk_Manager::head->chunk_must_grow (size))
    Chunk_Manager::head = Chunk_Manager::make_new_chunk (max (Chunk_Manager::head->chunk_size () * 2, size), Chunk_Manager::head);

//  Chunk_Manager::head->print_stats (size);

  return Chunk_Manager::head->get_n_bytes (size);
}

void *
operator new (size_t size, New_Pool new_pool)
{
  if (::use_pool || new_pool == PRIVATE_POOL)
    return Chunk_Manager::alloc_bytes (size);
  else
    return (void *) malloc (size);
}
void
mark_memory (int start_using_the_pool)
{
  ::use_pool = start_using_the_pool;
}

void
release_memory (void)
{
  Chunk_Manager::release_chunks ();
}

/* We need this deletion operator in order to make the linker happy. */

void 
operator delete (void *)
{
}
