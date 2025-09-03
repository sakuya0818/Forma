#include "SplashScreen.h"
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QPainterPath>

SplashScreen* SplashScreen::m_instance = nullptr;

// 构造函数
SplashScreen::SplashScreen(const QPixmap& pixmap)
	: QSplashScreen(pixmap)
	, m_percent(0)
{
}

// 析构函数
SplashScreen::~SplashScreen()
{

}

// 单例模式
SplashScreen* SplashScreen::getInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new SplashScreen(QPixmap(":/Resource/Image/Forma.png"));
	}

	return m_instance;
}

// 绘制事件
void SplashScreen::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	const int OFFSET_VALUE = 70;
	const int SLIDER_HEIGHT = 10;
	const int BORDER_X_RADIUS = 8;
	const int BORDER_Y_RADIUS = 4;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QFont font(QString("微软雅黑"));
	QPen painterPen;
	painterPen.setColor(Qt::white);
	painter.setPen(painterPen);
	QBrush brush(QColor(100, 100, 100, 255));
	painter.setBrush(brush);
	painter.drawRoundedRect(OFFSET_VALUE, this->height() - OFFSET_VALUE
		, (this->width() - OFFSET_VALUE * 2)
		, BORDER_X_RADIUS, BORDER_Y_RADIUS, SLIDER_HEIGHT);

	// 绘制logo和logo文字
	if (!m_pixLogo.isNull())
	{
		painter.drawPixmap(OFFSET_VALUE, (this->height() - m_pixLogo.height()) / 2, m_pixLogo);

		font.setPixelSize(35);
		font.setBold(true);
		painter.setFont(font);
		painterPen.setColor(Qt::white);
		painter.setPen(painterPen);
		painter.drawText(0, this->height() / 2 - OFFSET_VALUE / 2, this->width(), OFFSET_VALUE, Qt::AlignCenter, m_textLogo);
	}

	// 绘制消息文本
	{
		font.setPixelSize(16);
		font.setBold(true);
		painter.setFont(font);
		painterPen.setColor(Qt::blue);
		painter.setPen(painterPen);
		painter.drawText(0, this->height() - OFFSET_VALUE * 2, this->width(), OFFSET_VALUE, Qt::AlignCenter, m_message);
	}

	// 绘制进度条
	{
		painterPen.setColor(Qt::white);
		painter.setPen(painterPen);
		if (m_percent != 0)
		{
			QBrush brushProgress(QColor(150, 230, 100, 255));
			painter.setBrush(brushProgress);
			painter.drawRoundedRect(OFFSET_VALUE, this->height() - OFFSET_VALUE
				, (this->rect().width() - OFFSET_VALUE * 2) * m_percent / 100
				, BORDER_X_RADIUS, BORDER_Y_RADIUS, SLIDER_HEIGHT);
		}
	}

	// 绘制进度文本
	{
		font.setPixelSize(14);
		font.setBold(false);
		painter.setFont(font);
		painterPen.setColor(Qt::white);
		painter.setPen(painterPen);
		QString drawText = QString::number(m_percent, 'f', 0) + QString("%");
		painter.drawText(0, this->height() - OFFSET_VALUE, this->width(), OFFSET_VALUE, Qt::AlignCenter, drawText);
		painter.end();
	}

	// 绘制圆角窗体
	{
		QPainterPath painterPathPath;
		QRectF rect = QRectF(0, 0, this->width(), this->height());
		painterPathPath.addRoundRect(rect, BORDER_X_RADIUS, BORDER_X_RADIUS);
		QPolygon polygon = painterPathPath.toFillPolygon().toPolygon();
		QRegion region(polygon);
		this->setMask(region);
	}
}

// 设置进度
void SplashScreen::setStagePercent(const int& percent, const QString& message)
{
	if (this->isHidden())
	{
		this->show();
	}
	if (!this->isActiveWindow())
	{
		this->activateWindow();
		this->raise();
	}
	m_message = message;

	while (m_percent < percent)
	{
		m_percent = m_percent + 0.2;
		qApp->processEvents(QEventLoop::AllEvents, 1);
		this->repaint();
	}
}

// 开始
void SplashScreen::setStart(QWidget* widget, const QString& title, const QString& logoFile)
{
	if (nullptr != widget)
	{
		m_mainWidget = widget;
		m_pixLogo = QPixmap(logoFile);
		m_textLogo = title;
		m_mainWidget->setWindowOpacity(0.0);
		if (nullptr == m_propertyAnimation)
		{
			m_propertyAnimation = new QPropertyAnimation(m_mainWidget, "windowOpacity");
			m_propertyAnimation->setDuration(ANIMATION_DURATION);
			m_propertyAnimation->setStartValue(0.0);
			m_propertyAnimation->setEndValue(1.0);
		}
	}
}

// 结束
void SplashScreen::setFinish()
{
	this->close();
	if (nullptr != m_mainWidget)
	{
		m_mainWidget->activateWindow();
		m_mainWidget->raise();
	}
	if (nullptr != m_propertyAnimation)
	{
		m_propertyAnimation->start();
	}
}