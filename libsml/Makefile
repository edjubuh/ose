ROOT=..
SML=$(ROOT)/libsml
FIRMWARE=$(ROOT)/firmware

-include $(ROOT)/common.mk

SOURCES=$(wildcard $(SML)/*.c)
OBJECTS=$(SOURCES:.c=.o)
ARCHIVE=libsml.a
ARCHIVER=ar
ARCHFLAGS=rvs

.PHONY: all copy clean

all: $(SOURCES) $(ARCHIVE) copy clean
	

$(ARCHIVE): $(OBJECTS)
	$(ARCHIVER) $(ARCHFLAGS) $(ARCHIVE) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

copy:
	cp $(SML)/$(ARCHIVE) $(FIRMWARE)/

clean:
	rm -f $(SML)/*.o $(SML)/*.a
