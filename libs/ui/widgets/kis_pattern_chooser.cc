/*
 *  Copyright (c) 2004 Adrian Page <adrian@pagenet.plus.com>
 *  Copyright (C) 2011 Srikanth Tiyyagura <srikanth.tulasiram@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "widgets/kis_pattern_chooser.h"

#include <math.h>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QShowEvent>

#include <klocalizedstring.h>
#include <KoResourceItemChooser.h>
#include <KoResourceServerAdapter.h>
#include <KoResourceServerProvider.h>

#include "kis_signals_blocker.h"

#include "kis_global.h"
#include <kis_config.h>
#include <resources/KoPattern.h>

#include <ksqueezedtextlabel.h>

KisPatternChooser::KisPatternChooser(QWidget *parent)
        : QFrame(parent)
{
    m_lblName = new KSqueezedTextLabel(this);
    m_lblName->setTextElideMode(Qt::ElideLeft);

    KoResourceServer<KoPattern> * rserver = KoResourceServerProvider::instance()->patternServer();
    QSharedPointer<KoAbstractResourceServerAdapter> adapter (new KoResourceServerAdapter<KoPattern>(rserver));
    m_itemChooser = new KoResourceItemChooser(adapter, this, true);
    m_itemChooser->setPreviewTiled(true);
    m_itemChooser->setPreviewOrientation(Qt::Horizontal);
    m_itemChooser->showTaggingBar(true);
    m_itemChooser->setSynced(true);

    connect(m_itemChooser, SIGNAL(resourceSelected(KoResource *)),
            this, SLOT(update(KoResource *)));

    connect(m_itemChooser, SIGNAL(resourceSelected(KoResource *)),
            this, SIGNAL(resourceSelected(KoResource *)));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_lblName);
    mainLayout->addWidget(m_itemChooser, 10);

    KisConfig cfg;
    m_itemChooser->configureKineticScrolling(cfg.kineticScrollingGesture(),
                                         cfg.kineticScrollingSensitivity(),
                                         cfg.kineticScrollingScrollbar());

    setLayout(mainLayout);
}

KisPatternChooser::~KisPatternChooser()
{
}

KoResource *  KisPatternChooser::currentResource()
{
    if (!m_itemChooser->currentResource()) {
        KoResourceServer<KoPattern> * rserver = KoResourceServerProvider::instance()->patternServer();
        if (rserver->resources().size() > 0) {
            KisSignalsBlocker blocker(m_itemChooser);
            m_itemChooser->setCurrentResource(rserver->resources().first());
        }
    }
    return m_itemChooser->currentResource();
}

void KisPatternChooser::setCurrentPattern(KoResource *resource)
{
    m_itemChooser->setCurrentResource(resource);
}

void KisPatternChooser::setCurrentItem(int row, int column)
{
    m_itemChooser->setCurrentItem(row, column);
    if (currentResource()) {
        update(currentResource());
    }
}

void KisPatternChooser::setPreviewOrientation(Qt::Orientation orientation)
{
    m_itemChooser->setPreviewOrientation(orientation);
}

void KisPatternChooser::update(KoResource * resource)
{
    m_lblName->setFixedWidth(m_itemChooser->width());
    KoPattern *pattern = static_cast<KoPattern *>(resource);
    m_lblName->setText(QString("%1 (%2 x %3)").arg(i18n(pattern->name().toUtf8().data())).arg(pattern->width()).arg(pattern->height()));
}

void KisPatternChooser::setGrayscalePreview(bool grayscale)
{
    m_itemChooser->setGrayscalePreview(grayscale);
}


