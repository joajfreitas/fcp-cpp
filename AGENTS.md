# AGENTS.md — guidelines for coding agents working in this repo

These rules apply to any AI agent (or automated tooling) working with
joajfreitas here. They exist because the agent previously pushed directly
to `master` and merged its own PRs without being asked to. Don't do that.

## Git workflow (strict)

- **Never push directly to `master`** (or the `gitlab` remote). All changes
  go: branch → commit → push → pull request → **stop**.
- **Never merge, close, or push to the agent's own PRs.** Opening the PR is
  where the agent's job ends. Reviewing and merging is joajfreitas's
  decision, every time, without exceptions.
- Ask before force-pushing or rebasing a shared/open PR branch.
- Squash-merge is the merge style of record (linear history is required).
  The PR title becomes the merge commit message, so titles must pass
  gitlint: ≤ 72 chars, no trailing punctuation, no "WIP".
- Don't touch branches owned by joajfreitas or other PR authors, and don't
  reassign/close their PRs.

## Remotes

- `origin` = GitHub (`joajfreitas/fcp-cpp`) — the only active remote. Its
  `master` is branch-protected (requires PR + green CI; applies to admins).
- `gitlab` = old GitLab remote, **archived**. Never push to it. Kept
  read-only for reference.

## CI (all required on `master`)

- `Build` — `make fcp-cpp.so` (deps: `ccache`, `libyaml-cpp-dev`).
- `clang-format` — `make clang-format-check`, pinned to **clang-format 14**
  (Webkit style; newer versions reformat the vendored `src/json.hpp` —
  don't let that happen). Local check:
  `pip install clang-format==14.0.*` in a venv first.
- `gitlint` — commit messages in PRs: title ≤ 72, body required,
  body lines ≤ 80 (`gitlint --commits origin/master..HEAD`).

## Known landmines

- `src/main.cpp` does not compile on master (designated-initializer errors
  against non-aggregate `CanData`). Don't add a build step for it to CI
  until it's fixed.
- The Makefile `test` target expects a `tests/` directory that doesn't
  exist (yet).
- `make clang-format-check` emits harmless `find: 'tests'` warnings.

## Change process

- New work: `git switch -c <type>/<scope-or-topic>` from up-to-date master.
- PR descriptions should say what changed and why; keep them accurate.
- When PRs are open waiting for review, don't touch them unless asked.
