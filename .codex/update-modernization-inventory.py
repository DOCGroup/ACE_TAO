#!/usr/bin/env python3
"""Refresh the local ACE/TAO modernization inventory from tracked files."""

import argparse
import csv
import re
import subprocess
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
INVENTORY = Path(__file__).resolve().parent / "modernization-inventory.tsv"
TASK_STATUS = Path(__file__).resolve().parent / "modernization-task-status.tsv"
PROGRESS = Path(__file__).resolve().parent / "modernization-progress.md"
SOURCE_SUFFIXES = {
    ".c", ".cc", ".cpp", ".cxx", ".C", ".h", ".hh", ".hpp", ".hxx",
    ".inl", ".ipp", ".idl", ".l", ".ll", ".y", ".yy", ".mpc",
}
FIELDS = [
    "directory", "source_files", "mpc_files", "mpc_projects",
    "files_changed_from_base", "local_changed_files", "work_state",
    "audit_state", "tasks_done", "open_items", "validation", "last_commit",
]
PROJECT_RE = re.compile(r"(?m)^\s*project\s*\(")
TASKS = {
    "CONST": "East-side const and immutable locals/handles",
    "SCOPE_REDUCTION": "Keep variables and declarations in the narrowest useful scope",
    "VALUE_INIT": "Value initialization and simpler constructors",
    "NULLPTR": "Eligible nullptr modernization",
    "OVERRIDE_DEFAULT_DELETE": "override/default/delete and exported destructor placement",
    "OVERRIDE": "Missing override specifiers",
    "EXPORTED_DESTRUCTORS": "Out-of-line destructor symbols for exported classes",
    "EXPLICIT": "Single-argument constructor explicitness",
    "COPY_MOVE": "Copy/move support review",
    "ATOMIC": "Atomic state and memory-order review",
    "THREAD_SAFETY": "Synchronization and data-race review",
    "USING": "Suitable typedef-to-using conversions",
    "CONSTEXPR": "Internal constants and linkage",
    "OWNERSHIP": "Manual ownership and smart pointers",
    "CONTAINERS": "Manual arrays/containers and exception-safety cost",
    "CONTAINER_OWNERSHIP": "Standard containers and ownership of container elements",
    "C_STRING": "Plain C-string handling",
    "STD_NARROW_STRING": "Use std string functions for narrow C strings",
    "PREFIX_INC": "Prefer prefix increment and decrement when equivalent",
    "BRACES": "Consistent braces around control-flow bodies",
    "CONST_GUARD_MACROS": "Const guard macro usage while preserving ACE_GUARD conventions",
    "MOVE_SITES": "Audit std::move sites for moved-from use and ownership correctness",
    "MOVE_SEMANTICS": "Copy/move semantics and moved-from object safety",
    "SIZE_TYPES": "Count and size types",
    "IDL_GENERATION": "tao_idl/gperf inputs and generated output",
    "CLI_OPTIONS": "Grouped command-line settings",
    "LOG_FORMAT": "ACE logging format specifiers",
    "ACE_SIZE_LOG_FORMAT": "ACE fixed-width and size-oriented logging formats",
    "WINDOWS_CTOR_ACCESS": "Constructor access and ownership in Windows-specific classes",
    "TEST_ASSERTIONS": "Regression test assertions",
    "CLEANUP": "Unused helpers and stale commented code",
    "UNUSED_INCLUDES": "Unused include removal",
    "SPECIAL_MEMBER_COMMENTS": "Tree-wide removal of special-member label comments",
    "STYLE": "Blank lines, layout, line length, formatting",
    "BUILD_TEST": "Regeneration, realclean, focused build/test",
    "TEXT_API": "ACE_TCHAR and text-aware API usage",
    "WINDOWS_RESOURCE_LIFETIME": "Windows system resource and platform handle lifetime",
    "DIRECTORY_PASS": "Validated directory modernization batch",
}
TASK_FIELDS = ["directory", "task", "status", "evidence_commit", "notes"]
VALID_TASK_STATES = {"done", "not-applicable", "needs-work"}


def git(*args):
    return subprocess.check_output(["git", *args], cwd=ROOT, text=True)


def changed_paths(*args):
    output = git("diff", "--name-only", *args)
    return {line for line in output.splitlines() if line.startswith(("ACE/", "TAO/"))}


def read_task_status():
    if not TASK_STATUS.exists():
        return {}
    with TASK_STATUS.open(newline="") as source:
        return {
            (row["directory"], row["task"]): row
            for row in csv.DictReader(source, delimiter="\t")
        }


