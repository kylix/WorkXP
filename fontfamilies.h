#ifndef FONTFAMILIES_H
#define FONTFAMILIES_H

#include <QFont>
#include <QStringList>

class FontFamilies
{
public:
    FontFamilies();
    QFont resort() const;

private:
    QStringList m_families;
};

#endif // FONTFAMILIES_H
