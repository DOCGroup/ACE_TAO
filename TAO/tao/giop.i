// This may look like C, but it's really -*- C++ -*-

ACE_INLINE void 
GIOP::Invocation::get_value (CORBA::TypeCode_ptr tc,
			     void *value,
			     CORBA::Environment &env)
{
  (void) stream.decode (tc, value, 0, env);
}

void 
GIOP::Invocation::put_param (CORBA::TypeCode_ptr tc, 
			     void *value, 
			     CORBA::Environment &env)
{
  (void) stream.encode (tc, value, 0, env);
}
