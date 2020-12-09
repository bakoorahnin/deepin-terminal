/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     sunchengxi <sunchengxi@uniontech.com>
*
* Maintainer: sunchengxi <sunchengxi@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "customthemesettingdialog.h"
#include "utils.h"

#include <DDialog>
#include <DAbstractDialog>
#include <DLineEdit>
#include <DKeySequenceEdit>
#include <DLabel>
#include <DWindowCloseButton>
#include <DPushButton>
#include <DSuggestButton>
#include <DFontSizeManager>
#include <DApplicationHelper>
#include <DVerticalLine>

#include <QVBoxLayout>
#include <QAction>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QTimer>



TitleStyleRadioButton::TitleStyleRadioButton(const QString &text, QWidget *parent): DRadioButton(text, parent)
{

}

/*******************************************************************************
 1. @函数:    setBackGroundColor
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    设置背景色
*******************************************************************************/
void TitleStyleRadioButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mouseClick = true;
    }
    DRadioButton::mousePressEvent(event);
}

/*******************************************************************************
 1. @函数:    ColorPushButton
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    构造函数初始化
*******************************************************************************/
ColorPushButton::ColorPushButton(QWidget *parent): DPushButton(parent)
{
    setFocusPolicy(Qt::TabFocus);
}

/*******************************************************************************
 1. @函数:    setBackGroundColor
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    设置背景色
*******************************************************************************/
void ColorPushButton::setBackGroundColor(const QColor &color)
{
    m_color = color;
    update();
}

/*******************************************************************************
 1. @函数:    getBackGroundColor
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    getBackGroundColor
*******************************************************************************/
QColor ColorPushButton::getBackGroundColor()
{
    return  m_color;
}

/*******************************************************************************
 1. @函数:    paintEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    处理重绘事件
*******************************************************************************/
void ColorPushButton::paintEvent(QPaintEvent *event)
{

    QPainter *painter = new QPainter(this);
    //去锯齿
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setOpacity(1);

    //绘制背景色
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(QRectF(3, 3, 28, 28), 8, 8);
    painter->fillPath(backgroundPath, QColor(m_color));

    //tab焦点存在，绘制边框
    if (m_isFocus) {
        //边框绘制路径
        QPainterPath pathFrame;
        pathFrame.addRoundedRect(QRectF(1, 1, 32, 32), 8, 8);

        //绘画边框
        QPen framePen;
        DPalette pax = DApplicationHelper::instance()->palette(this);
        //获取活动色
        framePen = QPen(pax.color(DPalette::Highlight), 2);
        painter->setPen(framePen);
        painter->drawPath(pathFrame);
    }
    painter->end();
    event->accept();
}

/*******************************************************************************
 1. @函数:    focusInEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    焦点进入事件
*******************************************************************************/
void ColorPushButton::focusInEvent(QFocusEvent *event)
{
    // 焦点入
    if (Qt::TabFocusReason == event->reason() || Qt::BacktabFocusReason == event->reason()) {
        m_isFocus = true;
    } else if (Qt::ActiveWindowFocusReason == event->reason() && m_isFocus) {
        //取色面板退出时，是否仍然保留选中焦点，如果是键盘控制的情况，仍然保持保持焦点的状态
        m_isFocus = true;
    } else {
        //除了键盘操作的其他情况，都不保持焦点状态
        m_isFocus = false;
    }
    DPushButton::focusInEvent(event);
}

/*******************************************************************************
 1. @函数:    focusOutEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    焦点离开事件
*******************************************************************************/
void ColorPushButton::focusOutEvent(QFocusEvent *event)
{
    // 焦点Tab出
    if ((Qt::TabFocusReason == event->reason()) || (Qt::BacktabFocusReason == event->reason())) {
        qDebug() << "ColorPushButton::focusOutEvent-------163" ;
        m_isFocus = false;
    }
    DPushButton::focusOutEvent(event);
}

/*******************************************************************************
 1. @函数:    keyPressEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    按键按下事件
*******************************************************************************/
void ColorPushButton::keyPressEvent(QKeyEvent *event)
{
    if ((Qt::Key_Return == event->key()) || (Qt::Key_Enter == event->key())) {
        m_isFocus = true;
    }

    DPushButton::keyPressEvent(event);
}

