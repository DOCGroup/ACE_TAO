# This test confirms that CORBA::is_nil works

TAO implements `CORBA::is_nil` as a function template, which merely
checks the argument for zero. The template is specialized for
`CORBA::Object_ptr`, and this specialization additionally checks
whether a non-NULL argument might be an unevaluated nil object
reference.

Because template specialization is used and not an ordinary overloaded
function, the specialized variant is *not* called for pointers to
derived classes. This will work only if TAO ensures that all nil
object references that have been narrowed to a derived class are
converted to null pointers, i.e., to ensure that the generic template
produces the correct result.

This test has the server return a nil object reference and the client
uses lazy resource usage, to confirm that CORBA::is_nil correctly
identifies the nil object reference. See issue #1203, which pointed
out that only CORBA::Object_ptr has a template specialization for
CORBA::is_nil, but not derived interfaces.
