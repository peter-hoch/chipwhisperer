
TARGET = simpleserial-xor

SRC += simpleserial-xor.c

SS_VER = SS_VER_2_1

FIRMWAREPATH = ../.

# Mbed TLS AES
CRYPTO_TARGET = MBEDTLS
CRYPTO_OPTIONS = AES128C

include $(FIRMWAREPATH)/crypto/Makefile.crypto
include $(FIRMWAREPATH)/simpleserial/Makefile.simpleserial
include $(FIRMWAREPATH)/Makefile.inc
