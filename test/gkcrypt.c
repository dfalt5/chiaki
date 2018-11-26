/*
 * This file is part of Chiaki.
 *
 * Chiaki is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chiaki is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Chiaki.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <munit.h>

#include <chiaki/ecdh.h>

static MunitResult test_ecdh(const MunitParameter params[], void *user)
{
	static const uint8_t handshake_key[] = { 0xfc, 0x5d, 0x4b, 0xa0, 0x3a, 0x35, 0x3a, 0xbb, 0x6a, 0x7f, 0xac, 0x79, 0x1b, 0x17, 0xbb, 0x34 };
	static const uint8_t local_private_key[] = { 0x16, 0xe7, 0x5d, 0xcb, 0xda, 0x98, 0x55, 0xfb, 0x6b, 0xef, 0xdd, 0x8a, 0xa5, 0xf1, 0x6e, 0x7f, 0x46, 0xfd, 0xe1, 0xd2, 0x27, 0x97, 0x3, 0x60, 0x18, 0x72, 0xd8, 0x4b, 0x15, 0x38, 0xd9, 0x0 };
	static const uint8_t local_public_key[] = { 0x4, 0xf4, 0xa, 0xf1, 0x35, 0xa4, 0x88, 0x94, 0x36, 0xce, 0xe5, 0x2b, 0x5c, 0x73, 0xa3, 0x3e, 0xc5, 0xad, 0xb, 0xe0, 0x95, 0x2f, 0x57, 0xf4, 0xf0, 0xed, 0xc, 0x80, 0xb0, 0xbe, 0xda, 0x7c, 0xa6, 0x43, 0x78, 0x93, 0x93, 0xa5, 0x94, 0x7e, 0x9f, 0xaa, 0x3f, 0x67, 0x95, 0xc9, 0xaa, 0x9, 0xa9, 0x63, 0x25, 0xdf, 0xe8, 0x50, 0xbf, 0xc3, 0xf1, 0xdb, 0x62, 0xa5, 0xa, 0xbf, 0xb0, 0xff, 0xf7 };
	static const uint8_t local_public_key_sig[] = { 0x99, 0xb5, 0xcb, 0xb5, 0x37, 0x18, 0xb, 0xfc, 0x55, 0xda, 0x43, 0x7f, 0x44, 0x76, 0xa8, 0x17, 0xc9, 0x37, 0xfe, 0x56, 0x1b, 0x8a, 0xbe, 0xc, 0x41, 0x12, 0xab, 0x71, 0xf5, 0xa6, 0x8d, 0x29 };
	static const uint8_t remote_public_key[] = { 0x4, 0xdf, 0xef, 0x8, 0xbb, 0xa8, 0x56, 0xf2, 0xb4, 0x4b, 0x8a, 0xe, 0x4f, 0x44, 0x20, 0x3f, 0x8e, 0x49, 0x3f, 0xee, 0xd4, 0x3c, 0xe9, 0x3a, 0xfe, 0x5c, 0x64, 0x67, 0x77, 0x20, 0x15, 0x7c, 0x59, 0x10, 0x15, 0x67, 0x94, 0xae, 0x5f, 0x2, 0x4a, 0xad, 0xc, 0xce, 0xfa, 0x14, 0x15, 0xa, 0xab, 0xee, 0x8, 0xb, 0x14, 0x12, 0x76, 0xea, 0x3e, 0xc0, 0xd5, 0x65, 0xf4, 0x68, 0x77, 0xa3, 0xca };
	static const uint8_t remote_public_key_sig[] = { 0x13, 0xc5, 0x89, 0xe2, 0x3b, 0x72, 0x85, 0x24, 0xa9, 0x9f, 0x96, 0x80, 0x3, 0xa1, 0x81, 0x30, 0x59, 0x68, 0xf1, 0xbb, 0xb6, 0x4d, 0xc4, 0xa7, 0x6c, 0xce, 0xf6, 0x79, 0x4c, 0xeb, 0x2d, 0x98 };
	static const uint8_t secret[] = { 0xb8, 0x1c, 0x61, 0x46, 0xe7, 0x49, 0x73, 0x8c, 0x96, 0x30, 0xca, 0x13, 0xff, 0x71, 0xe5, 0x9b, 0x3b, 0xf9, 0x41, 0x98, 0xd4, 0x67, 0xa5, 0xa2, 0xbc, 0x78, 0x4, 0x92, 0x81, 0x43, 0xec, 0x1d };

	ChiakiECDH ecdh;
	ChiakiErrorCode err = chiaki_ecdh_init(&ecdh);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;

	err = chiaki_ecdh_set_local_key(&ecdh, local_private_key, sizeof(local_private_key), local_public_key, sizeof(local_public_key));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;

	uint8_t local_public_key_result[128];
	size_t local_public_key_result_size = sizeof(local_public_key_result);
	uint8_t local_public_key_sig_result[32];
	size_t local_public_key_sig_result_size = sizeof(local_public_key_sig_result);
	chiaki_ecdh_get_local_pub_key(&ecdh, local_public_key_result, &local_public_key_result_size, handshake_key, local_public_key_sig_result, &local_public_key_sig_result_size);

	munit_assert_size(local_public_key_result_size, ==, sizeof(local_public_key));
	munit_assert_memory_equal(sizeof(local_public_key), local_public_key_result, local_public_key);

	munit_assert_size(local_public_key_sig_result_size, ==, sizeof(local_public_key_sig));
	munit_assert_memory_equal(sizeof(local_public_key_sig), local_public_key_sig_result, local_public_key_sig);

	uint8_t secret_result[128];
	chiaki_ecdh_derive_secret(&ecdh, secret_result,
			remote_public_key, sizeof(remote_public_key),
			handshake_key,
			remote_public_key_sig, sizeof(remote_public_key_sig));

	munit_assert_size(CHIAKI_ECDH_SECRET_SIZE, ==, sizeof(secret));
	munit_assert_memory_equal(sizeof(secret), secret_result, secret);

	chiaki_ecdh_fini(&ecdh);

	return MUNIT_OK;
}


MunitTest tests_gkcrypt[] = {
	{
		"/ecdh",
		test_ecdh,
		NULL,
		NULL,
	 	MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};