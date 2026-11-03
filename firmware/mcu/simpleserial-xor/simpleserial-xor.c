#include "hal.h" // HAL function definitions
#include "simpleserial.h" // include simpleserial
#include "mbedtls/aes.h"
#include <stdint.h>
#include <stdlib.h>
static uint8_t key[16] = {0}; // initialize key as all 0's
static uint8_t ctr_counter[16] = {0};

// Read key in
uint8_t get_key(uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    if (len != 16) {
        return 0x04; // SS_ERR_LEN
    }
    for (uint8_t i = 0; i < 16; i++) {
        key[i] = buf[i]; // set key
    }

    return 0x00; //SS_ERR_OK
}

uint8_t set_ctr(uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    if (len != 16) {
        return 0x04; // SS_ERR_LEN
    }
    for (uint8_t i = 0; i < 16; i++) {
        ctr_counter[i] = buf[i];
    }

    return 0x00; // SS_ERR_OK
}

// read plaintext, do encryption, then send ciphertext back
uint8_t xor_inc(uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    if (len != 16) {
        return 0x04; // SS_ERR_LEN
    }

    trigger_high();
    for (uint8_t i = 0; i < 16; i++) {
        buf[i] ^= key[i]; // do "encryption"
    }
    trigger_low();

    simpleserial_put('r', 16, buf); // send ciphertext back

    return 0x00; //SS_ERR_OK
}

uint8_t aes_ctr_inc(uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    if (len != 32) {
        return 0x04; // SS_ERR_LEN
    }

    mbedtls_aes_context ctx;
    uint8_t nonce_counter[16];
    uint8_t stream_block[16] = {0};
    size_t nc_off = 0;
    for (uint8_t i = 0; i < 16; i++) {
        nonce_counter[i] = ctr_counter[i];
    }

    mbedtls_aes_init(&ctx);
    if (mbedtls_aes_setkey_enc(&ctx, key, 128) != 0) {
        mbedtls_aes_free(&ctx);
        return SS_ERR_CMD;
    }

    trigger_high();
    int status = mbedtls_aes_crypt_ctr(&ctx, 32, &nc_off, nonce_counter, stream_block, buf, buf);
    trigger_low();
    mbedtls_aes_free(&ctx);

    if (status != 0) {
        return SS_ERR_CMD;
    }

    simpleserial_put('r', 32, buf);

    return 0x00; // SS_ERR_OK
}

uint8_t aes_ctr_dec(uint8_t cmd, uint8_t scmd, uint8_t len, uint8_t *buf)
{
    return aes_ctr_inc(cmd, scmd, len, buf);
}

int main(void)
{
    // setup
    platform_init();
    init_uart();
    trigger_setup();
    simpleserial_init();

    // tell simpleserial_get to look for 'a' and 'b' packets
    // and to do these two functions when they're received
    // NOTE: 'p' and 'k' in SSV2 are changed to 0x01 with a varying scmd in Python, so they're not recommended for custom firmware.
    simpleserial_addcmd('a', 16, xor_inc);
    simpleserial_addcmd('b', 16, get_key);
    simpleserial_addcmd('c', 32, aes_ctr_inc);
    simpleserial_addcmd('d', 32, aes_ctr_dec);
    simpleserial_addcmd('e', 16, set_ctr);

    // look for simpleserial packets
    while(1)
        simpleserial_get();
}

