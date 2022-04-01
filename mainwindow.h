#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class PreferenceDialog;
class LeftBar;

namespace Ui {
class MainWindow;
}

class SvgDocument;
class QSvgNode;
class Document;
class BaseElement;
class TextElement;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void newFile(bool);

    void init();
private:
    Ui::MainWindow *ui;
    SvgDocument * svgDocument;
    PreferenceDialog * m_preferencesDialog;
    Document * m_document;
    LeftBar * m_leftBar;
    QMenu * menuLayers;

    void debugSvgNodes(QList<QSvgNode*> nodes);



    TextElement * activeTextElement();
    void setElementTransformProperties(BaseElement * element);
    void setElementTextProperties(BaseElement * element);
    void setTextAlignment(Qt::Alignment alignment);


private slots:
    void on_actionPreferences_triggered();
    void on_actionOpen_triggered();

    void on_actionMachine_toggled(bool arg1);

    void on_actionLayers_toggled(bool arg1);

    void on_actionRectangle_toggled(bool arg1);
    
    void on_actionSelect_triggered(bool arg1);

    void on_actionLine_triggered(bool checked);

    void on_actionCurve_triggered(bool checked);

    void on_actionText_triggered(bool checked);

    void on_comboFontName_currentFontChanged(const QFont &arg1);

    void on_editFontSize_valueChanged(int arg1);

    void on_actionTextAlignLeft_triggered(bool checked);

    void on_actionTextAlignRight_triggered(bool checked);

    void on_actionTextAlignJustify_triggered(bool checked);

    void on_actionTextAlignCenter_triggered(bool checked);

    void on_actionTextBold_triggered(bool checked);

    void on_actionTextItalic_triggered(bool checked);

    void on_actionTextUnderline_triggered(bool checked);

    void on_actionLockScale_triggered(bool checked);

signals:
    void documentChanged(Document *);

};

#endif // MAINWINDOW_H
