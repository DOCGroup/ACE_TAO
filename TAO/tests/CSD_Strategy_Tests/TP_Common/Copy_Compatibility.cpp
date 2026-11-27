/**
 * @file Copy_Compatibility.cpp
 *
 * Check the copy behavior of the thread-pool queue visitors.
 */

#include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.h"
#include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.h"

#include <type_traits>

static_assert (std::is_copy_constructible_v<TAO::CSD::TP_Cancel_Visitor>);
static_assert (std::is_move_assignable_v<TAO::CSD::TP_Cancel_Visitor>);
static_assert (std::is_copy_constructible_v<TAO::CSD::TP_Dispatchable_Visitor>);
static_assert (std::is_move_assignable_v<TAO::CSD::TP_Dispatchable_Visitor>);
