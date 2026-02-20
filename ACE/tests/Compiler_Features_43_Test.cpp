/**
 * Test runtime behavior when throwing a plain C++ pointer in recursion
 */

#include "test_config.h"
#include <cstdio>
#include <vector>
#include <memory>

namespace DynamicAny {
  struct DynAny { virtual ~DynAny() = default; };
  struct DynValue : virtual DynAny {};
  struct DynValueCommon : virtual DynAny {};
}

struct TAO_DynCommon : virtual DynamicAny::DynAny {
  virtual ~TAO_DynCommon()
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("~TAO_DynCommon %@\n"), this));
  }
};
struct TAO_DynAny_i : virtual TAO_DynCommon, virtual DynamicAny::DynAny {
    virtual ~TAO_DynAny_i() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("~TAO_DynAny_i %@\n"), this)); }
};
struct TAO_DynValueCommon_i : virtual TAO_DynAny_i, virtual DynamicAny::DynValueCommon {
    virtual ~TAO_DynValueCommon_i() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("~TAO_DynValueCommon_i %@\n"), this)); }
};
// ============================================================
// CreateDynAnyUtils<T>
// ============================================================
template<typename DA_IMPL>
struct CreateDynAnyUtils {
    static DA_IMPL* create(bool trigger, int depth = 0) {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("[Create depth=%d] new DA_IMPL\n"), depth));
        DA_IMPL* p = new DA_IMPL();
        std::unique_ptr<DA_IMPL> guard(p);
        try {
            p->from_inputCDR(trigger, depth);
        }
        catch (DA_IMPL* original) {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("!!! CAUGHT pointer %@ at depth %d (destroying blank %@) !!!\n"),
                original, depth, p));
            return original;
        }
        return guard.release();
    }
};
// ============================================================
// TAO_DynValue_i
// ============================================================
struct TAO_DynValue_i : virtual DynamicAny::DynValue, virtual TAO_DynValueCommon_i {
    TAO_DynValue_i() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_DynValue_i ctor %@\n"), this)); }
    ~TAO_DynValue_i() override { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("~TAO_DynValue_i dtor %@\n"), this)); }
    static TAO_DynValue_i* cached ;
    std::vector<void*> da_members_;
    void from_inputCDR(bool trigger_indirection, int depth) {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("  from_inputCDR depth=%d this=%@\n"), depth, this));
        // Simulates deep recursion of ACE_TAO (exactly like in the real DynValue_i)
        if (depth < 3) {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT("    Creating nested member at depth %d\n"), depth + 1));
            try {
                auto* member = CreateDynAnyUtils<TAO_DynValue_i>::create(trigger_indirection, depth + 1);
                da_members_.push_back(member);
            }
            catch (TAO_DynValue_i* orig) {
                ACE_DEBUG ((LM_DEBUG,ACE_TEXT("    Nested member threw indirection %@\n"), orig));
                da_members_.push_back(orig);
            }
        }
        if (trigger_indirection && cached && depth >= 2) {
            ACE_DEBUG ((LM_DEBUG,ACE_TEXT("*** INDIRECTION - THROWING CACHED POINTER %@ ***\n"), cached));
            cached->_add_ref();
            throw cached; // throw of the raw pointer
        }
        if (!cached) cached = this;
    }
    void _add_ref() { ACE_DEBUG ((LM_DEBUG, ACE_TEXT("    _add_ref on %@\n"), this)); }
};
TAO_DynValue_i* TAO_DynValue_i::cached = nullptr;

int run_main (int, ACE_TCHAR*[])
{
  ACE_START_TEST (ACE_TEXT ("Compiler_Features_43_Test"));

  int res = {};

  TAO_DynValue_i::cached = nullptr;
  try
  {
    auto* result = CreateDynAnyUtils<TAO_DynValue_i>::create(true, 0);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Success - returned %@\n"), result));
    delete result;
  }
  catch (...) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT("In outer catch\n")));
      res = -1;
  }

  ACE_END_TEST;

  return res;
}
