package gestionesartoria.model;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

@SuppressWarnings("serial")
public class Progetto implements Serializable {
    private static final long serialVersionUID = 1L;
    private static final DateTimeFormatter DATE_FORMATTER = DateTimeFormatter.ofPattern("dd/MM/yyyy");

    private String id;
    private String idCliente;
    private String tipoCapo;
    private String idSartoAssegnato;
    private StatoProgetto statoAvanzamento;
    private LocalDate dataInizio;
    private LocalDate dataConsegnaPrevista;
    private double prezzoFinale;
    private List<ComponenteUsato> componentiUsati;

    public Progetto() {
        this.componentiUsati = new ArrayList<>();
        this.statoAvanzamento = StatoProgetto.IN_ATTESA;
    }

    public Progetto(String id, String idCliente, String tipoCapo, String idSartoAssegnato,
                    StatoProgetto statoAvanzamento, LocalDate dataInizio, LocalDate dataConsegnaPrevista,
                    double prezzoFinale) {
        this.id = id;
        this.idCliente = idCliente;
        this.tipoCapo = tipoCapo;
        this.idSartoAssegnato = idSartoAssegnato;
        this.statoAvanzamento = (statoAvanzamento != null) ? statoAvanzamento : StatoProgetto.IN_ATTESA;
        this.dataInizio = dataInizio;
        this.dataConsegnaPrevista = dataConsegnaPrevista;
        this.prezzoFinale = prezzoFinale;
        this.componentiUsati = new ArrayList<>();
    }

    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getIdCliente() { return idCliente; }
    public void setIdCliente(String idCliente) { this.idCliente = idCliente; }

    public String getTipoCapo() { return tipoCapo; }
    public void setTipoCapo(String tipoCapo) { this.tipoCapo = tipoCapo; }

    public String getIdSartoAssegnato() { return idSartoAssegnato; }
    public void setIdSartoAssegnato(String idSartoAssegnato) { this.idSartoAssegnato = idSartoAssegnato; }

    public StatoProgetto getStatoAvanzamento() { return statoAvanzamento; }
    public void setStatoAvanzamento(StatoProgetto statoAvanzamento) { this.statoAvanzamento = statoAvanzamento; }

    public LocalDate getDataInizio() { return dataInizio; }
    public void setDataInizio(LocalDate dataInizio) { this.dataInizio = dataInizio; }

    public String getDataInizioFormattata() {
        return (dataInizio != null) ? dataInizio.format(DATE_FORMATTER) : "";
    }

    public LocalDate getDataConsegnaPrevista() { return dataConsegnaPrevista; }
    public void setDataConsegnaPrevista(LocalDate dataConsegnaPrevista) { this.dataConsegnaPrevista = dataConsegnaPrevista; }

    public String getDataConsegnaPrevistaFormattata() {
        return (dataConsegnaPrevista != null) ? dataConsegnaPrevista.format(DATE_FORMATTER) : "";
    }

    public double getPrezzoFinale() { return prezzoFinale; }
    public void setPrezzoFinale(double prezzoFinale) { this.prezzoFinale = prezzoFinale; }

    public List<ComponenteUsato> getComponentiUsati() { return componentiUsati; }
    public void setComponentiUsati(List<ComponenteUsato> componentiUsati) { this.componentiUsati = componentiUsati; }

    public void addComponenteUsato(ComponenteUsato comp) {
        if (this.componentiUsati == null) {
            this.componentiUsati = new ArrayList<>();
        }
        this.componentiUsati.add(comp);
    }

    public double calcolaCostoMateriali() {
        if (componentiUsati == null) return 0.0;
        double sum = 0.0;
        for (ComponenteUsato c : componentiUsati) {
            sum += c.getCostoTotale();
        }
        return sum;
    }

    @Override
    public String toString() {
        String stato = (statoAvanzamento != null) ? statoAvanzamento.getLabel() : "NON DEFINITO";
        return String.format("%s - %s [%s] € %.2f", id, tipoCapo, stato, prezzoFinale);
    }
}
