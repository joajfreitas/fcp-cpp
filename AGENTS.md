# AGENTS.md — guidelines for coding agents working in this repo

These rules apply to any AI agent (or automated tooling) working here.

## Git workflow (strict)

- **Never push directly to the default branch.** All changes go:
  branch → commit → push → pull request → **stop**.
- **Never merge, close, or reassign the agent's own PRs.** Opening the PR
  is where the agent's job ends. Reviewing and merging is the maintainer's
  decision, every time, without exceptions.
- Requested changes are answered with new commits on the PR branch.
- Never force-push or rebase a PR branch; ask first.
- PRs are merged with merge commits, as-is: **squash merges are not
  used**. Every commit that lands on the default branch must pass
  gitlint on its own: ≤ 72-char title, no trailing punctuation, no
  "WIP", body required, body lines ≤ 80.
- **Keep commits atomic**: one logical change per commit, each
  self-contained and reviewable on its own; the message body explains
  that one change, not the whole PR.
- **No Conventional Commits.** Write titles as plain sentences, without
  `type:`, `type(scope):` or `type(scope)!` prefixes (enforced by the
  checked-in `.gitlint` config).
- **Never merge the default branch into feature branches.** To bring a
  feature branch up to date, rebase it — and ask before force-pushing.
- Don't touch branches owned by other PR authors.

## Repo hygiene

- Committed documentation and CI describe the repository, its build and
  its CI only — never a specific machine's local setup. Hardcoded
  usernames, absolute paths, and extra/fork remotes are none of the
  repo's business; do not mention them in committed files.
- Reason about remotes by role (upstream/primary vs. personal copy),
  never by names from any particular clone.

## CI (all required on the default branch)

- `Build` — `make fcp-cpp.so` (deps: `ccache`, `libyaml-cpp-dev`).
- `clang-format` — `make clang-format-check`, pinned to **clang-format 14**
  (Webkit style; newer versions reformat the vendored `src/json.hpp` —
  don't let that happen). Local check: install `clang-format==14.0.*`
  in a venv first.
- `gitlint` — commit messages in PRs: title ≤ 72, body required, body
  lines ≤ 80, and no Conventional Commits prefixes
  (`/tmp/gitlint/bin/gitlint --commits <default-branch>..HEAD`).

## Known landmines

- `src/main.cpp` does not compile on the default branch
  (designated-initializer errors against non-aggregate `CanData`).
  Don't add a build step for it to CI until it's fixed.
- The Makefile `test` target expects a `tests/` directory that doesn't
  exist (yet).
- `make clang-format-check` emits harmless `find: 'tests'` warnings.

## Change process

- New work: `git switch -c <type>/<scope-or-topic>` from an up-to-date
  default branch.
- PR descriptions should say what changed and why; keep them accurate.
- When PRs are open waiting for review, don't touch them unless asked.
