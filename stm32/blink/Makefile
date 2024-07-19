PROJECT = blink
OPENCM3_DIR=../libopencm3

CFILES = blink.c

# TODO - you will need to edit these two lines!
DEVICE=STM32F103C8T6
ARCH_FLAGS = -mcpu=cortex-m3 -mthumb -msoft-float

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
