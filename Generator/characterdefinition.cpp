#include "characterdefinition.h"

/*! Constructor
 *
 * Standard
 */
CharacterDefinition::CharacterDefinition()
{
}

/*! Constructor
 *
 * Creates a CharacterDefinition object with a range of characters.
 * Based on character table used by the system like UTF8.
 * @param amount    How much character should be used of that range.
 * @param from      First character in range.
 * @param to        Last character in range.
 */
CharacterDefinition::CharacterDefinition(unsigned short amount, QChar from, QChar to) :
    m_amount(amount)
{
    if ((from.isLower() && to.isUpper()) || (from.isUpper() && to.isLower())) {
        buildRange(from.toLower(), to.toLower());
        buildRange(from.toUpper(), to.toUpper());
    } else {
        buildRange(from, to);
    }
}

/*! Constructor
 *
 * Creates a CharacterDefinition object with a set of character.
 * @param amount            How much characters should be taken from that set.
 * @param characterList     A list of characters.
 */
CharacterDefinition::CharacterDefinition(unsigned short amount, QList<QChar> characterList) :
    m_amount(amount)
{
    for (QChar character : characterList) {
        m_characterList << character;
    }
}

/*! Private
 *
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