/*******************************************************************************
 1. @函数:    mousePressEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    鼠标按下事件
*******************************************************************************/
void ColorPushButton::mousePressEvent(QMouseEvent *event)
{
    emit clearFocussSignal();
    DPushButton::mousePressEvent(event);
}



CustomThemeSettingDialog::CustomThemeSettingDialog(QWidget *parent) : DAbstractDialog(parent)
    , m_themePreviewArea(new ThemePreviewArea)
    , m_titleStyleButtonGroup(new QButtonGroup)
    , m_foregroundButton(new ColorPushButton(this))
    , m_backgroundButton(new ColorPushButton(this))
    , m_ps1Button(new ColorPushButton(this))
    , m_ps2Button(new ColorPushButton(this))
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    initUITitle();
    initUI();
    initTitleConnections();
    setFixedSize(459, 378);
}

/*******************************************************************************
 1. @函数:    initUITitle
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    标题栏初始化
*******************************************************************************/
void CustomThemeSettingDialog::initUITitle()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 11);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    m_titleBar = new QWidget(this);
    m_titleBar->setFixedHeight(50);
    m_titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_titleBar->setLayout(titleLayout);

    m_logoIcon = new DLabel(this);
    m_logoIcon->setFixedSize(QSize(50, 50));
    m_logoIcon->setFocusPolicy(Qt::NoFocus);
    m_logoIcon->setAttribute(Qt::WA_TransparentForMouseEvents);

    m_closeButton = new DWindowCloseButton(this);
    m_closeButton->setFocusPolicy(Qt::TabFocus);
    m_closeButton->setIconSize(QSize(50, 50));

    m_titleText = new DLabel(this);
    m_titleText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_titleText->setAlignment(Qt::AlignCenter);
    DFontSizeManager::instance()->bind(m_titleText, DFontSizeManager::T6, QFont::Medium);
    // 字色
    DPalette palette = m_titleText->palette();
    QColor color;
    if (DApplicationHelper::DarkType == DApplicationHelper::instance()->themeType()) {
        color = QColor::fromRgb(192, 198, 212, 255);
    } else {
        color = QColor::fromRgb(0, 26, 46, 255);
    }
    palette.setBrush(QPalette::WindowText, color);
    m_titleText->setPalette(palette);

    titleLayout->addWidget(m_logoIcon, 0, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addWidget(m_titleText, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    titleLayout->addWidget(m_closeButton, 0, Qt::AlignRight | Qt::AlignTop);

    //Dialog content
    m_contentLayout = new QVBoxLayout();
    m_contentLayout->setSpacing(0);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);

    m_content = new QWidget(this);
    m_content->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_content->setLayout(m_contentLayout);

    mainLayout->addWidget(m_titleBar, 0, Qt::AlignTop);
    mainLayout->addWidget(m_content);
    setLayout(mainLayout);

    m_titleText->setText(tr("Custom Theme"));

    m_mainLayout = mainLayout;
}

