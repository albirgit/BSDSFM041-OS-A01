# REPORT.md

## Feature-2: Multi-file Project

### Q1. Explain the linking rule in this part's Makefile: `$(TARGET): $(OBJECTS)`. How does it differ from a Makefile rule that links against a library?

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

### Q2. What is a git tag and why is it useful in a project? What is the difference between a simple tag and an annotated tag?

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

### Q3. What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries (like your client executable) to it?

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

---

## Feature-3: Static Library

### Q1. Compare the Makefile from Part 2 and Part 3. What are the key differences in the variables and rules that enable the creation of a static library?

**Part 2 Makefile (`src/makefile` — direct multifile build):**
```makefile
CFLAGS += -I$(INCLUDE_DIR)

SOURCES = main.c mystrfunctions.c myfilefunctions.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = $(BIN_DIR)/$(TARGET)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
```

**Part 3 Makefile (`src/makefile` — static library build):**
```makefile
CFLAGS += -I$(INCLUDE_DIR)

LIB_OBJECTS = mystrfunctions.o myfilefunctions.o
MAIN_OBJECT = main.o

STATIC_LIB = $(LIB_DIR)/lib$(LIBNAME).a
EXECUTABLE = $(BIN_DIR)/$(TARGET)

all: $(EXECUTABLE)

$(STATIC_LIB): $(LIB_OBJECTS)
	ar rcs $@ $^

$(EXECUTABLE): $(MAIN_OBJECT) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJECT) -L$(LIB_DIR) -l$(LIBNAME)
```

**Key differences:**

1. **New variables introduced:** `LIB_DIR` and `LIBNAME` are added, and the single `OBJECTS` variable is split into two separate groups: `LIB_OBJECTS` (the utility functions that go into the library) and `MAIN_OBJECT` (the driver code that uses the library). This separation is necessary because these two groups of object files are treated completely differently during the build.

2. **A new intermediate target — the static library itself:** Part 2 has only one build target (the executable). Part 3 introduces an additional target, `$(STATIC_LIB)`, which is built *before* the executable. Its rule uses the `ar rcs` command instead of `gcc`, since we are archiving object files into a `.a` file rather than compiling or linking them into an executable.

3. **The linking rule changes from direct object linking to library linking:** In Part 2, the executable's rule links the object files directly:
   ```makefile
   $(CC) $(CFLAGS) -o $@ $^
   ```
   In Part 3, the executable's rule instead links against the archive using `-L` and `-l`:
   ```makefile
   $(CC) $(CFLAGS) -o $@ $(MAIN_OBJECT) -L$(LIB_DIR) -l$(LIBNAME)
   ```
   This tells the linker to search `$(LIB_DIR)` for a library named `lib$(LIBNAME).a` and pull in only the object code needed to resolve `main.o`'s function calls, rather than the object files being passed to the compiler directly.

4. **Dependency chain becomes deeper:** In Part 3, `$(EXECUTABLE)` now depends on `$(STATIC_LIB)` as well as `$(MAIN_OBJECT)`, so `make` must first ensure the static library is up to date before it attempts to link the final executable — this is what makes the recursive dependency-based rebuild work correctly whenever any of the utility source files change.

---

### Q2. What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?

The **`ar`** (archiver) command is used to create, modify, and extract from **archive files** — most commonly to bundle multiple compiled object files (`.o`) into a single **static library** file (conventionally named `libname.a`). In this project, `ar rcs lib/libmyutils.a mystrfunctions.o myfilefunctions.o` combines the two object files into one archive that can later be linked into any program using `-lmyutils`.

The flags used:
- **`r`** — insert/replace the given object files into the archive
- **`c`** — create the archive if it doesn't already exist
- **`s`** — write (or update) an **index/symbol table** inside the archive

**Why `ranlib` is often used immediately after `ar`:**

When a static library is built, the linker needs a fast way to know *which object file inside the archive defines which function/symbol*, so that when it's resolving an undefined reference (e.g. `mystrlen`), it can quickly find the right `.o` file to pull in — without having to scan every object file in the archive one by one.

`ranlib` generates (or regenerates) this **symbol index** inside the archive. Historically, `ar` alone did not create this index, so `ranlib libname.a` had to be run separately afterward to make the archive linkable. On modern systems, the `s` flag in `ar rcs` already performs the same job internally (essentially calling `ranlib` automatically), which is why our command already includes `s` — but it's still common practice, especially in older scripts, tutorials, and portable build systems, to run `ranlib` explicitly right after `ar` to guarantee the index exists and is current, regardless of which `ar` flags were used.

---

### Q3. When you run `nm` on your `client_static` executable, are the symbols for functions like `mystrlen` present? What does this tell you about how static linking works?

Yes — when running `nm bin/client_static`, symbols like `mystrlen`, `mystrcpy`, `mystrncpy`, `mystrcat`, `wordCount`, and `mygrep` **are present**, typically marked with the type **`T`** (defined in the text/code section of the executable).

This confirms how **static linking** works: unlike dynamic linking (where a program merely references an external `.so` file that is loaded at runtime), static linking **physically copies the machine code** of every function that is actually used from the `.a` archive **directly into the final executable** at link time. The linker looks through `libmyutils.a`, finds the object files that define the symbols `main.o` needs (`mystrfunctions.o` and `myfilefunctions.o`), and merges their compiled code into `client_static`.

