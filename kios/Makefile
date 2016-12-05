include $(TOPDIR)/rules.mk
PKG_NAME:=kios
PKG_RELEASE:=1
PKG_VERSION:=$(Version)
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Wise kios
  DEPENDS:=+libpthread +libopenssl 
#  +libopenssl

endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile

	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)

 
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/bin/AppData
	$(INSTALL_DIR) $(1)/usr/bin/AppData/kios
	$(CP) -rf $(PKG_BUILD_DIR)/config/* $(1)/usr/bin/AppData/kios/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/kios $(1)/usr/bin/AppData/kios/kios
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/kios.init $(1)/etc/init.d/kios
	ln -sf /usr/bin/AppData/kios/kios $(1)/usr/bin/kios
endef

$(eval $(call BuildPackage,kios,+libpthread +libopenssl ))
