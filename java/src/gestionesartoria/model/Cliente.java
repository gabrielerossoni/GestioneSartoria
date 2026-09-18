package gestionesartoria.model;

import java.io.Serializable;

public class Cliente implements Serializable {
    private static final long serialVersionUID = 1L;

    private String id;
    private String codiceFiscale;
    private String nome;
    private String cognome;
    private String telefono;
    private String email;
    private String noteMisure;

    public Cliente() {
    }

    public Cliente(String id, String codiceFiscale, String nome, String cognome, String telefono, String email, String noteMisure) {
        this.id = id;
        this.codiceFiscale = codiceFiscale;
        this.nome = nome;
        this.cognome = cognome;
        this.telefono = telefono;
        this.email = email;
        this.noteMisure = noteMisure;
    }

    public String getId() { return id; }
    public void setId(String id) { this.id = id; }

    public String getCodiceFiscale() { return codiceFiscale; }
    public void setCodiceFiscale(String codiceFiscale) { this.codiceFiscale = codiceFiscale; }

    public String getNome() { return nome; }
    public void setNome(String nome) { this.nome = nome; }

    public String getCognome() { return cognome; }
    public void setCognome(String cognome) { this.cognome = cognome; }

    public String getTelefono() { return telefono; }
    public void setTelefono(String telefono) { this.telefono = telefono; }

    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }

    public String getNoteMisure() { return noteMisure; }
    public void setNoteMisure(String noteMisure) { this.noteMisure = noteMisure; }

    public String getNomeCompleto() {
        return nome + " " + cognome;
    }

    @Override
    public String toString() {
        return id + " - " + getNomeCompleto() + " (" + codiceFiscale + ")";
    }
}
