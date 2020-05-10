ARCH_HEADER = jit-arch-@JIT_ARCH@.h

BUILT_SOURCES = jit-arch.h jit-opcode.h

libjitincludedir = $(includedir)/jit
dist_libjitinclude_HEADERS = \
	jit.h \
	jit-apply.h \
	jit-block.h \
	jit-common.h \
	jit-context.h \
	jit-debugger.h \
	jit-defs.h \
	jit-dump.h \
	jit-dynamic.h \
	jit-elf.h \
	jit-except.h \
	jit-function.h \
	jit-init.h \
	jit-insn.h \
	jit-intrinsic.h \
	jit-memory.h \
	jit-meta.h \
	jit-objmodel.h \
	jit-objmodel-private.h \
	jit-opcode-compat.h \
	jit-opcode.h \
	jit-plus.h \
	jit-type.h \
	jit-unwind.h \
	jit-util.h \
	jit-value.h \
	jit-vmem.h \
	jit-walk.h

nodist_libjitinclude_HEADERS = \
	jit-arch.h

noinst_HEADERS = jit-arch-generic.h jit-arch-x86.h jit-arch-x86-64.h

DISTCLEANFILES = jit-arch.h jit-defs.h jit-opcode.h

jit-arch.h: $(ARCH_HEADER)
	rm -f $@
	$(LN_S) $(srcdir)/$(ARCH_HEADER) $@

jit-opcode.h: $(top_srcdir)/jit/jit-opcodes.ops
	$(top_builddir)/tools/gen-ops -H $(top_srcdir)/jit/jit-opcodes.ops >jit-opcode.h
