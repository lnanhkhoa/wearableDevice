#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc13x0_sdk_1_50_00_08/source;C:/ti/simplelink_cc13x0_sdk_1_50_00_08/kernel/tirtos/packages;D:/ti/ccsv7/ccs_base;C:/Users/AppsTI/workspace_blestack/simple_peripheral_cc1350lp_app_FlashROM/.config
override XDCROOT = D:/ti/xdctools_3_50_02_20_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc13x0_sdk_1_50_00_08/source;C:/ti/simplelink_cc13x0_sdk_1_50_00_08/kernel/tirtos/packages;D:/ti/ccsv7/ccs_base;C:/Users/AppsTI/workspace_blestack/simple_peripheral_cc1350lp_app_FlashROM/.config;D:/ti/xdctools_3_50_02_20_core/packages;..
HOSTOS = Windows
endif
