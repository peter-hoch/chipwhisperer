#include "hal.h"
#include "simpleserial.h"

#include "mbedtls/aes.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

static uint8_t key[16] = {0};
static uint8_t ctr_counter[16] = {0};


/*
 * SimpleSerial command 'b':
 * Set the 128-bit AES key.
 */
uint8_t get_key( uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    (void)cmd;
    (void)scmd;

    if (len != sizeof(key)) {
        return SS_ERR_LEN;
    }

    memcpy(key, buf, sizeof(key));

    return SS_ERR_OK;
}


/*
 * SimpleSerial command 'e':
 * Set the initial 128-bit CTR counter.
 */
uint8_t set_ctr( uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    (void)cmd;
    (void)scmd;

    if (len != sizeof(ctr_counter)) {
        return SS_ERR_LEN;
    }

    memcpy(ctr_counter, buf, sizeof(ctr_counter));

    return SS_ERR_OK;
}


/*
 * SimpleSerial command 'a':
 * Original XOR example.
 */
uint8_t xor_inc( uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    (void)cmd;
    (void)scmd;

    if (len != 16) {
        return SS_ERR_LEN;
    }

    trigger_high();

    for (uint8_t i = 0; i < 16; i++) {
        buf[i] ^= key[i];
    }

    trigger_low();

    simpleserial_put('r', 16, buf);

    return SS_ERR_OK;
}


/*
 * SimpleSerial command 'c':
 * Encrypt 32 bytes in-place with AES-128-CTR.
 *
 * Mbed TLS increments a local copy of ctr_counter. The global counter is
 * therefore unchanged between requests.
 */
uint8_t aes_ctr_inc( uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    (void)cmd;
    (void)scmd;

    if (len != 32) {
        return SS_ERR_LEN;
    }

    mbedtls_aes_context ctx;

    uint8_t nonce_counter[16];
    uint8_t stream_block[16] = {0};

    size_t nc_off = 0;
    int status;

    memcpy( nonce_counter, ctr_counter, sizeof(nonce_counter));

    mbedtls_aes_init(&ctx);

    status = mbedtls_aes_setkey_enc( &ctx, key, 128);

    if (status != 0) {
        mbedtls_aes_free(&ctx);
        return SS_ERR_CMD;
    }

    trigger_high();

    status = mbedtls_aes_crypt_ctr( &ctx, len, &nc_off, nonce_counter, stream_block, buf, buf);

    trigger_low();

    mbedtls_aes_free(&ctx);

    if (status != 0) {
        return SS_ERR_CMD;
    }

    simpleserial_put('r', len, buf);

    return SS_ERR_OK;
}


/*
 * AES-CTR encryption and decryption are identical.
 */
uint8_t aes_ctr_dec( uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    return aes_ctr_inc(cmd, scmd, len, buf);
}


int main(void)
{
    platform_init();
    init_uart();
    trigger_setup();
    simpleserial_init();

    simpleserial_addcmd('a', 16, xor_inc);
    simpleserial_addcmd('b', 16, get_key);
    simpleserial_addcmd('c', 32, aes_ctr_inc);
    simpleserial_addcmd('d', 32, aes_ctr_dec);
    simpleserial_addcmd('e', 16, set_ctr);

    while (1) {
        simpleserial_get();
    }

    return 0;
}