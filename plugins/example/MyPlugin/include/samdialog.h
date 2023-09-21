#ifndef SAMDIALOG_H
#define SAMDIALOG_H
#include "ui_samdialog.h"
#include <QDialog>




class SAMDialog : public QDialog, public Ui::SAMDialog
{
    Q_OBJECT

public:
    explicit SAMDialog(QWidget *parent = nullptr);
    // ~SAMDialog();

// private:
//     Ui::SAMDialog *ui;
};

#endif // SAMDIALOG_H
