#ifndef SAMDIALOG_H
#define SAMDIALOG_H

#include <QDialog>

namespace Ui {
class SAMDialog;
}

class SAMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SAMDialog(QWidget *parent = nullptr);
    ~SAMDialog();

private:
    Ui::SAMDialog *ui;
};

#endif // SAMDIALOG_H
