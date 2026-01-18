#include "loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QHBoxLayout>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    applyStyles();

    resize(400, 500);
    setWindowTitle("EKNMusic - Login");
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Logo with image
    logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignCenter);
    QPixmap logo(":/resources/images/photo_2025-12-09_23-12-39.png");
    if (!logo.isNull()) {
        logoLabel->setPixmap(logo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel->setText("♫ EKNMUSIC");
        logoLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: #000000;");
    }
    logoLabel->setStyleSheet("margin-bottom: 20px;");

    // Manga subtitle
    titleLabel = new QLabel("Log in to EKNMusic", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 22px;"
        "color: #333333;"
        "margin-bottom: 40px;"
        "letter-spacing: 2px;"
        "font-weight: bold;"
    );

    // Email input
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("⚡ Email or username");
    emailInput->setMinimumHeight(50);

    // Password input
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("🔒 Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(50);

    // Remember me checkbox
    rememberMeCheckbox = new QCheckBox("Remember me", this);

    // Login button with gradient
    loginButton = new QPushButton("LOGIN", this);
    loginButton->setMinimumHeight(55);
    loginButton->setCursor(Qt::PointingHandCursor);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    // Sign up button
    signUpButton = new QPushButton("Don't have an account? Sign up", this);
    signUpButton->setMinimumHeight(45);
    signUpButton->setCursor(Qt::PointingHandCursor);
    connect(signUpButton, &QPushButton::clicked, this, &LoginWindow::onSignUpClicked);

    // Add widgets to layout
    mainLayout->addStretch();
    mainLayout->addWidget(logoLabel);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(emailInput);
    mainLayout->addWidget(passwordInput);
    mainLayout->addWidget(rememberMeCheckbox);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(signUpButton);
    mainLayout->addStretch();
}

void LoginWindow::applyStyles()
{
    // Simple minimalist style
    setStyleSheet(
        "QWidget {"
        "   background-color: #FFFFFF;"
        "}"
        "QLineEdit {"
        "   background-color: #FFFFFF;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 6px;"
        "   padding: 12px 16px;"
        "   font-size: 14px;"
        "   color: #000000;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #000000;"
        "   background-color: #fafafa;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #999999;"
        "}"
        "QCheckBox {"
        "   color: #666666;"
        "   font-size: 13px;"
        "   spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "   width: 16px;"
        "   height: 16px;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 3px;"
        "   background-color: #FFFFFF;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color: #000000;"
        "   border: 1px solid #000000;"
        "}"
        "QPushButton {"
        "   background-color: #000000;"
        "   color: #FFFFFF;"
        "   border: none;"
        "   border-radius: 6px;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "   padding: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #333333;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #000000;"
        "}"
        "QPushButton#signUpButton {"
        "   background-color: transparent;"
        "   color: #666666;"
        "   border: 1px solid #d0d0d0;"
        "}"
        "QPushButton#signUpButton:hover {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "   border: 1px solid #000000;"
        "}"
    );

    signUpButton->setObjectName("signUpButton");
}

void LoginWindow::onLoginClicked()
{
    QString email = emailInput->text();
    QString password = passwordInput->text();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both email and password.");
        return;
    }

    // For iteration 1, we'll accept any non-empty credentials
    // TODO: Implement real authentication in future iterations

    // Create and show main window
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    // Close login window
    this->close();
}

void LoginWindow::onSignUpClicked()
{
    QMessageBox::information(this, "Sign Up", "Sign up functionality will be available in a future update.");
}
