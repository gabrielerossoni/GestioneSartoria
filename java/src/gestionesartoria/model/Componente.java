package gestionesartoria.model;

import java.io.Serializable;

public class Componente implements Serializable {
    private static final long serialVersionUID = 1L;

    private String id;
    private String nome;
    private String categoria;
    private double quantitaDisponibile;
    private String unitaMisura;
    private double costoUnitario;
    private String idFornitore;
    private int pianoMagazzino; // 0, 1, 2
    private String scaffale;
    private double sogliaMinima;

    public Componente() {
    }

    public Componente(String id, String nome, String categoria, double quantitaDisponibile,
                      String unitaMisura, double costoUnitario, String idFornitore,
                      int pianoMagazzino, String scaffale, double sogliaMinima) {
        this.id = id;
        this.nome = nome;
        this.categoria = categoria;
        this.quantitaDisponibile = quantitaDisponibile;
        this.unitaMisura = unitaMisura;
        this.costoUnitario = costoUnitario;
        this.idFornitore = idFornitore;
        this.pianoMagazzino = pianoMagazzino;
        this.scaffale = scaffale;
        this.sogliaMinima = sogliaMinima;
    }

    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }

    public String getCategoria() { return categoria; }
    public void setCategoria(String categoria) { this.categoria = categoria; }

    public double getQuantitaDisponibile() { return quantitaDisponibile; }
    public void setQuantitaDisponibile(double quantitaDisponibile) { this.quantitaDisponibile = quantitaDisponibile; }

    public String getUnitaMisura() { return unitaMisura; }
    public void setUnitaMisura(String unitaMisura) { this.unitaMisura = unitaMisura; }

    public double getCostoUnitario() { return costoUnitario; }
    public void setCostoUnitario(double costoUnitario) { this.costoUnitario = costoUnitario; }

    public String getIdFornitore() { return idFornitore; }
    public void setIdFornitore(String idFornitore) { this.idFornitore = idFornitore; }

    public int getPianoMagazzino() { return pianoMagazzino; }
    public void setPianoMagazzino(int pianoMagazzino) { this.pianoMagazzino = pianoMagazzino; }

    public String getScaffale() { return scaffale; }
    public void setScaffale(String scaffale) { this.scaffale = scaffale; }

    public double getSogliaMinima() { return sogliaMinima; }
    public void setSogliaMinima(double sogliaMinima) { this.sogliaMinima = sogliaMinima; }

    public boolean isSottoscorta() {
        return quantitaDisponibile <= sogliaMinima;
    }

    public double getValoreTotale() {
        return quantitaDisponibile * costoUnitario;
    }

    public String getDescrizionePiano() {
        switch (pianoMagazzino) {
            case 0: return "Piano 0 (Scorte pesanti / Ricevimento)";
            case 1: return "Piano 1 (Tessuti e Rotoli)";
            case 2: return "Piano 2 (Minuteria e Accessori)";
            default: return "Piano " + pianoMagazzino;
        }
    }

    @Override
    public String toString() {
        return id + " - " + nome + " [" + String.format("%.2f", quantitaDisponibile) + " " + unitaMisura + " | Scaffale: " + scaffale + "]";
    }
}
