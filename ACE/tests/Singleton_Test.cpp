/**
 *  @file    Singleton_Test.cpp
 *
 *    This is a simple test of ACE Singleton
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */

#include "test_config.h"
#include "ace/Singleton.h"
#include <type_traits>

using Singleton = ACE_Singleton<int, ACE_SYNCH_RECURSIVE_MUTEX>;
using Unmanaged_Singleton = ACE_Unmanaged_Singleton<int, ACE_SYNCH_RECURSIVE_MUTEX>;
using TSS_Singleton = ACE_TSS_Singleton<int, ACE_SYNCH_RECURSIVE_MUTEX>;
using Unmanaged_TSS_Singleton = ACE_Unmanaged_TSS_Singleton<int, ACE_SYNCH_RECURSIVE_MUTEX>;
using DLL_Singleton = ACE_DLL_Singleton_T<int, ACE_SYNCH_RECURSIVE_MUTEX>;

static_assert (!std::is_copy_constructible_v<Singleton>);
static_assert (!std::is_move_constructible_v<Singleton>);
static_assert (!std::is_copy_assignable_v<Singleton>);
static_assert (!std::is_move_assignable_v<Singleton>);

static_assert (!std::is_copy_constructible_v<Unmanaged_Singleton>);
static_assert (!std::is_move_constructible_v<Unmanaged_Singleton>);
static_assert (!std::is_copy_assignable_v<Unmanaged_Singleton>);
static_assert (!std::is_move_assignable_v<Unmanaged_Singleton>);

static_assert (!std::is_copy_constructible_v<TSS_Singleton>);
static_assert (!std::is_move_constructible_v<TSS_Singleton>);
static_assert (!std::is_copy_assignable_v<TSS_Singleton>);
static_assert (!std::is_move_assignable_v<TSS_Singleton>);

static_assert (!std::is_copy_constructible_v<Unmanaged_TSS_Singleton>);
static_assert (!std::is_move_constructible_v<Unmanaged_TSS_Singleton>);
static_assert (!std::is_copy_assignable_v<Unmanaged_TSS_Singleton>);
static_assert (!std::is_move_assignable_v<Unmanaged_TSS_Singleton>);

static_assert (!std::is_copy_constructible_v<DLL_Singleton>);
static_assert (!std::is_move_constructible_v<DLL_Singleton>);
static_assert (!std::is_copy_assignable_v<DLL_Singleton>);
static_assert (!std::is_move_assignable_v<DLL_Singleton>);

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Singleton_Test"));

  int *i = ACE_Singleton <int, ACE_SYNCH_RECURSIVE_MUTEX>::instance ();
  *i = 3;
  ACE_Singleton <int, ACE_SYNCH_RECURSIVE_MUTEX>::close ();

  ACE_END_TEST;

  return 0;
}

