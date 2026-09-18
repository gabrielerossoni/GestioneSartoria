package gestionesartoria.model;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class Personale implements Serializable {
    private static final long serialVersionUID = 1L;
    private static final DateTimeFormatter DATE_FORMATTER = DateTimeFormatter.ofPattern("dd/MM/yyyy");

    private String id;
    private String nome;
    private String cognome;
    private String ruolo;
    private String tipoContratto;
    private double retribuzioneMensile;
    private LocalDate dataAssunzione;
    private String stato; // ATTIVO, FERIE, CESSATO

    public Personale() {
    }

    public Personale(String id, String nome, String cognome, String ruolo, String tipoContratto,
                     double retribuzioneMensile, LocalDate dataAssunzione, String stato) {
        this.id = id;
        this.nome = nome;
        this.cognome = cognome;
        this.ruolo = ruolo;
        this.tipoContratto = tipoContratto;
        this.retribuzioneMensile = retribuzioneMensile;
        this.dataAssunzione = dataAssunzione;
        this.stato = (stato != null && !stato.trim().isEmpty()) ? stato : "ATTIVO";
    }

    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }

    public String getCognome() { return cognome; }
    public void setCognome(String cognome) { this.cognome = cognome; }

    public String getRuolo() { return ruolo; }
    public void setRuolo(String ruolo) { this.ruolo = ruolo; }

    public String getTipoContratto() { return tipoContratto; }
    public void setTipoContratto(String tipoContratto) { this.tipoContratto = tipoContratto; }

    public double getRetribuzioneMensile() { return retribuzioneMensile; }
    public void setRetribuzioneMensile(double retribuzioneMensile) { this.retribuzioneMensile = retribuzioneMensile; }

    public LocalDate getDataAssunzione() { return dataAssunzione; }
    public void setDataAssunzione(LocalDate dataAssunzione) { this.dataAssunzione = dataAssunzione; }

    public String getDataAssunzioneFormattata() {
        return (dataAssunzione != null) ? dataAssunzione.format(DATE_FORMATTER) : "";
    }

    public String getStato() { return stato; }
    public void setStato(String stato) { this.stato = stato; }

    public String getNomeCompleto() {
        return nome + " " + cognome;
    }

    @Override
    public String toString() {
        return id + " - " + getNomeCompleto() + " (" + ruolo + ")";
    }
}
