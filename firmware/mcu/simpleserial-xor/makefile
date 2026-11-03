# Final output name
TARGET = simpleserial-xor

# Application source
SRC += simpleserial-xor.c

# SimpleSerial 2.1
SS_VER = SS_VER_2_1

# Use ChipWhisperer's bundled Mbed TLS implementation
CRYPTO_TARGET = MBEDTLS
CRYPTO_OPTIONS = AES128

# ChipWhisperer firmware root
FIRMWAREPATH = ../.

# SimpleSerial build support
include ../simpleserial/Makefile.simpleserial

# Main ChipWhisperer build system
include $(FIRMWAREPATH)/Makefile.inc

