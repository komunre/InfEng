TITLE_ID = VITAPSYOP
TARGET = vita_psyop

PSVITAIP = 192.168.1.163

OBJS = glad.o main.o model.o global.o genterrain.o

LIBS_GLOBAL = -lcglm -lfreetype -lpng -ljpeg -lz -lm -lc

LIBS_VITA = -lvitaGL -lSceDisplay_stub -lSceGxm_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lScePgf_stub \
	-lSceCommonDialog_stub -lfreetype -lpng -ljpeg -lz -lm -lc \
	-lmathneon -lvitashark -lSceShaccCgExt -ltaihen_stub \
	-lSceShaccCg_stub -lSceKernelDmacMgr_stub -lSceAppMgr_stub \
	-lSceCommonDialog_stub -lSceSysmem_stub -lm -L./lib/cglm/vita

LIBS_DESKTOP = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -I./lib/glad/include
LIBS = $(LIBS_DESKTOP)

#PREFIX = arm-vita-eabi
PREFIX=
CC = $(PREFIX)-gcc
CFLAGS = -Wl,-q -Wall -O3 -g -I./lib/cglm/include/
ASFLAGS = $(CFLAGS)

PARSECOREPATH = ~/vita-parse-core/main.py

all: $(TARGET).elf

vita: $(TARGET).vpk

send: $(TARGET).vpk
	echo destroy | nc $(PSVITAIP) 1338
	curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/
	curl -T param.sfo ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/sce_sys/
	echo launch $(TITLE_ID) | nc $(PSVITAIP) 1338

dump:
	./download_coredump.sh $(PSVITAIP)
	python2 $(PARSECOREPATH) ./coredump ./$(TARGET).elf

%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "$(TARGET)" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin $@

eboot.bin: $(TARGET).velf
	vita-make-fself -s $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS_GLOBAL) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ $(LIBS_GLOBAL) $(LIBS) -c -o $@

glad.o: lib/glad/src/glad.c
	$(CC) $(CFLAGS) $^ $(LIBS_GLOBAL) $(LIBS) -c -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(OBJS) \
		eboot.bin param.sfo
