// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ScrollbarPainter_h
#define ScrollbarPainter_h

#include "platform/heap/Handle.h"
#include "platform/scroll/Scrollbar.h"

namespace blink {

class GraphicsContext;
class IntRect;
class LayoutPoint;
class LayoutRect;
class LayoutScrollbar;
class LayoutScrollbarPart;

class ScrollbarPainter {
    STACK_ALLOCATED();
    WTF_MAKE_NONCOPYABLE(ScrollbarPainter);
public:
    explicit ScrollbarPainter(LayoutScrollbar& layoutScrollbar) : m_layoutScrollbar(&layoutScrollbar) { }

    void paintPart(GraphicsContext*, ScrollbarPart, const IntRect&);
    static void paintIntoRect(LayoutScrollbarPart*, GraphicsContext*, const LayoutPoint& paintOffset, const LayoutRect&);

private:
    RawPtrWillBeMember<LayoutScrollbar> m_layoutScrollbar;
};

} // namespace blink

#endif // ScrollbarPainter_h
