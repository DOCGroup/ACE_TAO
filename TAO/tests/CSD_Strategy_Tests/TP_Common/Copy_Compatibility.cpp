/**
 * @file Copy_Compatibility.cpp
 *
 * Check the copy behavior of the thread-pool queue visitors.
 */

#include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.h"
#include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.h"

#include <type_traits>

static_assert (std::is_copy_constructible<TAO::CSD::TP_Cancel_Visitor>::value);
static_assert (std::is_move_assignable<TAO::CSD::TP_Cancel_Visitor>::value);
static_assert (std::is_copy_constructible<TAO::CSD::TP_Dispatchable_Visitor>::value);
static_assert (std::is_move_assignable<TAO::CSD::TP_Dispatchable_Visitor>::value);
