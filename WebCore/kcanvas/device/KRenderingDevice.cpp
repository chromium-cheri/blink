/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2006       Alexander Kellett <lypanov@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "config.h"
#include "KRenderingDevice.h"

KCanvasCommonArgs::KCanvasCommonArgs()
{
    m_path = 0;
    m_renderStyle = 0;
    m_renderPath = 0;
}

KCanvasCommonArgs::~KCanvasCommonArgs()
{
}

KCanvasPath* KCanvasCommonArgs::path() const
{
    return m_path.get();
}

void KCanvasCommonArgs::setPath(KCanvasPath* path)
{
    m_path = path;
}

khtml::RenderStyle *KCanvasCommonArgs::renderStyle() const
{
    return m_renderStyle;
}

void KCanvasCommonArgs::setRenderStyle(khtml::RenderStyle *renderStyle)
{
    m_renderStyle = renderStyle;
}

const RenderPath* KCanvasCommonArgs::renderPath() const
{
    return m_renderPath;
}

void KCanvasCommonArgs::setRenderPath(const RenderPath *renderPath)
{
    m_renderPath = renderPath;
}

KRenderingDevice::KRenderingDevice()
{
}

KRenderingDevice::~KRenderingDevice()
{
}

KRenderingDeviceContext *KRenderingDevice::currentContext() const
{
    return m_contextStack.current();
}

KRenderingDeviceContext *KRenderingDevice::popContext()
{
    return m_contextStack.pop();
}

void KRenderingDevice::pushContext(KRenderingDeviceContext *context)
{
    m_contextStack.push(context);
}

// vim:ts=4:noet
