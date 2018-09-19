/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MTP_SERVER_H
#define _MTP_SERVER_H

#include "MtpRequestPacket.h"
#include "MtpDataPacket.h"
#include "MtpResponsePacket.h"
#include "MtpEventPacket.h"
#include "mtp.h"
#include "MtpUtils.h"
#include "IMtpHandle.h"

#include <utils/threads.h>
#include <queue>
#include <memory>
#include <mutex>

class MtpDatabase;
class MtpStorage;

class MtpServer {

private:
    MtpDatabase*        mDatabase;

    // appear as a PTP device
    bool                mPtp;

    // group to own new files and folders
    int                 mFileGroup;
    // permissions for new files and directories
    int                 mFilePermission;
    int                 mDirectoryPermission;

    // Manufacturer to report in DeviceInfo
    MtpString           mDeviceInfoManufacturer;
    // Model to report in DeviceInfo
    MtpString           mDeviceInfoModel;
    // Device version to report in DeviceInfo
    MtpString           mDeviceInfoDeviceVersion;
    // Serial number to report in DeviceInfo
    MtpString           mDeviceInfoSerialNumber;

    // current session ID
    MtpSessionID        mSessionID;
    // true if we have an open session and mSessionID is valid
    bool                mSessionOpen;

    MtpRequestPacket    mRequest;
    MtpDataPacket       mData;
    MtpResponsePacket   mResponse;

    MtpEventPacket      mEvent;

    MtpStorageList      mStorages;

    static IMtpHandle*  sHandle;

    // handle for new object, set by SendObjectInfo and used by SendObject
    MtpObjectHandle     mSendObjectHandle;
    MtpObjectFormat     mSendObjectFormat;
    MtpString           mSendObjectFilePath;
    size_t              mSendObjectFileSize;

    android::Mutex               mMutex;

    // represents an MTP object that is being edited using the android extensions
    // for direct editing (BeginEditObject, SendPartialObject, TruncateObject and EndEditObject)
    class ObjectEdit {
        public:
        MtpObjectHandle     mHandle;
        MtpString           mPath;
        uint64_t            mSize;
        MtpObjectFormat     mFormat;
        int                 mFD;

        ObjectEdit(MtpObjectHandle handle, const char* path, uint64_t size,
            MtpObjectFormat format, int fd)
                : mHandle(handle), mPath(path), mSize(size), mFormat(format), mFD(fd) {
            }

        virtual ~ObjectEdit() {
            close(mFD);
        }
    };
    android::Vector<ObjectEdit*>  mObjectEditList;

public:
                        MtpServer(MtpDatabase* database, bool ptp,
                                    int fileGroup, int filePerm, int directoryPerm,
                                    const MtpString& deviceInfoManufacturer,
                                    const MtpString& deviceInfoModel,
                                    const MtpString& deviceInfoDeviceVersion,
                                    const MtpString& deviceInfoSerialNumber);
    virtual             ~MtpServer();

    MtpStorage*         getStorage(MtpStorageID id);
    inline bool         hasStorage() { return mStorages.size() > 0; }
    bool                hasStorage(MtpStorageID id);
    void                addStorage(MtpStorage* storage);
    void                removeStorage(MtpStorage* storage);

    static int          configure(bool usePtp);
    void                run();

    void                sendObjectAdded(MtpObjectHandle handle);
    void                sendObjectRemoved(MtpObjectHandle handle);
    void                sendObjectUpdated(MtpObjectHandle handle);
    void                sendDevicePropertyChanged(MtpDeviceProperty property);

private:
    void                sendStoreAdded(MtpStorageID id);
    void                sendStoreRemoved(MtpStorageID id);
    void                sendEvent(MtpEventCode code, uint32_t param1);

    void                addEditObject(MtpObjectHandle handle, MtpString& path,
                                uint64_t size, MtpObjectFormat format, int fd);
    ObjectEdit*         getEditObject(MtpObjectHandle handle);
    void                removeEditObject(MtpObjectHandle handle);
    void                commitEdit(ObjectEdit* edit);

    bool                handleRequest();

    MtpResponseCode     doGetDeviceInfo();
    MtpResponseCode     doOpenSession();
    MtpResponseCode     doCloseSession();
    MtpResponseCode     doGetStorageIDs();
    MtpResponseCode     doGetStorageInfo();
    MtpResponseCode     doGetObjectPropsSupported();
    MtpResponseCode     doGetObjectHandles();
    MtpResponseCode     doGetNumObjects();
    MtpResponseCode     doGetObjectReferences();
    MtpResponseCode     doSetObjectReferences();
    MtpResponseCode     doGetObjectPropValue();
    MtpResponseCode     doSetObjectPropValue();
    MtpResponseCode     doGetDevicePropValue();
    MtpResponseCode     doSetDevicePropValue();
    MtpResponseCode     doResetDevicePropValue();
    MtpResponseCode     doGetObjectPropList();
    MtpResponseCode     doGetObjectInfo();
    MtpResponseCode     doGetObject();
    MtpResponseCode     doGetThumb();
    MtpResponseCode     doGetPartialObject(MtpOperationCode operation);
    MtpResponseCode     doSendObjectInfo();
    MtpResponseCode     doSendObject();
    MtpResponseCode     doDeleteObject();
    MtpResponseCode     doGetObjectPropDesc();
    MtpResponseCode     doGetDevicePropDesc();
    MtpResponseCode     doSendPartialObject();
    MtpResponseCode     doTruncateObject();
    MtpResponseCode     doBeginEditObject();
    MtpResponseCode     doEndEditObject();

    bool                SetProperty(const std::string& key, const std::string& value);
};

#endif // _MTP_SERVER_H
