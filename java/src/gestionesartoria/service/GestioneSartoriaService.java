package gestionesartoria.service;

import gestionesartoria.model.Cliente;
import gestionesartoria.model.Componente;
import gestionesartoria.model.ComponenteUsato;
import gestionesartoria.model.Fornitore;
import gestionesartoria.model.Magazzino;
import gestionesartoria.model.Personale;
import gestionesartoria.model.Progetto;
import gestionesartoria.model.StatoProgetto;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

public class GestioneSartoriaService {

    private final List<Fornitore> fornitori = new ArrayList<>();
    private final List<Cliente> clienti = new ArrayList<>();
    private final List<Personale> personale = new ArrayList<>();
    private final List<Progetto> progetti = new ArrayList<>();

    public GestioneSartoriaService() {
    }

    // --- ID GENERATOR ---
    public String generaProssimoIdFornitore() {
        int max = 0;
        for (Fornitore f : fornitori) {
            try {
                if (f.getId() != null && f.getId().startsWith("FOR")) {
                    int val = Integer.parseInt(f.getId().substring(3));
                    if (val > max) max = val;
                }
            } catch (NumberFormatException ignored) {
            }
        }
        return String.format("FOR%04d", max + 1);
    }

    public String generaProssimoIdCliente() {
        int max = 0;
        for (Cliente c : clienti) {
            try {
                if (c.getId() != null && c.getId().startsWith("CLI")) {
                    int val = Integer.parseInt(c.getId().substring(3));
                    if (val > max) max = val;
                }
            } catch (NumberFormatException ignored) {
            }
        }
        return String.format("CLI%04d", max + 1);
    }

    public String generaProssimoIdPersonale() {
        int max = 0;
        for (Personale p : personale) {
            try {
                if (p.getId() != null && p.getId().startsWith("DIP")) {
                    int val = Integer.parseInt(p.getId().substring(3));
                    if (val > max) max = val;
                }
            } catch (NumberFormatException ignored) {
            }
        }
        return String.format("DIP%04d", max + 1);
    }

    public String generaProssimoIdComponente() {
        int max = 0;
        for (Componente c : magazzino.getTuttiIComponenti()) {
            try {
                if (c.getId() != null && c.getId().startsWith("CMP")) {
                    int val = Integer.parseInt(c.getId().substring(3));
                    if (val > max) max = val;
                }
            } catch (NumberFormatException ignored) {
            }
        }
        return String.format("CMP%04d", max + 1);
    }

    public String generaProssimoIdProgetto() {
        int max = 0;
        for (Progetto p : progetti) {
            try {
                if (p.getId() != null && p.getId().startsWith("PRJ")) {
                    int val = Integer.parseInt(p.getId().substring(3));
                    if (val > max) max = val;
                }
            } catch (NumberFormatException ignored) {
            }
        }
        return String.format("PRJ%04d", max + 1);
    }

    // --- FORNITORI ---
    public List<Fornitore> getFornitori() { return fornitori; }

    public void aggiungiFornitore(Fornitore f) {
        if (f == null) return;
        if (f.getId() == null || f.getId().trim().isEmpty()) {
            f.setId(generaProssimoIdFornitore());
        }
        fornitori.add(f);
    }

    public boolean eliminaFornitore(String id) {
        return id != null && fornitori.removeIf(f -> f.getId() != null && f.getId().equalsIgnoreCase(id));
    }

    public Optional<Fornitore> cercaFornitorePerId(String id) {
        if (id == null || id.trim().isEmpty()) return Optional.empty();
        return fornitori.stream().filter(f -> f.getId() != null && f.getId().equalsIgnoreCase(id)).findFirst();
    }

