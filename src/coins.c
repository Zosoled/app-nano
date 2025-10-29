#include "coins.h"
#include "coins_dsl.h"

libn_coin_conf_t libn_coin_conf_D;

REGISTER_COINS(COIN(LIBN_COIN_TYPE_NANO,
                    {
                        .coinName = "Nano",
                        .coinBadge = &C_app_nano_14px,
                        .bip32Prefix = {HARDENED(44), HARDENED(165)},
                        .addressPrefix = "nano_",
                        .defaultUnit = "nano",
                        .defaultUnitScale = 30,  // 1 nano = 10^30 raw
                    })

                   COIN(LIBN_COIN_TYPE_BANANO,
                        {
                            .coinName = "Banano",
                            .coinBadge = &C_app_banano_14px,
                            .bip32Prefix = {HARDENED(44), HARDENED(198)},
                            .addressPrefix = "ban_",
                            .defaultUnit = "banano",
                            .defaultUnitScale = 29,  // 1 banano = 10^29 raw
                        }))
