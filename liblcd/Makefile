ROOT=..
LCD=$(ROOT)/liblcd
FIRMWARE=$(ROOT)/firmware

-include $(ROOT)/common.mk

SOURCES=$(wildcard $(LCD)/*.c)
OBJECTS=$(SOURCES:.c=.o)
ARCHIVE=liblcd.a
ARCHIVER=ar
ARCHFLAGS=rvs

.PHONY: all copy clean

all: $(SOURCES) $(ARCHIVE) copy clean
	

$(ARCHIVE): $(OBJECTS)
	$(ARCHIVER) $(ARCHFLAGS) $(ARCHIVE) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

copy:
	cp $(LCD)/$(ARCHIVE) $(FIRMWARE)/

clean:
	rm -f $(LCD)/*.o $(LCD)/*.a
