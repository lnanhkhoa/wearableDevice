#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc13x0_sdk_1_40_00_10/source;C:/ti/simplelink_cc13x0_sdk_1_40_00_10/kernel/tirtos/packages;C:/ti/ccs730/ccsv7/ccs_base;C:/Users/User/workspace_v7/simple_peripheral_cc1350lp_app_FlashROM/.config
override XDCROOT = C:/ti/ccs730/xdctools_3_50_03_33_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc13x0_sdk_1_40_00_10/source;C:/ti/simplelink_cc13x0_sdk_1_40_00_10/kernel/tirtos/packages;C:/ti/ccs730/ccsv7/ccs_base;C:/Users/User/workspace_v7/simple_peripheral_cc1350lp_app_FlashROM/.config;C:/ti/ccs730/xdctools_3_50_03_33_core/packages;..
HOSTOS = Windows
endif
