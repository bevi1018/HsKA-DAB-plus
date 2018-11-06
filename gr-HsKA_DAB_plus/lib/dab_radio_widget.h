#ifndef DAB_RADIO_WIDGET_H
#define DAB_RADIO_WIDGET_H

#include <QWidget>
#include <HsKA_DAB_plus/py_callback.h>
#include <functional>

namespace Ui
{
	class dab_radio_widget;
}

/**
 * \brief QWidget fuer die grafische Benutzeroberflaeche
 * \author Vivian Becher
 * \author David Kohler
**/
class dab_radio_widget : public QWidget
{
	Q_OBJECT
	
public:
	/**
	 * Konstruktor
	 * \param parent QWidget des Parents
	**/
	explicit dab_radio_widget(QWidget *parent = 0);
	
	/**
	 * Destruktor
	**/
	~dab_radio_widget();
	
	/**
	 * Setzt den Text des Ensemble-Name-Labels
	 * \param name Text, der als Ensemble-Name gesetzt wird
	**/
	void set_ensemble_name(char *name);
	
	/**
	 * Setzt den Text des Service-Count-Labels
	 * \param service_count Text, der als Service-Count gesetzt wird
	**/
	void set_service_count(char *service_count);
	
	/**
	 * Loescht die aktuelle Senderliste
	**/
	void clear_sender_list();
	
	/**
	 * Fuegt einen neuen Eintrag der Senderliste hinzu.
	 * \param name Name des Senders, der hinzugefuegt wird
	**/
	void add_to_sender_list(char *name);
	
	/**
	 * Gibt die Anzahl der Sender zurueck.
	 * \return Anzahl der Sender
	**/
	uint32_t get_sender_list_length();
	
	/**
	 * Setzt die anzuzeigende Senderliste
	 * \param sender_list Liste mit den Sendernamen
	**/
	void set_sender_list(QStringList &sender_list);
	
	/**
	 * Gibt den in der GUI ausgewaehlten Index in der Senderliste zurueck.
	 * \return Index in der Senderliste
	**/
	int32_t get_selected_sender_index();
	
	/**
	 * Setzt den anzuzeigenden Musiktitel
	 * \param title Anzuzeigender Musiktitel
	**/
	void set_title(char *title);
	
	/**
	 * Setzt die anzuzeigende Uhrzeit
	 * \param time Anzuzeigende Uhrzeit
	**/
	void set_time(char *time);
	
	/**
	 * Setzt das anzuzeigende Datum
	 * \param date Anzuzeigende Datum
	**/
	void set_date(char *date);
	
	/**
	 * Setzt die anzuzeigende Land
	 * \param country Anzuzeigendes Land
	**/
	void set_country(char *country);
	
	/**
	 * Setzt den anzuzeigenden Programm-Typ
	 * \param type Anzuzeigende Programm-Typ
	**/
	void set_program_type(char *type);
		
	/**
	 * Setzt den Callback, der aufgerufen wird, sobald die Centerfrequenz geandert wurde.
	 * \param center_freq_callback Callback, mit dem die Centerfrequenz des SDR gesetzt werden kann.
	 * \param post_callback Callback, der nach einer Aenderung der SDR-Centerfrequenz aufgerufen wird (z.B. zum Aendern eines Labels)
	**/
	void set_center_freq_callback(py_callback &center_freq_callback, std::function<void(double)> post_callback);
	
protected:
	/**
	 * Wird automatisch aufgerufen, sobald die GUI neu gezeichnet wird.
	 * \param event Gibt Infos darueber, wie gezeichnet werden soll.
	**/
	void paintEvent(QPaintEvent* event);
	
private:
	Ui::dab_radio_widget *ui;						///< QWidget fuer die anzuzeigende GUI
	py_callback *center_frequency_callback;			///< Callback, mit dem die Centerfrequenz des SDR gesetzt werden kann.
	std::function<void(double)> m_post_callback;	///< Callback, der nach einer Aenderung der SDR-Centerfrequenz aufgerufen wird (z.B. zum Aendern eines Labels)
	
private slots:
	/**
	 * Wird aufgerufen, wenn die Centerfrequenz in der GUI geaendert wird.
	 * \param freq Die neue Centerfrequenz, die eingestellt wurde.
	**/
	void center_frequency_changed(double freq);
};

#endif // DAB_RADIO_WIDGET_H
