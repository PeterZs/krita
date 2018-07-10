#include <QPointer>
#include <QScopedPointer>
#include <QGridLayout>

#include <kis_icon.h>

#include <ui_WdgPaletteListWidget.h>
#include "KisPaletteListWidget.h"
#include "KisPaletteListWidget_p.h"

/*
bool KisPaletteView::addGroupWithDialog()
{
    KoDialog *window = new KoDialog();
    window->setWindowTitle(i18nc("@title:window","Add a new group"));
    QFormLayout *editableItems = new QFormLayout();
    window->mainWidget()->setLayout(editableItems);
    QLineEdit *lnName = new QLineEdit();
    editableItems->addRow(i18nc("Name for a group", "Name"), lnName);
    lnName->setText(i18nc("Part of default name for a new group", "Color Group")+""+QString::number(m_d->model->colorSet()->getGroupNames().size()+1));
    if (window->exec() == KoDialog::Accepted) {
        QString groupName = lnName->text();
        m_d->model->addGroup(groupName);
        m_d->model->colorSet()->save();
        return true;
    }
    return false;
}
*/


KisPaletteListWidget::KisPaletteListWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui_WdgPaletteListWidget)
    , m_d(new Private(this))
{
    m_ui->setupUi(this);
    m_ui->bnAdd->setIcon(KisIconUtils::loadIcon("list-add"));
    m_ui->bnRemove->setIcon(KisIconUtils::loadIcon("list-remove"));
    m_ui->bnEdit->setIcon(KisIconUtils::loadIcon("edit-rename"));
    m_ui->bnImport->setIcon(KisIconUtils::loadIcon("document-import"));
    m_ui->bnExport->setIcon(KisIconUtils::loadIcon("document-export"));
    m_ui->viewPalette->setItemDelegate(m_d->delegate.data());
    m_ui->viewPalette->setModel(m_d->model.data());
    m_ui->viewPalette->verticalHeader()->setDefaultSectionSize(30); // set row height
}

KisPaletteListWidget::~KisPaletteListWidget()
{ }