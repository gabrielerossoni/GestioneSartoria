package gestionesartoria.model;

import java.io.Serializable;
import java.util.*;
import java.util.stream.Collectors;

/**
 * Entità aziendale di primo livello che modella il Magazzino della Sartoria Digitale.
 * Incapsula l'architettura logistica ripartita sui 3 Piani (Piano 0, 1, 2)
 * e fornisce i metodi centralizzati per la gestione delle giacenze, sottoscorta e inventario monetario.
 */
public class Magazzino implements Serializable {
    private static final long serialVersionUID = 1L;

    private String denominazioneSede;
    private String indirizzo;
    private final Map<Integer, PianoMagazzino> piani = new TreeMap<>();

    public Magazzino() {
        inizializzaPianiPredefiniti();
    }

    public Magazzino(String denominazioneSede, String indirizzo) {
        this.denominazioneSede = denominazioneSede;
        this.indirizzo = indirizzo;
        inizializzaPianiPredefiniti();
    }

    private void inizializzaPianiPredefiniti() {
        piani.put(0, new PianoMagazzino(0, "Ricevimento Merci e Scorte Pesanti", "Stoccaggio rotoli voluminosi, pezze grezze, interfodere e materiali da sballare"));
        piani.put(1, new PianoMagazzino(1, "Reparto Tessuti Pregiati e Rotoli", "Conservazione protetta per sete naturali, lane pregiate, lini, velluti e fodere Bemberg"));
        piani.put(2, new PianoMagazzino(2, "Minuteria, Filati, Bottoni e Accessori", "Cassettiere verticali per bottoni madreperla/corno, cerniere lampo, rocchetti e nastri"));
    }

    public String getDenominazioneSede() {
        return denominazioneSede;
    }

    public void setDenominazioneSede(String denominazioneSede) {
        this.denominazioneSede = denominazioneSede;
    }

    public String getIndirizzo() {
        return indirizzo;
    }

    public void setIndirizzo(String indirizzo) {
        this.indirizzo = indirizzo;
    }

    public Map<Integer, PianoMagazzino> getPiani() {
        return piani;
    }

    public PianoMagazzino getPiano(int numeroPiano) {
        return piani.get(numeroPiano);
    }

    /**
     * Restituisce la lista unificata di tutti i componenti stoccati nell'intero magazzino (tutti i piani).
     */
    public List<Componente> getTuttiIComponenti() {
        List<Componente> tutti = new ArrayList<>();
        for (PianoMagazzino piano : piani.values()) {
            tutti.addAll(piano.getComponenti());
        }
        return tutti;
    }

    /**
     * Aggiunge un componente posizionandolo nel rispettivo piano logistico.
     */
    public void aggiungiComponente(Componente c) {
        if (c == null) return;
        int pianoId = c.getPianoMagazzino();
        PianoMagazzino piano = piani.get(pianoId);
        if (piano == null) {
            piano = new PianoMagazzino(pianoId, "Piano " + pianoId, "Reparto stoccaggio aggiuntivo");
            piani.put(pianoId, piano);
        }
        piano.aggiungiComponente(c);
    }

    /**
     * Rimuove un componente ricercandolo su tutti i piani.
     */
    public boolean eliminaComponente(String idComponente) {
        boolean rimosso = false;
        for (PianoMagazzino piano : piani.values()) {
            if (piano.rimuoviComponente(idComponente)) {
                rimosso = true;
            }
        }
        return rimosso;
    }

    /**
     * Cerca un componente per codice identificativo univoco (es. CMP0001).
     */
    public Optional<Componente> cercaComponentePerId(String idComponente) {
        return getTuttiIComponenti().stream()
                .filter(c -> c.getId() != null && c.getId().equalsIgnoreCase(idComponente))
                .findFirst();
    }

    /**
     * Sposta un componente da un piano ad un altro aggiornandone piano e scaffale.
     */
    public boolean spostaComponente(String idComponente, int nuovoPiano, String nuovoScaffale) {
        Optional<Componente> opt = cercaComponentePerId(idComponente);
        if (opt.isPresent()) {
            Componente c = opt.get();
            eliminaComponente(idComponente);
            c.setPianoMagazzino(nuovoPiano);
            if (nuovoScaffale != null && !nuovoScaffale.trim().isEmpty()) {
                c.setScaffale(nuovoScaffale);
            }
            aggiungiComponente(c);
            return true;
        }
        return false;
    }

    /**
     * Calcola il valore economico monetario totale di tutte le merci dell'intero magazzino sartoriale.
     */
    public double getValoreEconomicoTotale() {
        return piani.values().stream()
                .mapToDouble(PianoMagazzino::calcolaValoreTotalePiano)
                .sum();
    }

    /**
     * Calcola il valore economico stoccato in un singolo piano.
     */
    public double getValoreEconomicoPiano(int piano) {
        PianoMagazzino p = piani.get(piano);
        return (p != null) ? p.calcolaValoreTotalePiano() : 0.0;
    }

    /**
     * Estrae tutti i componenti sotto soglia minima di sicurezza dell'intero magazzino.
     */
    public List<Componente> getComponentiSottoscorta() {
        return getTuttiIComponenti().stream()
                .filter(Componente::isSottoscorta)
                .collect(Collectors.toList());
    }

    /**
     * Restituisce i componenti stoccati al piano specificato.
     */
    public List<Componente> getComponentiPerPiano(int piano) {
        PianoMagazzino p = piani.get(piano);
        return (p != null) ? new ArrayList<>(p.getComponenti()) : Collections.emptyList();
    }

    /**
     * Svuota tutti i componenti mantenendo la configurazione dei piani.
     */
    public void svuota() {
        for (PianoMagazzino piano : piani.values()) {
            piano.getComponenti().clear();
        }
    }
}
