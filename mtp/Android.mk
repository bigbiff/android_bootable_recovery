LOCAL_PATH := $(call my-dir)

# Build libtwrpmtp library

include $(CLEAR_VARS)
LOCAL_MODULE := libtwrpmtp
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS = -D_FILE_OFFSET_BITS=64 -DMTP_DEVICE -DMTP_HOST -fno-strict-aliasing
LOCAL_C_INCLUDES += $(LOCAL_PATH) bionic frameworks/base/include system/core/include bionic/libc/private/ bootable/recovery/twrplibusbhost/include
ifeq ($(shell test $(PLATFORM_SDK_VERSION) -lt 23; echo $$?),0)
    LOCAL_C_INCLUDES += external/stlport/stlport
    LOCAL_SHARED_LIBRARIES += libstlport
else
    LOCAL_SHARED_LIBRARIES += libc++
endif

LOCAL_SHARED_LIBRARIES += libcutils

LOCAL_SRC_FILES = \
    btree.cpp \
    MtpDataPacket.cpp \
    MtpDebug.cpp \
    MtpDevice.cpp \
    MtpDevHandle.cpp \
    MtpDeviceInfo.cpp \
    MtpEventPacket.cpp \
    MtpObjectInfo.cpp \
    MtpPacket.cpp \
    MtpProperty.cpp \
    MtpRequestPacket.cpp \
    MtpResponsePacket.cpp \
    MtpServer.cpp \
    MtpStorage.cpp \
    MtpStorageInfo.cpp \
    MtpStringBuffer.cpp \
    MtpUtils.cpp \
    mtp_MtpServer.cpp \
    twrpMtp.cpp \
    mtp_MtpDatabase.cpp \
    node.cpp

ifeq ($(shell test $(PLATFORM_SDK_VERSION) -gt 25; echo $$?),0)
    LOCAL_CFLAGS += -D_FFS_DEVICE
    LOCAL_SRC_FILES += \
        MtpFfsHandle.cpp
endif

LOCAL_SHARED_LIBRARIES += libz libc libusbhost libstdc++ libdl libcutils libutils libaosprecovery libselinux
LOCAL_C_INCLUDES += bootable/recovery/twrplibusbhost/include

ifneq ($(TW_MTP_DEVICE),)
	LOCAL_CFLAGS += -DUSB_MTP_DEVICE=$(TW_MTP_DEVICE)
endif
ifeq ($(shell test $(PLATFORM_SDK_VERSION) -gt 25; echo $$?),0)
    LOCAL_CFLAGS += -DHAS_USBHOST_TIMEOUT
endif

include $(BUILD_SHARED_LIBRARY)

# Build twrpmtp binary / executable

include $(CLEAR_VARS)

LOCAL_MODULE := twrpmtp
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS = -D_FILE_OFFSET_BITS=64 -DMTP_DEVICE -DMTP_HOST -DTWRPMTP
LOCAL_C_INCLUDES += $(LOCAL_PATH) bionic frameworks/base/include system/core/include bionic/libc/private/
LOCAL_SRC_FILES = \
    btree.cpp \
    MtpDataPacket.cpp \
    MtpDebug.cpp \
    MtpDevice.cpp \
    MtpDeviceInfo.cpp \
    MtpEventPacket.cpp \
    MtpObjectInfo.cpp \
    MtpPacket.cpp \
    MtpProperty.cpp \
    MtpRequestPacket.cpp \
    MtpResponsePacket.cpp \
    MtpServer.cpp \
    MtpStorage.cpp \
    MtpStorageInfo.cpp \
    MtpStringBuffer.cpp \
    MtpUtils.cpp \
    mtp_MtpServer.cpp \
    twrpMtp.cpp \
    mtp_MtpDatabase.cpp \
    node.cpp
LOCAL_SHARED_LIBRARIES += libz libc libusbhost libstdc++ libdl libcutils libutils libaosprecovery
include $(BUILD_EXECUTABLE)
