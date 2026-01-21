#ifndef EKNMINTERCOMRADIOPAGE_H
#define EKNMINTERCOMRADIOPAGE_H

#include "baseradiopage.h"

/**
 * @brief EKNM Intercom Radio Station Page
 *
 * Concrete implementation of BaseRadioPage for EKNM Intercom station
 */
class EknmIntercomRadioPage : public BaseRadioPage
{
    Q_OBJECT

public:
    explicit EknmIntercomRadioPage(QWidget *parent = nullptr);
    ~EknmIntercomRadioPage() override;

protected:
    QString getStationName() const override;
    QString getStationId() const override;
    QString getRequestSongUrl() const override;
};

#endif // EKNMINTERCOMRADIOPAGE_H
