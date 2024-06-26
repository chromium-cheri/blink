// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CompositorWorkerMessagingProxy_h
#define CompositorWorkerMessagingProxy_h

#include "core/workers/WorkerMessagingProxy.h"
#include "wtf/FastAllocBase.h"

namespace blink {

class CompositorWorkerMessagingProxy final : public WorkerMessagingProxy {
    WTF_MAKE_FAST_ALLOCATED(CompositorWorkerMessagingProxy);
public:
    explicit CompositorWorkerMessagingProxy(InProcessWorkerBase*);

protected:
    ~CompositorWorkerMessagingProxy() override;

    PassRefPtr<WorkerThread> createWorkerThread(double originTime) override;
};

} // namespace blink

#endif // CompositorWorkerMessagingProxy_h
