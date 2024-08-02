# IDL Relative Include Test

Like with C/C++, `#include ""` should be able to be used with `tao_idl` to
include a path relative to the including file. If the `tao_idl` falls back to
`‑‑preprocessor‑input copy`, then this test will fail. If it does then this
shouldn't affect normal usage of TAO, but `tao_idl` should be updated for the
failing case so that it can pass the IDL file directly to the preprocessor.

See `‑‑preprocessor‑input` in `TAO/docs/compiler.html` and `DRV_cpp_post_init`
in `TAO/TAO_IDL/driver/drv_preproc.cpp` for more information.
