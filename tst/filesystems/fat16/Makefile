#
# @file Makefile
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

NAME = fat16_suite
BOARD ?= linux

TIMEOUT = 60

CDEFS += CONFIG_MINIMAL_SYSTEM=1 \
	CONFIG_FS_CMD_THRD_LIST=1

SIMBA_ROOT ?= ../../..
include $(SIMBA_ROOT)/make/app.mk
