/**
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright (C) 1997 Martin Jones (mjones@kde.org)
 *           (C) 1997 Torben Weis (weis@kde.org)
 *           (C) 1998 Waldo Bastian (bastian@kde.org)
 *           (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "RenderTableRow.h"

#include "CachedImage.h"
#include "Document.h"
#include "HTMLNames.h"
#include "RenderTableCell.h"
#include "RenderView.h"

#if ENABLE(WML)
#include "WMLNames.h"
#endif

namespace WebCore {

using namespace HTMLNames;

RenderTableRow::RenderTableRow(Node* node)
    : RenderBox(node)
{
    // init RenderObject attributes
    setInline(false);   // our object is not Inline
}

void RenderTableRow::destroy()
{
    RenderTableSection* recalcSection = section();
    
    RenderBox::destroy();
    
    if (recalcSection)
        recalcSection->setNeedsCellRecalc();
}

void RenderTableRow::styleWillChange(StyleDifference diff, const RenderStyle* newStyle)
{
    if (section() && style() && style()->height() != newStyle->height())
        section()->setNeedsCellRecalc();

    ASSERT(newStyle->display() == TABLE_ROW);

    RenderBox::styleWillChange(diff, newStyle);
}

void RenderTableRow::addChild(RenderObject* child, RenderObject* beforeChild)
{
    // Make sure we don't append things after :after-generated content if we have it.
    if (!beforeChild && isAfterContent(lastChild()))
        beforeChild = lastChild();

    bool isTableRow = node() && node()->hasTagName(trTag);

#if ENABLE(WML)
    if (!isTableRow && node() && node()->isWMLElement())
        isTableRow = node()->hasTagName(WMLNames::trTag);
#endif

    if (!child->isTableCell()) {
        if (isTableRow && child->node() && child->node()->hasTagName(formTag) && document()->isHTMLDocument()) {
            RenderBox::addChild(child, beforeChild);
            return;
        }

        RenderObject* last = beforeChild;
        if (!last)
            last = lastChild();
        if (last && last->isAnonymous() && last->isTableCell()) {
            last->addChild(child);
            return;
        }

        // If beforeChild is inside an anonymous cell, insert into the cell.
        if (last && !last->isTableCell() && last->parent() && last->parent()->isAnonymous()) {
            last->parent()->addChild(child, beforeChild);
            return;
        }

        RenderTableCell* cell = new (renderArena()) RenderTableCell(document() /* anonymous object */);
        RefPtr<RenderStyle> newStyle = RenderStyle::create();
        newStyle->inheritFrom(style());
        newStyle->setDisplay(TABLE_CELL);
        cell->setStyle(newStyle.release());
        addChild(cell, beforeChild);
        cell->addChild(child);
        return;
    } 
    
    // If the next renderer is actually wrapped in an anonymous table cell, we need to go up and find that.
    while (beforeChild && beforeChild->parent() != this)
        beforeChild = beforeChild->parent();

    RenderTableCell* cell = static_cast<RenderTableCell*>(child);

    // Generated content can result in us having a null section so make sure to null check our parent.
    if (parent())
        section()->addCell(cell, this);

    ASSERT(!beforeChild || beforeChild->isTableCell() || isTableRow && beforeChild->node() && beforeChild->node()->hasTagName(formTag) && document()->isHTMLDocument());
    RenderBox::addChild(cell, beforeChild);

    if (beforeChild || nextSibling())
        section()->setNeedsCellRecalc();
}

void RenderTableRow::layout()
{
    ASSERT(needsLayout());

    // Table rows do not add translation.
    LayoutStateMaintainer statePusher(view(), this, IntSize());

    for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
        if (child->isTableCell()) {
            RenderTableCell* cell = static_cast<RenderTableCell*>(child);
            if (child->needsLayout()) {
                cell->calcVerticalMargins();
                cell->layout();
            }
        }
    }

    // We only ever need to repaint if our cells didn't, which menas that they didn't need
    // layout, so we know that our bounds didn't change. This code is just making up for
    // the fact that we did not repaint in setStyle() because we had a layout hint.
    // We cannot call repaint() because our clippedOverflowRectForRepaint() is taken from the
    // parent table, and being mid-layout, that is invalid. Instead, we repaint our cells.
    if (selfNeedsLayout() && checkForRepaintDuringLayout()) {
        for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
            if (child->isTableCell())
                child->repaint();
        }
    }

    statePusher.pop();
    setNeedsLayout(false);
}

IntRect RenderTableRow::clippedOverflowRectForRepaint(RenderBoxModelObject* repaintContainer)
{
    // For now, just repaint the whole table.
    // FIXME: Find a better way to do this, e.g., need to repaint all the cells that we
    // might have propagated a background color into.
    // FIXME: do repaintContainer checks here
    if (RenderTable* parentTable = table())
        return parentTable->clippedOverflowRectForRepaint(repaintContainer);

    return IntRect();
}

// Hit Testing
bool RenderTableRow::nodeAtPoint(const HitTestRequest& request, HitTestResult& result, int x, int y, int tx, int ty, HitTestAction action)
{
    // Table rows cannot ever be hit tested.  Effectively they do not exist.
    // Just forward to our children always.
    for (RenderObject* child = lastChild(); child; child = child->previousSibling()) {
        // FIXME: We have to skip over inline flows, since they can show up inside table rows
        // at the moment (a demoted inline <form> for example). If we ever implement a
        // table-specific hit-test method (which we should do for performance reasons anyway),
        // then we can remove this check.
        if (child->isTableCell() && !toRenderBox(child)->hasSelfPaintingLayer() && child->nodeAtPoint(request, result, x, y, tx, ty, action)) {
            updateHitTestResult(result, IntPoint(x - tx, y - ty));
            return true;
        }
    }
    
    return false;
}

void RenderTableRow::paint(PaintInfo& paintInfo, int tx, int ty)
{
    ASSERT(hasSelfPaintingLayer());
    if (!layer())
        return;
    for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
        if (child->isTableCell()) {
            // Paint the row background behind the cell.
            if (paintInfo.phase == PaintPhaseBlockBackground || paintInfo.phase == PaintPhaseChildBlockBackground) {
                RenderTableCell* cell = static_cast<RenderTableCell*>(child);
                cell->paintBackgroundsBehindCell(paintInfo, tx, ty, this);
            }
            if (!toRenderBox(child)->hasSelfPaintingLayer())
                child->paint(paintInfo, tx, ty);
        }
    }
}

void RenderTableRow::imageChanged(WrappedImagePtr, const IntRect*)
{
    // FIXME: Examine cells and repaint only the rect the image paints in.
    repaint();
}

} // namespace WebCore
