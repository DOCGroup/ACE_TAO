$Id$

This test is intended to demonstrate that Var types can be
dereferenced to the pointer type using a casting operation.

This behavior is dangerous in that the reference count is not
increased by extracting the pointer, thus using for anything more than
simple status testing should not be done.