/*******************************************************************************
 1. @函数:    initUI
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    工作区初始化
*******************************************************************************/
void CustomThemeSettingDialog::initUI()
{
    QWidget *contentFrame = new QWidget;

    QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(10, 0, 10, 0);
    //
    QHBoxLayout *themePreviewAreatLayout = new QHBoxLayout;
    themePreviewAreatLayout->setSpacing(0);
    themePreviewAreatLayout->setContentsMargins(0, 0, 0, 0);
    m_themePreviewArea->setLayout(themePreviewAreatLayout);

    //
    QWidget *titleStyleFrame = new QWidget;
    QHBoxLayout *titleStyleLayout = new QHBoxLayout;
    titleStyleLayout->setSpacing(0);
    titleStyleLayout->setContentsMargins(0, 0, 0, 0);
    titleStyleFrame->setLayout(titleStyleLayout);

    DLabel *titleStyleLabel = new DLabel(tr("Style:"));
    DFontSizeManager::instance()->bind(titleStyleLabel, DFontSizeManager::T6, QFont::Normal);
    titleStyleLabel->setFixedSize(64, 22);

    m_lightRadioButton = new TitleStyleRadioButton(tr("Light"));
    DFontSizeManager::instance()->bind(m_lightRadioButton, DFontSizeManager::T6, QFont::Normal);
    //单选框只设置长度限制，不做高度限制，否则最新dtk选中框容易出现截断
    m_lightRadioButton->setFixedWidth(64);


    m_darkRadioButton = new TitleStyleRadioButton(tr("Dark"));
    DFontSizeManager::instance()->bind(m_darkRadioButton, DFontSizeManager::T6, QFont::Normal);
    //单选框只设置长度限制，不做高度限制，否则最新dtk选中框容易出现截断
    m_darkRadioButton->setFixedWidth(64);

    m_darkRadioButton->setFocusPolicy(Qt::TabFocus);
    m_lightRadioButton->setFocusPolicy(Qt::TabFocus);
    m_foregroundButton->setFocusPolicy(Qt::TabFocus);
    m_backgroundButton->setFocusPolicy(Qt::TabFocus);

    //将浅色标题风格单选按钮放入单选按钮分组中
    m_titleStyleButtonGroup->addButton(m_lightRadioButton);
    //将深色标题风格单选按钮放入单选按钮分组中
    m_titleStyleButtonGroup->addButton(m_darkRadioButton);


    connect(m_darkRadioButton, &DRadioButton::toggled, this, [ = ]() {
        if (m_darkRadioButton->isChecked()) {
            m_themePreviewArea->setTitleStyle("Dark");
        }
    });
    connect(m_lightRadioButton, &DRadioButton::toggled, this, [ = ]() {
        if (m_lightRadioButton->isChecked()) {
            m_themePreviewArea->setTitleStyle("Light");
        }
    });
    //鼠标点击单选按钮时清除tab键盘控制的焦点
    connect(m_darkRadioButton, &DRadioButton::clicked, this, [ = ]() {
        TitleStyleRadioButton *radioButton = qobject_cast<TitleStyleRadioButton *>(sender());
        if (radioButton && radioButton->m_mouseClick) {
            clearFocussSlot();
            radioButton->m_mouseClick = false;
        }
    });
    //鼠标点击单选按钮时清除tab键盘控制的焦点
    connect(m_lightRadioButton, &DRadioButton::clicked, this, [ = ]() {
        TitleStyleRadioButton *radioButton = qobject_cast<TitleStyleRadioButton *>(sender());
        if (radioButton && radioButton->m_mouseClick) {
            clearFocussSlot();
            radioButton->m_mouseClick = false;
        }
    });

    titleStyleLayout->addWidget(titleStyleLabel);
    titleStyleLayout->addSpacing(35);
    titleStyleLayout->addWidget(m_lightRadioButton);
    titleStyleLayout->addSpacing(22);
    titleStyleLayout->addWidget(m_darkRadioButton);
    titleStyleLayout->addStretch();

    //
    QWidget *foregroundAndBackgroundFrame = new QWidget;
    QHBoxLayout *foregroundAndBackgroundLayout = new QHBoxLayout;
    foregroundAndBackgroundLayout->setContentsMargins(0, 0, 0, 0);
    foregroundAndBackgroundFrame->setLayout(foregroundAndBackgroundLayout);

    m_foregroundColorLabel = new DLabel(tr("Fore color:"));
    DFontSizeManager::instance()->bind(m_foregroundColorLabel, DFontSizeManager::T6, QFont::Normal);
    m_foregroundColorLabel->setFixedSize(92, 20);

    m_foregroundButton->setFixedSize(34, 34);


    m_backgroundColorLabel = new DLabel(tr("Back color:"));
    DFontSizeManager::instance()->bind(m_backgroundColorLabel, DFontSizeManager::T6, QFont::Normal);
    m_backgroundColorLabel->setFixedSize(92, 20);
    m_backgroundButton->setFixedSize(34, 34);

    foregroundAndBackgroundLayout->addWidget(m_foregroundColorLabel);
    foregroundAndBackgroundLayout->addWidget(m_foregroundButton);
    foregroundAndBackgroundLayout->addSpacing(103);
    foregroundAndBackgroundLayout->addWidget(m_backgroundColorLabel);
    foregroundAndBackgroundLayout->addWidget(m_backgroundButton);
    foregroundAndBackgroundLayout->addStretch();

    //
    QWidget *ps1AndPs2Frame = new QWidget;
    QHBoxLayout *ps1AndPs2Layout = new QHBoxLayout;
    ps1AndPs2Layout->setContentsMargins(0, 0, 0, 0);
    ps1AndPs2Frame->setLayout(ps1AndPs2Layout);

    m_ps1ColorLabel = new DLabel(tr("Prompt PS1:"));
    DFontSizeManager::instance()->bind(m_ps1ColorLabel, DFontSizeManager::T6, QFont::Normal);
    m_ps1ColorLabel->setFixedSize(92, 20);
    m_ps1Button->setFixedSize(34, 34);
    m_ps2ColorLabel = new DLabel(tr("Prompt PS2:"));
    DFontSizeManager::instance()->bind(m_ps2ColorLabel, DFontSizeManager::T6, QFont::Normal);
    m_ps2ColorLabel->setFixedSize(92, 20);
    m_ps2Button->setFixedSize(34, 34);

    ps1AndPs2Layout->addWidget(m_ps1ColorLabel);
    ps1AndPs2Layout->addWidget(m_ps1Button);
    ps1AndPs2Layout->addSpacing(103);
    ps1AndPs2Layout->addWidget(m_ps2ColorLabel);
    ps1AndPs2Layout->addWidget(m_ps2Button);
    ps1AndPs2Layout->addStretch();

    contentLayout->addWidget(m_themePreviewArea);
    contentLayout->addSpacing(19);
    contentLayout->addWidget(titleStyleFrame);
    contentLayout->addSpacing(15);
    contentLayout->addWidget(foregroundAndBackgroundFrame);
    contentLayout->addSpacing(15);
    contentLayout->addWidget(ps1AndPs2Frame);
    contentLayout->addSpacing(25);

    contentFrame->setLayout(contentLayout);
    m_contentLayout->addWidget(contentFrame);
    addCancelConfirmButtons();

    connect(m_foregroundButton, &DPushButton::clicked, this, &CustomThemeSettingDialog::onSelectColor);
    connect(m_backgroundButton, &DPushButton::clicked, this, &CustomThemeSettingDialog::onSelectColor);
    connect(m_ps1Button, &DPushButton::clicked, this, &CustomThemeSettingDialog::onSelectColor);
    connect(m_ps2Button, &DPushButton::clicked, this, &CustomThemeSettingDialog::onSelectColor);

    connect(m_foregroundButton, &ColorPushButton::clearFocussSignal, this, &CustomThemeSettingDialog::clearFocussSlot);
    connect(m_backgroundButton, &ColorPushButton::clearFocussSignal, this, &CustomThemeSettingDialog::clearFocussSlot);
    connect(m_ps1Button, &ColorPushButton::clearFocussSignal, this, &CustomThemeSettingDialog::clearFocussSlot);
    connect(m_ps2Button, &ColorPushButton::clearFocussSignal, this, &CustomThemeSettingDialog::clearFocussSlot);

    loadConfiguration();
}

