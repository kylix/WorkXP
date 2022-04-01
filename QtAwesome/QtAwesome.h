/**
 * QtAwesome - use font-awesome (or other font icons) in your c++ / Qt Application
 *
 * MIT Licensed
 *
 * Copyright 2013-2015 - Reliable Bits Software by Blommers IT. All Rights Reserved.
 * Author Rick Blommers
 */

#ifndef QTAWESOME_H
#define QTAWESOME_H

#include "QtAwesomeAnim.h"

#include <QIcon>
#include <QIconEngine>
#include <QPainter>
#include <QRect>
#include <QVariantMap>

#define QTAWESOME QtAwesome::instance()

/// A list of all icon-names with the codepoint (unicode-value) on the right
/// You can use the names on the page  http://fortawesome.github.io/Font-Awesome/design.html
namespace fa {
  enum icon {
      aligncenter                      = 0xf037,
      alignjustify                     = 0xf039,
      alignleft                        = 0xf036,
      alignright                       = 0xf038,
      angledown                        = 0xf107,
      angleleft                        = 0xf104,
      angleright                       = 0xf105,
      angleup                          = 0xf106,
      bold                             = 0xe859,
      italic                           = 0xe858,
      underline                        = 0xe87b,
      chevrondown                      = 0xf078,
      chevronleft                      = 0xf053,
      chevronright                     = 0xf054,
      chevronup                        = 0xf077,
      eye                              = 0xf06e,
      eyeslash                         = 0xf070,
      plus                             = 0xf067,
      minus                            = 0xf068,
      selection                        = 0xe609,
      selfselection                      = 0xe68d,
      home                             = 0xe605,
      circle                           = 0xe735,
      rectangle                        = 0xe673,
      curve                            = 0xe602,
      line                             = 0xe632,
      point                            = 0xf26e,
      fittocanvas                      = 0xf2ba,
      refresh                          = 0xf021,
      close                            = 0xe74c,
      gear                             = 0xf2bc,
      theme                            = 0xf2bb,
      canvas                           = 0xf2b9,
      rotate                           = 0xf170,
      flashoff                         = 0xe81a,
      flashon                          = 0xeb73,
      remotecontroller                 = 0xe608,
      distributevertically             = 0xe601,
      distributehorizontally           = 0xe600,
      layers                           = 0xe664,
      bitmap                           = 0xe631,
      text                             = 0xe684,
      alignedgeleft                    = 0xe62f,
      alignedgeright                   = 0xe62e,
      alignedgevcenter                 = 0xe630,
      alignedgetop                     = 0xe62c,
      alignedgebottom                  = 0xe62d,
      alignedgehcenter                 = 0xe62b,
      lock                             = 0xe6c3,
      unlock                           = 0xe72c,
      trash                            = 0xe726
  };
}



//---------------------------------------------------------------------------------------

class QtAwesomeIconPainter;

/// The main class for managing icons
/// This class requires a 2-phase construction. You must first create the class and then initialize it via an init* method
class QtAwesome : public QObject
{
Q_OBJECT

public:

    //Add by huanglingzhi
    static QtAwesome* instance(QObject* parent=0) throw();

    explicit QtAwesome(QObject *parent = 0);
    virtual ~QtAwesome();

    void init( const QString& fontname );
    bool initFontAwesome();

    void addNamedCodepoint( const QString& name, int codePoint );
    QHash<QString,int> namedCodePoints() { return namedCodepoints_; }

    void setDefaultOption( const QString& name, const QVariant& value  );
    QVariant defaultOption( const QString& name );

    QIcon icon( int character, const QVariantMap& options = QVariantMap() );
    QIcon icon( const QString& name, const QVariantMap& options = QVariantMap() );
    QIcon icon(QtAwesomeIconPainter* painter, const QVariantMap& optionMap = QVariantMap() );

    void give( const QString& name, QtAwesomeIconPainter* painter );

    QFont font( int size );

    /// Returns the font-name that is used as icon-map
    QString fontName() { return fontName_ ; }

private:
    /// the single instance member
    static QtAwesome *_instance;

    QString fontName_;                                     ///< The font name used for this map
    QHash<QString,int> namedCodepoints_;                   ///< A map with names mapped to code-points

    QHash<QString, QtAwesomeIconPainter*> painterMap_;     ///< A map of custom painters
    QVariantMap defaultOptions_;                           ///< The default icon options
    QtAwesomeIconPainter* fontIconPainter_;                ///< A special painter fo painting codepoints
};


//---------------------------------------------------------------------------------------


/// The QtAwesomeIconPainter is a specialized painter for painting icons
/// your can implement an iconpainter to create custom font-icon code
class QtAwesomeIconPainter
{
public:
    virtual ~QtAwesomeIconPainter() {}
    virtual void paint( QtAwesome* awesome, QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options ) = 0;
};

Q_DECLARE_METATYPE(QtAwesomeAnimation*)

#endif // QTAWESOME_H
