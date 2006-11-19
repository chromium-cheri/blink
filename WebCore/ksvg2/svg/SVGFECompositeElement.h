/*
    Copyright (C) 2004, 2005 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>

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

#ifndef KSVG_SVGFECompositeElementImpl_H
#define KSVG_SVGFECompositeElementImpl_H
#ifdef SVG_SUPPORT

#include "SVGFEComposite.h"
#include "SVGFilterPrimitiveStandardAttributes.h"

namespace WebCore
{

    class SVGFECompositeElement : public SVGFilterPrimitiveStandardAttributes
    {
    public:
        SVGFECompositeElement(const QualifiedName&, Document*);
        virtual ~SVGFECompositeElement();

        // 'SVGFECompositeElement' functions
        // Derived from: 'Element'
        virtual void parseMappedAttribute(MappedAttribute* attr);

        virtual SVGFEComposite* filterEffect() const;

    protected:
        virtual const SVGElement* contextElement() const { return this; }

    private:
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, String, String, In1, in1)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, String, String, In2, in2)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, int, int, _operator, _operator)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, double, double, K1, k1)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, double, double, K2, k2)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, double, double, K3, k3)
        ANIMATED_PROPERTY_DECLARATIONS(SVGFECompositeElement, double, double, K4, k4)
        mutable SVGFEComposite* m_filterEffect;
    };

} // namespace WebCore

#endif // SVG_SUPPORT
#endif

// vim:ts=4:noet
