#include "config.h"


const uint8_t PRNE_PROG_VER[16] = { 0x11, 0xf7, 0x6b, 0x87, 0x62, 0x1a, 0x47, 0x9c, 0xa2, 0x18, 0x5c, 0x55, 0x40, 0x33, 0x7c, 0x9f };

const prne_arch_t prne_host_arch =
#ifdef __GNUC__
	#if defined(__i386__)
		PRNE_ARCH_I686
	#elif defined(__x86_64__)
		PRNE_ARCH_X86_64
	#elif defined(__ARM_ARCH_4T__)
		PRNE_ARCH_ARMV4T
	#elif defined(__ARM_ARCH_7A__)
		PRNE_ARCH_ARMV7
	#elif defined(__aarch64__)
		PRNE_ARCH_AARCH64
	#elif defined(__mips__)
		#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			PRNE_ARCH_MIPS
		#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			PRNE_ARCH_MPSL
		#else
			#error "FIXME!"
		#endif
	#elif defined(__powerpc__)
		PRNE_ARCH_PPC
	#elif defined(__SH4__)
		PRNE_ARCH_SH4
	#else
		#error "FIXME!"
	#endif
#else
	#error "FIXME!"
#endif
	;
