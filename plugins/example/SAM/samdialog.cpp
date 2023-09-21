#include "samdialog.h"
#include "ui_samdialog.h"

SAMDialog::SAMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SAMDialog)
{
    ui->setupUi(this);
}

SAMDialog::~SAMDialog()
{
    delete ui;
}
