/**
 * QtAwesome - use font-awesome (or other font icons) in your c++ / Qt Application
 *
 * MIT Licensed
 *
 * Copyright 2013-2016 - Reliable Bits Software by Blommers IT. All Rights Reserved.
 * Author Rick Blommers
 */

#include "QtAwesome.h"
#include "QtAwesomeAnim.h"

#include <QDebug>
#include <QFile>
#include <QFontDatabase>

QtAwesome * QtAwesome::_instance = 0;


/// The font-awesome icon painter
class QtAwesomeCharIconPainter: public QtAwesomeIconPainter
{

protected:

    QStringList optionKeysForModeAndState( const QString& key, QIcon::Mode mode, QIcon::State state)
    {
        QString modePostfix;
        switch(mode) {
            case QIcon::Disabled:
                modePostfix = "-disabled";
                break;
            case QIcon::Active:
                modePostfix = "-active";
                break;
            case QIcon::Selected:
                modePostfix = "-selected";
                break;
            default: // QIcon::Normal:
                // modePostfix = "";
                break;
        }

        QString statePostfix;
        if( state == QIcon::Off) {
            statePostfix = "-off";
        }

        // the keys that need to bet tested:   key-mode-state | key-mode | key-state | key
        QStringList result;
        if( !modePostfix.isEmpty() ) {
            if( !statePostfix.isEmpty() ) {
                result.push_back( key + modePostfix + statePostfix );
            }
            result.push_back( key + modePostfix );
        }
        if( !statePostfix.isEmpty() ) {
            result.push_back( key + statePostfix );
        }
        return result;
    }


    QVariant optionValueForModeAndState( const QString& baseKey, QIcon::Mode mode, QIcon::State state, const QVariantMap& options )
    {
        foreach( QString key, optionKeysForModeAndState(baseKey, mode, state) ) {
            //if ( options.contains(key) && options.value(key).toString().isEmpty()) qDebug() << "Not found:" << key;
            if( options.contains(key) && !(options.value(key).toString().isEmpty()) )
                return options.value(key);
        }

        return options.value(baseKey);
    }

public:


    virtual void paint( QtAwesome* awesome, QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options  )
    {
        painter->save();

        QVariant var =options.value("anim");
        QtAwesomeAnimation* anim = var.value<QtAwesomeAnimation*>();
        if( anim ) {
            anim->setup( *painter, rect );
        }

        // set the default options
        QColor color = optionValueForModeAndState("color", mode, state, options).value<QColor>();
        QString text = optionValueForModeAndState("text", mode, state, options).toString();

        Q_ASSERT(color.isValid());
        Q_ASSERT(!text.isEmpty());

        painter->setPen(color);

        // add some 'padding' around the icon
        int drawSize = qRound(rect.height()*options.value("scale-factor").toFloat());

        painter->setFont( awesome->font(drawSize) );
        painter->drawText( rect, text, QTextOption( Qt::AlignCenter|Qt::AlignVCenter ) );
        painter->restore();
    }

};


//---------------------------------------------------------------------------------------


/// The painter icon engine.
class QtAwesomeIconPainterIconEngine : public QIconEngine
{

public:

    QtAwesomeIconPainterIconEngine( QtAwesome* awesome, QtAwesomeIconPainter* painter, const QVariantMap& options  )
        : awesomeRef_(awesome)
        , iconPainterRef_(painter)
        , options_(options)
    {
    }

    virtual ~QtAwesomeIconPainterIconEngine() {}

    QtAwesomeIconPainterIconEngine* clone() const
    {
        return new QtAwesomeIconPainterIconEngine( awesomeRef_, iconPainterRef_, options_ );
    }

    virtual void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
    {
        Q_UNUSED( mode );
        Q_UNUSED( state );
        iconPainterRef_->paint( awesomeRef_, painter, rect, mode, state, options_ );
    }

    virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
    {
        QPixmap pm(size);
        pm.fill( Qt::transparent ); // we need transparency
        {
            QPainter p(&pm);
            paint(&p, QRect(QPoint(0,0),size), mode, state);
        }
        return pm;
    }

private:

    QtAwesome* awesomeRef_;                  ///< a reference to the QtAwesome instance
    QtAwesomeIconPainter* iconPainterRef_;   ///< a reference to the icon painter
    QVariantMap options_;                    ///< the options for this icon painter
};


//---------------------------------------------------------------------------------------



QtAwesome *QtAwesome::instance(QObject* parent) throw()
{
        if(_instance == 0) {
            _instance = new QtAwesome(parent);
        }
        return _instance;
}

