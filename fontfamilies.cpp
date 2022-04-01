#include "fontfamilies.h"

#include <QFontDatabase>
#include <QDebug>

FontFamilies::FontFamilies()
{
  m_families.append("Roboto");
  m_families.append("Helvetica Neue");
  m_families.append("Helvetica");
  m_families.append("Arial");
  m_families.append("Microsoft Yahei");
  m_families.append("Hiragino Sans GB");
  m_families.append("Heiti SC");
  m_families.append("WenQuanYi Micro Hei");
  m_families.append("sans-serif");
}


QFont FontFamilies::resort() const
{
    QFontDatabase fontDb;
    QStringList families = fontDb.families();
    QFont font;


    foreach(const QString  & family,m_families)
    {
        if (families.contains(family,Qt::CaseInsensitive)) {
            //if (fontDb.isPrivateFamily(family)) continue;
            font.setFamily(family);
            qDebug()<<"Resorted Font Family:"<<font.family()<<","<<font.key();
            break;
        }
    }

    return font;


}
