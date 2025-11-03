# Application Protocol Data Unit (APDU)

The communication protocol used by [BOLOS](https://ledger.readthedocs.io/en/latest/bolos/overview.html)
to exchange [APDU](https://en.wikipedia.org/wiki/Smart_card_application_protocol_data_unit)
is very close to [ISO 7816-4](https://www.iso.org/standard/77180.html) with a few differences:

- `Lc` length is always exactly 1 byte
- No `Le` field in APDU command
- Maximum size of APDU command is 260 bytes: 5 bytes of header + 255 bytes of data
- Maximum size of APDU response is 260 bytes: 258 bytes of response data + 2 bytes of status word

Status words tend to be similar to common [APDU responses](https://www.eftlab.com/knowledge-base/complete-list-of-apdu-responses/)
in the industry.

## Command APDU

| Field name | Length (bytes) | Description                                                           |
| ---------- | -------------- | --------------------------------------------------------------------- |
| CLA        | 1              | Instruction class - indicates the type of command                     |
| INS        | 1              | Instruction code - indicates the specific command                     |
| P1         | 1              | Instruction parameter 1 for the command                               |
| P2         | 1              | Instruction parameter 2 for the command                               |
| Lc         | 1              | The number of bytes of command data to follow (a value from 0 to 255) |
| CData      | (varies)       | Command data with `Lc` bytes                                          |

## Response APDU

| Field name | Length (bytes) | Description                                                                  |
| ---------- | -------------- | ---------------------------------------------------------------------------- |
| RData      | (varies)       | Response data (can be empty)                                                 |
| SW         | 2              | Status word containing command processing status (e.g. `0x9000` for success) |

## Nano ($XNO) APDU specification

### Get app configuration

Requests information about the app.

| CLA  | INS  | P1   | P2   | Lc   | CData  |
| ---- | ---- | ---- | ---- | ---- | ------ |
| 0xA1 | 0x01 | 0x00 | 0x00 | 0x00 | (none) |

Returns the name and version number of the app.

| RData                    | Length (bytes) |
| ------------------------ | -------------- |
| Major version            | 1              |
| Minor version            | 1              |
| Patch version            | 1              |
| Coin name length (bytes) | 1              |
| Coin name                | (varies)       |
| SW                       | 2              |

### Get address

Requests the account information at the _nᵗʰ_ BIP-32 derivation level.

| CLA  | INS  | P1                                     | P2   | Lc       | CData       |
| ---- | ---- | -------------------------------------- | ---- | -------- | ----------- |
| 0xA1 | 0x02 | 0x01 to display the address, else 0x00 | 0x00 | (4n + 1) | (see below) |

| CData                                | Length (bytes) |
| ------------------------------------ | -------------- |
| BIP-32 derivations (max 10)          | 1              |
| First derivation index (big endian)  | 4              |
| (_...additional derivation indexes_) | _4_            |

Returns the public key and encoded Nano address for the given BIP-32 derivation level.

| RData                          | Length (bytes) |
| ------------------------------ | -------------- |
| Public key                     | 32             |
| Account address length (bytes) | 1              |
| Account address                | (varies)       |
| SW                             | 2              |

### Cache block

Caches in memory the head block (frontier) of an account at the _nᵗʰ_ BIP-32 derivation level. The `sign` command uses this cached data to determine changes in account state.

| CLA  | INS  | P1   | P2   | Lc         | CData       |
| ---- | ---- | ---- | ---- | ---------- | ----------- |
| 0xA1 | 0x03 | 0x00 | 0x00 | (4n + 177) | (see below) |

| CData                                | Length (bytes) |
| ------------------------------------ | -------------- |
| BIP-32 derivations (max 10)          | 1              |
| First derivation index (big endian)  | 4              |
| (_...additional derivation indexes_) | _4_            |
| Parent block hash                    | 32             |
| Link                                 | 32             |
| Representative                       | 32             |
| Balance                              | 16             |
| Signature                            | 64             |

Returns a status word for the result.

| RData | Length (bytes) |
| ----- | -------------- |
| SW    | 2              |

### Sign block

Requests a signature for the provided block data using the account at the _nᵗʰ_ BIP-32 derivation level. For all transactions after the first one for a given account, the parent must be cached prior to signing.

| CLA  | INS  | P1   | P2   | Lc         | CData       |
| ---- | ---- | ---- | ---- | ---------- | ----------- |
| 0xA1 | 0x04 | 0x00 | 0x00 | (113 + 4n) | (see below) |

| CData                                | Length (bytes) |
| ------------------------------------ | -------------- |
| BIP-32 derivations (max 10)          | 1              |
| First derivation index (big endian)  | 4              |
| (_...additional derivation indexes_) | _4_            |
| Parent block hash                    | 32             |
| Link                                 | 32             |
| Representative                       | 32             |
| Balance                              | 16             |

Returns the signature for the provided block data.

| RData      | Length (bytes) |
| ---------- | -------------- |
| Block hash | 32             |
| Signature  | 64             |
| SW         | 2              |

### Sign nonce

Requests a signature for the provided nonce using the account at the _nᵗʰ_ BIP-32 derivation level. The actual messaage signed is a string which can be expressed as the following template literal:

`${coin_name}$ Signed Nonce:\n ${nonce_bytes}`

| CLA  | INS  | P1   | P2   | Lc        | CData       |
| ---- | ---- | ---- | ---- | --------- | ----------- |
| 0xA1 | 0x05 | 0x00 | 0x00 | (17 + 4n) | (see below) |

| CData                                | Length (bytes) |
| ------------------------------------ | -------------- |
| BIP-32 derivations (max 10)          | 1              |
| First derivation index (big endian)  | 4              |
| (_...additional derivation indexes_) | _4_            |
| Nonce                                | 16             |

Returns the signature for the provided nonce.

| RData     | Length (bytes) |
| --------- | -------------- |
| Signature | 64             |

## Transport protocol

### General transport description

Ledger APDUs requests and responses are encapsulated using a flexible protocol allowing to fragment large payloads over different underlying transport mechanisms.

The common transport header is defined as follows:

| Description                           | Length (bytes) |
| ------------------------------------- | -------------- |
| Communication channel ID (big endian) | 2              |
| Command tag                           | 1              |
| Packet sequence index (big endian)    | 2              |
| Payload                               | (varies)       |

The Communication channel ID allows commands multiplexing over the same physical link. It is not used for the time being, and should be set to 0101 to avoid compatibility issues with implementations ignoring a leading 00 byte.

The Command tag describes the message content. Use TAG_APDU (0x05) for standard APDU payloads, or TAG_PING (0x02) for a simple link test.

The Packet sequence index describes the current sequence for fragmented payloads. The first fragment index is 0x00.

### USB mapping

Messages are exchanged with the dongle over HID endpoints over interrupt transfers, with each chunk being 64 bytes long. The HID Report ID is ignored.

### Status words

The following standard Status Words are returned for Nano app APDUs; some are returned by specific commands and are described in the command documentation above.

| SW   | Description                                                                   |
| ---- | ----------------------------------------------------------------------------- |
| 6700 | Incorrect byte length of message                                              |
| 6982 | Security status not satisfied (dongle is locked or busy with another request) |
| 6A80 | Invalid input data                                                            |
| 6A81 | Failed to verify the provided signature                                       |
| 6A82 | Parent block data cache-miss (cache parent before signing)                    |
| 6B00 | Incorrect parameter P1 or P2                                                  |
| 6Fxx | Technical problem (Internal error, please report)                             |

The following standard Status Words are returned for all APDUs.

| SW   | SW name                    | Description                                      |
| ---- | -------------------------- | ------------------------------------------------ |
| 6985 | SW_DENY                    | Rejected by user                                 |
| 6A86 | SW_WRONG_P1P2              | Either P1 or P2 is incorrect                     |
| 6A87 | SW_WRONG_DATA_LENGTH       | Lc or minimum APDU length is incorrect           |
| 6D00 | SW_INS_NOT_SUPPORTED       | No command exists with INS                       |
| 6E00 | SW_CLA_NOT_SUPPORTED       | Bad CLA used for this application                |
| B000 | SW_WRONG_RESPONSE_LENGTH   | Wrong response length (buffer size problem)      |
| B001 | SW_DISPLAY_BIP32_PATH_FAIL | BIP32 path conversion to string failed           |
| B002 | SW_DISPLAY_ADDRESS_FAIL    | Address conversion to string failed              |
| B003 | SW_DISPLAY_AMOUNT_FAIL     | Amount conversion to string failed               |
| B004 | SW_WRONG_TX_LENGTH         | Wrong raw transaction length                     |
| B005 | SW_TX_PARSING_FAIL         | Failed to parse raw transaction                  |
| B006 | SW_TX_HASH_FAIL            | Failed to compute hash digest of raw transaction |
| B007 | SW_BAD_STATE               | Security issue with bad state                    |
| B008 | SW_SIGNATURE_FAIL          | Signature of raw transaction failed              |
| 9000 | OK                         | Success                                          |
