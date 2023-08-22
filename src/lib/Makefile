VERSION_MAJOR = 0
VERSION_MINOR = 1
TARGET        = static.lib

STD_DIR       = std
BUILD_DIR     = build
H_FILES       = *.h
RELS          = $(patsubst %.c,$(BUILD_DIR)/%.rel,$(wildcard *.c))

CC            = sdcc
AR            = sdcclib

CFLAGS        =
# CFLAGS       += -Werror
CFLAGS       += --nostdinc
ARFLAGS       =

all:check_env prepare $(TARGET)
	@echo "Build the static library Successfully."

check_env:
	@$(CC) --version > /dev/null 2>&1 || exit 1

prepare:
	@mkdir -p $(BUILD_DIR)

$(TARGET):$(RELS)
	@echo "  AR  " $^
	@$(AR) $(ARFLAGS) $(TARGET) $^

$(RELS):$(H_FILES)

$(BUILD_DIR)/%.rel:%.c
	@echo "  CC  " $<
	@$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/

.PHONY:clean
clean:
	@rm -rf $(BUILD_DIR) $(TARGET)

