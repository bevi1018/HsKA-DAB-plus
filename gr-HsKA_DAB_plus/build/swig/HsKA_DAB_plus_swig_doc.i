
/*
 * This file was automatically generated using swig_doc.py.
 *
 * Any changes to it will be lost next time it is regenerated.
 */




%feature("docstring") gr::HsKA_DAB_plus::CRC16 "

Constructor Specific Documentation:

Konstruktor

Args:
    polynom : Das Generatorpolynom der CRC16
    length : Die Laenge ueber die die CRC16 berechnet wird (inkl. der 2 Byte fuer die CRC16)
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::CRC16::make "

Constructor Specific Documentation:

Konstruktor

Args:
    polynom : Das Generatorpolynom der CRC16
    length : Die Laenge ueber die die CRC16 berechnet wird (inkl. der 2 Byte fuer die CRC16)
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::Departitioning "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    databaseID : ID der Datenbank, von der die eingestellte Subchannel ID gelesen werden kann (DB_SELECTED_SUBCHANNEL_ID)
    vector_length : Anzahl der OFDM Untertraeger
    debug_enable : 0 deaktiviert die Debugausgabe, sonst werden Debugdaten ausgegeben"

%feature("docstring") gr::HsKA_DAB_plus::Departitioning::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    databaseID : ID der Datenbank, von der die eingestellte Subchannel ID gelesen werden kann (DB_SELECTED_SUBCHANNEL_ID)
    vector_length : Anzahl der OFDM Untertraeger
    debug_enable : 0 deaktiviert die Debugausgabe, sonst werden Debugdaten ausgegeben"

%feature("docstring") gr::HsKA_DAB_plus::DQPSK_Demodulation "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger
    fft_length : Laenge der FFT"

%feature("docstring") gr::HsKA_DAB_plus::DQPSK_Demodulation::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger
    fft_length : Laenge der FFT"

%feature("docstring") gr::HsKA_DAB_plus::fib_decoder "

Constructor Specific Documentation:

Konstruktor

Args:
    databaseID : ID der Datenbank, in die die dekodierten Daten geschrieben werden sollen.
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::fib_decoder::make "

Constructor Specific Documentation:

Konstruktor

Args:
    databaseID : ID der Datenbank, in die die dekodierten Daten geschrieben werden sollen.
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::FIC_Scrambler "

Constructor Specific Documentation:

Konstruktor"

%feature("docstring") gr::HsKA_DAB_plus::FIC_Scrambler::make "

Constructor Specific Documentation:

Konstruktor"

%feature("docstring") gr::HsKA_DAB_plus::firecode_checker "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::firecode_checker::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::Frequency_Deinterleaver "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger
    fft_length : Laenge der FFT"

%feature("docstring") gr::HsKA_DAB_plus::Frequency_Deinterleaver::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger
    fft_length : Laenge der FFT"

%feature("docstring") gr::HsKA_DAB_plus::MP4_decoder "

Constructor Specific Documentation:

Konstruktor

Args:
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::MP4_decoder::make "

Constructor Specific Documentation:

Konstruktor

Args:
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::MSC_Scrambler "

Constructor Specific Documentation:

Konstruktor. Die Laenge des Vektors wird per Eingang mit uebergeben."

%feature("docstring") gr::HsKA_DAB_plus::MSC_Scrambler::make "

Constructor Specific Documentation:

Konstruktor. Die Laenge des Vektors wird per Eingang mit uebergeben."

%feature("docstring") gr::HsKA_DAB_plus::OFDM_coarse_frequency_correction "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    fft_length : Laenge der FFT
    num_carriers : Anzahl der OFDM Untertraeger
    cp_length : Laenge des Cyclic-Prefix
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::OFDM_coarse_frequency_correction::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    fft_length : Laenge der FFT
    num_carriers : Anzahl der OFDM Untertraeger
    cp_length : Laenge des Cyclic-Prefix
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::OFDM_Symbol_Synchronisation "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    N : Symboldauer in Samples (Ohne Guardintervall)
    L : Dauer des Guardintervalls in Samples
    SNR : Ungefaehres SNR des Signals
    average_count : Anzahl der Werte ueber die Epsilon und Theta gemittelt wird
    debug_enable : 0 deaktiviert die Debugausgabe; Sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::OFDM_Symbol_Synchronisation::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    N : Symboldauer in Samples (Ohne Guardintervall)
    L : Dauer des Guardintervalls in Samples
    SNR : Ungefaehres SNR des Signals
    average_count : Anzahl der Werte ueber die Epsilon und Theta gemittelt wird
    debug_enable : 0 deaktiviert die Debugausgabe; Sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::reed_solomon_decoder "