The practical implications of this are:
- The resulting executable is **self-contained** — it does not need `libmyutils.a` to be present on the system at runtime in order to run, since the code has already been embedded into the binary.
- The executable's file size is **larger** than it would be with dynamic linking, since it carries a full copy of the library code rather than just a reference to it.
- Because the code is copied in at compile/link time (not resolved at runtime), running `nm` on the executable shows these function symbols as fully **defined** (`T`) rather than **undefined** (`U`) — undefined symbols are only expected for functions that come from *shared* system libraries (like `printf` from libc, which is typically dynamically linked unless the whole binary is statically compiled).

---

## Feature-4: Dynamic Library

### Q1. What is Position-Independent Code (-fPIC) and why is it a fundamental requirement for creating shared libraries?

**Position-Independent Code (PIC)** is machine code that does not depend on being loaded at any specific, fixed memory address. Normally, when a compiler generates code, it can assume the program will always be loaded at the same base address, and it may hard-code absolute memory addresses for functions, global variables, and jump targets. PIC avoids this by using **relative addressing** (offsets from the current instruction pointer, or through an indirection table) instead of absolute addresses, so the same compiled code can correctly run no matter where in memory it ends up being placed.

The `-fPIC` flag tells `gcc` to generate this kind of relocatable, address-independent machine code when compiling the object files (`mystrfunctions_pic.o`, `myfilefunctions_pic.o` in this project).

**Why it's fundamental for shared libraries:**

A shared library (`.so`) is not loaded at a fixed address. Multiple different programs may load the same `.so` file at the same time, and the operating system may place it at a **different base address in each process's address space** (this also enables security features like ASLR — Address Space Layout Randomization). If the library's code contained hard-coded absolute addresses, it would only work correctly when loaded at the one specific address it was compiled for — which is not guaranteed for a shared library that many independent programs load simultaneously, each with their own memory layout.

By compiling the library's object files with `-fPIC`, the resulting `.so` can be safely loaded at **any** base address in any process, and its internal function calls and data references will still resolve correctly because they're computed relative to wherever the library actually landed in memory — not to a fixed address baked in at compile time.

Static libraries (`.a`) don't need this, because their object code is copied directly into the final executable at link time and becomes part of that one program's fixed layout — it never needs to be relocated independently at runtime the way a shared library does.

---

### Q2. Explain the difference in file size between your static and dynamic clients. Why does this difference exist?

In this project:
```
client_static   20K
client_dynamic  17K
```

`client_static` is larger because, during static linking, the linker **copies the actual machine code** of every function used from `libmyutils.a` (`mystrlen`, `mystrcpy`, `mystrncpy`, `mystrcat`, `wordCount`, `mygrep`, etc.) directly into the executable file itself. The executable becomes a fully self-contained binary that includes both `main.c`'s code *and* a full copy of the utility library's code.

`client_dynamic` is smaller because, during dynamic linking, the executable does **not** contain a copy of the library's code at all. Instead, it only contains:
- References/stubs to the functions it needs (like `mystrlen`, `wordCount`, etc.)
- A note in its dynamic section that it depends on `libmyutils.so`

The actual function code stays entirely inside `libmyutils.so`, which is loaded into memory separately by the operating system's dynamic loader when the program starts (and can be shared in memory across multiple running programs that use the same library, rather than each program carrying its own private copy).

This is the core space/efficiency trade-off between static and dynamic linking: static linking produces a larger but fully self-contained executable, while dynamic linking produces a smaller executable that depends on an external `.so` file being present and loadable at runtime.

---

### Q3. What is the LD_LIBRARY_PATH environment variable? Why was it necessary to set it for your program to run, and what does this tell you about the responsibilities of the operating system's dynamic loader?

**`LD_LIBRARY_PATH`** is an environment variable that tells the dynamic loader (`ld.so` / `ld-linux.so`) **additional directories to search** when looking for shared libraries (`.so` files) that a program depends on, before falling back to the standard system library paths (like `/lib`, `/usr/lib`, or the paths listed in `/etc/ld.so.conf`).

**Why it was necessary here:**

When `client_dynamic` was run without setting it, the program failed with an error like:
```
error while loading shared libraries: libmyutils.so: cannot open shared object file: No such file or directory
```

This happened because `libmyutils.so` lives in our project's own `lib/` directory — a **non-standard location** that the dynamic loader does not search by default. The executable itself only stores the *name* `libmyutils.so` (not the full path) as a dependency, so at runtime, the loader has no way of knowing where to find it unless it's told explicitly.

By running:
```bash
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
```
we added our project's `lib/` directory to the loader's search path, allowing it to locate and load `libmyutils.so` successfully. After this, `ldd bin/client_dynamic` correctly resolved the dependency to our local file (`libmyutils.so => /home/.../lib/libmyutils.so`).

**What this reveals about the dynamic loader's responsibilities:**

Unlike static linking — where all the necessary code is bundled into the executable at compile time and nothing further is needed at runtime — dynamic linking defers the job of locating and loading library code until the program actually starts running. This means the operating system's dynamic loader is responsible for:
- Reading the executable's list of required shared libraries
- Searching a defined set of directories (standard system paths, plus any paths from `LD_LIBRARY_PATH`, `rpath`/`runpath` embedded in the binary, and the system's library cache) to find each one
- Mapping each shared library into the process's memory at runtime
- Resolving the addresses of the functions/symbols the executable needs from those libraries (relocation)

If any required shared library cannot be found in any of these locations, the loader refuses to start the program at all — which is exactly the error we saw before setting `LD_LIBRARY_PATH`. This makes clear that a dynamically linked executable is not truly "complete" on its own; it has an implicit runtime contract with the operating system's loader to correctly locate its dependencies every time it's launched.
