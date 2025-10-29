/*******************************************************************************
 *   Ledger App for Nano ($XNO)
 *   (c) 2018 Mart Roosmaa
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/
#pragma once

#include "libn_types.h"

#define HARDENED(x) ((x) + 0x80000000)

extern libn_coin_conf_t libn_coin_conf_D;

void init_coin_config(libn_coin_type_t coin_type);

#define COIN_NAME           libn_coin_conf_D.coinName
#define COIN_BADGE          libn_coin_conf_D.coinBadge
#define COIN_BIP32_PREFIX   libn_coin_conf_D.bip32Prefix
#define COIN_ADDRESS_PREFIX libn_coin_conf_D.addressPrefix
#define COIN_UNIT           libn_coin_conf_D.defaultUnit
#define COIN_UNIT_SCALE     libn_coin_conf_D.defaultUnitScale
