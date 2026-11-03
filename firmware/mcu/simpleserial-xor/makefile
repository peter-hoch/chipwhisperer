# what the final build file will be called
TARGET = simpleserial-xor

# our C firmware file
SRC += simpleserial-xor.c

# Use simpleserial 2
SS_VER = SS_VER_2_1

# Use mbedTLS AES
CRYPTO_TARGET = MBEDTLS
CRYPTO_OPTIONS = AES128C

# include other required build files
include ../simpleserial/Makefile.simpleserial

FIRMWAREPATH = ../.
include $(FIRMWAREPATH)/Makefile.inc

