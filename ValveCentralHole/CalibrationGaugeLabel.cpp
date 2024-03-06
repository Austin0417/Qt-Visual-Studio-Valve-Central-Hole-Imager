#include "CalibrationGaugeLabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>

CalibrationGaugeLabel::CalibrationGaugeLabel(const std::unique_ptr<bool>& toggle, QWidget* parent) : helper_lines_toggled(toggle), QLabel(parent)
{

}


void CalibrationGaugeLabel::mousePressEvent(QMouseEvent* event)
{
	QLabel::mousePressEvent(event);

	if (*helper_lines_toggled)
	{
		line_draw_start_ = event->pos();
		is_mouse_currently_dragging = true;
	}
}

void CalibrationGaugeLabel::mouseReleaseEvent(QMouseEvent* event)
{
	QLabel::mouseReleaseEvent(event);

	if (*helper_lines_toggled)
	{
		line_draw_end_ = event->pos();
		is_mouse_currently_dragging = false;
		if (on_mouse_release_callback_)
		{
			// Before sending the start and end point of the drawn line, make sure the y coordinates for both lines are equal. If not, make the end point's y equal to the start's
			if (line_draw_start_.y() != line_draw_end_.y())
			{
				line_draw_end_.setY(line_draw_start_.y());
			}
			on_mouse_release_callback_(line_draw_start_, line_draw_end_);
		}
		update();
	}
}

void CalibrationGaugeLabel::mouseMoveEvent(QMouseEvent* event)
{
	QLabel::mouseMoveEvent(event);

	if (*helper_lines_toggled && is_mouse_currently_dragging)
	{
		line_draw_end_ = event->pos();
		update();
	}
}

void CalibrationGaugeLabel::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);

	if (original_pixmap_.isNull() && !pixmap().isNull())
	{
		original_pixmap_ = pixmap().copy();
	}

	if (*helper_lines_toggled && !pixmap().isNull())
	{
		if (!line_draw_start_.isNull() && !line_draw_end_.isNull())
		{
			setPixmap(original_pixmap_);
			QPixmap current_pixmap = pixmap();
			QPainter painter(&current_pixmap);
			QPen pen(QBrush(QColor(255, 0, 0)), 5);
			painter.setPen(pen);
			painter.drawLine(line_draw_start_, line_draw_end_);


			if (!is_mouse_currently_dragging)
			{
				int line_width = std::abs(line_draw_end_.x() - line_draw_start_.x());
				if (line_draw_start_.x() <= line_draw_end_.x())
				{
					painter.drawText(QPoint(line_draw_start_.x() + (line_width / 2),
						line_draw_start_.y() - 15),
						QString::number(line_width));

				}
				else
				{
					painter.drawText(QPoint(line_draw_end_.x() + (line_width / 2),
						line_draw_start_.y() - 15),
						QString::number(line_width));
				}
				line_draw_start_ = QPoint();
			}
			line_draw_end_ = QPoint();
			setPixmap(current_pixmap);
		}
	}
}

void CalibrationGaugeLabel::SetOnMouseReleaseCallback(const std::function<void(const QPoint&, const QPoint&)>& callback)
{
	on_mouse_release_callback_ = callback;
}

void CalibrationGaugeLabel::ClearDrawnLines()
{
	if (!original_pixmap_.isNull())
	{
		setPixmap(original_pixmap_);
	}
}

