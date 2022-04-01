#include "preferencedialog.h"
#include "ui_preferencedialog.h"
#include "QtAwesome/QtAwesome.h"
#include "languagemanager.h"
#include "preferences.h"
#include "commondef.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QFileInfo>
#include <QVariantHash>
#include <QDir>
#include <QDebug>

#define SET_TOGGLE(x,y) ui->x->setChecked(Preferences::instance()->y())
#define CONNECT_TOGGLE(x,y) connect(ui->x,&QtMaterialToggle::toggled,preferences,&Preferences::y)
PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);

    ui->tabs->addTab(tr("General"),QTAWESOME->icon(fa::gear));
    ui->tabs->addTab(tr("Canvas"),QTAWESOME->icon(fa::canvas));
    ui->tabs->addTab(tr("Theme"),QTAWESOME->icon(fa::theme));

    ui->toggleAutoSave->setText(tr("AutoSave"));
    ui->toggleAutoSave->setOrientation(Qt::Horizontal);

    ui->comboDefaultUnit->insertItem(0,"MM");
    ui->comboDefaultUnit->insertItem(1,"INCH");

    ui->comboStyle->insertItem(1,"Lighten");
    ui->comboStyle->insertItem(0,"Darken");

    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->tabs,&QtMaterialTabs::currentChanged,this,&PreferenceDialog::tabChanged);

    loadLanaguages();


    SET_TOGGLE(toggleAutoSave,autoSave);
    SET_TOGGLE(toggleOpenLast,openLastFileOnStartup);
    SET_TOGGLE(toggleHardware,hardwareAcceleration);
    SET_TOGGLE(toggleShowGrid,gridVisible);
    SET_TOGGLE(toggleShowRuler,rulerVisible);
    SET_TOGGLE(toggleSnapGrid,snapToGrid);

    int iLanguageIndex = ui->comboLanguages->findData(Preferences::instance()->language());
    if (iLanguageIndex>-1){
        ui->comboLanguages->setCurrentIndex(iLanguageIndex);
    }
    qDebug()<<"language:"<<Preferences::instance()->language();

    int iDefaultUnit = Preferences::instance()->defaultUnit();
    ui->comboDefaultUnit->setCurrentIndex((int)iDefaultUnit);

    Preferences * preferences = Preferences::instance();

    CONNECT_TOGGLE(toggleAutoSave, setAutoSave);
    CONNECT_TOGGLE(toggleOpenLast, setOpenLastFileOnStartup);
    CONNECT_TOGGLE(toggleHardware, setHardwareAcceleration);
    CONNECT_TOGGLE(toggleShowGrid, setGridVisible);
    CONNECT_TOGGLE(toggleShowRuler,setRulerVisible);
    CONNECT_TOGGLE(toggleSnapGrid, setSnapToGrid);
    connect(ui->comboDefaultUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        preferences->setDefaultUnit(index);
    });
    connect(ui->comboLanguages,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        qDebug()<<ui->comboLanguages->itemData(index).toString();
          preferences->setLanguage(ui->comboLanguages->itemData(index).toString());

    });


}


PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::loadLanaguages()
{
    const QString & sTranslationsDir = LANGUAGEMANAGER->translationsDir();
    QDir dir(sTranslationsDir);
    QFileInfo fileInfo(dir,"languages.json");
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QFile::ReadOnly)){
        qWarning()<<"Error when open file"<<fileInfo.absoluteFilePath()<<file.errorString();
    }
    QByteArray bytes = file.readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &e);
    if (e.error != QJsonParseError::NoError)
    {
                    qDebug() << e.errorString();
    }
    if (jsonDoc.isEmpty() || jsonDoc.isNull()){
        qDebug()<<"json file is empty or null";
    }
    QVariantHash hashMap = jsonDoc.object().toVariantHash();


    ui->comboLanguages->clear();
    const QStringList & listLanguages = LANGUAGEMANAGER->availableLanguages();
    QStringList::const_iterator it = listLanguages.begin();
    while(it!=listLanguages.end())
    {
        QString sLanguageName;
        const QString & sLanguage = *it;
        QVariant vLanguage = hashMap.value(sLanguage);
        if (!vLanguage.isNull()){
            sLanguageName = vLanguage.toString();
        }
        else {
            QLocale locale(sLanguage);
            sLanguageName= QString(QLatin1String("%1 (%2)"))
                .arg(QLocale::languageToString(locale.language()))
                .arg(QLocale::countryToString(locale.country()));

        }
        ui->comboLanguages->addItem(sLanguageName,sLanguage);
        it++;
    }

}


void PreferenceDialog::tabChanged(int index)
{
    ui->stackedWidget->blockSignals(true);
    ui->stackedWidget->setCurrentIndex(index);
    ui->stackedWidget->blockSignals(false);


}