Constructor Specific Documentation:

Konstruktor

Args:
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv."

%feature("docstring") gr::HsKA_DAB_plus::reed_solomon_decoder::make "

Constructor Specific Documentation:

Konstruktor

Args:
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv."

%feature("docstring") gr::HsKA_DAB_plus::Remove_First_OFDM_Symbol "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger"

%feature("docstring") gr::HsKA_DAB_plus::Remove_First_OFDM_Symbol::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger"

%feature("docstring") gr::HsKA_DAB_plus::Soft_Demapper "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger"

%feature("docstring") gr::HsKA_DAB_plus::Soft_Demapper::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM Untertraeger"

%feature("docstring") gr::HsKA_DAB_plus::Super_Frame_Synchronisation "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    T_NULL : Gibt die Laenge des Nullsymbols in Samples an.
    LOOK_AHEAD : Gibt an, ueber wie viele Samples die Steigung der gemittelten Leistung berechnet wird.
    debug_enable : 0 Deaktivierte Debugausgabe. Sonst Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::Super_Frame_Synchronisation::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    T_NULL : Gibt die Laenge des Nullsymbols in Samples an.
    LOOK_AHEAD : Gibt an, ueber wie viele Samples die Steigung der gemittelten Leistung berechnet wird.
    debug_enable : 0 Deaktivierte Debugausgabe. Sonst Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::time_deinterleaver "

Constructor Specific Documentation:

Konstruktor fuer diesen Block.

Args:
    vector_length : Laenge des Vektors, ueber den das Timedeinterleaving durchgefuehrt wird."

%feature("docstring") gr::HsKA_DAB_plus::time_deinterleaver::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block.

Args:
    vector_length : Laenge des Vektors, ueber den das Timedeinterleaving durchgefuehrt wird."

%feature("docstring") gr::HsKA_DAB_plus::unpuncture_fic "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM-Untertraeger"

%feature("docstring") gr::HsKA_DAB_plus::unpuncture_fic::make "

Constructor Specific Documentation:

Konstruktor fuer diesen Block

Args:
    vector_length : Anzahl der OFDM-Untertraeger"

%feature("docstring") gr::HsKA_DAB_plus::unpuncture_msc "

Constructor Specific Documentation:

Konstruktor

Args:
    databaseID : ID zur Database, aus der die Punktierungsdaten gelesen werden.
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::unpuncture_msc::make "

Constructor Specific Documentation:

Konstruktor

Args:
    databaseID : ID zur Database, aus der die Punktierungsdaten gelesen werden.
    debug_enable : 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv"

%feature("docstring") gr::HsKA_DAB_plus::viterbi_wrapper "

Constructor Specific Documentation:

Konstruktor

Args:
    fsm_input_size : Anzahl der Eingaenge in die Finite State Machine
    fsm_output_size : Anzahl der Ausgaenge in die Finite State Machine
    fsm_table : Vektor der Generatorsequenzen fuer die Finite State Machine
    init_state : Status der Finite State Machine beim Start des Viterbi-Algorithmus
    final_state : Status der Finite State Machne beim Ende des Viterbi-Algorithmus
    dimension : Dimension des Codes
    table : Tabelle mit den moeglichen Zustaenden"

%feature("docstring") gr::HsKA_DAB_plus::viterbi_wrapper::make "

Constructor Specific Documentation:

Konstruktor

Args:
    fsm_input_size : Anzahl der Eingaenge in die Finite State Machine
    fsm_output_size : Anzahl der Ausgaenge in die Finite State Machine
    fsm_table : Vektor der Generatorsequenzen fuer die Finite State Machine
    init_state : Status der Finite State Machine beim Start des Viterbi-Algorithmus
    final_state : Status der Finite State Machne beim Ende des Viterbi-Algorithmus
    dimension : Dimension des Codes
    table : Tabelle mit den moeglichen Zustaenden"











