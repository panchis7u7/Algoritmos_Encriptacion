CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG) --libs gtk4)
GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)
GLIB_COMPILE_SCHEMAS = $(shell $(PKGCONFIG) --variable=glib_compile_schemas gio-2.0)
SRC=src
OBJ=obj
BIN=bin
SRCS=$(wildcard $(SRC)/*.c)
BUILT_SRC = resources.c

#OBJS = $(OBJ)/$(BUILT_SRC:.c=.o) $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: mainapp

org.gtk.mainapp.gschema.valid: org.gtk.mainapp.gschema.xml
	$(GLIB_COMPILE_SCHEMAS) --strict --dry-run --schema-file=$< && mkdir -p $(@D)/ui && touch $@

gschemas.compiled: org.gtk.mainapp.gschema.valid
	$(GLIB_COMPILE_SCHEMAS) .

$(SRC)/resources.c: mainapp.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=. --generate-dependencies mainapp.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) mainapp.gresource.xml --target=$@ --sourcedir=. --generate-source

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c -o $(OBJ)/$(@F) $(CFLAGS) $<

mainapp: $(OBJS) gschemas.compiled
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f org.gtk.mainapp.gschema.valid
	rm -f gschemas.compiled
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f mainapp