package gestionesartoria.model;

import java.io.Serializable;

public class Fornitore implements Serializable {
    private static final long serialVersionUID = 1L;

    private String id;
    private String nome;
    private String cognome;
    private String partitaIva;
    private String telefono;
    private String email;
    private String indirizzo;

    public Fornitore() {
    }

    public Fornitore(String id, String nome, String cognome, String partitaIva, String telefono, String email, String indirizzo) {
        this.id = id;
        this.nome = nome;
        this.cognome = cognome;
        this.partitaIva = partitaIva;
        this.telefono = telefono;
        this.email = email;
        this.indirizzo = indirizzo;
    }

    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }

    public String getCognome() { return cognome; }
    public void setCognome(String cognome) { this.cognome = cognome; }

    public String getPartitaIva() { return partitaIva; }
    public void setPartitaIva(String partitaIva) { this.partitaIva = partitaIva; }

    public String getTelefono() { return telefono; }
    public void setTelefono(String telefono) { this.telefono = telefono; }

    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }

    public String getIndirizzo() { return indirizzo; }
    public void setIndirizzo(String indirizzo) { this.indirizzo = indirizzo; }

    public String getDenominazioneCompleta() {
        if (cognome != null && !cognome.trim().isEmpty()) {
            return nome + " " + cognome;
        }
        return nome;
    }

    @Override
    public String toString() {
        return id + " - " + getDenominazioneCompleta() + " (P.IVA: " + partitaIva + ")";
    }
}
