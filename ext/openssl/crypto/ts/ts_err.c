/*
 * Generated by util/mkerr.pl DO NOT EDIT
 * Copyright 1995-2017 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <openssl/err.h>
#include <openssl/tserr.h>

#ifndef OPENSSL_NO_ERR

static const ERR_STRING_DATA TS_str_functs[] = {
    {ERR_PACK(ERR_LIB_TS, TS_F_DEF_SERIAL_CB, 0), "def_serial_cb"},
    {ERR_PACK(ERR_LIB_TS, TS_F_DEF_TIME_CB, 0), "def_time_cb"},
    {ERR_PACK(ERR_LIB_TS, TS_F_ESS_ADD_SIGNING_CERT, 0),
     "ess_add_signing_cert"},
    {ERR_PACK(ERR_LIB_TS, TS_F_ESS_ADD_SIGNING_CERT_V2, 0),
     "ess_add_signing_cert_v2"},
    {ERR_PACK(ERR_LIB_TS, TS_F_ESS_CERT_ID_NEW_INIT, 0),
     "ess_CERT_ID_new_init"},
    {ERR_PACK(ERR_LIB_TS, TS_F_ESS_CERT_ID_V2_NEW_INIT, 0),
     "ess_cert_id_v2_new_init"},
    {ERR_PACK(ERR_LIB_TS, TS_F_ESS_SIGNING_CERT_NEW_INIT, 0),
     "ess_SIGNING_CERT_new_init"},
    {ERR_PACK(ERR_LIB_TS, TS_F_ESS_SIGNING_CERT_V2_NEW_INIT, 0),
     "ess_signing_cert_v2_new_init"},
    {ERR_PACK(ERR_LIB_TS, TS_F_INT_TS_RESP_VERIFY_TOKEN, 0),
     "int_ts_RESP_verify_token"},
    {ERR_PACK(ERR_LIB_TS, TS_F_PKCS7_TO_TS_TST_INFO, 0),
     "PKCS7_to_TS_TST_INFO"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_ACCURACY_SET_MICROS, 0),
     "TS_ACCURACY_set_micros"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_ACCURACY_SET_MILLIS, 0),
     "TS_ACCURACY_set_millis"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_ACCURACY_SET_SECONDS, 0),
     "TS_ACCURACY_set_seconds"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CHECK_IMPRINTS, 0), "ts_check_imprints"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CHECK_NONCES, 0), "ts_check_nonces"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CHECK_POLICY, 0), "ts_check_policy"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CHECK_SIGNING_CERTS, 0),
     "ts_check_signing_certs"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CHECK_STATUS_INFO, 0),
     "ts_check_status_info"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_COMPUTE_IMPRINT, 0), "ts_compute_imprint"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CONF_INVALID, 0), "ts_CONF_invalid"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CONF_LOAD_CERT, 0), "TS_CONF_load_cert"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CONF_LOAD_CERTS, 0), "TS_CONF_load_certs"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CONF_LOAD_KEY, 0), "TS_CONF_load_key"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CONF_LOOKUP_FAIL, 0), "ts_CONF_lookup_fail"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_CONF_SET_DEFAULT_ENGINE, 0),
     "TS_CONF_set_default_engine"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_GET_STATUS_TEXT, 0), "ts_get_status_text"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_MSG_IMPRINT_SET_ALGO, 0),
     "TS_MSG_IMPRINT_set_algo"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_REQ_SET_MSG_IMPRINT, 0),
     "TS_REQ_set_msg_imprint"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_REQ_SET_NONCE, 0), "TS_REQ_set_nonce"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_REQ_SET_POLICY_ID, 0),
     "TS_REQ_set_policy_id"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CREATE_RESPONSE, 0),
     "TS_RESP_create_response"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CREATE_TST_INFO, 0),
     "ts_RESP_create_tst_info"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_ADD_FAILURE_INFO, 0),
     "TS_RESP_CTX_add_failure_info"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_ADD_MD, 0), "TS_RESP_CTX_add_md"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_ADD_POLICY, 0),
     "TS_RESP_CTX_add_policy"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_NEW, 0), "TS_RESP_CTX_new"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_SET_ACCURACY, 0),
     "TS_RESP_CTX_set_accuracy"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_SET_CERTS, 0),
     "TS_RESP_CTX_set_certs"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_SET_DEF_POLICY, 0),
     "TS_RESP_CTX_set_def_policy"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_SET_SIGNER_CERT, 0),
     "TS_RESP_CTX_set_signer_cert"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_CTX_SET_STATUS_INFO, 0),
     "TS_RESP_CTX_set_status_info"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_GET_POLICY, 0), "ts_RESP_get_policy"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_SET_GENTIME_WITH_PRECISION, 0),
     "TS_RESP_set_genTime_with_precision"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_SET_STATUS_INFO, 0),
     "TS_RESP_set_status_info"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_SET_TST_INFO, 0),
     "TS_RESP_set_tst_info"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_SIGN, 0), "ts_RESP_sign"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_RESP_VERIFY_SIGNATURE, 0),
     "TS_RESP_verify_signature"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_ACCURACY, 0),
     "TS_TST_INFO_set_accuracy"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_MSG_IMPRINT, 0),
     "TS_TST_INFO_set_msg_imprint"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_NONCE, 0),
     "TS_TST_INFO_set_nonce"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_POLICY_ID, 0),
     "TS_TST_INFO_set_policy_id"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_SERIAL, 0),
     "TS_TST_INFO_set_serial"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_TIME, 0),
     "TS_TST_INFO_set_time"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_TST_INFO_SET_TSA, 0), "TS_TST_INFO_set_tsa"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_VERIFY, 0), ""},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_VERIFY_CERT, 0), "ts_verify_cert"},
    {ERR_PACK(ERR_LIB_TS, TS_F_TS_VERIFY_CTX_NEW, 0), "TS_VERIFY_CTX_new"},
    {0, NULL}
};

static const ERR_STRING_DATA TS_str_reasons[] = {
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_BAD_PKCS7_TYPE), "bad pkcs7 type"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_BAD_TYPE), "bad type"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_CANNOT_LOAD_CERT), "cannot load certificate"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_CANNOT_LOAD_KEY), "cannot load private key"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_CERTIFICATE_VERIFY_ERROR),
    "certificate verify error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_COULD_NOT_SET_ENGINE),
    "could not set engine"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_COULD_NOT_SET_TIME), "could not set time"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_DETACHED_CONTENT), "detached content"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_ESS_ADD_SIGNING_CERT_ERROR),
    "ess add signing cert error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_ESS_ADD_SIGNING_CERT_V2_ERROR),
    "ess add signing cert v2 error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_ESS_SIGNING_CERTIFICATE_ERROR),
    "ess signing certificate error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_INVALID_NULL_POINTER),
    "invalid null pointer"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_INVALID_SIGNER_CERTIFICATE_PURPOSE),
    "invalid signer certificate purpose"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_MESSAGE_IMPRINT_MISMATCH),
    "message imprint mismatch"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_NONCE_MISMATCH), "nonce mismatch"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_NONCE_NOT_RETURNED), "nonce not returned"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_NO_CONTENT), "no content"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_NO_TIME_STAMP_TOKEN), "no time stamp token"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_PKCS7_ADD_SIGNATURE_ERROR),
    "pkcs7 add signature error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_PKCS7_ADD_SIGNED_ATTR_ERROR),
    "pkcs7 add signed attr error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_PKCS7_TO_TS_TST_INFO_FAILED),
    "pkcs7 to ts tst info failed"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_POLICY_MISMATCH), "policy mismatch"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_PRIVATE_KEY_DOES_NOT_MATCH_CERTIFICATE),
    "private key does not match certificate"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_RESPONSE_SETUP_ERROR),
    "response setup error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_SIGNATURE_FAILURE), "signature failure"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_THERE_MUST_BE_ONE_SIGNER),
    "there must be one signer"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TIME_SYSCALL_ERROR), "time syscall error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TOKEN_NOT_PRESENT), "token not present"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TOKEN_PRESENT), "token present"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TSA_NAME_MISMATCH), "tsa name mismatch"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TSA_UNTRUSTED), "tsa untrusted"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TST_INFO_SETUP_ERROR),
    "tst info setup error"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_TS_DATASIGN), "ts datasign"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_UNACCEPTABLE_POLICY), "unacceptable policy"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_UNSUPPORTED_MD_ALGORITHM),
    "unsupported md algorithm"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_UNSUPPORTED_VERSION), "unsupported version"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_VAR_BAD_VALUE), "var bad value"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_VAR_LOOKUP_FAILURE),
    "cannot find config variable"},
    {ERR_PACK(ERR_LIB_TS, 0, TS_R_WRONG_CONTENT_TYPE), "wrong content type"},
    {0, NULL}
};

#endif

int ERR_load_TS_strings(void)
{
#ifndef OPENSSL_NO_ERR
    if (ERR_func_error_string(TS_str_functs[0].error) == NULL) {
        ERR_load_strings_const(TS_str_functs);
        ERR_load_strings_const(TS_str_reasons);
    }
#endif
    return 1;
}
