# REPORT.md

## 1. Explain the linking rule in this part's Makefile: `$(TARGET): $(OBJECTS)`. How does it differ from a Makefile rule that links against a library?

```makefile
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)
```

This follows the standard makefile rule format discussed in the lecture: `target: dependency1 dependency2 … dependencyN` followed by a `<tab> command`. Here, `$(TARGET)` (the final executable, `bin/client`) is the **target**, and `$(OBJECTS)` (i.e. `main.o`, `mystrfunctions.o`, `myfilefunctions.o`) are its **dependencies** — the files that are needed to build the target. As per how `make` works, `make` checks the timestamps of these dependencies: if any object file is newer than the target (or the target doesn't exist yet), the target is considered out of date, and the **command** on the next line is executed to rebuild it. That command invokes `gcc` to **link** all the object files together into a single executable.

This is a **direct object-file linking rule**: the compiler is simply combining pre-compiled `.o` files (our own code) into one binary. No external library code is being pulled in beyond what we wrote ourselves and the standard C library (linked implicitly by the compiler).

A rule that links against a **library** would differ in that it needs to tell the linker where to find the library and which one to use, typically using two extra flags:

```makefile
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -L/path/to/lib -lmylibrary
```

- **`-L`** specifies the directory to search for library files.
- **`-l`** specifies the library name to link against (e.g. `-lm` for the math library, which links `libm.so`, or `-lpthread` for POSIX threads).

So the key difference is: our current rule links only our own object files together, while a library-linking rule additionally tells the linker to pull in externally compiled code (static `.a` or shared `.so` libraries) using the `-L` and `-l` flags.

---

## 2. What is a git tag and why is it useful in a project? What is the difference between a simple tag and an annotated tag?

A **git tag** is a fixed pointer/marker attached to a specific commit in a repository's history, usually used to mark important points such as release versions (e.g. `v1.0`, `v2.1.3`). Unlike branches, tags don't move — once created, they permanently point to that exact commit.

**Why tags are useful:**
- They make it easy to identify and return to a specific stable version of the project (e.g. "the code exactly as it was for release v1.0").
- Useful for releases, so users and developers can download or check out a known working version instead of guessing which commit was stable.
- They provide clear checkpoints in project history for debugging, rollback, or comparison between versions.

**Simple tag vs Annotated tag:**

| | Simple Tag | Annotated Tag |
|---|---|---|
| Command | `git tag v1.0` | `git tag -a v1.0 -m "message"` |
| Stored as | Just a pointer to a commit | A full Git object with its own metadata |
| Contains | Nothing extra | Tagger name, email, date, and message |
| Use case | Quick, temporary local markers | Official releases, permanent records |

A **simple (lightweight) tag** is just a named reference to a commit, with no additional information — essentially just a label.

An **annotated tag** is a full object stored in Git's database, including the tagger's name, email, date, and a message (much like a commit object). Annotated tags are recommended for releases because they carry metadata and can also be **GPG-signed** for verification, making them more suitable for official, traceable versioning.

---

## 3. What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries (like your client executable) to it?

A **GitHub Release** is a way to package and publish a specific, tagged version of a project in a user-friendly format on the repository's page. It builds on top of a git tag, adding a title, description (release notes), and the ability to attach files — turning a raw commit/tag into a formal, distributable version of the software.

**Purpose of a Release:**
- Clearly communicates to users and collaborators which version is considered stable/ready for use.
- Provides a changelog/description explaining what's new or fixed in that version.
- Makes it easy for others to find and download a specific version without needing to clone the whole repository or navigate git history.

**Significance of attaching binaries:**

Attaching a compiled binary (like our `bin/client` executable) means users can **download and run the program directly**, without needing to have a compiler, the source code, or build tools installed. This is especially important because:
- Not everyone downloading the project wants to compile it themselves.
- It separates **source code distribution** (via the repo) from **software distribution** (via the release binary).
- It ensures the exact tested, working version of the compiled program is available, matching the specific tagged commit — so there's no ambiguity about which source state the binary corresponds to.
