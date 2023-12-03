#   make LESSON=03-interrupts [flash]
#   make lessons | probe | clean
#
# LAYERS in a lesson's lesson.mk pulls in earlier lessons; a file in a later
# layer replaces the one at the same path in an earlier layer.

LESSON  ?= 01-boot
LESSONS := $(sort $(wildcard [0-9][0-9]-*))

ifeq ($(wildcard $(LESSON)/kmain.c),)
$(error Unknown lesson '$(LESSON)'. Available: $(LESSONS))
endif

LAYERS :=
-include $(LESSON)/lesson.mk
LAYERS += $(LESSON)

BUILD_DIR := build/$(LESSON)
TARGET    := $(BUILD_DIR)/duos
LINKER    := common/boot/linker.ld

# ---------------------------------------------------------------- sources

find_srcs = $(shell find $(1) -name '*.c' -o -name '*.S')
reverse   = $(if $(1),$(call reverse,$(wordlist 2,$(words $(1)),$(1))) $(firstword $(1)))

define add_layer
LAYER_SRCS := $$(filter-out $$(addprefix %/,$$(patsubst $(1)/%,%,$$(call find_srcs,$(1)))),$$(LAYER_SRCS)) $$(call find_srcs,$(1))
endef

LAYER_SRCS :=
$(foreach L,$(LAYERS),$(eval $(call add_layer,$(L))))

SRCS     := $(call find_srcs,common) $(LAYER_SRCS)
OBJS     := $(SRCS:%=$(BUILD_DIR)/%.o)
INC_DIRS := $(foreach L,$(call reverse,$(LAYERS)) common,$(shell find $(L) -type d))

# ---------------------------------------------------------------- tools

CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

ARCH    := -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16
OPT     ?= -Og
CFLAGS  := $(ARCH) $(OPT) -g3 -std=gnu17 -ffreestanding -Wall -Wextra \
	   -Wno-unused-parameter -MMD -MP $(addprefix -I,$(INC_DIRS))
ASFLAGS := $(ARCH) -g3 -MMD -MP $(addprefix -I,$(INC_DIRS))
# libgcc for the double maths in %f
LDFLAGS := $(ARCH) -nostdlib -T $(LINKER) -Wl,-Map=$(TARGET).map
LDLIBS  := -lgcc

# ---------------------------------------------------------------- rules

.PHONY: all lessons flash probe clean

all: $(TARGET).bin

$(TARGET).elf: $(OBJS) $(LINKER)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

flash: $(TARGET).bin
	st-flash --reset write $< 0x08000000

probe:
	st-info --probe --connect-under-reset

lessons:
	@for l in $(LESSONS); do $(MAKE) --no-print-directory LESSON=$$l || exit 1; done

clean:
	rm -rf build

-include $(OBJS:.o=.d)
