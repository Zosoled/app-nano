# Nano ($XNO) Transaction Serialization

## Overview

The transaction serialization presented is for the Nano block lattice (see [Links](#links)).

## Amount units

The unit used in transactions is called _raw_ and is a 128-bit value. The most common base unit, and the unit displayed on the device when confirming transactions, is _nano_ which is 10³⁰ raw.

## Address format

Nano addresses are base-32 numbers derived from the public key and a checksum of the public key. It is prefixed with the text `nano_` to more easily distinguish it as an address.

## Structure

### Transaction

| Field            | Size (bytes) | Description                                                                |
| ---------------- | :----------: | -------------------------------------------------------------------------- |
| `preamble`       |      32      | Always the value 0x6                                                       |
| `account`        |      32      | Public key of the account being rebalanced                                 |
| `previous`       |      32      | Hash of the account's head block, or 0 for the account's first transaction |
| `representative` |      32      | Representative for the account for network consensus                       |
| `balance`        |      16      | Resulting balance of the account after the transaction                     |
| `link`           |      32      | Variable (see below)                                                       |

### Link

Depending on the action each transaction intends to perform, the `link` field will have a different value.

| Action    | Link value             | Description                                                              |
| --------- | ---------------------- | ------------------------------------------------------------------------ |
| `change`  | Constant (0x0)         | Transaction is changing the account's representative and not its balance |
| `send`    | Public key (recipient) | Destination account which will receive funds                             |
| `receive` | Block hash (origin)    | Corresponding transaction which sent the funds to be received            |

### Signature

Deterministic EdDSA ([RFC 6979](https://tools.ietf.org/html/rfc8419)) is used to sign transactions on the [Curve25519](https://www.rfc-editor.org/rfc/rfc8032) elliptic curve with BLAKE2b used for the internal hash instead of SHA-512.

The signed message is `m = BLAKE2b(preamble || account || previous || representative || balance || link)`.

### Fee

There is no fee in the transaction structure because Nano is a feeless currency.

## Links

- [Nano Transaction](https://docs.nano.org/integration-guides/the-basics/#blocks-specifications)
- [Nano Consensus Mechanism](https://docs.nano.org/protocol-design/orv-consensus/)
