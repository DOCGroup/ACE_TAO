// $Id$

#ifndef ACE_THREAD_SPECIFIC_H

// Define a class that will be stored in thread-specific data.  Note
// that as far as this class is concerned it's just a regular C++
// class.  The ACE_TSS wrapper transparently ensures that objects of
// this class will be placed in thread-specific storage.  All calls on
// ACE_TSS::operator->() are delegated to the appropriate method in
// the Errno class.

class Errno
{
public:
  int error (void) { return this->errno_; }
  void error (int i) { this->errno_ = i; }

  int line (void) { return this->lineno_; }
  void line (int l) { this->lineno_ = l; }

  // Errno::flags_ is a static variable, so we've got to protect it
  // with a mutex since it isn't kept in thread-specific storage.
  int flags (void)
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Errno::lock_, -1);

    return Errno::flags_;
  }

  void flags (int f)
  {
    ACE_GUARD (ACE_Thread_Mutex, ace_mon, Errno::lock_);

    Errno::flags_ = f;
  }

private:
  // = errno_ and lineno_ will be thread-specific data so they don't
  // need a lock.
  int errno_;
  int lineno_;

  static int flags_;
#if defined (ACE_HAS_THREADS)
  // flags_ needs a lock.
  static ACE_Thread_Mutex lock_;
#endif /* ACE_HAS_THREADS */
};

#endif /* ACE_THREAD_SPECIFIC_H */
