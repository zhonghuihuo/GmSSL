/* ====================================================================
 * Copyright (c) 2016 The GmSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the GmSSL Project.
 *    (http://gmssl.org/)"
 *
 * 4. The name "GmSSL Project" must not be used to endorse or promote
 *    products derived from this software without prior written
 *    permission. For written permission, please contact
 *    guanzhi1980@gmail.com.
 *
 * 5. Products derived from this software may not be called "GmSSL"
 *    nor may "GmSSL" appear in their names without prior written
 *    permission of the GmSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the GmSSL Project
 *    (http://gmssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE GmSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE GmSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/asn1.h>
#include <openssl/asn1t.h>
#include <openssl/objects.h>
#include <openssl/sm9.h>
#include "sm9_lcl.h"

static int sm9_master_key_cb(int operation, ASN1_VALUE **pval,
	const ASN1_ITEM *it, void *exarg)
{
	if (operation == ASN1_OP_NEW_PRE) {
		*pval = (ASN1_VALUE *)SM9_MASTER_KEY_new();
		if (*pval)
			return 2;
		return 0;
	} else if (operation == ASN1_OP_FREE_PRE) {
		SM9_MASTER_KEY_free((SM9_MASTER_KEY *)*pval);
		*pval = NULL;
		return 2;
	}
	return 1;
}

ASN1_SEQUENCE_cb(SM9MasterSecret, sm9_master_key_cb) = {
	ASN1_SIMPLE(SM9_MASTER_KEY, pairing, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_MASTER_KEY, scheme, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_MASTER_KEY, hash1, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_MASTER_KEY, pointPpub, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9_MASTER_KEY, masterSecret, BIGNUM)
} ASN1_SEQUENCE_END_cb(SM9_MASTER_KEY, SM9MasterSecret)
IMPLEMENT_ASN1_ENCODE_FUNCTIONS_const_fname(SM9_MASTER_KEY,SM9MasterSecret,SM9MasterSecret)

ASN1_SEQUENCE_cb(SM9PublicParameters, sm9_master_key_cb) = {
	ASN1_SIMPLE(SM9_MASTER_KEY, pairing, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_MASTER_KEY, scheme, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_MASTER_KEY, hash1, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_MASTER_KEY, pointPpub, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END_cb(SM9_MASTER_KEY, SM9PublicParameters)
IMPLEMENT_ASN1_ENCODE_FUNCTIONS_const_fname(SM9_MASTER_KEY,SM9PublicParameters,SM9PublicParameters)


static int sm9_key_cb(int operation, ASN1_VALUE **pval,
	const ASN1_ITEM *it, void *exarg)
{
	if (operation == ASN1_OP_NEW_PRE) {
		*pval = (ASN1_VALUE *)SM9_KEY_new();
		if (*pval)
			return 2;
		return 0;
	} else if (operation == ASN1_OP_FREE_PRE) {
		SM9_KEY_free((SM9_KEY *)*pval);
		*pval = NULL;
		return 2;
	}
	return 1;
}

ASN1_SEQUENCE_cb(SM9PrivateKey, sm9_key_cb) = {
	ASN1_SIMPLE(SM9_KEY, pairing, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_KEY, scheme, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_KEY, hash1, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_KEY, pointPpub, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9_KEY, identity, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9_KEY, publicPoint, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9_KEY, privatePoint, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END_cb(SM9_KEY, SM9PrivateKey)
IMPLEMENT_ASN1_ENCODE_FUNCTIONS_const_fname(SM9_KEY,SM9PrivateKey,SM9PrivateKey)

ASN1_SEQUENCE_cb(SM9PublicKey, sm9_key_cb) = {
	ASN1_SIMPLE(SM9_KEY, pairing, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_KEY, scheme, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_KEY, hash1, ASN1_OBJECT),
	ASN1_SIMPLE(SM9_KEY, pointPpub, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9_KEY, identity, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9_KEY, publicPoint, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END_cb(SM9_KEY, SM9PublicKey)
IMPLEMENT_ASN1_ENCODE_FUNCTIONS_const_fname(SM9_KEY,SM9PublicKey,SM9PublicKey)

ASN1_SEQUENCE(SM9Ciphertext) = {
	ASN1_SIMPLE(SM9Ciphertext, pointC1, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9Ciphertext, c2, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SM9Ciphertext, c3, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(SM9Ciphertext)
IMPLEMENT_ASN1_FUNCTIONS(SM9Ciphertext)
IMPLEMENT_ASN1_DUP_FUNCTION(SM9Ciphertext)

ASN1_SEQUENCE(SM9Signature) = {
	ASN1_SIMPLE(SM9Signature, h, BIGNUM),
	ASN1_SIMPLE(SM9Signature, pointS, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(SM9Signature)
IMPLEMENT_ASN1_FUNCTIONS(SM9Signature)
IMPLEMENT_ASN1_DUP_FUNCTION(SM9Signature)

int SM9PublicKey_gmtls_encode(SM9PublicKey *pk, unsigned char key[1024])
{
	return 0;
}

#ifndef OPENSSL_NO_STDIO
SM9MasterSecret *d2i_SM9MasterSecret_fp(FILE *fp, SM9MasterSecret **msk)
{
	return ASN1_item_d2i_fp(ASN1_ITEM_rptr(SM9MasterSecret), fp, msk);
}

int i2d_SM9MasterSecret_fp(FILE *fp, SM9MasterSecret *msk)
{
	return ASN1_item_i2d_fp(ASN1_ITEM_rptr(SM9MasterSecret), fp, msk);
}

SM9PublicParameters *d2i_SM9PublicParameters_fp(FILE *fp, SM9PublicParameters **mpk)
{
	return ASN1_item_d2i_fp(ASN1_ITEM_rptr(SM9PublicParameters), fp, mpk);
}

int i2d_SM9PublicParameters_fp(FILE *fp, SM9PublicParameters *mpk)
{
	return ASN1_item_i2d_fp(ASN1_ITEM_rptr(SM9PublicParameters), fp, mpk);
}

SM9PrivateKey *d2i_SM9PrivateKey_fp(FILE *fp, SM9PrivateKey **sk)
{
	return ASN1_item_d2i_fp(ASN1_ITEM_rptr(SM9PrivateKey), fp, sk);
}

int i2d_SM9PrivateKey_fp(FILE *fp, SM9PrivateKey *sk)
{
	return ASN1_item_i2d_fp(ASN1_ITEM_rptr(SM9PrivateKey), fp, sk);
}

SM9Signature *d2i_SM9Signature_fp(FILE *fp, SM9Signature **sig)
{
	return ASN1_item_d2i_fp(ASN1_ITEM_rptr(SM9Signature), fp, sig);
}

int i2d_SM9Signature_fp(FILE *fp, SM9Signature *sig)
{
	return ASN1_item_i2d_fp(ASN1_ITEM_rptr(SM9Signature), fp, sig);
}

SM9Ciphertext *d2i_SM9Ciphertext_fp(FILE *fp, SM9Ciphertext **c)
{
	return ASN1_item_d2i_fp(ASN1_ITEM_rptr(SM9Ciphertext), fp, c);
}

int i2d_SM9Ciphertext_fp(FILE *fp, SM9Ciphertext *c)
{
	return ASN1_item_i2d_fp(ASN1_ITEM_rptr(SM9Ciphertext), fp, c);
}
#endif
