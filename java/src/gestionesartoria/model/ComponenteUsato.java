package gestionesartoria.model;

import java.io.Serializable;

public class ComponenteUsato implements Serializable {
    private static final long serialVersionUID = 1L;

    private String idComponente;
    private double quantitaUsata;
    private String nomeComponente;
    private String unitaMisura;
    private double costoUnitario;

    public ComponenteUsato() {
    }

    public ComponenteUsato(String idComponente, double quantitaUsata) {
        this.idComponente = idComponente;
        this.quantitaUsata = quantitaUsata;
    }

    public ComponenteUsato(String idComponente, String nomeComponente, double quantitaUsata, String unitaMisura, double costoUnitario) {
        this.idComponente = idComponente;
        this.nomeComponente = nomeComponente;
        this.quantitaUsata = quantitaUsata;
        this.unitaMisura = unitaMisura;
        this.costoUnitario = costoUnitario;
    }

    public String getIdComponente() { return idComponente; }
    public void setIdComponente(String idComponente) { this.idComponente = idComponente; }

    public double getQuantitaUsata() { return quantitaUsata; }
    public void setQuantitaUsata(double quantitaUsata) { this.quantitaUsata = quantitaUsata; }

    public String getNomeComponente() { return nomeComponente; }
    public void setNomeComponente(String nomeComponente) { this.nomeComponente = nomeComponente; }

    public String getUnitaMisura() { return unitaMisura; }
    public void setUnitaMisura(String unitaMisura) { this.unitaMisura = unitaMisura; }

    public double getCostoUnitario() { return costoUnitario; }
    public void setCostoUnitario(double costoUnitario) { this.costoUnitario = costoUnitario; }

    public double getCostoTotale() {
        return quantitaUsata * costoUnitario;
    }

    @Override
    public String toString() {
        String label = (nomeComponente != null) ? nomeComponente : idComponente;
        String um = (unitaMisura != null) ? unitaMisura : "";
        return label + ": " + String.format("%.2f", quantitaUsata) + " " + um;
    }
}
