package gestionesartoria.gui.dnd;

import gestionesartoria.model.Componente;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;

public class ComponenteTransferable implements Transferable {

    public static final DataFlavor COMPONENTE_FLAVOR = new DataFlavor(Componente.class, "Componente Magazzino");

    private final Componente componente;

    public ComponenteTransferable(Componente componente) {
        this.componente = componente;
    }

    @Override
    public DataFlavor[] getTransferDataFlavors() {
        return new DataFlavor[] { COMPONENTE_FLAVOR, DataFlavor.stringFlavor };
    }

    @Override
    public boolean isDataFlavorSupported(DataFlavor flavor) {
        return COMPONENTE_FLAVOR.equals(flavor) || DataFlavor.stringFlavor.equals(flavor);
    }

    @Override
    public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
        if (COMPONENTE_FLAVOR.equals(flavor)) {
            return componente;
        } else if (DataFlavor.stringFlavor.equals(flavor)) {
            return componente.getId() + ";" + componente.getNome() + ";" + componente.getQuantitaDisponibile() + ";" + componente.getUnitaMisura();
        }
        throw new UnsupportedFlavorException(flavor);
    }
}
