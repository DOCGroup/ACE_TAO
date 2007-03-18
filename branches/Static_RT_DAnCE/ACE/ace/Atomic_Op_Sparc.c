/* $Id$
 *
 * This is a C file for a reason.  The Sun C++ compiler does not accept
 * inline assembler.
 *
 * Portions of this code are based on atomic operations found in the
 * linux kernel source code.
 */

#if defined (ACE_INCLUDE_ATOMIC_OP_SPARC)

#if defined (__sparcv9)

unsigned long
ace_atomic_add_long (volatile unsigned long *dest, long rhs)
{
  __asm ("restore\n"
         "ldx [%o0], %o2\n"
         ".again_add:\n"
         "add %o2, %o1, %o3\n"
         "casx [%o0], %o2, %o3\n"
         "cmp %o2, %o3\n"
         "bne,pn %xcc, .again_add\n"
         "mov %o3, %o2\n"
         "retl\n"
         "add %o2, %o1, %o0\n");
}

unsigned long
ace_atomic_swap_long (volatile unsigned long *dest, unsigned long rhs)
{
  __asm ("restore\n"
         "ldx [%o0], %o2\n"
         ".again_swap:\n"
         "mov %o1, %o3\n"
         "casx [%o0], %o2, %o3\n"
         "cmp %o2, %o3\n"
         "bne,pn %xcc, .again_swap\n"
         "mov %o3, %o2\n"
         "retl\n"
         "mov %o3, %o0\n");
}

unsigned long
ace_atomic_swap_add_long (volatile unsigned long *dest, long rhs)
{
  __asm ("restore\n"
         "ldx [%o0], %o2\n"
         ".again_swap_add:\n"
         "mov %o2, %o4\n"
         "add %o2, %o1, %o3\n"
         "casx [%o0], %o2, %o3\n"
         "cmp %o2, %o3\n"
         "bne,pn %xcc, .again_swap_add\n"
         "mov %o3, %o2\n"
         "retl\n"
         "mov %o4, %o0\n");
}

#else

unsigned long
ace_atomic_add_long (volatile unsigned long *dest, long rhs)
{
  __asm ("restore\n"
         "ld [%o0], %o2\n"
         ".again_add:\n"
         "add %o2, %o1, %o3\n"
         "cas [%o0], %o2, %o3\n"
         "cmp %o2, %o3\n"
         "bne,pn %icc, .again_add\n"
         "mov %o3, %o2\n"
         "retl\n"
         "add %o2, %o1, %o0\n");
}

unsigned long
ace_atomic_swap_long (volatile unsigned long *dest, unsigned long rhs)
{
  __asm ("restore\n"
         "ld [%o0], %o2\n"
         ".again_swap:\n"
         "mov %o1, %o3\n"
         "cas [%o0], %o2, %o3\n"
         "cmp %o2, %o3\n"
         "bne,pn %icc, .again_swap\n"
         "mov %o3, %o2\n"
         "retl\n"
         "mov %o3, %o0\n");
}

unsigned long
ace_atomic_swap_add_long (volatile unsigned long *dest, long rhs)
{
  __asm ("restore\n"
         "ld [%o0], %o2\n"
         ".again_swap_add:\n"
         "mov %o2, %o4\n"
         "add %o2, %o1, %o3\n"
         "cas [%o0], %o2, %o3\n"
         "cmp %o2, %o3\n"
         "bne,pn %icc, .again_swap_add\n"
         "mov %o3, %o2\n"
         "retl\n"
         "mov %o4, %o0\n");
}

# endif /* __sparcv9 */

#elif !defined (__GNUC__)
/* Make compilers stop complaining about an empty translation unit */
static int shut_up_compiler = 0;
#endif /* ACE_INCLUDE_ATOMIC_OP_SPARC */
