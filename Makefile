ifneq (,$(filter openthread_core,$(USEMODULE)))
 DIRS += src/core
 DIRS += src/core/common
 DIRS += src/core/mac
 DIRS += src/core/thread
 DIRS += src/core/coap
 DIRS += src/core/crypto
 DIRS += src/core/net
 DIRS += src/core/api
 #DIRS += src/core/meshcop
 DIRS += third_party/mbedtls
 DIRS += third_party/mbedtls/repo/library
endif

ifneq (,$(filter openthread_core,$(USEMODULE)))
 DIRS += src/cli
endif

ifneq (,$(filter openthread_ncp,$(USEMODULE)))
 DIRS += src/ncp
endif

#CPPFLAGS = -DMBEDTLS_CONFIG_FILE='\"mbedtls-config.h\"'

include $(RIOTBASE)/Makefile.base
