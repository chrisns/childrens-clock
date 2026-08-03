# ponytail: this repo's only C code is header-only (see src/childrensclock.h,
# included straight into ESPHome's build and unit-tested via Ceedling), so
# there's no top-level build system for CodeQL's C/C++ autobuilder to find.
# Give it one trivial target so it can actually extract and scan the code.
all:
	$(CC) -x c -c src/childrensclock.h -o /dev/null
