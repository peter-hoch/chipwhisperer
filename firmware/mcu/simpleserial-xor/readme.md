# simpleserial xor sample

[source](https://chipwhisperer.readthedocs.io/en/latest/custom-firmware.html)

## build

```bash
make -j PLATFORM=CW308_SAM4S
```

```bash
make clean
```

## AES-CTR command

Send a 16-byte key with command `b` and a 16-byte initial counter block with command `e`. The counter defaults to zero if `e` is not sent. Send a 32-byte plaintext with command `c` to encrypt or a 32-byte ciphertext with command `d` to decrypt. The 32-byte result is returned with response command `r`. Each operation starts from the stored counter value, allowing `d` to reverse `c`.  
Use a unique counter value for each encryption with the same key; do not reuse a key/counter pair for real encryption.