/*******************************************************************************
 1. @函数:    initTitleConnections
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    标题栏初始化信号槽
*******************************************************************************/
void CustomThemeSettingDialog::initTitleConnections()
{
    connect(m_closeButton, &DWindowCloseButton::clicked, this, [this]() {
        loadConfiguration();
        reject();
    });
    // 字体颜色随主题变化变化
    connect(DApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, m_titleText, [ = ](DGuiApplicationHelper::ColorType themeType) {
        DPalette palette = m_titleText->palette();
        QColor color;
        if (DApplicationHelper::DarkType == themeType) {
            color = QColor::fromRgb(192, 198, 212, 255);
        } else {
            color = QColor::fromRgb(0, 26, 46, 255);
        }
        palette.setBrush(QPalette::WindowText, color);
        m_titleText->setPalette(palette);
    });
}

/*******************************************************************************
 1. @函数:    addCancelConfirmButtons
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    增加确认取消按钮
*******************************************************************************/
void CustomThemeSettingDialog::addCancelConfirmButtons()
{
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(9);
    buttonsLayout->setContentsMargins(10, 0, 10, 0);

    QFont btnFont;
    m_cancelBtn = new DPushButton(this);
    m_cancelBtn->setFixedWidth(209);
    m_cancelBtn->setFixedHeight(36);
    m_cancelBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_cancelBtn->setFont(btnFont);
    m_cancelBtn->setText(tr("Cancel"));
    Utils::setSpaceInWord(m_cancelBtn);

    m_confirmBtn = new DSuggestButton(this);
    m_confirmBtn->setFixedWidth(209);
    m_confirmBtn->setFixedHeight(36);
    m_confirmBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_confirmBtn->setFont(btnFont);
    m_confirmBtn->setText(tr("Confirm"));
    Utils::setSpaceInWord(m_confirmBtn);

    m_cancelBtn->setFocusPolicy(Qt::TabFocus);
    m_confirmBtn->setFocusPolicy(Qt::TabFocus);

    //设置回车键默认响应的按钮
    m_confirmBtn->setDefault(true);

    setTabOrder(m_confirmBtn, m_closeButton);//设置右上角关闭按钮的tab键控制顺序

    DVerticalLine *verticalLine = new DVerticalLine(this);
    DPalette pa = DApplicationHelper::instance()->palette(verticalLine);
    QColor splitColor = pa.color(DPalette::ItemBackground);
    pa.setBrush(DPalette::Background, splitColor);
    verticalLine->setPalette(pa);
    verticalLine->setBackgroundRole(QPalette::Background);
    verticalLine->setAutoFillBackground(true);
    verticalLine->setFixedSize(3, 28);

    buttonsLayout->addWidget(m_cancelBtn);
    buttonsLayout->addWidget(verticalLine);
    buttonsLayout->addWidget(m_confirmBtn);
    m_confirmBtn->setDefault(true);

    m_mainLayout->addLayout(buttonsLayout);

    connect(m_cancelBtn, &DPushButton::clicked, this, [ = ]() {
        m_confirmBtn->setFocus();
        qDebug() << "------------reject()-------------";
        loadConfiguration();
        reject();
    });
    connect(m_confirmBtn, &DSuggestButton::clicked, this, [ = ]() {
        //重置单选按钮的tab焦点状态
        resetFocusState();

        m_confirmBtn->setFocus();

        if (m_darkRadioButton->isChecked()) {
            Settings::instance()->themeSetting->setValue("CustomTheme/TitleStyle", "Dark");

        } else {
            Settings::instance()->themeSetting->setValue("CustomTheme/TitleStyle", "Light");
        }

        Settings::instance()->themeSetting->setValue("Foreground/Color", Settings::instance()->color2str(m_foregroundButton->getBackGroundColor()));
        Settings::instance()->themeSetting->setValue("Background/Color", Settings::instance()->color2str(m_backgroundButton->getBackGroundColor()));
        Settings::instance()->themeSetting->setValue("Color2/Color", Settings::instance()->color2str(m_ps1Button->getBackGroundColor()));
        Settings::instance()->themeSetting->setValue("Color2Intense/Color", Settings::instance()->color2str(m_ps1Button->getBackGroundColor()));
        Settings::instance()->themeSetting->setValue("Color4/Color", Settings::instance()->color2str(m_ps2Button->getBackGroundColor()));
        Settings::instance()->themeSetting->setValue("Color4Intense/Color", Settings::instance()->color2str(m_ps2Button->getBackGroundColor()));

        Settings::instance()->m_customThemeModify = true;
        accept();
    });

}

