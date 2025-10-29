# $NANO application : Common Technical Specifications

## About

This specification describes the APDU messages interface to communicate with the $NANO application.

## Wallet usage APDUs

### Get address

#### Description

This command returns the public key and the encoded address for the given BIP 32 path.

#### Coding

**Command**

| _CLA_ | _INS_ | _P1_                            | _P2_ | _Lc_ | _Le_ |
| ----- | ----- | ------------------------------- | ---- | ---- | ---- |
| A1    | 02    | 00 : do not display the address |      |      |      |
|       |       | 01 : display the address        |      |      |      |

**Input data**

| _Description_                                    | _Length_ |
| ------------------------------------------------ | -------- |
| Number of BIP 32 derivations to perform (max 10) | 1        |
| First derivation index (big endian)              | 4        |
| ...                                              | 4        |
| Last derivation index (big endian)               | 4        |

**Output data**

| _Description_          | _Length_ |
| ---------------------- | -------- |
| Public key             | 32       |
| Account address length | 1        |
| Account address        | var      |

### Cache block

#### Description

This command caches the frontier block in memory. The sign block command uses this cached data to determine the changes in account state.

#### Coding

**Command**

| _CLA_ | _INS_ | _P1_ | _P2_ | _Lc_ | _Le_ |
| ----- | ----- | ---- | ---- | ---- | ---- |
| A1    | 03    | 00   | 00   |      |      |

**Input data**

| _Description_                                    | _Length_ |
| ------------------------------------------------ | -------- |
| Number of BIP 32 derivations to perform (max 10) | 1        |
| First derivation index (big endian)              | 4        |
| ...                                              | 4        |
| Last derivation index (big endian)               | 4        |
| Parent block hash                                | 32       |
| Link                                             | 32       |
| Representative                                   | 32       |
| Balance                                          | 16       |
| Signature                                        | 64       |

**Output data**

_None_

### Sign block

#### Description

This command returns the signature for the provided universal block data. For non-null parent blocks the validate block command needs to be called before the this command.

#### Coding

**Command**

| _CLA_ | _INS_ | _P1_ | _P2_ | _Lc_ | _Le_ |
| ----- | ----- | ---- | ---- | ---- | ---- |
| A1    | 04    | 00   | 00   |      |      |

**Input data**

| _Description_                                    | _Length_ |
| ------------------------------------------------ | -------- |
| Number of BIP 32 derivations to perform (max 10) | 1        |
| First derivation index (big endian)              | 4        |
| ...                                              | 4        |
| Last derivation index (big endian)               | 4        |
| Parent block hash                                | 32       |
| Link                                             | 32       |
| Representative                                   | 32       |
| Balance                                          | 16       |

**Output data**

| _Description_ | _Length_ |
| ------------- | -------- |
| Block hash    | 32       |
| Signature     | 64       |

## Test and utility APDUs

### Get app configuration

#### Description

This command returns the application configuration.

#### Coding

**Command**

| _CLA_ | _INS_ | _P1_ | _P2_ | _Lc_ | _Le_ |
| ----- | ----- | ---- | ---- | ---- | ---- |
| A1    | 01    | 00   | 00   | 00   |      |

**Input data**

_None_

**Output data**

| _Description_     | _Length_ |
| ----------------- | -------- |
| Major app version | 1        |
| Minor app version | 1        |
| Patch app version | 1        |
| Coin name length  | 1        |
| Coin name         | var      |

### Sign nonce

#### Description

This command signs a 128bit nonce and returns the signature. `"Nano Signed Nonce:\n" + nonceBytes` is the message that gets signed with the private key. This method is meant to be used as a soft-authentication (eg for APIs), to prove that the Ledger with the private key is plugged into the computer.

#### Coding

**Command**

| _CLA_ | _INS_ | _P1_ | _P2_ | _Lc_ | _Le_ |
| ----- | ----- | ---- | ---- | ---- | ---- |
| A1    | 05    | 00   | 00   |      |      |

**Input data**

| _Description_                                    | _Length_ |
| ------------------------------------------------ | -------- |
| Number of BIP 32 derivations to perform (max 10) | 1        |
| First derivation index (big endian)              | 4        |
| ...                                              | 4        |
| Last derivation index (big endian)               | 4        |
| Nonce                                            | 16       |

**Output data**

| _Description_ | _Length_ |
| ------------- | -------- |
| Signature     | 64       |

## Transport protocol

### General transport description

Ledger APDUs requests and responses are encapsulated using a flexible protocol allowing to fragment large payloads over different underlying transport mechanisms.

The common transport header is defined as follows:

| _Description_                         | _Length_ |
| ------------------------------------- | -------- |
| Communication channel ID (big endian) | 2        |
| Command tag                           | 1        |
| Packet sequence index (big endian)    | 2        |
| Payload                               | var      |

The Communication channel ID allows commands multiplexing over the same physical link. It is not used for the time being, and should be set to 0101 to avoid compatibility issues with implementations ignoring a leading 00 byte.

The Command tag describes the message content. Use TAG_APDU (0x05) for standard APDU payloads, or TAG_PING (0x02) for a simple link test.

The Packet sequence index describes the current sequence for fragmented payloads. The first fragment index is 0x00.

### APDU Command payload encoding

APDU Command payloads are encoded as follows:

| _Description_            | _Length_ |
| ------------------------ | -------- |
| APDU length (big endian) | 2        |
| APDU CLA                 | 1        |
| APDU INS                 | 1        |
| APDU P1                  | 1        |
| APDU P2                  | 1        |
| APDU length              | 1        |
| Optional APDU data       | var      |

APDU payload is encoded according to the APDU case

| Case Number | _Lc_ | _Le_ | Case description                                        |
| ----------- | ---- | ---- | ------------------------------------------------------- |
| 1           | 0    | 0    | No data in either direction - L is set to 00            |
| 2           | 0    | !0   | Input Data present, no Output Data - L is set to Lc     |
| 3           | !0   | 0    | Output Data present, no Input Data - L is set to Le     |
| 4           | !0   | !0   | Both Input and Output Data are present - L is set to Lc |

### APDU Response payload encoding

APDU Response payloads are encoded as follows:

| _Description_                      | _Length_ |
| ---------------------------------- | -------- |
| APDU response length (big endian)  | 2        |
| APDU response data and Status Word | var      |

### USB mapping

Messages are exchanged with the dongle over HID endpoints over interrupt transfers, with each chunk being 64 bytes long. The HID Report ID is ignored.

## Status words

The following standard Status Words are returned for all APDUs - some specific Status Words can be used for specific commands and are mentioned in the command description.

**Status words**

| _SW_ | _Description_                                                                 |
| ---- | ----------------------------------------------------------------------------- |
| 6700 | Incorrect length                                                              |
| 6982 | Security status not satisfied (dongle is locked or busy with another request) |
| 6985 | User declined the request                                                     |
| 6A80 | Invalid input data                                                            |
| 6A81 | Failed to verify the provided signature                                       |
| 6A82 | Parent block data cache-miss (cache parent before sign)                       |
| 6B00 | Incorrect parameter P1 or P2                                                  |
| 6Fxx | Technical problem (Internal error, please report)                             |
| 9000 | Normal ending of the command                                                  |
