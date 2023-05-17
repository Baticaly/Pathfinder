################################################################################
#
# PATHFINDER
#
################################################################################

PATHFINDER_VERSION = 1.0
PATHFINDER_SITE = ./package/pathfinder/src
PATHFINDER_SITE_METHOD = local

define PATHFINDER_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define PATHFINDER_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/pathfinder $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/pathfinder-service $(TARGET_DIR)/etc/init.d
endef

$(eval $(generic-package))