/*******************************************************************************
 1. @函数:    onSelectColor
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    选择配色槽
*******************************************************************************/
void CustomThemeSettingDialog::onSelectColor()
{
    ColorPushButton *pushButton = qobject_cast<ColorPushButton *>(sender());
    if (pushButton) {
        QColor newColor = DColorDialog::getColor(pushButton->getBackGroundColor(), this);
        if (newColor.isValid()) {
            pushButton->setBackGroundColor(newColor);
            if (pushButton == m_foregroundButton) {
                m_themePreviewArea->setForegroundgroundColor(newColor);
            }
            if (pushButton == m_backgroundButton) {
                m_themePreviewArea->setBackgroundColor(newColor);
            }
            if (pushButton == m_ps1Button) {
                m_themePreviewArea->setPs1Color(newColor);
            }
            if (pushButton == m_ps2Button) {
                m_themePreviewArea->setPs2Color(newColor);
            }
        }
    }
}

/*******************************************************************************
 1. @函数:    clearFocussSlot
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    清理按钮焦点槽
*******************************************************************************/
void CustomThemeSettingDialog::clearFocussSlot()
{
    m_closeButton->clearFocus();
    m_darkRadioButton->clearFocus();
    m_lightRadioButton->clearFocus();
    m_foregroundButton->clearFocus();
    m_backgroundButton->clearFocus();
    m_ps1Button->clearFocus();
    m_ps2Button->clearFocus();
    m_cancelBtn->clearFocus();
    m_confirmBtn->clearFocus();

    m_foregroundButton->m_isFocus = false;
    m_backgroundButton->m_isFocus = false;
    m_ps1Button->m_isFocus = false;
    m_ps2Button->m_isFocus = false;

    m_logoIcon->setFocus();
}

