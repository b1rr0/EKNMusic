#include "eknmintercomradiopage.h"
#include "services/radioservice.h"

EknmIntercomRadioPage::EknmIntercomRadioPage(QWidget *parent)
    : BaseRadioPage(RadioService::instance(), parent)
{
}

EknmIntercomRadioPage::~EknmIntercomRadioPage()
{
}

QString EknmIntercomRadioPage::getStationName() const
{
    return "EKNM INTERCOM";
}

QString EknmIntercomRadioPage::getStationId() const
{
    return "eknm_intercom";
}

QString EknmIntercomRadioPage::getRequestSongUrl() const
{
    return "https://radio.eknm.in/public/eknm_intercom";
}
