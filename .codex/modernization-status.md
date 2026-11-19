# ACE/TAO modernization tracker

The scope is the complete ACE/TAO tree, including TAO_IDL, IFR_Service,
examples, tests, tools, generated-code inputs, and platform-specific code.

## The tracker files

- [modernization-inventory.tsv](modernization-inventory.tsv) is the directory
  and project coverage index. It includes one row for each tracked directory
  containing C/C++ source, IDL, lex/yacc input, or an MPC project.
- [modernization-task-status.tsv](modernization-task-status.tsv) is the
  task-by-directory record. It keeps completed work visible independently of
  the current branch diff. Rows are sparse: a missing directory/task row means
  `not-reviewed`, not “not applicable” or “never changed.”
- [modernization-progress.md](modernization-progress.md) is the generated
  summary by task and by validated batch.
- [modernization-ledger.md](modernization-ledger.md) is the detailed history.
  Keep its entries when updating the tracker; they preserve what changed, why,
  how it was validated, and relevant failures or limitations.

The records separate Git deltas, modernization tasks, and validation history.
`work-recorded` means there is a branch diff in that directory;
`no-branch-delta` means no diff from the selected base and says nothing about
earlier work. `batch-complete` means one scoped batch was implemented and
validated, not that all modernization work there is done. Each reviewed task is
recorded as `done`, `not-applicable`, or `needs-work`; absent task rows are
`not-reviewed`. This preserves completed work even after its branch is merged
or the base changes. Only mark `audit_state` as `audited` after every applicable
task has been reviewed and no follow-up remains.

## Global follow-up

- Review sequence buffers according to their IDL type. An IDL
  `sequence<char>` is length-delimited and is not guaranteed to have a trailing
  NUL, so compare its `length()` and bytes (for example with `std::memcmp`);
  do not use `strcmp`/`std::strcmp` on `get_buffer()`. Use string comparison
  only for IDL `string` values or buffers whose null termination is guaranteed.
  The repository-wide audit found no other current `sequence<char>`/`strcmp`
  misuse. Include this check when auditing existing and newly modernized code.
- In ACE/ace production sources, the remaining `ACE_OS` string calls were
  checked and are wide-character-aware `ACE_TCHAR` operations or implementations
  of ACE's own wrappers. Keep them; retain direct `ACE_OS` calls in unit tests
  that explicitly test those APIs.
- TODO for the entire ACE/TAO tree: revisit every source directory, including
  directories already modernized, and remove comments anywhere in the codebase
  that merely label a constructor, destructor, copy/move operation, or
  assignment operator (for example, `// constructor`, `// destructor`, or
  `// copy constructor`). These labels do not add useful information. This
  applies to every current and future directory pass, headers, sources, tests,
  and generator inputs; it remains open even in directories with earlier
  completed task records. Track it as `SPECIAL_MEMBER_COMMENTS` per directory,
  with missing rows remaining open work. Review generated output through its
  generator inputs where those are in scope. Keep comments that explain
  behavior, ownership, or non-obvious intent.
- Apply the review lessons from OpenDDS PR 5327 when changing TAO IDL APIs or
  their clients: keep C++03 syntax confined to compatibility branches;
  centralize compatibility helpers instead of copying them; avoid conversion
  wrappers and temporary strings when an API already returns the required
  type; bind repeated standard-container results by const reference when
  lifetime permits; handle narrow and wide string literals through distinct
  accessors; and convert `std::string` to `c_str()` before passing it through
  ACE variadic logging. Review related callers outside the immediate diff, too.
  If one feature macro gates multiple independently evolving APIs, document
  that coupling or use separate macros.
- TODO for the full ACE/TAO tree: audit existing `std::move` calls whenever
  move support is added or changed. For CORBA sequences, verify that the source
  is non-const, is not read again after the move, and leaves a valid moved-from
  state (including `maximum()`, buffer ownership, and `release()`). Also review
  nearby sequence copies for cases where the source is dead and moving would
  avoid an allocation/copy. Do not infer safety solely from a successful build.
  The current TAO sequence move call sites were reviewed on 2026-11-16; no
  moved-from misuse was found. On 2026-11-17, a further 82 non-sequence
  ownership moves in TAO AV, `TAO_Acceptor_Registry`, logWalker, ASNMP,
  `ACE/websvcs/lib/URL_Addr.cpp`, TAO Naming, `Active_Object_Map.cpp`, and
  `Object_Adapter.cpp` were reviewed without finding moved-from use. A
  repository-wide audit remains open.
- TODO for the full ACE/TAO tree: when calling an operation whose parameter is
  `bool` or `CORBA::Boolean`, pass `true`/`false` instead of `1`/`0`. Revisit
  directories already modernized as well as new changes; track this as
  `BOOL_ARGUMENTS`.

## Inventory columns

- `source_files`, `mpc_files`, and `mpc_projects` count tracked work files and
  MPC declarations directly in that directory.
- `files_changed_from_base` counts branch changes relative to the selected Git
  base; `local_changed_files` counts uncommitted or staged source changes.
