package gestionesartoria.gui.dnd;

import gestionesartoria.model.Componente;
import gestionesartoria.model.ComponenteUsato;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;

public class ComponenteTransferable implements Transferable {

    public static final DataFlavor COMPONENTE_FLAVOR = new DataFlavor(Componente.class, "Componente Magazzino");
    public static final DataFlavor COMPONENTE_USATO_FLAVOR = new DataFlavor(ComponenteUsato.class, "Materiale assegnato");

    private final Componente componente;
    private final ComponenteUsato componenteUsato;

    public ComponenteTransferable(Componente componente) {
        this.componente = componente;
        this.componenteUsato = null;
    }

    public ComponenteTransferable(ComponenteUsato componenteUsato) {
        this.componente = null;
        this.componenteUsato = componenteUsato;
    }

    @Override
    public DataFlavor[] getTransferDataFlavors() {
        if (componenteUsato != null) {
            return new DataFlavor[] { COMPONENTE_USATO_FLAVOR };
        }
        return new DataFlavor[] { COMPONENTE_FLAVOR, DataFlavor.stringFlavor };
    }

    @Override
    public boolean isDataFlavorSupported(DataFlavor flavor) {
        return COMPONENTE_FLAVOR.equals(flavor)
            || COMPONENTE_USATO_FLAVOR.equals(flavor)
            || DataFlavor.stringFlavor.equals(flavor);
    }

    @Override
    public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
        if (COMPONENTE_FLAVOR.equals(flavor)) {
            return componente;
        } else if (COMPONENTE_USATO_FLAVOR.equals(flavor)) {
            return componenteUsato;
        } else if (DataFlavor.stringFlavor.equals(flavor)) {
            return componente.getId() + ";" + componente.getNome() + ";" + componente.getQuantitaDisponibile() + ";" + componente.getUnitaMisura();
        }
        throw new UnsupportedFlavorException(flavor);
    }
}