def write_task_status(task_rows):
    with TASK_STATUS.open("w", newline="") as destination:
        writer = csv.DictWriter(
            destination, fieldnames=TASK_FIELDS, delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        for key in sorted(task_rows):
            writer.writerow({field: task_rows[key].get(field) or "-" for field in TASK_FIELDS})


def write_progress(rows, task_rows, base):
    reviewed_by_task = Counter()
    state_by_task = {task: Counter() for task in TASKS}
    for row in task_rows.values():
        state_by_task[row["task"]][row["status"]] += 1
        reviewed_by_task[row["task"]] += 1

    batches = [row for row in rows if row["work_state"] == "batch-complete"]
    changed = sum(bool(int(row["files_changed_from_base"])) for row in rows)
    fully_audited = sum(row["audit_state"] == "audited" for row in rows)
    directories_with_task_records = len({row["directory"] for row in task_rows.values()})

    with PROGRESS.open("w") as output:
        output.write("# ACE/TAO modernization progress\n\n")
        output.write(f"Inventory base: `{base}`. Counts are directory-level, not executable/project counts.\n\n")
        output.write(
            f"Scope: **{len(rows)} directories**; **{changed}** differ from the base; "
            f"**{directories_with_task_records}** have task-level review records; "
            f"**{len(batches)}** have a recorded validated batch; **{fully_audited}** "
            "have a fully completed directory checklist. These are separate measures, "
            "not an overall completion percentage.\n\n"
        )
        output.write("## Progress by modernization task\n\n")
        output.write("| Task | Reviewed | Done | Not applicable | Needs work |\n")
        output.write("| --- | ---: | ---: | ---: | ---: |\n")
        for task, description in TASKS.items():
            counts = state_by_task[task]
            output.write(
                f"| `{task}` — {description} | {reviewed_by_task[task]} | "
                f"{counts['done']} | {counts['not-applicable']} | {counts['needs-work']} |\n"
            )

        output.write(
            "\n`SPECIAL_MEMBER_COMMENTS` remains a tree-wide follow-up even where a "
            "directory has a historical `done` or `not-applicable` row: revisit all "
            "source directories and generator inputs, including previously modernized "
            "areas. These counts show recorded directory reviews, not completion of "
            "the global sweep.\n"
        )

        output.write("\n## Recorded batches\n\n")
        output.write("| Directory | Completed task areas | Open items | Checklist | Commit |\n")
        output.write("| --- | --- | --- | --- | --- |\n")
        for row in batches:
            output.write(
                f"| `{row['directory']}` | {row['tasks_done']} | {row['open_items']} | "
                f"{row['audit_state']} | "
                f"`{row['last_commit']}` |\n"
            )
        output.write(
            "\nA task is counted only when its row is `done`, `not-applicable`, or `needs-work` "
            "in `modernization-task-status.tsv`. Missing rows mean **not reviewed**. "
            "The directory inventory keeps the scope and current open items; the ledger "
            "keeps the detailed change and validation history.\n"
        )


def update_status_snapshot(base, rows, mpc_files, total_projects, task_rows):
    status_file = Path(__file__).resolve().parent / "modernization-status.md"
    contents = status_file.read_text()
    start_marker = "<!-- generated-snapshot:start -->"
    end_marker = "<!-- generated-snapshot:end -->"
    if start_marker not in contents or end_marker not in contents:
        raise ValueError(f"Missing generated snapshot markers in {status_file}")

    changed = sum(bool(int(row["files_changed_from_base"])) for row in rows)
    touched_projects = sum(
        int(row["mpc_projects"]) for row in rows if int(row["files_changed_from_base"])
    )
    batches = sum(row["work_state"] == "batch-complete" for row in rows)
    full_audits = sum(row["audit_state"] == "audited" for row in rows)
    no_delta = sum(row["work_state"] == "no-branch-delta" for row in rows)
    recorded_task_count = len(task_rows)
    task_directories = len({row["directory"] for row in task_rows.values()})
    snapshot = (
        "\nThe inventory was generated against `" + base + "`. It contains "
        f"{len(rows):,} source/project directories, {mpc_files:,} MPC files, and "
        f"{total_projects:,} MPC project declarations. The branch differs from the base "
        f"in {changed:,} directories, containing {touched_projects:,} project declarations. "
        "These describe touched scope, not completion.\n\n"
        f"Task-level tracking currently has {recorded_task_count} reviewed task records "
        f"across {task_directories} directories; {batches} scoped batches are marked "
        f"validated, and {full_audits} directories have a full checklist audit. "
        f"There are {no_delta} directories with no branch delta. Earlier work remains "
        "preserved in the ledger and Git history; no overall modernization percentage is "
        "claimed because applicability has not been reviewed for every directory/task pair.\n"
    )
    start = contents.index(start_marker) + len(start_marker)
    end = contents.index(end_marker)
    status_file.write_text(contents[:start] + snapshot + contents[end:])


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base", default="origin/master")
    args = parser.parse_args()

    tracked = git("ls-files", "-z").split("\0")
    tracked = [Path(path) for path in tracked if path]
    tracked_code = [
        path for path in tracked
        if path.parts[0] in {"ACE", "TAO"} and path.suffix in SOURCE_SUFFIXES
    ]

    directory_data = {}
    for path in tracked_code:
        directory = path.parent.as_posix()
        record = directory_data.setdefault(
            directory, {"source_files": 0, "mpc_files": 0, "mpc_projects": 0}
        )
        record["source_files"] += 1
        if path.suffix == ".mpc":
            record["mpc_files"] += 1
            record["mpc_projects"] += len(PROJECT_RE.findall(path.read_text(errors="replace")))

    branch_changes = changed_paths(f"{args.base}...HEAD")
    local_changes = changed_paths("HEAD") | changed_paths("--cached", "HEAD")
    changed_by_directory = Counter(str(Path(path).parent) for path in branch_changes)
    local_by_directory = Counter(str(Path(path).parent) for path in local_changes)

    prior = {}
    if INVENTORY.exists():
        with INVENTORY.open(newline="") as source:
            prior = {
                row["directory"]: {
                    field: "" if value == "-" else value for field, value in row.items()
                }
                for row in csv.DictReader(source, delimiter="\t")
            }

    task_rows = read_task_status()
    # One-time migration: preserve every completed task already recorded in the inventory.
    for directory, old_row in prior.items():
        for task in old_row.get("tasks_done", "").split(";"):
            task = task.strip()
            if task in TASKS:
                key = (directory, task)
                task_rows.setdefault(key, {
                    "directory": directory,
                    "task": task,
                    "status": "done",
                    "evidence_commit": old_row.get("last_commit", "-"),
                    "notes": "Migrated from existing inventory; see modernization-ledger.md",
                })

    for key, row in task_rows.items():
        if row["task"] not in TASKS or row["status"] not in VALID_TASK_STATES:
            raise ValueError(f"Invalid modernization task record: {row}")

    rows = []
    for directory in sorted(directory_data):
        current = directory_data[directory]
        row = {field: "" for field in FIELDS}
        row.update(prior.get(directory, {}))
        row.update({
            "directory": directory,
            "source_files": str(current["source_files"]),
            "mpc_files": str(current["mpc_files"]),
            "mpc_projects": str(current["mpc_projects"]),
            "files_changed_from_base": str(changed_by_directory[directory]),
            "local_changed_files": str(local_by_directory[directory]),
        })
        recorded_tasks = {
            task: task_row["status"]
            for (task_directory, task), task_row in task_rows.items()
            if task_directory == directory and task_row["status"] in {"done", "not-applicable"}
        }
        if recorded_tasks:
            row["tasks_done"] = "; ".join(
                f"{task}=n/a" if recorded_tasks[task] == "not-applicable" else task
                for task in TASKS if task in recorded_tasks
            )
        if not row["work_state"]:
            row["work_state"] = (
                "work-recorded" if changed_by_directory[directory] else "no-branch-delta"
            )
        elif row["work_state"] == "not-started" and not changed_by_directory[directory]:
            row["work_state"] = "no-branch-delta"
        if not row["audit_state"]:
            row["audit_state"] = "not-audited"
        if not row["open_items"] and row["audit_state"] != "audited":
            row["open_items"] = "Directory checklist not yet fully audited"
        rows.append(row)

    # Keep historical rows for paths removed from the current tree.
    for directory, old_row in sorted(prior.items()):
        if directory not in directory_data:
            old_row["work_state"] = "removed-from-tree"
            rows.append(old_row)

    with INVENTORY.open("w", newline="") as destination:
        writer = csv.DictWriter(destination, fieldnames=FIELDS, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows({field: row.get(field) or "-" for field in FIELDS} for row in rows)

    write_task_status(task_rows)
    write_progress(rows, task_rows, args.base)

    tracked_projects = sum(int(row["mpc_projects"]) for row in rows)
    mpc_files = sum(int(row["mpc_files"]) for row in rows)
    update_status_snapshot(args.base, rows, mpc_files, tracked_projects, task_rows)
    changed_dirs = sum(bool(int(row["files_changed_from_base"])) for row in rows)
    complete_audits = sum(row["audit_state"] == "audited" for row in rows)
    active_batches = sum(row["work_state"] == "batch-complete" for row in rows)
    audited_projects = sum(
        int(row["mpc_projects"]) for row in rows if row["audit_state"] == "audited"
    )
    touched_projects = sum(
        int(row["mpc_projects"]) for row in rows if int(row["files_changed_from_base"])
    )
    total_dirs = len(directory_data)
    work_states = Counter(row["work_state"] for row in rows)
    audit_states = Counter(row["audit_state"] for row in rows)
    print(f"Inventory: {INVENTORY.relative_to(ROOT)}")
    print(f"Task status: {TASK_STATUS.relative_to(ROOT)}")
    print(f"Progress report: {PROGRESS.relative_to(ROOT)}")
    print(f"Source/project directories: {len(directory_data)}")
    print(f"Tracked MPC files / project declarations: {mpc_files} / {tracked_projects}")
    print(f"Directories changed from {args.base}: {changed_dirs}")
    print(f"Projects declared in changed directories: {touched_projects} / {tracked_projects}")
    print("Work states: " + ", ".join(f"{state}={count}" for state, count in sorted(work_states.items())))
    print(f"Batches marked complete: {active_batches}")
    print("Audit states: " + ", ".join(f"{state}={count}" for state, count in sorted(audit_states.items())))
    print(f"Directories fully audited: {complete_audits} / {total_dirs}")
    print(f"Projects in fully audited directories: {audited_projects} / {tracked_projects}")
    print("Changed directories are evidence of edits, not a claim that every task was reviewed.")


if __name__ == "__main__":
    main()
