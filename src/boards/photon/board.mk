#
# @file src/boards/photon/board.mk
# @version 6.0.0
#
# @section License
# Copyright (C) 2014-2016, Erik Moqvist
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# This file is part of the Simba project.
#

INC += $(SIMBA_ROOT)/src/boards/photon
SRC += $(SIMBA_ROOT)/src/boards/photon/board.c

# WiFi module driver from Particle IO.
INC += $(SIMBA_ROOT)/3pp/wiced/WWD/include
INC += $(SIMBA_ROOT)/3pp/wiced/WWD/internal/bus_protocols/SDIO
INC += $(SIMBA_ROOT)/3pp/wiced/WWD/internal/chips/43362A2
LDFLAGS_AFTER += -lwwdsdio
LIBPATH += "$(SIMBA_ROOT)/3pp/wiced/WWD/lib"

BOARD_HOMEPAGE = "https://docs.particle.io/datasheets/photon-datasheet/"
BOARD_PINOUT = "photon-pinout.png"
BOARD_DESC = "Particle IO Photon"

LIBPATH += "$(SIMBA_ROOT)/src/boards/$(BOARD)"
LINKER_SCRIPT ?= script.ld

MCU = stm32f205rg
SERIAL_PORT ?= /dev/arduino
BOARD_PY = $(SIMBA_ROOT)/src/boards/photon/board.py

upload:
	@echo "Uploading $(BIN)"
	python -u $(BOARD_PY) upload --port $(SERIAL_PORT) $(BIN)

rerun:
	@echo "Running $(EXE)"
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --baudrate $(BAUDRATE) \
	 		    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0
