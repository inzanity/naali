// For conditions of distribution and use, see copyright notice in license.txt

#include "StableHeaders.h"
#include "CommunicationWidget.h"

#include <QWidget>
#include <QStackedLayout>
#include <QPlainTextEdit>
#include <QTimer>

namespace CoreUi
{
    CommunicationWidget::CommunicationWidget() :
        QGraphicsProxyWidget(),
        internal_widget_(new QWidget()),
        current_controller_(0),
        viewmode_(Normal)
    {
        Initialise();
        ChangeView(viewmode_);
    }

    // Public

    void CommunicationWidget::UpdateController(QObject *controller)
    {
        // Disconnect previous
        if (current_controller_)
        {
            this->disconnect(SIGNAL( SendMessageToServer(const QString&) ));
            this->disconnect(current_controller_);
        }

        // Connect present controller
        current_controller_ = controller;
        connect(current_controller_, SIGNAL( MessageReceived(bool, QString, QString, QString) ),
                this, SLOT( ShowIncomingMessage(bool, QString, QString, QString) ));
        connect(this, SIGNAL( SendMessageToServer(const QString&) ), 
                current_controller_, SLOT( SendChatMessageToServer(const QString&) ));

        // Clear old ui messages from history view
        if (history_view_text_edit_)
            history_view_text_edit_->clear();
    }

    // Private

    void CommunicationWidget::Initialise()
    {
        setupUi(internal_widget_);
        setWidget(internal_widget_);
        imButton->setEnabled(false);

        // Stacked layout
        stacked_layout_ = new QStackedLayout(chatContentWidget);
        stacked_layout_->setMargin(0);
        contentContainerLayout->addLayout(stacked_layout_);

        // History view mode
        history_view_text_edit_ = new QPlainTextEdit(chatContentWidget);
        history_view_text_edit_->setReadOnly(true);
        history_view_text_edit_->setObjectName("historyViewTextEdit");
        history_view_text_edit_->setStyleSheet("QPlainTextEdit#historyViewTextEdit { background-color: rgba(0,0,0,175); }");
        history_view_text_edit_->setFont(QFont("Arial", 10));
        stacked_layout_->addWidget(history_view_text_edit_);

        // Slim view mode
        normal_view_widget_ = new NormalChatViewWidget(chatContentWidget);
        stacked_layout_->addWidget(normal_view_widget_);

        stacked_layout_->setCurrentWidget(normal_view_widget_);

        connect(viewModeButton, SIGNAL( clicked() ), SLOT( ChangeViewPressed() ));
        connect(chatLineEdit, SIGNAL( returnPressed() ), SLOT( SendMessageRequested() ));
    }

    void CommunicationWidget::ChangeViewPressed()
    {
        switch (viewmode_)
        {
            case Normal:
                ChangeView(History);
                break;
            case History:
                ChangeView(Normal);
                break;
        }
    }

    void CommunicationWidget::ChangeView(ViewMode new_mode)
    {
        viewmode_ = new_mode; 
        switch (viewmode_)
        {
            case Normal:
                chatControlsWidget->setStyleSheet("QWidget#chatControlsWidget { border-top: 1px solid rgba(255,255,255,200); border-right: 1px solid rgba(255,255,255,200); border-top-right-radius: 10px; }");
                chatContentWidget->setStyleSheet("QWidget#chatContentWidget { background-color: transparent; border: 0px; border-radius: 0px; }");
                stacked_layout_->setCurrentWidget(normal_view_widget_);
                viewModeButton->setText("History");
                break;
            case History:
                chatControlsWidget->setStyleSheet("QWidget#chatControlsWidget { border-top: 0px; border-top-right-radius: 0px; }");
                chatContentWidget->setStyleSheet("QWidget#chatContentWidget { background-color: rgba(0,0,0,175);"
                                                 "border-top: 1px solid rgba(255,255,255,200); border-right: 1px solid rgba(255,255,255,200); border-top-right-radius: 10px; }");
                stacked_layout_->setCurrentWidget(history_view_text_edit_);
                viewModeButton->setText("Slim");
                break;
        }
    }

    void CommunicationWidget::ShowIncomingMessage(bool self_sent_message, QString sender, QString timestamp, QString message)
    {
        // History view
        QString htmlcontent("<span style='color:grey;'>[");
        htmlcontent.append(timestamp);
        if (!self_sent_message)
            htmlcontent.append("]</span> <span style='color:#0099FF;'>");
        else
            htmlcontent.append("]</span> <span style='color:#FF3330;'>");
        htmlcontent.append(sender);
        htmlcontent.append(": </span><span style='color:white;'>");
        htmlcontent.append(message);
        htmlcontent.append("</span>");
        history_view_text_edit_->appendHtml(htmlcontent);

        // Normal view
        if (!self_sent_message)
            normal_view_widget_->ShowChatMessage(QString("%1: %2").arg(sender, message));
        else
            normal_view_widget_->ShowChatMessage(QString("Me: %1").arg(message));
    }

    void CommunicationWidget::SendMessageRequested()
    {
        if (chatLineEdit->text().isEmpty())
            return;

        QString message = chatLineEdit->text();
        chatLineEdit->clear();
        emit SendMessageToServer(message);
    }

    // NormalChatViewWidget : QWidget

    NormalChatViewWidget::NormalChatViewWidget(QWidget *parent) :
        QWidget(parent)
    {
        setObjectName("normalChatViewWidget");
        setStyleSheet("QWidget#normalChatViewWidget { background-color: transparent; }");

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(4);
        layout->addSpacerItem(new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding));
        setLayout(layout);
    }

    void NormalChatViewWidget::ShowChatMessage(QString message)
    {
        ChatLabel *chat_label = new ChatLabel(message);
        layout()->addWidget(chat_label);
        connect(chat_label, SIGNAL( DestroyMe(ChatLabel*) ), SLOT( RemoveChatLabel(ChatLabel*) ));
    }

    void NormalChatViewWidget::RemoveChatLabel(ChatLabel *label)
    {
        int index = layout()->indexOf(label);
        if (index != -1)
        {
            layout()->removeItem(layout()->itemAt(index));
            SAFE_DELETE(label);
            if (layout()->count() < 4)
                updateGeometry();
        }
    }

    // ChatLabel

    ChatLabel::ChatLabel(QString message) :
        QLabel(message)
    {
        setFont(QFont("Arial", 12));
        setStyleSheet("background-color: rgba(0,0,0,175); color: white; border-radius: 5px; padding: 3px;");
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QTimer::singleShot(6000, this, SLOT(TimeOut()));
    }

    void ChatLabel::TimeOut()
    {
        emit DestroyMe(this);
    }
}