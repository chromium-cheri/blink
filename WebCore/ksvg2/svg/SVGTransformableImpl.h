/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

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
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef KSVG_SVGTransformableImpl_H
#define KSVG_SVGTransformableImpl_H
#if SVG_SUPPORT

#include "SVGLocatableImpl.h"

class QMatrix;

namespace WebCore {
    class NodeImpl;
    class AtomicString;
    class AttributeImpl;
};

namespace WebCore
{
    class SVGMatrixImpl;
    class SVGTransformListImpl;
    class SVGAnimatedTransformListImpl;
    class SVGTransformableImpl : public SVGLocatableImpl
    {
    public:
        SVGTransformableImpl();
        virtual ~SVGTransformableImpl();

        // 'SVGTransformable' functions
        virtual SVGAnimatedTransformListImpl *transform() const = 0;
        virtual SVGMatrixImpl *localMatrix() const = 0;
        
        virtual void updateLocalTransform(SVGTransformListImpl *localTransforms) = 0;
        
        static void parseTransformAttribute(SVGTransformListImpl *list, const AtomicString& transform);
    };
};

#endif // SVG_SUPPORT
#endif

// vim:ts=4:noet
