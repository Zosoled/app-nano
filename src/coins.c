#include "coins.h"

libn_coin_conf_t libn_coin_conf_D;

void init_coin_config(libn_coin_type_t coin_type) {
    switch (coin_type) {
        case LIBN_COIN_TYPE_NANO: {
            libn_coin_conf_D = (libn_coin_conf_t){
                .coinName = "Nano",
                .coinBadge = &C_app_nano_14px,
                .bip32Prefix = {HARDENED(44), HARDENED(165)},
                .addressPrefix = "nano_",
                .defaultUnit = "nano",
                .defaultUnitScale = 30,  // 1 nano = 10^30 raw
            };
            return;
        }
        case LIBN_COIN_TYPE_BANANO: {
            libn_coin_conf_D = (libn_coin_conf_t){
                .coinName = "Banano",
                .coinBadge = &C_app_banano_14px,
                .bip32Prefix = {HARDENED(44), HARDENED(198)},
                .addressPrefix = "ban_",
                .defaultUnit = "banano",
                .defaultUnitScale = 29,  // 1 banano = 10^29 raw
            };
            return;
        }
    }
    /* avoid default statement to get warnings when a case was missed */
    app_exit();
}
