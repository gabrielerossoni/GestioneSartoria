package gestionesartoria.service;

import gestionesartoria.model.Cliente;
import gestionesartoria.model.Componente;
import gestionesartoria.model.Fornitore;
import gestionesartoria.model.Personale;
import gestionesartoria.model.Progetto;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.List;

public class FilePersistenceService {

    private static final String DEFAULT_SAVE_PATH = "data/archivio_sartoria_v2.dat";

    public static boolean salvaDati(GestioneSartoriaService service, String percorsoFile) {
        String path = (percorsoFile != null && !percorsoFile.trim().isEmpty()) ? percorsoFile : DEFAULT_SAVE_PATH;
        try {
            File file = new File(path);
            File parent = file.getParentFile();
            if (parent != null && !parent.exists()) {
                parent.mkdirs();
            }

            try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(file))) {
                oos.writeObject(service.getFornitori());
                oos.writeObject(service.getClienti());
                oos.writeObject(service.getPersonale());
                oos.writeObject(service.getComponenti());
                oos.writeObject(service.getProgetti());
            }
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    @SuppressWarnings("unchecked")
    public static boolean caricaDati(GestioneSartoriaService service, String percorsoFile) {
        String path = (percorsoFile != null && !percorsoFile.trim().isEmpty()) ? percorsoFile : DEFAULT_SAVE_PATH;
        File file = new File(path);
        if (!file.exists()) {
            return false;
        }

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file))) {
            List<Fornitore> fList = (List<Fornitore>) ois.readObject();
            List<Cliente> cList = (List<Cliente>) ois.readObject();
            List<Personale> pList = (List<Personale>) ois.readObject();
            List<Componente> compList = (List<Componente>) ois.readObject();
            List<Progetto> prjList = (List<Progetto>) ois.readObject();

            service.getFornitori().clear();
            service.getFornitori().addAll(fList);

            service.getClienti().clear();
            service.getClienti().addAll(cList);

            service.getPersonale().clear();
            service.getPersonale().addAll(pList);

            service.sostituisciComponenti(compList);

            service.getProgetti().clear();
            service.getProgetti().addAll(prjList);

            return true;
        } catch (IOException | ClassNotFoundException | ClassCastException e) {
            return false;
        }
    }
}
