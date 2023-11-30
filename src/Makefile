PROJECT       = Auto Power Switch
VERSION_MAJOR = 0
VERSION_MINOR = 1
CHIP          = STC8G1K17A
TARGET        = fw_$(CHIP)_$(VERSION_MAJOR)_$(VERSION_MINOR).hex

BUILD_DIR     = build
LIB_DIR       = lib
H_FILES       = *.h
RELS          = $(patsubst %.c,$(BUILD_DIR)/%.rel,$(wildcard *.c))
IHX_FILE      = $(BUILD_DIR)/fw.ihx
MEM_FILE      = $(BUILD_DIR)/fw.mem
LIB_FILE      = $(LIB_DIR)/static.lib
LIB_SRC_LIST  = $(LIB_DIR)/*.c $(LIB_DIR)/*.h
CC            = sdcc
IHX2HEX       = packihx

CFLAGS        = -DTARGET_CHIP_$(CHIP)
# CFLAGS       += -Werror
CFLAGS       += --nostdinc
LDFLAGS       = --nostdinc

ifeq ($(CHIP), STC8G1K17A)
LDFLAGS      += --xram-size 1024
LDFLAGS      += --code-size 17408
endif

all:check_env prepare lib $(TARGET) meminfo
	@echo "All done."

check_env:
	@$(CC) --version > /dev/null 2>&1 || exit 1

prepare:
	@mkdir -p $(BUILD_DIR)

.PHONY:lib
lib:$(LIB_SRC_LIST)
	@make -C $(LIB_DIR)

$(TARGET):$(IHX_FILE)
	@echo "Convert file "$(IHX_FILE)" to "$(TARGET)
	@$(IHX2HEX) $(IHX_FILE) > $(TARGET) 2>/dev/null

$(IHX_FILE):$(RELS) $(LIB_FILE)
	@echo "  LD  " $^
	@$(CC) $(LDFLAGS) $^ $(LIB_FILE) -o $(IHX_FILE)
	@echo "Generate file "$(IHX_FILE)

$(RELS):$(H_FILES)

$(BUILD_DIR)/%.rel:%.c
	@echo "  CC  " $<
	@$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/

.PHONY:meminfo
meminfo:
	@echo
	@echo "+-----------------------------------------------+"
	@echo "|     M E M O R Y     I N F O R M A T I O N     |"
	@echo "+-----------------------------------------------+"
	@echo
	@cat $(MEM_FILE)

.PHONY:clean
clean:
	@echo "Clear temporary files."
	@make -C $(LIB_DIR) clean
	@rm -rf $(BUILD_DIR)

.PHONY:disclean
disclean:clean
	@echo "Clear target file."
	@rm -f $(TARGET)