/*******************************************************************************
 1. @函数:    loadConfiguration
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    加载主题配置
*******************************************************************************/
void CustomThemeSettingDialog::loadConfiguration()
{
    //重置单选按钮的tab焦点状态
    resetFocusState();

    if ("Light" == Settings::instance()->themeSetting->value("CustomTheme/TitleStyle")) {
        m_lightRadioButton->setChecked(true);
    } else {
        m_darkRadioButton->setChecked(true);
    }

    QColor foregroundColorParameter;
    QSettings themeSetting(Settings::instance()->m_configCustomThemePath, QSettings::IniFormat);
    QPalette palette;
    QStringList strList = Settings::instance()->themeSetting->value("Foreground/Color").toStringList();

    if (strList.size() != 3) {
        qDebug() << "strList.size()!=3";
        palette.setColor(QPalette::Background, QColor(0, 255, 0));
        foregroundColorParameter = QColor(0, 255, 0);
    } else {
        palette.setColor(QPalette::Background, QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt()));
        qDebug() << strList[0] << strList[1] << strList[2];
        foregroundColorParameter = QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
    }
    m_foregroundButton->setBackGroundColor(foregroundColorParameter);

    //
    QColor backgroundColorParameter;
    strList.clear();
    strList = Settings::instance()->themeSetting->value("Background/Color").toStringList();
    if (strList.size() != 3) {
        qDebug() << "strList.size()!=3";
        palette.setColor(QPalette::Background, QColor(37, 37, 37));
        backgroundColorParameter = QColor(37, 37, 37);
    } else {
        palette.setColor(QPalette::Background, QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt()));
        backgroundColorParameter = QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
    }
    m_backgroundButton->setBackGroundColor(backgroundColorParameter);

    //
    QColor ps1ColorParameter;
    strList.clear();
    strList = Settings::instance()->themeSetting->value("Color2Intense/Color").toStringList();
    if (strList.size() != 3) {
        qDebug() << "strList.size()!=3";
        palette.setColor(QPalette::Background, QColor(133, 153, 0));
        ps1ColorParameter = QColor(133, 153, 0);
    } else {
        palette.setColor(QPalette::Background, QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt()));
        ps1ColorParameter = QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
    }
    m_ps1Button->setBackGroundColor(ps1ColorParameter);

    //
    QColor ps2ColorParameter;
    strList.clear();
    strList = Settings::instance()->themeSetting->value("Color4Intense/Color").toStringList();
    if (strList.size() != 3) {
        qDebug() << "strList.size()!=3";
        palette.setColor(QPalette::Background, QColor(52, 101, 164));
        ps2ColorParameter = QColor(52, 101, 164);
    } else {
        palette.setColor(QPalette::Background, QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt()));
        ps2ColorParameter = QColor(strList[0].toInt(), strList[1].toInt(), strList[2].toInt());
    }
    m_ps2Button->setBackGroundColor(ps2ColorParameter);

    m_themePreviewArea->setAllColorParameter(foregroundColorParameter, backgroundColorParameter, ps1ColorParameter, ps2ColorParameter);
}

/*******************************************************************************
 1. @函数:    keyPressEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    键盘事件
*******************************************************************************/
void CustomThemeSettingDialog::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Escape == event->key()) {
        loadConfiguration();
        reject();
    }
}

/*******************************************************************************
 1. @函数:    showEvent
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    显示事件
*******************************************************************************/
void CustomThemeSettingDialog::showEvent(QShowEvent *event)
{
    clearFocussSlot();
    DAbstractDialog::showEvent(event);
}

/*******************************************************************************
 1. @函数:    resetFocusState
 2. @作者:    ut000125 sunchengxi
 3. @日期:    2020-12-01
 4. @说明:    重置单选按钮的tab焦点状态，保证每次打开时，第一个单选按钮是tab键盘控制的选中按钮
*******************************************************************************/
void CustomThemeSettingDialog::resetFocusState()
{
    m_darkRadioButton->setFocusPolicy(Qt::NoFocus);
    m_darkRadioButton->setFocusPolicy(Qt::TabFocus);
    m_lightRadioButton->setFocus();
}

