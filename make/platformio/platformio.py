#!/usr/bin/env python

from __future__ import print_function

import subprocess
import argparse
import json
import errno
import os
import shutil
import fnmatch
import hashlib


SIMBA_GEN_C_FMT = """
#include "simba.h"

const FAR char sysinfo[] = "app:    myapp built - by -.\\r\\n"
                           "board:  {board}\\r\\n"
                           "mcu:    {mcu}\\r\\n";
"""

PLATFORMIO_SCONSSCRIPT_FMT = """#
# @file make/platformio.sconscript
# @version {version}
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

import os
from os.path import join
import subprocess
import shutil

from SCons.Script import DefaultEnvironment
from platformio.builder.tools.platformio import SRC_DEFAULT_FILTER

# Map the PlatformIO board name to the Simba board name
BOARD_MAP = {{
    "due": "arduino_due",
    "megaatmega2560": "arduino_mega",
    "nanoatmega328": "arduino_nano",
    "uno": "arduino_uno"
}}

# Map the PlatformIO board name to the Simba board name
SUPPORTED_BOARDS = [
    "arduino_due",
    "arduino_mega",
    "arduino_nano",
    "arduino_uno",
    "esp12e",
    "esp01"
]


BOARDS = {boards}

def add_include_paths(env, paths):
    \"\"\"Add given list of include paths.

    \"\"\"

    for path in paths:
        env.VariantDirWrap(
            join("$BUILD_DIR", "SimbaFramework", path),
            join("$PLATFORMFW_DIR", path)
        )
        env.Append(CPPPATH=[join("$BUILD_DIR", "SimbaFramework", path)])


def set_default_values(env):
    \"\"\"Set default values in the environment.

    \"\"\"

    if "VERSION" not in env:
        with open(env.subst(join("$PLATFORMFW_DIR", "VERSION.txt"))) as f:
            env.Append(VERSION=f.read().strip())

    if "NAME" not in env:
        env.Append(NAME="app_name")


def setup_mcu_esp(env, linker_script, flash_size_map):
    \"\"\"Setup the ESP environment.

    \"\"\"

    builders = {{}}

    for key, value in env["BUILDERS"].items():
        if key not in ["ElfToBin"]:
            builders[key] = value

    def build_function(target, source, env):
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .text -O binary "
            + str(source[0]) + " eagle.app.v6.text.bin",
            shell=True)
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .data -O binary "
            + str(source[0]) + " eagle.app.v6.data.bin",
            shell=True)
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .rodata -O binary "
            + str(source[0]) + " eagle.app.v6.rodata.bin",
            shell=True)
        subprocess.check_call(
            "xtensa-lx106-elf-objcopy --only-section .irom0.text -O binary "
            + str(source[0]) + " eagle.app.v6.irom0text.bin",
            shell=True)
        subprocess.check_call(
            env.subst('$PYTHONEXE $PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/tools/gen_appbin.py') + " " + str(source[0]) + " 2 0 0 " + flash_size_map,
            shell=True)
        shutil.copy("eagle.app.flash.bin", str(target[0]))

    builders["ElfToBin"] = env.Builder(
        action=build_function,
        suffix=".bin")

    env.Replace(BUILDERS=builders)

    env.Replace(UPLOADCMD=' '.join([
        '"$PYTHONEXE" "$PLATFORMFW_DIR/3pp/esptool/esptool.py"',
        '--baud 230400',
        '--port $UPLOAD_PORT',
        'write_flash',
        '0x00000 $PLATFORMFW_DIR/3pp/ESP8266_RTOS_SDK/bin/boot_v1.4.bin',
        '0x01000 $BUILD_DIR/firmware.bin']))



def create_src_filter(srcs):
    \"\"\"Create the source filter string.
    \"\"\"

    src_filter = [
        "-<3pp/>",
        "-<src/>",
        "-<tst/>"
    ]

    for src in srcs:
        src_filter.append("+<{{}}>".format(src))

    return SRC_DEFAULT_FILTER + " " + " ".join(src_filter)


def setup_mcu_esp(env, linker_script, flash_size_map):
    \"\"\"Setup the ESP environment.

    \"\"\"

    linkflags = []

    for flag in env["LINKFLAGS"]:
        if "-Wl,-wrap,register_chipv6_phy" in flag:
            continue
        linkflags.append(flag)
    env.Replace(LINKFLAGS=linkflags)

    ccflags = []

    for flag in env["CCFLAGS"]:
        if "-Werror" in flag:
            continue
        ccflags.append(flag)
    env.Replace(CCFLAGS=ccflags)

    env.Append(LIBS=[
        "-lminic",
        "-lgcc",
        "-lhal",
        "-lphy",
        "-lpp",
        "-lnet80211",
        "-lwpa",
        "-lcrypto",
        "-lmain",
        "-lfreertos",
        "-llwip"
    ])

    env.Append(
        BUILDERS=dict(
            ElfToBin=Builder(
                action=" ".join([
                    '"$OBJCOPY"',
                    "-eo",
                    '"%s"' % join("$PLATFORMFW_DIR", "3pp", "esp8266Arduino",
                                  "2.3.0", "bootloaders", "eboot", "eboot.elf"),
                    "-bo", "$TARGET",
                    "-bm", "$BOARD_FLASH_MODE",
                    "-bf", "${{__get_board_f_flash(__env__)}}",
                    "-bz", "${{__get_flash_size(__env__)}}",
                    "-bs", ".text",
                    "-bp", "4096",
                    "-ec",
                    "-eo", "$SOURCES",
                    "-bs", ".irom0.text",
                    "-bs", ".text",
                    "-bs", ".data",
                    "-bs", ".rodata",
                    "-bc", "-ec"
                ]),
                suffix=".bin"
            )
        )
    )


def setup_board_esp12e(env):
    \"\"\"Setup the ESP12E environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.4m.ld", "6")


def setup_board_esp01(env):
    \"\"\"Setup the ESP01 environment.

    \"\"\"

    setup_mcu_esp(env, "simba.flash.1m.ld", "2")


env = DefaultEnvironment()

set_default_values(env)

# Rename the name of boards that have different name in PlatformIO and
# Simba.
if env["BOARD"] in BOARD_MAP:
    env["BOARD"] = BOARD_MAP[env["BOARD"]]

board = env.subst(env["BOARD"])

if board not in SUPPORTED_BOARDS:
   raise ValueError("BOARD {{}} is not supported by Simba.".format(board))

# Add the default configuration for the board.
add_include_paths(env, BOARDS[board]["inc"])
env.Replace(CPPDEFINES=BOARDS[board]["cdefs"])
src_filter = create_src_filter(BOARDS[board]["src"])
env.Replace(CCFLAGS=BOARDS[board]["cflags"])
env.Replace(LINKFLAGS=BOARDS[board]["ldflags"])
env.Replace(LIBPATH=[os.path.join("$PLATFORMFW_DIR", path)
                     for path in BOARDS[board]["libpath"]])
board_options = env["BOARD_OPTIONS"]
board_options['build']['ldscript'] = "script.ld"
env.Replace(BOARD_OPTIONS=board_options)
env.Replace(MCU_DESC=BOARDS[board]["mcu_desc"])
env.Replace(BOARD_DESC=BOARDS[board]["board_desc"])

# Always replace the map file path.
linkflags = []

for flag in env["LINKFLAGS"]:
    if flag.startswith("-Wl,-Map="):
        flag = "-Wl,-Map=$BUILD_DIR/firmware.map"
    linkflags.append(flag)
env.Replace(LINKFLAGS=linkflags)

# For some boards the configuration has to be modified.
if board == "arduino_due":
    linkflags = []

    for flag in env["LINKFLAGS"]:
        if flag.startswith("-T"):
            continue
        linkflags.append(flag)
    env.Replace(LINKFLAGS=linkflags)
elif board == "esp12e":
    setup_board_esp12e(env)
elif board == "esp01":
    setup_board_esp01(env)

# generated files
SIMBA_GEN_C = "$BUILD_DIR/SimbaFramework/simba_gen.c"

# create a list of all sources
variant_dir = join("$BUILD_DIR", "SimbaFramework")
src_dir = join("$PLATFORMFW_DIR")

source_files = []

for src in env.LookupSources(variant_dir, src_dir, True, src_filter):
    source_files.append(env.Object(src))

# Command to generate simba_gen.c
env.Command(SIMBA_GEN_C,
            source_files,
            ('"$PYTHONEXE" "$PLATFORMFW_DIR/src/kernel/tools/gen.py" "$NAME" "$VERSION" '
             '"$BOARD_DESC" "$MCU_DESC" "$TARGET" $SOURCES'))
source_files.append(SIMBA_GEN_C)

lib = env.Library(target=join("$BUILD_DIR", "SimbaFramework"), source=source_files)

env.Append(LIBS=[lib])
"""


