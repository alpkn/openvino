// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <memory>

#include "ie_blob.h"

namespace InferenceEngine {

Blob::Blob(const Blob& blob): tensorDesc(blob.tensorDesc), roiPtr(blob.roiPtr ? new ROIData(*blob.roiPtr) : nullptr) {}

void Blob::setROI(const ROIData& roiData) {
    roiPtr.reset(new ROIData(roiData));
    tensorDesc = make_roi_tensor_desc(tensorDesc, roiPtr->roi);
}

Blob::Ptr Blob::CreateROIBlob(const ROI& roi) const {
    Blob::Ptr roiBlob = std::shared_ptr<Blob>(this->shallowClone());
    try {
        roiBlob->setROI(ROIData{roi, this->shared_from_this()});
        return roiBlob->shared_from_this();
    } catch (const std::exception& ex) {
        THROW_IE_EXCEPTION << "Cannot create ROI blob with the specified parameters: " << ex.what();
    }
}

bool operator==(const ROI& l, const ROI& r) {
    return l.id == r.id && l.posX == r.posX && l.posY == r.posY && l.sizeX == r.sizeX && l.sizeY == r.sizeY;
}

bool operator!=(const ROI& l, const ROI& r) {
    return !operator==(l, r);
}

}  // namespace InferenceEngine