    public List<Fornitore> filtraFornitori(String testo) {
        if (testo == null || testo.trim().isEmpty()) return new ArrayList<>(fornitori);
        String q = testo.trim().toLowerCase();
        return fornitori.stream()
            .filter(f -> (f.getId() != null && f.getId().toLowerCase().contains(q))
                || (f.getNome() != null && f.getNome().toLowerCase().contains(q))
                        || (f.getCognome() != null && f.getCognome().toLowerCase().contains(q))
                        || (f.getPartitaIva() != null && f.getPartitaIva().toLowerCase().contains(q)))
                .collect(Collectors.toList());
    }

    // --- CLIENTI ---
    public List<Cliente> getClienti() { return clienti; }

    public void aggiungiCliente(Cliente c) {
        if (c == null) return;
        if (c.getId() == null || c.getId().trim().isEmpty()) {
            c.setId(generaProssimoIdCliente());
        }
        clienti.add(c);
    }

    public boolean eliminaCliente(String id) {
        return id != null && clienti.removeIf(c -> c.getId() != null && c.getId().equalsIgnoreCase(id));
    }

    public Optional<Cliente> cercaClientePerId(String id) {
        if (id == null || id.trim().isEmpty()) return Optional.empty();
        return clienti.stream().filter(c -> c.getId() != null && c.getId().equalsIgnoreCase(id)).findFirst();
    }

    public List<Cliente> filtraClienti(String testo) {
        if (testo == null || testo.trim().isEmpty()) return new ArrayList<>(clienti);
        String q = testo.trim().toLowerCase();
        return clienti.stream()
            .filter(c -> (c.getId() != null && c.getId().toLowerCase().contains(q))
                || (c.getNome() != null && c.getNome().toLowerCase().contains(q))
                || (c.getCognome() != null && c.getCognome().toLowerCase().contains(q))
                        || (c.getCodiceFiscale() != null && c.getCodiceFiscale().toLowerCase().contains(q)))
                .collect(Collectors.toList());
    }

    // --- PERSONALE ---
    public List<Personale> getPersonale() { return personale; }

    public void aggiungiPersonale(Personale p) {
        if (p == null) return;
        if (p.getId() == null || p.getId().trim().isEmpty()) {
            p.setId(generaProssimoIdPersonale());
        }
        personale.add(p);
    }

    public boolean eliminaPersonale(String id) {
        return id != null && personale.removeIf(p -> p.getId() != null && p.getId().equalsIgnoreCase(id));
    }

    public Optional<Personale> cercaPersonalePerId(String id) {
        if (id == null || id.trim().isEmpty()) return Optional.empty();
        return personale.stream().filter(p -> p.getId() != null && p.getId().equalsIgnoreCase(id)).findFirst();
    }

    public List<Personale> filtraPersonalePerContratto(String contratto) {
        if (contratto == null || contratto.equalsIgnoreCase("TUTTI") || contratto.trim().isEmpty()) {
            return new ArrayList<>(personale);
        }
        return personale.stream()
                .filter(p -> p.getTipoContratto() != null && p.getTipoContratto().equalsIgnoreCase(contratto))
                .collect(Collectors.toList());
    }

    // --- COMPONENTI & MAGAZZINO A 3 PIANI ---
    private final Magazzino magazzino = new Magazzino("Laboratorio Centrale Sartoria", "Via Porta Venezia, Milano");

    public Magazzino getMagazzino() {
        return magazzino;
    }

    public List<Componente> getComponenti() {
        return magazzino.getTuttiIComponenti();
    }

    public void aggiungiComponente(Componente c) {
        if (c == null) return;
        if (c.getId() == null || c.getId().trim().isEmpty()) {
            c.setId(generaProssimoIdComponente());
        }
        magazzino.aggiungiComponente(c);
    }

    public boolean eliminaComponente(String id) {
        return magazzino.eliminaComponente(id);
    }

    public Optional<Componente> cercaComponentePerId(String id) {
        return magazzino.cercaComponentePerId(id);
    }

    public List<Componente> getComponentiPerPiano(int piano) {
        return magazzino.getComponentiPerPiano(piano);
    }

    public List<Componente> getComponentiSottoscorta() {
        return magazzino.getComponentiSottoscorta();
    }

