#pragma once
#include <QLabel>
#include <functional>



class CalibrationGaugeLabel : public QLabel
{
	Q_OBJECT
private:
	QPixmap original_pixmap_;
	const std::unique_ptr<bool>& helper_lines_toggled;
	QPoint line_draw_start_;
	QPoint line_draw_end_;
	bool is_mouse_currently_dragging = false;
	std::function<void(const QPoint&, const QPoint&)> on_mouse_release_callback_;

public:
	CalibrationGaugeLabel(const std::unique_ptr<bool>& toggle, QWidget* parent = nullptr);
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void paintEvent(QPaintEvent* event) override;
	void SetOnMouseReleaseCallback(const std::function<void(const QPoint&, const QPoint&)>& callback);
	void ClearDrawnLines();
};

