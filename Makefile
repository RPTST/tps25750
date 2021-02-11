# Copyright 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
#
# Makefile for the TPS25750 USB-PD.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=tps25750
PKG_VERSION:=1.1.0.0
PKG_RELEASE:=1

#SOURCE_DIR:=./

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	CATEGORY:=Test Programs
	SUBMENU:=Trieste
	TITLE:=USB-PD
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) $(SOURCE_DIR)/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/description
	PTC application.
endef

define Build/Compile
	$(TARGET_CC) -Wall -Werror -o $(PKG_BUILD_DIR)/tps $(PKG_BUILD_DIR)/src/tps_main.c -lfdt
endef

define Package/$(PKG_NAME)/install
	echo "In Package/$(PKG_NAME)/install: Bello"
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) -m 0755 $(PKG_BUILD_DIR)/tps $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
