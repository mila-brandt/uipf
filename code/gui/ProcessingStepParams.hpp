#ifndef PROCESSINGSTEPPARAMS_H
#define PROCESSINGSTEPPARAMS_H

#include <QAbstractTableModel>
#include <QStyleOptionViewItem>

#include "../framework/ProcessingStep.hpp"

namespace uipf {

class ProcessingStepParams : public QAbstractTableModel
{
    Q_OBJECT

public:
	// constructor
    ProcessingStepParams(QObject *parent);

    // sets the number of rows for the widget
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
	// sets the number of columns for the widget
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

	// this method is triggered when a parameter has been edited
	bool setData(const QModelIndex &index, const QVariant &value, int role);

	// describes the data that is displayed in the widget
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	// describes what is displayed in the headers of the table
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // sets the processing step this widget represents and updates the content
    void setProcessingStep(uipf::ProcessingStep);

    // set flags for the fields, allows them to be editable
	Qt::ItemFlags flags (const QModelIndex &index) const;

private:
	// the processing step represented by this widget
	ProcessingStep step_;

	// vector of available param names, index is equal to index in the widget
	std::vector<std::string> paramNames_;
};

}; // namespace

#endif // PROCESSINGSTEPPARAMS_H
