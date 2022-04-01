#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "svgdocument.h"
#include "workspace.h"
#include "zoomadjust.h"
#include "document.h"
#include "dockpanel.h"
#include "connectionform.h"
#include "preferencedialog.h"
#include "leftbar.h"
#include "elementfactory.h"
#include "rectelement.h"
#include "lineelement.h"
#include "bezierelement.h"
#include "textelement.h"
#include "QtAwesome/QtAwesome.h"


#include <QFileDialog>
#include <private/qsvgnode_p.h>
#include <QXmlStreamReader>
#include <QFile>
#include <QStack>
#include <private/qsvggraphics_p.h>
#include <private/qsvgstructure_p.h>
#include <qtmaterialdrawer.h>
#include <QTextDocument>



static BaseElement * createRectElement(BaseElement *parent, BaseSpace *space, QRectF initRect)
{
   return new RectElement(parent,space,initRect);
}

static BaseElement * createLineElement(BaseElement *parent, BaseSpace *space, QRectF initRect)
{
   return new LineElement(parent,space,initRect);
}

static BaseElement * createBezierElement(BaseElement *parent, BaseSpace *space, QRectF initRect)
{
   return new BezierElement(parent,space,initRect);
}

static BaseElement * createTextElement(BaseElement *parent, BaseSpace *space, QRectF initRect)
{
   return new TextElement(parent,space,initRect);
}

template<typename T,typename V>
static void setOnSilent(T * widget, V value){
    widget->blockSignals(true);
    widget->setValue(value);
    widget->blockSignals(false);
}

