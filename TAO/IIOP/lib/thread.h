// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    thread.h
//
// = DESCRIPTION
//     
//    Simple thread utility classes
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

// @@ Please remove this file...

// Instances of these classes are placed at the beginning of a lexical
// scope to ensure that all exits from that scope restore thread or
// synchronization state to its original values.  Thread state
// includes ansynchronous cancellability, synchronization state
// includes locks that cover data structures that are shared between
// threads.
//
// NOTE: supports only POSIX and SVR4 threads for the moment.  Win32
// threads can be supported sometime later.

#ifndef	_THREAD_HH
#define	_THREAD_HH

#ifdef	unix
#include	<unistd.h>		// may define _POSIX_THREADS
#endif	// unix

#ifdef	_POSIX_THREADS
#include	<pthread.h>

//
// This ugliness is called for by OSF/1 v3.0, which defines the POSIX.1c
// symbol "_POSIX_THREADS" even though it is noncompliant.
//
// We make the assumption that PTHREAD_MUTEX_INITIALIXER is a preprocessor
// symbol, although POSIX does not require that, since since it works on
// all the POSIX.1c implementations we've seen.
//
#	ifndef	PTHREAD_MUTEX_INITIALIZER
#		undef _POSIX_THREADS
#	endif	// not really POSIX.1c threads
#endif

//
// Use native threading on Solaris 2.2 and later.
//
// XXX this is a temporary kluge; "autoconf" should kick in the
// appropriate POSIX threads emulation when it's not native.
//
#if defined (sun) && !defined (_POSIX_THREADS) && !defined (FAKE_POSIX_THREADS) 
#	define	FAKE_POSIX_THREADS
#endif	// 2.2 <= "solaris version < 2.5


#ifdef	FAKE_POSIX_THREADS
//
// To use SVR4 native threads, enable FAKE_POSIX_THREADS.  This is
// intentionally not autoconfigured, since it's only really needed on
// Solaris versions before 2.5.  POSIX threads are the way to go
// longer term, on all compliant platforms.
//
// NOTE:  this is only a partial implementation !!!  Enough to make
// the IIOP engine work as it's coded as of 9-Aug-95.  If you start
// to use different POSIX options, these macros could stop working.
//
#define	_POSIX_THREADS		// only as much as is defined here!

#include	<thread.h>
#include	<synch.h>

typedef	thread_t		pthread_t;
typedef	mutex_t			pthread_mutex_t;
typedef cond_t			pthread_cond_t;
typedef	unsigned long		pthread_attr_t;
typedef thread_key_t		pthread_key_t;


#define	PTHREAD_MUTEX_INITIALIZER	DEFAULTMUTEX
#define	PTHREAD_COND_INITIALIZER	DEFAULTCV

#define	pthread_mutex_lock	mutex_lock
#define	pthread_mutex_unlock	mutex_unlock
#define	pthread_cond_wait	cond_wait

struct pthread_once_t {
    mutex_t	lock;
    int		called_once;
};

#define	PTHREAD_ONCE_INIT	{ DEFAULTMUTEX, 0 }

inline void
pthread_once (pthread_once_t *var, void (*fn)())
{
    ACE_OS::mutex_lock (&var->lock);
    if (var->called_once == 0) {
	(fn)();
	var->called_once = 1;
    }
    ACE_OS::mutex_unlock (&var->lock);
}

#define	pthread_self		thr_self

#define	pthread_key_create	thr_keycreate
#define	pthread_attr_init(ap)	((*ap) = 0)
#define	pthread_attr_setdetachstate(ap,v) ((*ap) |= (v))

#define	PTHREAD_CREATE_DETACHED	THR_DETACHED

inline void *
pthread_getspecific (pthread_key_t key)
{
    void		*vp;

    ACE_OS::thr_getspecific (key, &vp);
    return vp;
}

#define	pthread_setspecific	thr_setspecific

inline int
pthread_create (
    pthread_t		*tidp,
    pthread_attr_t	*attrs,
    void		*(func)(void *),
    void		*arg
)
{
    return ACE_OS::thr_create (0, 0, func, arg, *attrs, tidp);
}

#endif	// FAKE_POSIX_THREADS


#ifdef	_POSIX_THREADS
//
// Stick one of these at the beginning of a block that hosts a critical
// section, passing it a pointer to the lock guarding that section.  The
// destructor will release it on all code paths, eliminating one class of
// common threading errors.
//
class Critical {
  public:
			Critical (pthread_mutex_t *l) : lock (l)
				{ enter (); }

			~Critical ()
				{ leave (); }
			
    void		leave ()
				{ (void) pthread_mutex_unlock (lock); }
    void		enter ()
				{ (void) pthread_mutex_lock (lock); }
    void		pause (pthread_cond_t *condition)
			    { (void) pthread_cond_wait (condition, lock); }

  private:
    pthread_mutex_t	*lock;
};


//
// Stick one of these at the beginning of a block that can't support
// asynchronous cancellation, and which must be cancel-safe.
//
class ForceSynchronousCancel {
  public:
    //
    // MIT Pthreads 1.60 doesn't include cancellation; this is good, it
    // gives time to ensure that stack unwinding for cancellation (in C)
    // interoperates with unwinding for C++ exceptions so that both
    // resource reclamation systems interwork correctly.
    //
#ifdef	PTHREAD_CANCEL_DEFERRED
    ForceSynchronousCancel ()
    { (void) pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, &old_type); }

    ~ForceSynchronousCancel ()
    { (void) pthread_setcanceltype (old_type, &old_type); }

  private:
    int			old_type;
#endif	// PTHREAD_CANCEL_DEFERRED
};

#endif	// _POSIX_THREADS

#endif	// _THREAD_HH
