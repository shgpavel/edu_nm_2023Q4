CFLAGS := -Wall -Wextra -O1 -std=c2x $(CFLAGS)
LDFLAGS := -lm -ljemalloc -lcurl $(LDFLAGS)

CC = clang
LD = clang

OUT_O_DIR = build
SRCS_DIRS = . methods types draw
SRCS = $(wildcard $(addsuffix /*.c, $(SRCS_DIRS)))
OBJS := $(addprefix $(OUT_O_DIR)/,$(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

all: $(OUT_O_DIR)/main

$(OUT_O_DIR)/main: $(OBJS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJS): $(OUT_O_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS): $(OUT_O_DIR)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

clean:
	rm -rf $(OBJS) $(DEPS) $(OUT_O_DIR)
	rm -f lagr.csv newton.csv lin.csv quad.csv qube.csv
	rm -f newtonp.csv matrix.csv s20.csv

NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
