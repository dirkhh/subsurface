// SPDX-License-Identifier: GPL-2.0
#ifndef DATATRAK_HEADER_H
#define DATATRAK_HEADER_H

#include <string.h>
#include "libdivecomputer.h"

struct models_table_t {
	int model_num;
	int libdc_num;
	const char *name;
	dc_family_t type;
};

/*
 * Set of known models and equivalences with libdivecomputer.
 * Not included 0x14, 0x24, 0x41, and 0x73 known to exist, but not its model name.
 * Unknown model equivalence is set to Air X which should cover most profiles.
 * Nitrox and 02 models seems to keep its number more seriously than earlier
 * series and OEMs. Info for unknown  models is always welcome.
 */
static const struct models_table_t g_models[] = {
	{0x00,	0x00,	"Manually entered dive",	DC_FAMILY_NULL},
	{0x1B,	0x3F,	"Uwatec Aladin Pro",		DC_FAMILY_UWATEC_ALADIN},
	{0x1C,	0x1C,	"Uwatec Aladin Air",		DC_FAMILY_UWATEC_ALADIN},
	{0x1D,	0x3F,	"Spiro Monitor 2 plus",		DC_FAMILY_UWATEC_ALADIN},
	{0x1E,	0x3E,	"Uwatec Aladin Sport",		DC_FAMILY_UWATEC_ALADIN},
	{0x1F,	0x3F,	"Uwatec Aladin Pro",		DC_FAMILY_UWATEC_ALADIN},
	{0x34,	0x44,	"Uwatec Aladin Air X/Z",	DC_FAMILY_UWATEC_ALADIN},
	{0x3D,	0x3F,	"Spiro Monitor 2 plus",		DC_FAMILY_UWATEC_ALADIN},
	{0x3E,	0x3E,	"Uwatec Aladin Sport",		DC_FAMILY_UWATEC_ALADIN},
	{0x3F,	0x3F,	"Uwatec Aladin Pro",		DC_FAMILY_UWATEC_ALADIN},
	{0x44,	0x44,	"Uwatec Aladin Air X/Z",	DC_FAMILY_UWATEC_ALADIN},
	{0x48,	0x1C,	"Spiro Monitor 3 Air",		DC_FAMILY_UWATEC_ALADIN},
	{0xA4,	0xA4,	"Uwatec Aladin Air X/Z O2",	DC_FAMILY_UWATEC_ALADIN},
	{0xB1,	0x3E,	"Citizen Hyper Aqualand",	DC_FAMILY_UWATEC_ALADIN},
	{0xB2,	0x3F,	"Citizen ProMaster",		DC_FAMILY_UWATEC_ALADIN},
	{0xB3,	0x3F,	"Mares Guardian",		DC_FAMILY_UWATEC_ALADIN},
	{0xF4,	0xF4,	"Uwatec Aladin Air X/Z Nitrox",	DC_FAMILY_UWATEC_ALADIN},
	{0xFF,	0xFF,	"Uwatec Aladin Pro Nitrox",	DC_FAMILY_UWATEC_ALADIN},
	{0xEE,	0x44,	"Uwatec Unknown model",		DC_FAMILY_UWATEC_ALADIN},
};

extern struct sample *add_sample(struct sample *sample, int time, struct divecomputer *dc);

#define JUMP(_ptr, _n) if ((long) (_ptr += _n) > maxbuf) goto bail

#define read_bytes(_n) \
	switch (_n) { \
		case 1: \
			if (fread (&lector_bytes, sizeof(char), _n, archivo) != _n) \
				goto bail; \
			tmp_1byte = lector_bytes[0]; \
			break; \
		case 2: \
			if (fread (&lector_bytes, sizeof(char), _n, archivo) != _n) \
				goto bail; \
			tmp_2bytes = two_bytes_to_int (lector_bytes[1], lector_bytes[0]); \
			break; \
		default: \
			if (fread (&lector_word, sizeof(char), _n, archivo) != _n) \
				goto bail; \
			tmp_4bytes = four_bytes_to_long(lector_word[3], lector_word[2], lector_word[1], lector_word[0]); \
			break; \
	}

#define read_string(_property) \
	unsigned char *_property##tmp = (unsigned char *)calloc(tmp_1byte + 1, 1); \
	if (fread((char *)_property##tmp, 1, tmp_1byte, archivo) != tmp_1byte) { \
		free(_property##tmp); \
		goto bail; \
	} \
	_property = (unsigned char *)strcat(to_utf8(_property##tmp), ""); \
	free(_property##tmp);

#endif // DATATRAK_HEADER_H
