#include "characterdefinition.h"

/**
 * Constructor
 * Standard
 */
CharacterDefinition::CharacterDefinition()
{
}

/**
 * Constructor
 * Creates a CharacterDefinition object with a range of characters.
 * @param amount
 * @param from
 * @param to
 */
CharacterDefinition::CharacterDefinition(unsigned short amount, QChar from, QChar to) :
    m_amount(amount)
{
    if (from.isLower() && to.isUpper() || from.isUpper() && to.isLower()) {
        buildRange(from.toLower(), to.toLower());
        buildRange(from.toUpper(), to.toUpper());
    } else {
        buildRange(from, to);
    }
}

/**
 * Constructor
 * Creates a CharacterDefinition object with a set of character.
 * @param amount
 * @param characterList
 */
CharacterDefinition::CharacterDefinition(unsigned short amount, QList<QChar> characterList) :
    m_amount(amount)
{
    for (QChar character : characterList) {
        m_characterList << character;
    }
}

/**
 * Build a list of chararcters in a range.
 * @param from      Start of range.
 * @param to        End od range.
 */
void CharacterDefinition::buildRange(const QChar &from, const QChar &to)
{
    ushort current = from.unicode();
    m_characterList << from;
    do {
        m_characterList << QChar(++current);
    } while (current != to);
}