QtAwesome::QtAwesome( QObject* parent )
    : QObject( parent )
    , namedCodepoints_()
{
    // initialize the default options
    setDefaultOption( "color", QColor(50,50,50) );
    setDefaultOption( "color-disabled", QColor(70,70,70,60));
    setDefaultOption( "color-active", QColor(10,10,10));
    setDefaultOption( "color-selected", QColor(10,10,10));
    setDefaultOption( "scale-factor", 0.9 );

    setDefaultOption( "text", QVariant() );
    setDefaultOption( "text-disabled", QVariant() );
    setDefaultOption( "text-active", QVariant() );
    setDefaultOption( "text-selected", QVariant() );

    fontIconPainter_ = new QtAwesomeCharIconPainter();

}


QtAwesome::~QtAwesome()
{
    delete fontIconPainter_;
//    delete errorIconPainter_;
    qDeleteAll(painterMap_);
}

/// initializes the QtAwesome icon factory with the given fontname
void QtAwesome::init(const QString& fontname)
{
    fontName_ = fontname;
}

struct FANameIcon {
  const char *name;
  fa::icon icon;
};

static const FANameIcon faNameIconArray[] = {
      { "aligncenter"                      , fa::aligncenter                      },
      { "alignjustify"                     , fa::alignjustify                     },
      { "alignleft"                        , fa::alignleft                        },
      { "alignright"                       , fa::alignright                       },
      { "angledown"                        , fa::angledown                        },
      { "angleleft"                        , fa::angleleft                        },
      { "angleright"                       , fa::angleright                       },
      { "angleup"                          , fa::angleup                          },
      { "bold"                             , fa::bold                             },
      { "chevrondown"                      , fa::chevrondown                      },
      { "chevronleft"                      , fa::chevronleft                      },
      { "chevronright"                     , fa::chevronright                     },
      { "chevronup"                        , fa::chevronup                        },
      { "circle"                           , fa::circle                           },
      { "close"                            , fa::close                            },
      { "eye"                              , fa::eye                              },
      { "eyeslash"                         , fa::eyeslash                         },
      { "gear"                             , fa::gear                             },
      { "home"                             , fa::home                             },
      { "italic"                           , fa::italic                           },
      { "minus"                            , fa::minus                            },
      { "plus"                             , fa::plus                             },
      { "refresh"                          , fa::refresh                          },
      { "underline"                        , fa::underline                        },
      { "selection"                        , fa::selection                        },
      { "selfselection"                    , fa::selfselection                    },
      { "rectangle"                        , fa::rectangle                        },
      { "curve"                            , fa::curve                            },
      { "line"                             , fa::line                             },
      { "point"                            , fa::point                            },
      { "fittocanvas"                      , fa::fittocanvas                      },
      { "theme"                            , fa::theme                            },
      { "canvas"                           , fa::canvas                           },
      { "rotate"                           , fa::rotate                           },
      { "flashoff"                         , fa::flashoff                         },
      { "flashon"                          , fa::flashon                          },
      { "remotecontroller"                 , fa::remotecontroller                 },
      { "distributevertically"             , fa::distributevertically             },
      { "distributehorizontally"           , fa::distributehorizontally           },
      { "layers"                           , fa::layers                           },
      { "bitmap"                           , fa::bitmap                           },
      { "text"                             , fa::text                             },
      { "alignedgeleft"                    , fa::alignedgeleft                    },
      { "alignedgeright"                   , fa::alignedgeright                   },
      { "alignedgevcenter"                 , fa::alignedgevcenter                 },
      { "alignedgetop"                     , fa::alignedgetop                     },
      { "alignedgebottom"                  , fa::alignedgebottom                  },
      { "alignedgehcenter"                 , fa::alignedgehcenter                 }

};