    public double getValoreEconomicoTotaleMagazzino() {
        return magazzino.getValoreEconomicoTotale();
    }

    public double getValoreEconomicoPiano(int piano) {
        return magazzino.getValoreEconomicoPiano(piano);
    }

    // --- PROGETTI & DISTINTA BASE ---
    public List<Progetto> getProgetti() { return progetti; }

    public void aggiungiProgetto(Progetto p) {
        if (p == null) return;
        if (p.getId() == null || p.getId().trim().isEmpty()) {
            p.setId(generaProssimoIdProgetto());
        }
        progetti.add(p);
    }

    public boolean eliminaProgetto(String id) {
        return id != null && progetti.removeIf(p -> p.getId() != null && p.getId().equalsIgnoreCase(id));
    }

    public Optional<Progetto> cercaProgettoPerId(String id) {
        if (id == null || id.trim().isEmpty()) return Optional.empty();
        return progetti.stream()
                .filter(p -> p.getId() != null && p.getId().equalsIgnoreCase(id))
                .findFirst();
    }

    public List<Progetto> getProgettiPerStato(StatoProgetto stato) {
        return progetti.stream()
                .filter(p -> p.getStatoAvanzamento() == stato)
                .collect(Collectors.toList());
    }

    /**
     * Preleva componenti dal magazzino e li assegna al progetto (decrementando la giacenza).
     */
    public boolean assegnaMaterialeAProgetto(String idProgetto, String idComponente, double quantita) {
        Optional<Progetto> prjOpt = cercaProgettoPerId(idProgetto);
        Optional<Componente> cmpOpt = cercaComponentePerId(idComponente);

        if (prjOpt.isEmpty() || cmpOpt.isEmpty() || quantita <= 0) {
            return false;
        }

        Progetto p = prjOpt.get();
        Componente c = cmpOpt.get();

        if (quantita > c.getQuantitaDisponibile()) {
            return false;
        }

        c.setQuantitaDisponibile(c.getQuantitaDisponibile() - quantita);

        ComponenteUsato cu = new ComponenteUsato(
                c.getId(),
                c.getNome(),
            quantita,
                c.getUnitaMisura(),
                c.getCostoUnitario()
        );
        p.addComponenteUsato(cu);
        return true;
    }

    /** Restituisce al magazzino un materiale precedentemente assegnato a una commessa. */
    public boolean restituisciMaterialeDaProgetto(String idProgetto, String idComponente, double quantita) {
        Optional<Progetto> progettoTrovato = cercaProgettoPerId(idProgetto);
        Optional<Componente> componenteTrovato = cercaComponentePerId(idComponente);

        if (progettoTrovato.isEmpty() || componenteTrovato.isEmpty() || quantita <= 0) {
            return false;
        }

        Progetto progetto = progettoTrovato.get();
        Componente componente = componenteTrovato.get();

        for (int i = 0; i < progetto.getComponentiUsati().size(); i++) {
            ComponenteUsato componenteUsato = progetto.getComponentiUsati().get(i);
            if (idComponente.equalsIgnoreCase(componenteUsato.getIdComponente())
                    && quantita <= componenteUsato.getQuantitaUsata()) {
                componente.setQuantitaDisponibile(componente.getQuantitaDisponibile() + quantita);
                double quantitaRestante = componenteUsato.getQuantitaUsata() - quantita;

                if (quantitaRestante == 0) {
                    progetto.getComponentiUsati().remove(i);
                } else {
                    componenteUsato.setQuantitaUsata(quantitaRestante);
                }
                return true;
            }
        }
        return false;
    }

    /**
     * Sposta un componente a un altro piano/scaffale
     */
    public boolean spostaComponentePiano(String idComponente, int nuovoPiano, String nuovoScaffale) {
        return magazzino.spostaComponente(idComponente, nuovoPiano, nuovoScaffale);
    }

    public void sostituisciComponenti(List<Componente> componenti) {
        magazzino.sostituisciComponenti(componenti);
    }

}
