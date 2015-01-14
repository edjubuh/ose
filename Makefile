# Universal C Makefile for MCU targets

# Path to project root (for top-level, so the project is in ./; first-level, ../; etc.)
ROOT=.
# Binary output directory
BINDIR=$(ROOT)/bin
# Subdirectories to include in the build
SUBDIRS=src
# Additional, manual flags that the user can input
# By default, left as blank
ADDITIONAL=

# Nothing below here needs to be modified by typical users

# Include common aspects of this project
-include $(ROOT)/common.mk

ASMSRC:=$(wildcard *.$(ASMEXT))
ASMOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(ASMSRC:.$(ASMEXT)=.o))
HEADERS:=$(wildcard *.$(HEXT))
CSRC=$(wildcard *.$(CEXT))
COBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CSRC:.$(CEXT)=.o))
CPPSRC:=$(wildcard *.$(CPPEXT))
CPPOBJ:=$(patsubst %.o,$(BINDIR)/%.o,$(CPPSRC:.$(CPPEXT)=.o))
OUT:=$(BINDIR)/$(OUTNAME)

.PHONY: all clean upload _force_look

# By default, compile program
all: clean $(BINDIR) $(OUT)

# compile the dios project
# dios:
#	cd $(ROOT)/dios/
#	$(MAKE) -C make

# Remove all intermediate object files (remove the binary directory)
clean:
	-rm -f $(OUT)
	-rm -f $(BINDIR)/*.o $(BINDIR)/*.bin

# Uploads program to device
upload: all
	$(UPLOAD)

# Phony force-look target
_force_look:
	@true

# Looks in subdirectories for things to make
$(SUBDIRS): %: _force_look
	@$(MAKE) --no-print-directory -C $@

# Ensure binary directory exists
$(BINDIR):
	-@mkdir -p $(BINDIR)

# Compile program
$(OUT): $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	@echo LN $(BINDIR)/*.o $(LIBRARIES) $(LIBSML) $(LIBLCD) to $@
	@$(CC) $(LDFLAGS) $(BINDIR)/*.o $(LIBRARIES) $(LIBSML) $(LIBLCD) -o $@
	@$(MCUPREFIX)size $(SIZEFLAGS) $(OUT)
	$(MCUPREPARE)

# Assembly source file management
$(ASMOBJ): $(BINDIR)/%.o: %.$(ASMEXT) $(HEADERS)
	@echo AS $<
	@$(AS) $(AFLAGS) -o $@ $<

# Object management
$(COBJ): $(BINDIR)/%.o: %.$(CEXT) $(HEADERS)
	@echo CC $(INCLUDE) $<
	@$(CC) $(INCLUDE) $(CFLAGS) -o $@ $<

$(CPPOBJ): $(BINDIR)/%.o: %.$(CPPEXT) $(HEADERS)
	@echo CPC $(INCLUDE) $<
	@$(CPPCC) $(INCLUDE) $(CPPFLAGS) -o $@ $<
