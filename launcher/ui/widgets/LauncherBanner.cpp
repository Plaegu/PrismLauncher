// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "LauncherBanner.h"

#include <QFontMetrics>
#include <QLinearGradient>
#include <QPainter>
#include <QPaintEvent>

LauncherBanner::LauncherBanner(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void LauncherBanner::setImage(const QPixmap& image)
{
    m_image = image;
    m_scaled = QPixmap();
    m_scaledFor = QSize();
    update();
}

void LauncherBanner::setTitle(const QString& title)
{
    m_title = title;
    update();
}

void LauncherBanner::setSubtitle(const QString& subtitle)
{
    m_subtitle = subtitle;
    update();
}

QSize LauncherBanner::sizeHint() const
{
    return { 720, 360 };
}

QSize LauncherBanner::minimumSizeHint() const
{
    return { 320, 180 };
}

void LauncherBanner::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    const QRect area = rect();

    if (!m_image.isNull()) {
        // Scale to cover the whole area, cache the result per widget size.
        const qreal dpr = devicePixelRatioF();
        const QSize target = area.size() * dpr;
        if (m_scaledFor != target || m_scaled.isNull()) {
            m_scaled = m_image.scaled(target, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            m_scaled.setDevicePixelRatio(dpr);
            m_scaledFor = target;
        }
        const QSizeF logical = m_scaled.deviceIndependentSize();
        const QPointF topLeft((area.width() - logical.width()) / 2.0, (area.height() - logical.height()) / 2.0);
        painter.drawPixmap(topLeft, m_scaled);
    } else {
        // No image: a calm dark gradient with a hint of green.
        QLinearGradient bg(area.topLeft(), area.bottomRight());
        bg.setColorAt(0.0, QColor(0x1f, 0x3a, 0x2a));
        bg.setColorAt(0.55, QColor(0x1a, 0x22, 0x1e));
        bg.setColorAt(1.0, QColor(0x14, 0x14, 0x16));
        painter.fillRect(area, bg);
    }

    // Darken the bottom so the text is always readable.
    QLinearGradient shade(QPointF(0, area.height() * 0.35), QPointF(0, area.height()));
    shade.setColorAt(0.0, QColor(0, 0, 0, 0));
    shade.setColorAt(1.0, QColor(0, 0, 0, 200));
    painter.fillRect(area, shade);

    const int margin = 32;
    const int textWidth = area.width() - margin * 2;

    QFont subtitleFont = font();
    subtitleFont.setPointSizeF(qMax(10.0, font().pointSizeF() * 1.1));
    QFontMetrics subtitleMetrics(subtitleFont);

    QFont titleFont = font();
    titleFont.setPointSizeF(qMax(22.0, font().pointSizeF() * 2.6));
    titleFont.setWeight(QFont::Bold);
    QFontMetrics titleMetrics(titleFont);

    int y = area.height() - margin;

    if (!m_subtitle.isEmpty()) {
        painter.setFont(subtitleFont);
        painter.setPen(QColor(0xd4, 0xd4, 0xd8));
        const QString text = subtitleMetrics.elidedText(m_subtitle, Qt::ElideRight, textWidth);
        painter.drawText(QPoint(margin, y), text);
        y -= subtitleMetrics.height() + 6;
    }

    if (!m_title.isEmpty()) {
        painter.setFont(titleFont);
        painter.setPen(Qt::white);
        const QString text = titleMetrics.elidedText(m_title, Qt::ElideRight, textWidth);
        painter.drawText(QPoint(margin, y), text);
    }
}
