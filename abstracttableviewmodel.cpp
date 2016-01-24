#include "abstracttableviewmodel.h"

AbstractTableViewModel::AbstractTableViewModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

/**
 * Returns true if the model has any modifications after model
 * content was set.
 * @return
 */
bool AbstractTableViewModel::isModelModified() const
{
    return !m_modelRowState.isEmpty();
}

/**
 * If data in the model was modified then this modification
 * will be marked in a QHash. State 'Origin' is the standard
 * value und not kept here.
 * @param row
 * @param state
 */
void AbstractTableViewModel::setModelRowState(const int row, const AbstractTableViewModel::DataState newState)
{
    DataState currentState = m_modelRowState.value(row, Origin);
    if ( currentState == newState) {
        return;
    }
    if (newState == Origin) {
        // Origin is no modification. Entry will be removed.
        m_modelRowState.remove(row);
    } else {
        m_modelRowState.insert(row, newState);
    }
    emit isModelModifiedChanged();
    emit modelRowStateChanged(newState);
}

/**
 * Get information about a model row if it has its 'Origin' state or
 * if it has any modification.
 * @param row
 * @return
 */
AbstractTableViewModel::DataState AbstractTableViewModel::modelRowState(const int row) const
{
    return m_modelRowState.value(row, Origin);
}
