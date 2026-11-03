TARGET = simpleserial-xor

SRC += simpleserial-xor.c

SS_VER = SS_VER_2_1

# ChipWhisperer firmware root
FIRMWAREPATH = ../.

# Mbed TLS
CRYPTO_TARGET = MBEDTLS
CRYPTO_OPTIONS = AES128

# SimpleSerial
include $(FIRMWAREPATH)/simpleserial/Makefile.simpleserial

# ChipWhisperer build system
include $(FIRMWAREPATH)/Makefile.inc
