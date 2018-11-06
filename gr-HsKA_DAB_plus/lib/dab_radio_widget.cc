#include "dab_radio_widget.h"
#include "ui_dab_radio_widget.h"
#include <QPainter>
#include <QDoubleSpinBox>
#include <Python.h>
#include <iostream>

dab_radio_widget::dab_radio_widget(QWidget *parent)
	: QWidget(parent), ui(new Ui::dab_radio_widget())
{
	ui->setupUi(this);
	
	center_frequency_callback = 0;
		
	connect(ui->center_frequency, SIGNAL(valueChanged(double)), this, SLOT(center_frequency_changed(double)), Qt::QueuedConnection);
}

dab_radio_widget::~dab_radio_widget()
{
	delete ui;
}

void dab_radio_widget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	QWidget::paintEvent(event);
}

void dab_radio_widget::set_ensemble_name(char *name)
{
	ui->ensemble_name->setText(name);
}

void dab_radio_widget::set_service_count(char *service_count)
{
	ui->service_count->setText(service_count);
}

void dab_radio_widget::clear_sender_list()
{
	ui->available_sender->clear();
}

void dab_radio_widget::add_to_sender_list(char *name)
{
	ui->available_sender->addItem(name);
}

uint32_t dab_radio_widget::get_sender_list_length()
{
	return ui->available_sender->count();
}

void dab_radio_widget::set_sender_list(QStringList &sender_list)
{
	clear_sender_list();
	ui->available_sender->addItems(sender_list);
}

int32_t dab_radio_widget::get_selected_sender_index()
{
	return ui->available_sender->currentIndex();
}

void dab_radio_widget::set_title(char *title)
{
	ui->title->setText(title);
}

void dab_radio_widget::set_time(char *time)
{
	ui->time->setText(time);
}

void dab_radio_widget::set_date(char *date)
{
	ui->date->setText(date);
}

void dab_radio_widget::set_country(char *country)
{
	ui->country->setText(country);
}

void dab_radio_widget::set_program_type(char *type)
{
	ui->program_type->setText(type);
}

void dab_radio_widget::set_center_freq_callback(py_callback &center_freq_callback, std::function<void(double)> post_callback)
{
	center_frequency_callback = &center_freq_callback;
	m_post_callback = post_callback;
}

void dab_radio_widget::center_frequency_changed(double freq)
{
	if(center_frequency_callback)
	{
		PyGILState_STATE gstate = PyGILState_Ensure();
		center_frequency_callback->call(freq * 1.0e6);
		PyGILState_Release(gstate);
	}
	
	m_post_callback(freq * 1.0e6);
}