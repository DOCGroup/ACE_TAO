// $Id$

#include "ace/Atomic_Op.h"
#include "ace/OS.h"

ACE_RCSID(ace, Atomic_Op, "$Id$")

#if !defined (__ACE_INLINE__)
#include "ace/Atomic_Op.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_BUILTIN_ATOMIC_OP)

long (*ACE_Atomic_Op<ACE_Thread_Mutex, long>::increment_fn_) (volatile long *) = 0;
long (*ACE_Atomic_Op<ACE_Thread_Mutex, long>::decrement_fn_) (volatile long *) = 0;
long (*ACE_Atomic_Op<ACE_Thread_Mutex, long>::exchange_fn_) (volatile long *, long) = 0;
long (*ACE_Atomic_Op<ACE_Thread_Mutex, long>::exchange_add_fn_) (volatile long *, long) = 0;

void
ACE_Atomic_Op<ACE_Thread_Mutex, long>::init_functions (void)
{
  if (ACE_OS::num_processors () == 1)
    {
      increment_fn_ = single_cpu_increment;
      decrement_fn_ = single_cpu_decrement;
      exchange_fn_ = single_cpu_exchange;
      exchange_add_fn_ = single_cpu_exchange_add;
    }
  else
    {
      increment_fn_ = multi_cpu_increment;
      decrement_fn_ = multi_cpu_decrement;
      exchange_fn_ = multi_cpu_exchange;
      exchange_add_fn_ = multi_cpu_exchange_add;
    }
}

void
ACE_Atomic_Op<ACE_Thread_Mutex, long>::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

#if defined (_MSC_VER)
// Disable "no return value" warning, as we will be putting
// the return values directly into the EAX register.
#pragma warning (push)
#pragma warning (disable: 4035)
#endif /* _MSC_VER */

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::single_cpu_increment (volatile long *value)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  long tmp = 1;
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "xadd %0, (%1)" : "+r"(tmp) : "r"(addr) );
  return tmp + 1;
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::single_cpu_decrement (volatile long *value)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  long tmp = -1;
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "xadd %0, (%1)" : "+r"(tmp) : "r"(addr) );
  return tmp - 1;
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::single_cpu_exchange (
  volatile long *value,
  long rhs)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "xchg %0, (%1)" : "+r"(rhs) : "r"(addr) );
  return rhs;
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (rhs);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::single_cpu_exchange_add (volatile long *value,
                                                                long rhs)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "xadd %0, (%1)" : "+r"(rhs) : "r"(addr) );
  return rhs;
#elif defined (WIN32) && !defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
# if defined (_MSC_VER)
  __asm
    {
      mov eax, rhs
      mov edx, value
      xadd [edx], eax
    }
  // Return value is already in EAX register.
# elif defined (__BORLANDC__)
  _EAX = rhs;
  _EDX = ACE_reinterpret_cast (unsigned long, value);
  __emit__(0x0F, 0xC1, 0x02); // xadd [edx], eax
  // Return value is already in EAX register.
# else /* _MSC_VER */
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (rhs);
  ACE_NOTSUP_RETURN (-1);
# endif /* _MSC_VER */
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (rhs);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::multi_cpu_increment (volatile long *value)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  long tmp = 1;
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "lock ; xadd %0, (%1)" : "+r"(tmp) : "r"(addr) );
  return tmp + 1;
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::multi_cpu_decrement (volatile long *value)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  long tmp = -1;
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "lock ; xadd %0, (%1)" : "+r"(tmp) : "r"(addr) );
  return tmp - 1;
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::multi_cpu_exchange (
  volatile long *value,
  long rhs)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  // The XCHG instruction automatically follows LOCK semantics
  asm( "xchg %0, (%1)" : "+r"(rhs) : "r"(addr) );
  return rhs;
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (rhs);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

long
ACE_Atomic_Op<ACE_Thread_Mutex, long>::multi_cpu_exchange_add (volatile long *value,
                                                               long rhs)
{
#if defined (__GNUC__) && defined (ACE_HAS_PENTIUM)
  unsigned long addr = ACE_reinterpret_cast (unsigned long, value);
  asm( "lock ; xadd %0, (%1)" : "+r"(rhs) : "r"(addr) );
  return rhs;
#elif defined (WIN32) && !defined (ACE_HAS_INTERLOCKED_EXCHANGEADD)
# if defined (_MSC_VER)
  __asm
    {
      mov eax, rhs
      mov edx, value
      lock xadd [edx], eax
    }
  // Return value is already in EAX register.
# elif defined (__BORLANDC__)
  _EAX = rhs;
  _EDX = ACE_reinterpret_cast (unsigned long, value);
  __emit__(0xF0, 0x0F, 0xC1, 0x02); // lock xadd [edx], eax
  // Return value is already in EAX register.
# else /* _MSC_VER */
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (rhs);
  ACE_NOTSUP_RETURN (-1);
# endif /* _MSC_VER */
#else /* __GNUC__ && ACE_HAS_PENTIUM */
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (rhs);
  ACE_NOTSUP_RETURN (-1);
#endif /* __GNUC__ && ACE_HAS_PENTIUM */
}

#if defined (_MSC_VER)
#pragma warning (pop)
#endif /* _MSC_VER */

#endif /* ACE_HAS_BUILTIN_ATOMIC_OP */

#if defined (ACE_HAS_THREADS)
# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#  if !defined (ACE_HAS_BUILTIN_ATOMIC_OP)
template class ACE_Atomic_Op<ACE_Thread_Mutex, long>;
#  endif /* !ACE_HAS_BUILTIN_ATOMIC_OP */
template class ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>;
# elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  if !defined (ACE_HAS_BUILTIN_ATOMIC_OP)
#   pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, long>
#  endif /* !ACE_HAS_BUILTIN_ATOMIC_OP */
#  pragma instantiate ACE_Atomic_Op_Ex<ACE_Thread_Mutex, long>
# endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_THREADS */
