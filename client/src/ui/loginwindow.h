#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();
    void onSignUpClicked();

private:
    void setupUI();
    void applyStyles();

    // UI Components
    QVBoxLayout *mainLayout;
    QLabel *logoLabel;
    QLabel *titleLabel;
    QLineEdit *emailInput;
    QLineEdit *passwordInput;
    QCheckBox *rememberMeCheckbox;
    QPushButton *loginButton;
    QPushButton *signUpButton;
};

#endif // LOGINWINDOW_H
