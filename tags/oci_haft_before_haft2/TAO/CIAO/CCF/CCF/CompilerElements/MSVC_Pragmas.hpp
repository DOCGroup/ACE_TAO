#ifndef MSVC_PRAGMAS_HPP
#define MSVC_PRAGMAS_HPP


// Various warnings that have to be disabled for MSVC.

// 'this' : used in base member initializer list
#pragma warning(disable:4355)
// 'class1' : inherits 'class2::member' via dominance
#pragma warning(disable:4250)
// C++ Exception Specification ignored
#pragma warning(disable:4290)


#endif /* MSVC_PRAGMAS_HPP */