%feature("docstring") MovingSum::MovingSum "Konstruktor"

%feature("docstring") MovingSum::~MovingSum "Destruktor"

%feature("docstring") MovingSum::isReady "Gibt an, ob der Buffer gefuellt ist und damit die Summe verwendet werden kann"

%feature("docstring") MovingSum::getLength "Gibt die Anzahl der Werte an, die aufsummiert werden"

%feature("docstring") MovingSum::filter "Addiert einen neuen Wert auf die Summe und gibt die gleitende Summe zurueck."

%feature("docstring") py_callback "Klasse, die einen Python Callback darstellt, um z.B. einen GnuRadio Parameter zu ?ndern Von dieser Klasse in Python ableiten"

%feature("docstring") py_callback::~py_callback "Virtueller Destruktor"

%feature("docstring") py_callback::call "Virtuelle Funktion f?r den Aufruf der set-Funktion"

%feature("docstring") gr::HsKA_DAB_plus::radio_gui "Block der die zum Radio gehoerende GUI anzeigt."

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::radio_gui "Konstruktor"

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::~radio_gui "Destruktor"

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::pyqwidget "Gibt das QWidget als Python-Objekt zurueck."

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::set_center_freq_callback "Setzt den Callback, mit dem die Centerfrequenz des SDR gesetzt werden kann. Dafuer muss in Python von der py_callback-Klasse abgeleitet werden."

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::center_frequency_changed "Wird von der GUI aufgerufen, wenn die Centerfrequenz geaendert wurde."

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::update_thread "Thread, der jede Sekunde alle anzuzeigenden Werte aktualisiert."

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::convert_date "Wandelt das Modifizierte julianische Datum in Text um."

%feature("docstring") gr::HsKA_DAB_plus::radio_gui::build_sender_list "Liest alle Sendernamen anhand eines Arrays mit Service-IDs und speichert diese in ein String-Array."



%feature("docstring") RingBuffer::RingBuffer "Konstruktor"

%feature("docstring") RingBuffer::~RingBuffer "Destruktor"

%feature("docstring") RingBuffer::isReady "Gibt an, ob der Ringbuffer komplett gefuellt ist (erst dann sind alle Werte gueltig"

%feature("docstring") RingBuffer::getLength "Gibt die Laenge des Ringbuffers zurueck"

%feature("docstring") RingBuffer::pushValue "Speichert einen neuen Wert in den Ringbuffer. Der letzte Wert wird dabei geloescht."

%feature("docstring") RingBuffer::getValueAt "Gibt den Wert zurueck, der mit dem pushValue()-Aufruf uebergeben wurde, der offset zurueck liegt"

%feature("docstring") gr::HsKA_DAB_plus::shared_database "Threadsichere Datenbank, die es ermoeglicht Daten zwischen den Bloecken auszutauschen."

%feature("docstring") gr::HsKA_DAB_plus::shared_database::shared_database "Konstruktor (Fuer interne Zwecke)"

%feature("docstring") gr::HsKA_DAB_plus::shared_database::~shared_database "Destruktor"

%feature("docstring") gr::HsKA_DAB_plus::shared_database::variable_exists "Ueberprueft, ob die Variable mit der angegebenen ID existiert."

%feature("docstring") gr::HsKA_DAB_plus::shared_database::create_variable "Erzeugt eine Variable."

%feature("docstring") gr::HsKA_DAB_plus::shared_database::check_and_get_variable "Ueberprueft, ob eine Variable vorhanden ist. Ist dies der Fall, wird die Variable gelesen."

%feature("docstring") gr::HsKA_DAB_plus::shared_database::set_variable "Schreibt den Wert einer Variable. Die Variable wird bei Bedarf angelegt, sollte diese nicht vorhanden sein."

%feature("docstring") gr::HsKA_DAB_plus::shared_database::destroy_variable "Loescht eine Variable"

%feature("docstring") gr::HsKA_DAB_plus::shared_database::open_database "Oeffnet eine Datenbank."