template<typename T,typename V>
static void checkOnSilent(T * widget, V value){
    widget->blockSignals(true);
    widget->setChecked(value);
    widget->blockSignals(false);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    svgDocument(0),
    m_preferencesDialog(0),
    m_document(0),
    m_leftBar(new LeftBar())
{
    ui->setupUi(this);

    this->setWindowTitle("WorkXP");

    ui->leftPanel->hide();

    ui->bottomDock->hide();

    connect(ui->zoomPart,&ZoomAdjust::factorChanged,this,[this](int value){
        qreal scale = value/qreal(100);
        ui->workspace->zoom(scale);
    });

    connect(ui->zoomPart,&ZoomAdjust::zoomToFit,ui->workspace,&Workspace::zoomToFit);

    connect(ui->workspace,&Workspace::scaled,this,[=](){
        int factor = 100* ui->workspace->transform().m11();
        ui->zoomPart->setFactor(factor);
    });

    connect(ui->workspace->baseScene(),&BaseSpaceScene::elementUnpicked,this,[this](){
        const QList<QGraphicsItem *> & items = ui->workspace->baseScene()->items();
        QList<QGraphicsItem *>::const_iterator itItems = items.begin();
        for(;itItems!=items.end();++itItems){
            QGraphicsItem * item = (*itItems);
            if (item && (item->type()>BaseElement::Type)){
                BaseElement * e = dynamic_cast<BaseElement *>(item);
                disconnect(e,0,this,0);
            }
        }


        setOnSilent(ui->editH,0);
        setOnSilent(ui->editW,0);
        setOnSilent(ui->editX,0);
        setOnSilent(ui->editY,0);
        setOnSilent(ui->editAngle,0);
        setOnSilent(ui->editW,0);
        checkOnSilent(ui->actionLockScale,false);
//        checkOnSilent(ui->actionTextBold,false);
//        checkOnSilent(ui->actionTextItalic,false);
//        checkOnSilent(ui->actionTextUnderline,false);

        ui->positionPart->setEnabled(false);

    });

    connect(ui->workspace->baseScene(),&BaseSpaceScene::elementPicked,this,[this](BaseElement * element){

        ui->positionPart->setEnabled(true);
        ui->editH->setReadOnly(!element->resizable());
        ui->editW->setReadOnly(!element->resizable());
        ui->editX->setReadOnly(!element->movable());
        ui->editY->setReadOnly(!element->movable());
        ui->editAngle->setReadOnly(!element->rotable());
        ui->actionLockScale->setEnabled(element->resizable());

        setElementTransformProperties(element);
        setElementTextProperties(element);

        connect(element,&BaseElement::rotateAngleChanged,this,[this](qreal arg1){
            qreal v = arg1;
            int x = qRound(v);
            x = (x>360)?(x % 360):x;
            if (x!=this->ui->editAngle->value()){
                this->ui->editAngle->setValue(x);
            }
        });

        connect(element,&BaseElement::rectChanged,this,[this,element](const QRectF & ){
            setElementTransformProperties(element);

        });

        //connect(element,&BaseElement::posChanged,this,[this,element](const QPointF & ){
        //    setElementTransformProperties(element);
        //});
    });


    connect(ui->editAngle,QOverload<int>::of(&QSpinBox::valueChanged),this,[this](int value){
        BaseElement * element = this->ui->workspace->activeElement();
        if (!element) return;
        int x = qRound(element->rotateAngle());
        x = (x>360)?(x % 360):x;

        if (value!=x){
            element->setRotateAngle(value-x);
        }
    });

    connect(ui->editH,QOverload<int>::of(&QSpinBox::valueChanged),this,[this](int value){
        BaseElement * element = this->ui->workspace->activeElement();
        if (!element) return;
        QRect rect = element->defaultPhysicalRect();
        if (qAbs(rect.height())!=value){
            if (element->lockScale()){
                rect.setWidth(value * element->scaleRatio());
            }
            rect.setHeight(value);
            element->setDefaultPhysicalRect(rect);
        }
    });
    connect(ui->editW,QOverload<int>::of(&QSpinBox::valueChanged),this,[this](int value){
        BaseElement * element = this->ui->workspace->activeElement();
        if (!element) return;
        QRect rect = element->defaultPhysicalRect();
        if (qAbs(rect.width())!=value){
            if (element->lockScale()){
                rect.setHeight(value / element->scaleRatio());
            }
            rect.setWidth(value);
            element->setDefaultPhysicalRect(rect);
        }
    });
    connect(ui->editX,QOverload<int>::of(&QSpinBox::valueChanged),this,[this](int value){
        BaseElement * element = this->ui->workspace->activeElement();
        if (!element) return;
        QPoint pos = element->defaultPhysicalPos();
        if (pos.x()!=value){
            pos.setX(value);
            element->setDefaultPhysicalPos(pos);
        }
    });
    connect(ui->editY,QOverload<int>::of(&QSpinBox::valueChanged),this,[this](int value){
        BaseElement * element = this->ui->workspace->activeElement();
        if (!element) return;
        QPoint pos = element->defaultPhysicalPos();
        if (pos.y()!=value){
            pos.setY(value);
            element->setDefaultPhysicalPos(pos);
        }
    });


    ui->leftBar->addWidget(m_leftBar);
    connect(this,&MainWindow::documentChanged,ui->workspace,&Workspace::setDocument);

    ui->workspace->zoomToFit();

    ui->labelRefresh->setPixmap(QTAWESOME->icon(fa::rotate).pixmap(QSize(16,16)));

    ui->actionSelect->setIcon(QTAWESOME->icon(fa::selection));
    ui->actionPicture->setIcon(QTAWESOME->icon(fa::bitmap));
    ui->actionRectangle->setIcon(QTAWESOME->icon(fa::rectangle));
    ui->actionCircle->setIcon(QTAWESOME->icon(fa::circle));
    ui->actionLine->setIcon(QTAWESOME->icon(fa::line));
    ui->actionCurve->setIcon(QTAWESOME->icon(fa::curve));
    ui->actionText->setIcon(QTAWESOME->icon(fa::text));
    ui->actionMachine->setIcon(QTAWESOME->icon(fa::remotecontroller));
    ui->actionLayers->setIcon(QTAWESOME->icon(fa::layers));
    ui->actionAlignEdgeLeft->setIcon(QTAWESOME->icon(fa::alignedgeleft));
    ui->actionAlignEdgeVCenter->setIcon(QTAWESOME->icon(fa::alignedgevcenter));
    ui->actionAlignEdgeRight->setIcon(QTAWESOME->icon(fa::alignedgeright));
    ui->actionAlignEdgeBottom->setIcon(QTAWESOME->icon(fa::alignedgebottom));
    ui->actionAlignEdgeTop->setIcon(QTAWESOME->icon(fa::alignedgetop));
    ui->actionAlignEdgeHCenter->setIcon(QTAWESOME->icon(fa::alignedgehcenter));
    ui->actionDistributeVerti->setIcon(QTAWESOME->icon(fa::distributevertically));
    ui->actionDistributeHori->setIcon(QTAWESOME->icon(fa::distributehorizontally));
    ui->actionTextBold->setIcon(QTAWESOME->icon(fa::bold));
    ui->actionTextItalic->setIcon(QTAWESOME->icon(fa::italic));
    ui->actionTextUnderline->setIcon(QTAWESOME->icon(fa::underline));
    ui->actionTextAlignLeft->setIcon(QTAWESOME->icon(fa::alignedgeleft));
    ui->actionTextAlignRight->setIcon(QTAWESOME->icon(fa::alignedgeright));
    ui->actionTextAlignJustify->setIcon(QTAWESOME->icon(fa::alignjustify));
    ui->actionTextAlignCenter->setIcon(QTAWESOME->icon(fa::aligncenter));

    QIcon lockIcon;
    lockIcon.addPixmap(QTAWESOME->icon(fa::lock).pixmap(QSize(24,24)),QIcon::Normal,QIcon::On);
    lockIcon.addPixmap(QTAWESOME->icon(fa::unlock).pixmap(QSize(24,24)),QIcon::Normal,QIcon::Off);
    ui->actionLockScale->setIcon(lockIcon);

    QActionGroup * actionGroupTools = new QActionGroup(this);
    actionGroupTools->addAction(ui->actionSelect);
    actionGroupTools->addAction(ui->actionPicture);
    actionGroupTools->addAction(ui->actionRectangle);
    actionGroupTools->addAction(ui->actionCircle);
    actionGroupTools->addAction(ui->actionLine);
    actionGroupTools->addAction(ui->actionCurve);
    actionGroupTools->addAction(ui->actionText);

    QActionGroup * actionGroupPanels = new QActionGroup(this);
    actionGroupPanels->setExclusive(false);
    actionGroupPanels->addAction(ui->actionMachine);
    actionGroupPanels->addAction(ui->actionLayers);

    QActionGroup * actionGroupTextAlign = new QActionGroup(this);
    actionGroupTextAlign->addAction(ui->actionTextAlignLeft);
    actionGroupTextAlign->addAction(ui->actionTextAlignRight);
    actionGroupTextAlign->addAction(ui->actionTextAlignCenter);
    actionGroupTextAlign->addAction(ui->actionTextAlignJustify);

    m_leftBar->setSelectAction(ui->actionSelect);
    m_leftBar->setPictureAction(ui->actionPicture);
    m_leftBar->setRectangleAction(ui->actionRectangle);
    m_leftBar->setCircleAction(ui->actionCircle);
    m_leftBar->setLineAction(ui->actionLine);
    m_leftBar->setCurveAction(ui->actionCurve);
    m_leftBar->setTextAction(ui->actionText);
    m_leftBar->setMachineAction(ui->actionMachine);
    m_leftBar->setLayersAction(ui->actionLayers);

    ui->btnAlignEdgeLeft->setDefaultAction(ui->actionAlignEdgeLeft);
    ui->btnAlignEdgeVCenter->setDefaultAction(ui->actionAlignEdgeVCenter);
    ui->btnAlignEdgeRight->setDefaultAction(ui->actionAlignEdgeRight);
    ui->btnAlignEdgeTop->setDefaultAction(ui->actionAlignEdgeTop);
    ui->btnAlignEdgeHCenter->setDefaultAction(ui->actionAlignEdgeHCenter);
    ui->btnAlignEdgeBottom->setDefaultAction(ui->actionAlignEdgeBottom);
    ui->btnDistributeHori->setDefaultAction(ui->actionDistributeHori);
    ui->btnDistributeVert->setDefaultAction(ui->actionDistributeVerti);

    ui->btnTextBold->setDefaultAction(ui->actionTextBold);
    ui->btnTextItalic->setDefaultAction(ui->actionTextItalic);
    ui->btnTextUnderline->setDefaultAction(ui->actionTextUnderline);

    ui->btnAlignEdgeBottom->setDefaultAction(ui->actionAlignEdgeBottom);
    ui->btnAlignEdgeHCenter->setDefaultAction(ui->actionAlignEdgeHCenter);
    ui->btnAlignEdgeLeft->setDefaultAction(ui->actionAlignEdgeLeft);
    ui->btnAlignEdgeRight->setDefaultAction(ui->actionAlignEdgeRight);
    ui->btnAlignEdgeTop->setDefaultAction(ui->actionAlignEdgeTop);
    ui->btnAlignEdgeVCenter->setDefaultAction(ui->actionAlignEdgeVCenter);

    ui->btnTextAlignCenter->setDefaultAction(ui->actionTextAlignCenter);
    ui->btnTextAlignJustify->setDefaultAction(ui->actionTextAlignJustify);
    ui->btnTextAlignLeft->setDefaultAction(ui->actionTextAlignLeft);
    ui->btnTextAlignRight->setDefaultAction(ui->actionTextAlignRight);

    ui->btnLockScale->setDefaultAction(ui->actionLockScale);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::debugSvgNodes(QList<QSvgNode*> nodes)
{

    QList<QSvgNode*>::Iterator it = nodes.begin();
    while(it!=nodes.end()){
        QSvgNode * node = (*it);
        qDebug()<<"node id:"<<node->nodeId()<<" type:"<<node->type();

        switch(node->type()){
        //    case QSvgNode::DOC:
        case QSvgNode::G:
        case QSvgNode::DEFS:
        case QSvgNode::SWITCH:
            debugSvgNodes(((QSvgStructureNode *)node)->renderers());
            //    case QSvgNode::ANIMATION:
        case QSvgNode::ARC:
            break;
        case QSvgNode::CIRCLE:
            break;
        case QSvgNode::ELLIPSE:
            break;
        case QSvgNode::IMAGE:
            break;
        case QSvgNode::LINE:
            break;
        case QSvgNode::PATH:
            break;
        case QSvgNode::POLYGON:
            break;
        case QSvgNode::POLYLINE:
            break;
        case QSvgNode::RECT:
            //qDebug()<<((QSvgRect *)(node))->rect();
            break;
        case QSvgNode::TEXT:
            break;
        case QSvgNode::TEXTAREA:
            break;
        case QSvgNode::TSPAN:
            break;
        case QSvgNode::USE:
            break;
            //    case QSvgNode::VIDEO:
        default:
            break;
        }
        ++it;

    }

}


void MainWindow::setElementTransformProperties(BaseElement * element)
{
    QRect rect = element->defaultPhysicalRect();
    qreal h = qAbs(rect.height());
    qreal w = qAbs(rect.width());
    if (h!=ui->editH->value())
    {
        ui->editH->setValue(h);
    }
    if (w!=ui->editW->value())
    {
        ui->editW->setValue(w);
    }

    QPoint pos = element->defaultPhysicalPos();
    if (pos.x()!=ui->editX->value())
    {
        ui->editX->setValue(pos.x());
    }
    if (pos.y()!=ui->editY->value())
    {
        ui->editY->setValue(pos.y());
    }
    ui->actionLockScale->setChecked(element->lockScale());



}


/** Text Element **/
TextElement * MainWindow::activeTextElement()
{
    BaseElement * baseElement = ui->workspace->activeElement();
    if (!baseElement) {
        return 0;
    }
    if (baseElement->type()!=QGraphicsItem::UserType+GRAPHICSITEM_TYPE_TEXT) {
        return 0;
    }
    return dynamic_cast<TextElement *>(baseElement);
}


void MainWindow::setElementTextProperties(BaseElement *element)
{
    TextElement * textElement = qgraphicsitem_cast<TextElement *>(element);
    if (!textElement) {
        return;
    }

    QTextCursor cursor = textElement->textItem()->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    ui->comboFontName->setFont(charFormat.font());

    connect(textElement,
            &TextElement::cursorPositionChanged,
            this,
            [this](const QTextCursor & cursor){
        QTextCharFormat format = cursor.charFormat();
        QFont f = format.font();
        ui->comboFontName->setCurrentFont(f);
        ui->editFontSize->setValue(f.pointSize());
        ui->actionTextBold->setChecked(f.bold());
        ui->actionTextItalic->setChecked(f.italic());
        ui->actionTextUnderline->setChecked(f.underline());

        Qt::Alignment alignment  = cursor.blockFormat().alignment();
        if (alignment.testFlag(Qt::AlignLeading) || alignment.testFlag(Qt::AlignLeft)){
                ui->actionTextAlignLeft->setChecked(true);
        }
        if (alignment.testFlag(Qt::AlignTrailing) || alignment.testFlag(Qt::AlignRight)){
                ui->actionTextAlignRight->setChecked(true);
        }
        if (alignment.testFlag(Qt::AlignCenter)) {
                ui->actionTextAlignCenter->setChecked(true);
        }
        if (alignment.testFlag(Qt::AlignJustify)) {
                ui->actionTextAlignJustify->setChecked(true);
        }

    });
}

void MainWindow::setTextAlignment(Qt::Alignment alignment)
{
    TextElement * textElement = activeTextElement();
    if (!textElement) return;
    textElement->setAlignment(alignment);

}

void MainWindow::on_comboFontName_currentFontChanged(const QFont & arg1)
{
    TextElement * textElement = activeTextElement();
    if (!textElement) return;

    QTextCursor cursor = textElement->textItem()->textCursor();

    QTextCharFormat charFormat;
    charFormat.setFont(arg1,QTextCharFormat::FontPropertiesSpecifiedOnly);
    cursor.mergeCharFormat(charFormat);
    textElement->textItem()->setTextCursor(cursor);
}

void MainWindow::on_editFontSize_valueChanged(int arg1)
{
    TextElement * textElement = activeTextElement();
    if (!textElement) return;

    QTextCursor cursor = textElement->textItem()->textCursor();
    QTextCharFormat charFormat;
    QFont f;
    f.setPointSize(arg1);
    charFormat.setFont(f,QTextCharFormat::FontPropertiesSpecifiedOnly);
    cursor.mergeCharFormat(charFormat);
    textElement->textItem()->setTextCursor(cursor);

}

void MainWindow::on_actionText_triggered(bool checked)
{
    if (checked){
        ui->workspace->setDrawing(true,createTextElement);
    }

}


void MainWindow::on_actionTextAlignLeft_triggered(bool checked)
{
    if (checked)
    {
        setTextAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    }
}

void MainWindow::on_actionTextAlignRight_triggered(bool checked)
{
    if (checked)
    {
        setTextAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    }
}

void MainWindow::on_actionTextAlignJustify_triggered(bool checked)
{
    if (checked)
    {
        setTextAlignment(Qt::AlignJustify);
    }
}

void MainWindow::on_actionTextAlignCenter_triggered(bool checked)
{
    if (checked)
    {
        setTextAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_actionTextBold_triggered(bool checked)
{
    TextElement * textElement = activeTextElement();
    if (!textElement) return;
    QTextCursor cursor = textElement->textItem()->textCursor();
    QTextCharFormat charFormat;
    QFont f;
    f.setBold(checked);
    charFormat.setFont(f,QTextCharFormat::FontPropertiesSpecifiedOnly);
    cursor.mergeCharFormat(charFormat);
    textElement->textItem()->setTextCursor(cursor);
}

void MainWindow::on_actionTextItalic_triggered(bool checked)
{
    TextElement * textElement = activeTextElement();
    if (!textElement) return;
    QTextCursor cursor = textElement->textItem()->textCursor();
    QTextCharFormat charFormat;
    QFont f;
    f.setItalic(checked);
    charFormat.setFont(f,QTextCharFormat::FontPropertiesSpecifiedOnly);
    cursor.mergeCharFormat(charFormat);
    textElement->textItem()->setTextCursor(cursor);
}

void MainWindow::on_actionTextUnderline_triggered(bool checked)
{
    TextElement * textElement = activeTextElement();
    if (!textElement) return;
    QTextCursor cursor = textElement->textItem()->textCursor();
    QTextCharFormat charFormat;
    QFont f;
    f.setUnderline(checked);
    charFormat.setFont(f,QTextCharFormat::FontPropertiesSpecifiedOnly);
    cursor.mergeCharFormat(charFormat);
    textElement->textItem()->setTextCursor(cursor);

}

/** End of Text Element **/


void MainWindow::newFile(bool)
{
//    if (m_document){
//        delete m_document;
//        m_document = 0;
//    }
//    m_document = new Document(this);
//    emit documentChanged(m_document);
}

void MainWindow::init()
{
   ui->workspace->init();
   m_document = new Document(ui->workspace);
   // Disable the focus rect to get minimal repaints when scrolling on Mac.
   //ui->treeLayers->setAttribute(Qt::WA_MacShowFocusRect, false);
   ui->treeLayers->setModel(m_document);
 //  ui->treeLayers->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->treeLayers->setColumnWidth(0,50);
   ui->treeLayers->setColumnWidth(2,24);
   ui->treeLayers->setColumnWidth(3,24);
   ui->treeLayers->setColumnWidth(4,24);

   connect(ui->treeLayers,&QTreeView::clicked,[this](const QModelIndex & index){
       BaseElement * element = m_document->getItem(index);
       if (!element) return;
       switch(index.column()){
           case 2:
               element->setVisible(!element->isVisible());
               m_document->setData(index,element->isVisible()?QTAWESOME->icon(fa::eye):QTAWESOME->icon(fa::eyeslash),Qt::DecorationRole);
               break;
           case 3:
               element->setPickable(!element->pickable());
               m_document->setData(index,element->pickable()?QTAWESOME->icon(fa::unlock):QTAWESOME->icon(fa::lock),Qt::DecorationRole);
               break;
           case 4:
               element->deleteLater();
          default:
               break;
       }

   });

   connect(ui->treeLayers->selectionModel(),&QItemSelectionModel::selectionChanged,[this](const QItemSelection &selected, const QItemSelection &){

       QList<BaseElement *> elements;
       //DomTreeView * tree = qobject_cast<DomTreeView *>(sender());
       foreach(auto index,selected.indexes()){
           elements.append(static_cast<BaseElement *>(index.internalPointer()));
       }
       this->ui->workspace->pickElements(elements);

   });

   //BaseSpaceScene * spaceScene = static_cast<BaseSpaceScene *>(ui->workspace->scene());

//   connect(spaceScene,&BaseSpaceScene::elementPicked,[this](BaseElement * element){
//       QList<BaseElement *> elements;
//       this->ui->treeLayers->selectElements(elements);

//   });

}



void MainWindow::on_actionPreferences_triggered()
{
    if (!m_preferencesDialog) {
        m_preferencesDialog = new PreferenceDialog(this);
        m_preferencesDialog->setAttribute(Qt::WA_DeleteOnClose);
    }

    m_preferencesDialog->show();
    m_preferencesDialog->activateWindow();
    m_preferencesDialog->raise();

}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open SVG"), "/home/", tr("SVG Files (*.svg)"));
//    QFile file(fileName);
//    if (!file.open(QFile::ReadOnly)){
//        qCWarning(lcMainWindow, "Cannot open file '%s%' : '%s'",
//                  fileName.toLocal8Bit().constData(),
//                  file.errorString().toLocal8Bit().constData());
//    }
//    if (svgDocument) delete svgDocument;


//    debugSvgNodes(SvgDocument::load(fileName)->renderers());

    //svgDocument = (SvgDocument *)SvgDocument::load(fileName);
    //QList<QSvgNode*>  nodes = svgDocument->renderers();
    //QList<QSvgNode*>::iterator it = nodes.begin();
    //while(it!=nodes.end()){
    //    QSvgNode * node = (*it);
    //    ++it;
    //    qCDebug(lcMainWindow, "nodeId:'%s', type:'%d'", node->nodeId().toLocal8Bit().constData(),node->type());

    //}


//    while(svgHandler->)
//    qDebug()<<"Open SVG" << fileName << "success";
\
    //qCDebug(lcMainWindow,"Open SVG '%s' success",fileName.toLocal8Bit().constData());

    //QStack<QSvgNode*> nodes = svgHandler->m_nodes;
    //qDebug()<<svgHandler->m_nodes.count();
    //qDebug()<<svgHandler->errorString();
    //while(!nodes.isEmpty()){
    //    QSvgNode * node = nodes.pop();
    //}
    //svgHandler->errorString();


}

void MainWindow::on_actionMachine_toggled(bool arg1)
{
    if (arg1){
        ui->leftPanel->setVisible(true);
        ui->actionLayers->setChecked(!arg1);
        ui->splitter->moveChild(230,1);
        ui->leftStack->setCurrentIndex(0);
    }
    else {
        //ui->splitter->moveChild(0,1);
        ui->leftPanel->setVisible(false);
    }

}

void MainWindow::on_actionLayers_toggled(bool arg1)
{
    if (arg1){
        ui->leftPanel->setVisible(true);
        ui->actionMachine->setChecked(!arg1);
        ui->splitter->moveChild(230,1);
        ui->leftStack->setCurrentIndex(1);
    }
    else {
        ui->leftPanel->setVisible(false);
        //ui->splitter->moveChild(0,1);
    }

}

void MainWindow::on_actionRectangle_toggled(bool arg1)
{
    if (arg1){
        ui->workspace->setDrawing(true,createRectElement);
    }
}

void MainWindow::on_actionSelect_triggered(bool arg1)
{
    if (arg1){
        ui->workspace->setDrawing(false,nullptr);

    }
}

void MainWindow::on_actionLine_triggered(bool checked)
{
    if (checked){
        ui->workspace->setDrawing(true,createLineElement);
    }
}

void MainWindow::on_actionCurve_triggered(bool checked)
{
    if (checked){
        ui->workspace->setDrawing(true,createBezierElement);
    }

}




void MainWindow::on_actionLockScale_triggered(bool checked)
{
    BaseElement * element = this->ui->workspace->activeElement();
    if (!element) return;
    element->setLockScale(checked);

}
