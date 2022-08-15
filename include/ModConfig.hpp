#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(RainbowConfig,
    CONFIG_VALUE(RainbowEnabled, bool, "Rainbow Enabled", true);
    CONFIG_VALUE(RainbowSpeed, float, "Rainbow Speed", 1.0f);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(RainbowEnabled);
        CONFIG_INIT_VALUE(RainbowSpeed);
    )
)