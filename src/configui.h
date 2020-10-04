#ifndef CONFIGUI_H
#define CONFIGUI_H

#include <QWidget>

namespace Ui {
class ConfigUi;
}

class ConfigUi : public QWidget {
    Q_OBJECT

  public:
    explicit ConfigUi(QWidget *parent = nullptr);
    ~ConfigUi();

    bool getUseFrames() const;
    void setUseFrames(bool);

    QUrl getDSCoversPath() const;
    void setDSCoversPath(QUrl);

  private:
    Ui::ConfigUi *ui;
};

#endif // CONFIGUI_H