/// a specialized init function so font-awesome is loaded and initialized
/// this method return true on success, it will return false if the fnot cannot be initialized
/// To initialize QtAwesome with font-awesome you need to call this method
bool QtAwesome::initFontAwesome( )
{
    static int fontAwesomeFontId = -1;

    // only load font-awesome once
    if( fontAwesomeFontId < 0 ) {

        // The macro below internally calls "qInitResources_QtAwesome()". this initializes
        // the resource system. For a .pri project this isn't required, but when building and using a
        // static library the resource need to initialized first.
        ///
        // I've checked th qInitResource_* code and calling this method mutliple times shouldn't be any problem
        // (More info about this subject:  http://qt-project.org/wiki/QtResources)
        Q_INIT_RESOURCE(QtAwesome);

        // load the font file
        QFile res(":/fonts/iconfont.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            qDebug() << "Font awesome font could not be loaded!";
            return false;
        }
        QByteArray fontData( res.readAll() );
        res.close();

        // fetch the given font
        fontAwesomeFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontAwesomeFontId);
    if( !loadedFontFamilies.empty() ) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        qDebug() << "Font awesome font is empty?!";
        fontAwesomeFontId = -1; // restore the font-awesome id
        return false;
    }

    // intialize the map
    QHash<QString, int>& m = namedCodepoints_;
    for (unsigned i = 0; i < sizeof(faNameIconArray)/sizeof(FANameIcon); ++i) {
      m.insert(faNameIconArray[i].name, faNameIconArray[i].icon);
    }

    return true;
}

void QtAwesome::addNamedCodepoint( const QString& name, int codePoint)
{
    namedCodepoints_.insert( name, codePoint);
}


/// Sets a default option. These options are passed on to the icon painters
void QtAwesome::setDefaultOption(const QString& name, const QVariant& value)
{
    defaultOptions_.insert( name, value );
}


/// Returns the default option for the given name
QVariant QtAwesome::defaultOption(const QString& name)
{
    return defaultOptions_.value( name );
}


// internal helper method to merge to option amps
static QVariantMap mergeOptions( const QVariantMap& defaults, const QVariantMap& override )
{
    QVariantMap result= defaults;
    if( !override.isEmpty() ) {
        QMapIterator<QString,QVariant> itr(override);
        while( itr.hasNext() ) {
            itr.next();
            result.insert( itr.key(), itr.value() );
        }
    }
    return result;
}


/// Creates an icon with the given code-point
/// <code>
///     awesome->icon( icon_group )
/// </code>
QIcon QtAwesome::icon(int character, const QVariantMap &options)
{
    // create a merged QVariantMap to have default options and icon-specific options
    QVariantMap optionMap = mergeOptions( defaultOptions_, options );
    optionMap.insert("text", QString( QChar(static_cast<int>(character)) ) );

    return icon( fontIconPainter_, optionMap );
}



/// Creates an icon with the given name
///
/// You can use the icon names as defined on http://fortawesome.github.io/Font-Awesome/design.html  withour the 'icon-' prefix
/// @param name the name of the icon
/// @param options extra option to pass to the icon renderer
QIcon QtAwesome::icon(const QString& name, const QVariantMap& options)
{
    // when it's a named codepoint
    if( namedCodepoints_.count(name) ) {
        return icon( namedCodepoints_.value(name), options );
    }


    // create a merged QVariantMap to have default options and icon-specific options
    QVariantMap optionMap = mergeOptions( defaultOptions_, options );

    // this method first tries to retrieve the icon
    QtAwesomeIconPainter* painter = painterMap_.value(name);
    if( !painter ) {
        return QIcon();
    }

    return icon( painter, optionMap );
}

/// Create a dynamic icon by simlpy supplying a painter object
/// The ownership of the painter is NOT transfered.
/// @param painter a dynamic painter that is going to paint the icon
/// @param optionmap the options to pass to the painter
QIcon QtAwesome::icon(QtAwesomeIconPainter* painter, const QVariantMap& optionMap )
{
    // Warning, when you use memoryleak detection. You should turn it of for the next call
    // QIcon's placed in gui items are often cached and not deleted when my memory-leak detection checks for leaks.
    // I'm not sure if it's a Qt bug or something I do wrong
    QtAwesomeIconPainterIconEngine* engine = new QtAwesomeIconPainterIconEngine( this, painter, optionMap  );
    return QIcon( engine );
}

/// Adds a named icon-painter to the QtAwesome icon map
/// As the name applies the ownership is passed over to QtAwesome
///
/// @param name the name of the icon
/// @param painter the icon painter to add for this name
void QtAwesome::give(const QString& name, QtAwesomeIconPainter* painter)
{
    delete painterMap_.value( name );   // delete the old one
    painterMap_.insert( name, painter );
}

/// Creates/Gets the icon font with a given size in pixels. This can be usefull to use a label for displaying icons
/// Example:
///
///    QLabel* label = new QLabel( QChar( icon_group ) );
///    label->setFont( awesome->font(16) )
QFont QtAwesome::font( int size )
{
    QFont font( fontName_);
    font.setPixelSize(size);
    return font;
}
