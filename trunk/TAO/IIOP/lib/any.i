ACE_INLINE
void*
CORBA_Any::operator new (size_t, const void *p)
{
  return (void *) p;
}

ACE_INLINE
void*
CORBA_Any::operator new (size_t s)
{
  return ::operator new (s);
}

ACE_INLINE
void
CORBA_Any::operator delete (void *p)
{
  ::operator delete (p);
}
