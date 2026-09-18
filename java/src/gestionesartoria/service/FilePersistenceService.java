package gestionesartoria.service;

import gestionesartoria.model.*;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
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
            e.printStackTrace();
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
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
}