def create_database():
    """Generate the simba database with baord and mcu information.

    """

    return json.loads(subprocess.check_output(["dbgen.py"]))


def generate_platformio_sconsscript(database, version):
    """Generate the platformio scons script.

    """

    simba_root = os.environ["SIMBA_ROOT"]
    boards = {}

    # Only add selceted parts the database to the scons script for
    # less unnecessary information.
    for board, data in database["boards"].items():
        # Add everything we need, and a bit more.
        selected_data = {
            'inc': data['inc'],
            'cdefs': data['cdefs'],
            'src': data['src'],
            'cflags': data['cflags'],
            'libpath': data['libpath'],
            'ldflags': data['ldflags'],
            'linker_script': data['linker_script'],
            'board_desc': data['board_desc'],
            'mcu_desc': database['mcus'][data['mcu']]['mcu_desc']
        }
        boards[board] = selected_data
        
    outfile = os.path.join(simba_root, "make", "platformio.sconscript")
    with open(outfile, "w") as fout:
        fout.write(PLATFORMIO_SCONSSCRIPT_FMT.format(
            version=version,
            boards=json.dumps(boards, indent=4, sort_keys=True)))


def main():
    """Create the platformio build script.

    """

    parser = argparse.ArgumentParser()
    parser.add_argument("--version", required=True)
    args = parser.parse_args()

    print("Creating software database.")
    database = create_database()

    generate_platformio_sconsscript(database, args.version)


if __name__ == "__main__":
    main()
