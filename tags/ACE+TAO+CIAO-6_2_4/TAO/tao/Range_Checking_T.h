#ifndef guard_range_checking_hpp
#define guard_range_checking_hpp
/**
 * @file
 *
 * @brief Details can be found in the documentation for
 * TAO::details::generic_sequence
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "tao/Basic_Types.h"
#include "tao/SystemException.h"

#if !defined (TAO_CHECKED_SEQUENCE_INDEXING) && !defined (ACE_NDEBUG)
# define TAO_CHECKED_SEQUENCE_INDEXING 1
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
namespace details
{

/**
 * @struct range_checking
 *
 * @brief Configurable traits to tradeoff safety vs. performance in
 *        the implementation of TAO sequences.
 *
 * The CORBA specification grants certain latitude to implementors by
 * not defining the behavior of sequences under certain conditions.
 * Probably the most clear example is the operator[] access, where the
 * application <b>must</b> set the length to a high enough value
 * before using the operator[].
 *
 * Implementors that cater to high-performance applications tend to
 * exploit this latitude to the extreme, basically reasoning that
 * correct applications will behave normally, while incorrect
 * applications will crash, but those crashes will be detected during
 * development/testing.
 *
 * Realizing that this may be a bad tradeoff some implementors offer
 * compile-time hooks to control the behavior of sequences when used
 * improperly, some implementors may go as far as using run-time
 * hooks.
 *
 * The implementation of sequences calls the following template class
 * in points where the application may trigger undefined behavior.
 * The application developer can use partial (or full) template
 * specialization to introduce her own code at these critical points.
 *
 * Some examples may help, suppose you want to change your application
 * so for all sequence types the operator[] raises an exception if the
 * index is out of range.  Then you would provide the following
 * (partial) template specialization:
 *
 * <PRE>
 * template<typename T>
 * struct range_checking<T,true> {
 *   void check(CORBA::ULong index, CORBA::ULong length) {
 *     if (index < length)
 *       return;
 *     throw std::range_error("CORBA sequence range error");
 *   };
 * ...
 * ..
 * };
 * </PRE>
 *
 * This specialization must be introduced before any sequence code is
 * seen, therefore, the application would also need to define the
 * following macro in their $ACE_ROOT/ace/config.h file:
 *
 * - #define TAO_USER_DEFINED_SEQUENCE_SAFETY_TRAITS_INCLUDE "<filename here>"
 *
 * Likewise, if the application only wanted to check the range for a
 * special type, say some structure MyStruct, then they would provide
 * a full specialization.  Just for giggles, we will also introduce
 * run-time controls to this example:
 *
 * <PRE>
 * template<>
 * struct safety_traits<tao::details::value_traits<MyStruct>,true> {
 *   bool enable_range_checking;
 *   void check_range(CORBA::ULong index, CORBA::ULong length) {
 *     if (!enable_range_checking || index < length)
 *       return;
 *     throw std::range_error("CORBA sequence range error");
 *   };
 * ...
 * ..
 * };
 * </PRE>
 *
 *
 *
 * @todo There is no control on a per-sequence type basis, only on a
 *       per-underlying type basis, for example, the following two IDL
 *       sequences would get the same behavior:
 *       // IDL
 *       typedef sequence<MyStruct> MyStructSequence;
 *       typedef sequence<MyStruct> MyStructList;
 *
 * @todo There is no way to control behavior on a per-sequence basis,
 *       i.e. to have some sequences of longs checked while others are
 *       not.  This is easy to fix, simply:
 *       - make all members of safety_traits non-static
 *       - have each sequence contain their own instance of
 *         safety_traits
 *       - grant users read/write access to the safety_traits of each
 *         sequence
 *       but there are footprint consequences to that approach.  Until
 *       there is more demand to justify the cost, I will not
 *       implement such a change.
 */
template<typename T, bool dummy>
struct range_checking
{
  typedef T value_type;

  inline static void check(
      CORBA::ULong index,
      CORBA::ULong length,
      CORBA::ULong /* maximum */,
      char const * /* function_name */)
  {
    // Applications and tests can specialize this function to define
    // their own behavior
#if defined (TAO_CHECKED_SEQUENCE_INDEXING) && (TAO_CHECKED_SEQUENCE_INDEXING == 1)
    if (length <= index)
      throw CORBA::BAD_PARAM ();
#else
    ACE_UNUSED_ARG (index);
    ACE_UNUSED_ARG (length);
#endif // TAO_CHECKED_SEQUENCE_INDEXING
  }

  inline static void check_length(
      CORBA::ULong &new_length,
      CORBA::ULong maximum)
  {
    if (maximum < new_length)
      throw CORBA::BAD_PARAM ();
  }
};

} // namespace details
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(TAO_USER_DEFINED_SEQUENCE_RANGE_CHECKING_INCLUDE)
#  include TAO_USER_DEFINED_SEQUENCE_RANGE_CHECKING_INCLUDE
#endif // TAO_USER_DEFINED_SEQUENCE_RANGE_CHECKING_INCLUDE

#endif // guard_range_checking_hpp
