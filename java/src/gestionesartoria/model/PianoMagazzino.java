package gestionesartoria.model;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Rappresenta un singolo piano logistico e fisico del magazzino sartoriale.
 * Incapsula la lista dei componenti stoccati al piano, le coordinate e le metriche di inventario.
 */
public class PianoMagazzino implements Serializable {
    private static final long serialVersionUID = 1L;

    private int numeroPiano; // 0, 1, 2
    private String denominazione; // es. "Area Ricevimento Merci e Scorte Pesanti"
    private String descrizioneDettagliata;
    private final List<Componente> componenti = new ArrayList<>();

    public PianoMagazzino() {
    }

    public PianoMagazzino(int numeroPiano, String denominazione, String descrizioneDettagliata) {
        this.numeroPiano = numeroPiano;
        this.denominazione = denominazione;
        this.descrizioneDettagliata = descrizioneDettagliata;
    }

    public int getNumeroPiano() {
        return numeroPiano;
    }

    public void setNumeroPiano(int numeroPiano) {
        this.numeroPiano = numeroPiano;
    }

    public String getDenominazione() {
        return denominazione;
    }

    public void setDenominazione(String denominazione) {
        this.denominazione = denominazione;
    }

    public String getDescrizioneDettagliata() {
        return descrizioneDettagliata;
    }

    public void setDescrizioneDettagliata(String descrizioneDettagliata) {
        this.descrizioneDettagliata = descrizioneDettagliata;
    }

    public List<Componente> getComponenti() {
        return componenti;
    }

    public void aggiungiComponente(Componente c) {
        if (c != null) {
            c.setPianoMagazzino(this.numeroPiano);
            componenti.add(c);
        }
    }

    public boolean rimuoviComponente(String idComponente) {
        return componenti.removeIf(c -> c.getId() != null && c.getId().equalsIgnoreCase(idComponente));
    }

    /**
     * Calcola il valore economico complessivo delle merci stoccate in questo specifico piano.
     */
    public double calcolaValoreTotalePiano() {
        return componenti.stream()
                .mapToDouble(Componente::getValoreTotale)
                .sum();
    }

    /**
     * Restituisce tutti gli articoli presenti in questo piano la cui giacenza è inferiore o uguale alla soglia minima.
     */
    public List<Componente> getComponentiSottoscorta() {
        return componenti.stream()
                .filter(Componente::isSottoscorta)
                .collect(Collectors.toList());
    }

    /**
     * Calcola il totale degli articoli / rotoli stoccati in questo piano.
     */
    public int getTotaleArticoli() {
        return componenti.size();
    }

    @Override
    public String toString() {
        return "Piano " + numeroPiano + " - " + denominazione + " (" + getTotaleArticoli() + " articoli | € " + String.format("%.2f", calcolaValoreTotalePiano()) + ")";
    }
}