- `work_state` is `no-branch-delta`, `work-recorded`, `in-progress`,
  `batch-complete`, or `removed-from-tree`.
- `audit_state` is `not-audited`, `partial`, or `audited`. `partial` means some
  checklist items were reviewed, while candidates remain.
- `tasks_done` is a compact summary generated from the task-status file.
  `open_items` lists follow-up or explains that a full directory audit is
  pending.
- `validation` records the exact generator, build, and test commands/results;
  `last_commit` records the implementation commit when one exists.

Task codes for the task-status file and its `tasks_done` summary:

| Code | Review area |
| --- | --- |
| `CONST` | East-side const, const locals, and immutable handles |
| `VALUE_INIT` | Value-initialize scalar/member state; simplify constructors |
| `BOOL_ARGUMENTS` | Use Boolean literals for Boolean arguments |
| `NULLPTR` | Replace eligible null pointer constants with `nullptr` |
| `OVERRIDE_DEFAULT_DELETE` | Review override/default/delete and exported destructor placement |
| `EXPLICIT` | Review single-argument constructors for `explicit` |
| `COPY_MOVE` | Explicitly review copy/move support and deletion |
| `MOVE_SEMANTICS` | Audit existing `std::move` calls and moved-from use, especially for IDL sequences |
| `ATOMIC` | Review atomic state and memory-order requirements |
| `THREAD_SAFETY` | Review synchronization and data-race safety |
| `USING` | Convert suitable typedefs to using aliases |
| `CONSTEXPR` | Review internal constants for constexpr and linkage |
| `OWNERSHIP` | Replace suitable manual ownership with smart pointers |
| `CONTAINERS` | Review manual arrays/containers; keep ACE types when std adds exception handling |
| `C_STRING` | Replace plain narrow C-string wrappers where appropriate |
| `SIZE_TYPES` | Review count and size types where appropriate |
| `IDL_GENERATION` | Review tao_idl/gperf inputs and generated output when in scope |
| `CLI_OPTIONS` | Group multiple command-line settings into a named options type |
| `LOG_FORMAT` | Check ACE logging format specifiers against argument types |
| `TEST_ASSERTIONS` | Ensure regression tests fail when their expectations fail |
| `CLEANUP` | Remove unused helpers and stale commented-out code |
| `SCOPE_REDUCTION` | Declare locals at their point of use and narrow their lifetime |
| `SPECIAL_MEMBER_COMMENTS` | Across every directory, remove comments that only label constructors, destructors, copy/move operations, or assignments; review generated output through its generator inputs |
| `BRACES` | Add consistent braces around control-flow bodies |
| `STYLE` | Review blank lines, generated layout, line length, and formatting |
| `BUILD_TEST` | Regenerate as needed, realclean before build, and validate focused tests |

The task-status file has columns `directory`, `task`, `status`,
`evidence_commit`, and `notes`. Record each reviewed task there. Use
`not-applicable` only after checking that the task does not apply; use
`needs-work` for identified follow-up. Keep detail and validation commands in
the ledger. The inventory updater migrates existing `tasks_done` entries once
and then treats the task-status file as the source of truth.

## Recording a modernization batch

For each directory batch, add one task-status row for every area actually
reviewed, including `not-applicable` and `needs-work` findings. Keep any
remaining action in the inventory's `open_items`. Append the rationale,
commands, results, warnings, and commit to the ledger. After committing, set
the directory to `batch-complete` only when its focused validation succeeded,
record the task list and commit in the inventory, then run the updater. Mark a
directory `audited` only when every applicable task has been reviewed and no
follow-up remains. This lets batch progress accumulate without erasing the
history when a branch is rebased or merged.

## Refreshing the factual counts

From the repository root, after fetching the intended base:

```sh
python3 .codex/update-modernization-inventory.py --base origin/master
```

The updater refreshes tracked source/MPC counts and Git deltas, preserves
task-status records, and regenerates the progress summary. Changed-directory
and project counts describe touched scope, not completed modernization. The
ledger remains the detailed evidence for the work itself.

## Repository-wide follow-ups

- In every ACE/TAO directory modernization pass, check for comments that only label constructors or destructors and remove labels that add no information. Preserve comments that explain behavior, and review generated output separately from handwritten sources. Revisit directories already modernized as part of the follow-up audit.

## Current snapshot

<!-- generated-snapshot:start -->
The inventory was generated against `upstream/master`. It contains 1,268 source/project directories, 1,161 MPC files, and 2,963 MPC project declarations. The branch differs from the base in 1,230 directories, containing 2,951 project declarations. These describe touched scope, not completion.

Task-level tracking currently has 3839 reviewed task records across 1281 directories; 246 scoped batches are marked validated, and 54 directories have a full checklist audit. There are 124 directories with no branch delta. Earlier work remains preserved in the ledger and Git history; no overall modernization percentage is claimed because applicability has not been reviewed for every directory/task pair.
<!-- generated-snapshot:end -->
