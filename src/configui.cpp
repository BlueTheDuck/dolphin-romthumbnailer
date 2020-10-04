#include "configui.h"
#include "ui_configui.h"

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER_UI)
Q_LOGGING_CATEGORY(LOG_ROMTHUMBNAILER_UI, "romthumbnailer.ui")

ConfigUi::ConfigUi(QWidget *parent) : QWidget(parent), ui(new Ui::ConfigUi) {
    ui->setupUi(this);
}

ConfigUi::~ConfigUi() { delete ui; }

bool ConfigUi::getUseFrames() const { return ui->useFrames->isChecked(); }

void ConfigUi::setUseFrames(bool use) { ui->useFrames->setChecked(use); }

QUrl ConfigUi::getDSCoversPath() const { return ui->coversPath->url(); }

void ConfigUi::setDSCoversPath(QUrl path) {
    ui->coversPath->setUrl(path);
}
