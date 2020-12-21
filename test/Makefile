BIN ?= bin

CFLAGS += -O2
LIBS += -lbm
LIBS += -lm

tests = $(patsubst %.c,$(BIN)/%,$(wildcard *.c))

.PHONY: all clean

all: $(tests)

$(BIN)/%: %.c
	@mkdir -p "$(dir $@)"
	$(CXX) $^ -o $@ $(CFLAGS) $(LIBS)

clean:
	rm -rf $(BIN)

