package gestionesartoria.model;

public enum StatoProgetto {
    IN_ATTESA("IN ATTESA"),
    IN_LAVORAZIONE("IN LAVORAZIONE"),
    PRIMA_PROVA("PRIMA PROVA"),
    COMPLETATO("COMPLETATO"),
    CONSEGNATO("CONSEGNATO");

    private final String label;

    StatoProgetto(String label) {
        this.label = label;
    }

    public String getLabel() {
        return label;
    }

    public static StatoProgetto fromString(String text) {
        if (text == null) return IN_ATTESA;
        for (StatoProgetto s : StatoProgetto.values()) {
            if (s.label.equalsIgnoreCase(text.trim()) || s.name().equalsIgnoreCase(text.trim())) {
                return s;
            }
        }
        return IN_ATTESA;
    }

    @Override
    public String toString() {
        return label;
    }
}
