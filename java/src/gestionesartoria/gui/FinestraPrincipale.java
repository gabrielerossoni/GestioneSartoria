package gestionesartoria.gui;

import gestionesartoria.gui.dnd.ComponenteTransferable;
import gestionesartoria.model.*;
import gestionesartoria.service.FilePersistenceService;
import gestionesartoria.service.GestioneSartoriaService;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetAdapter;
import java.awt.dnd.DropTargetDropEvent;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.List;

/**
 * Finestra principale NetBeans GUI Form.
 * Supporta la vista Design di NetBeans tramite FinestraPrincipale.form
 */
@SuppressWarnings({"serial", "this-escape"})
public class FinestraPrincipale extends javax.swing.JFrame implements java.awt.event.ActionListener {

    private static final long serialVersionUID = 1L;
    private static final Color COLONNA_HEADER = new Color(15, 23, 42);
    private static final Color COLONNA_CHIARA = new Color(244, 247, 250);
    private static final Color COLONNA_ACCENT = new Color(37, 99, 235);
    private static final Color COLONNA_ACCENT_DARK = new Color(29, 78, 216);
    private static final Color COLONNA_BOTTONE_SECONDARIO = new Color(108, 117, 125);
    private static final Color COLONNA_BORDO = new Color(209, 213, 219);
    private static final Font FONT_TITOLO = new Font("Segoe UI", Font.BOLD, 20);
    private static final Font FONT_TESTO = new Font("Segoe UI", Font.PLAIN, 12);
    private static final Font FONT_TESTO_BOLD = new Font("Segoe UI", Font.BOLD, 12);

    private final GestioneSartoriaService service = new GestioneSartoriaService();

    // Componenti UI interni alle tab
    // --- TAB DASHBOARD ---
    private JLabel lblKpiCommesse;
    private JLabel lblKpiMagazzino;
    private JLabel lblKpiClienti;
    private JLabel lblKpiPersonale;
    private JTable tableAlertSottoscorta;
    private DefaultTableModel modelAlertSottoscorta;

    // --- TAB PROGETTI (DnD) ---
    private JTable tableProgetti;
    private DefaultTableModel modelProgetti;
    private JTable tableDistintaProgetto;
    private DefaultTableModel modelDistintaProgetto;
    private JLabel lblDettaglioProgetto;
    private JList<Componente> listCatalogoMaterialiPerDnD;
    private DefaultListModel<Componente> modelCatalogoMateriali;
    private JComboBox<StatoProgetto> comboNuovoStato;

    // --- TAB MAGAZZINO A 3 PIANI ---
    private JTable tableMagazzinoPiani;
    private DefaultTableModel modelMagazzinoPiani;
    private JComboBox<String> comboFiltroPiano;
    private JLabel lblValorePianoCorrente;
    private JButton btnNuovoComp;
    private JButton btnModificaQta;
    private JButton btnEliminaComp;

    // --- TAB CLIENTI ---
    private JTable tableClienti;
    private DefaultTableModel modelClienti;
    private JTextField txtCercaCliente;
    private JButton btnCercaCliente;
    private JButton btnResetCliente;
    private JButton btnNuovoCliente;
    private JButton btnEliminaCliente;

    // --- TAB PERSONALE ---
    private JTable tablePersonale;
    private DefaultTableModel modelPersonale;
    private JComboBox<String> comboFiltroContratto;
    private JButton btnNuovoDip;
    private JButton btnEliminaDip;

    // --- TAB FORNITORI ---
    private JTable tableFornitori;
    private DefaultTableModel modelFornitori;
    private JTextField txtCercaFornitore;
    private JButton btnCercaFornitore;
    private JButton btnResetFornitore;
    private JButton btnNuovoFor;
    private JButton btnEliminaFor;

    // --- TAB PROGETTI EXTRA ---
    private JButton btnNuovoProgetto;
    private JButton btnEliminaProgetto;
    private JButton btnAggiornaStato;

    /**
     * Creates new form FinestraPrincipale
     */
    public FinestraPrincipale() {
        initComponents();
        impostaStileUi();
        costruisciVisteAziendali();
        inizializzaDati();
        aggiornaTutteLeTabelle();
    }

    private void impostaStileUi() {
        panelHeader.setBackground(COLONNA_HEADER);
        panelHeader.setBorder(BorderFactory.createEmptyBorder(10, 0, 10, 0));
        panelStatusBar.setBackground(COLONNA_CHIARA);
        panelStatusBar.setBorder(BorderFactory.createMatteBorder(1, 0, 0, 0, COLONNA_BORDO));

        lblTitoloApp.setFont(FONT_TITOLO);
        lblTitoloApp.setForeground(Color.WHITE);
        lblSottotitolo.setFont(FONT_TESTO);
        lblSottotitolo.setForeground(new Color(204, 204, 204));

        impostaStileBottone(btnSalvaDati, COLONNA_ACCENT, COLONNA_ACCENT_DARK, Color.WHITE);
        impostaStileBottone(btnRicarica, COLONNA_BOTTONE_SECONDARIO, new Color(86, 94, 102), Color.WHITE);

        tabbedPanePrincipale.setBackground(COLONNA_CHIARA);
        tabbedPanePrincipale.setForeground(new Color(23, 35, 45));
        tabbedPanePrincipale.setFont(FONT_TESTO_BOLD);
    }

    private void impostaStileBottone(JButton bottone, Color base, Color hover, Color testo) {
        if (bottone == null) {
            return;
        }

        bottone.setFont(FONT_TESTO_BOLD);
        bottone.setBackground(base);
        bottone.setForeground(testo);
        bottone.setFocusPainted(false);
        bottone.setOpaque(true);
        bottone.setBorderPainted(true);
        bottone.setCursor(new Cursor(Cursor.HAND_CURSOR));
        bottone.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(base.darker(), 1, true),
                BorderFactory.createEmptyBorder(8, 14, 8, 14)
        ));

        bottone.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mouseEntered(java.awt.event.MouseEvent e) {
                bottone.setBackground(hover);
            }

            @Override
            public void mouseExited(java.awt.event.MouseEvent e) {
                bottone.setBackground(base);
            }
        });
    }

    @Override
    public void actionPerformed(java.awt.event.ActionEvent e) {
        Object source = e.getSource();

        if (source == btnSalvaDati) {
            salvaDatiSuFile();
            return;
        }

        if (source == btnRicarica) {
            ricaricaDati();
            return;
        }

        if (source == btnNuovoProgetto) {
            dialogNuovoProgetto();
            return;
        }

        if (source == btnEliminaProgetto) {
            eliminaProgettoSelezionato();
            return;
        }

        if (source == btnAggiornaStato) {
            aggiornaStatoProgettoSelezionato();
            return;
        }

        if (source == btnNuovoComp) {
            dialogNuovoComponente();
            return;
        }

        if (source == btnModificaQta) {
            modificaComponenteSelezionato();
            return;
        }

        if (source == btnEliminaComp) {
            eliminaComponenteSelezionato();
            return;
        }

        if (source == btnNuovoCliente) {
            dialogNuovoCliente();
            return;
        }

        if (source == btnEliminaCliente) {
            eliminaClienteSelezionato();
            return;
        }

        if (source == btnCercaCliente || source == btnResetCliente || source == txtCercaCliente) {
            if (source == btnResetCliente && txtCercaCliente != null) {
                txtCercaCliente.setText("");
            }
            aggiornaTabellaClienti();
            return;
        }

        if (source == btnNuovoDip) {
            dialogNuovoPersonale();
            return;
        }

        if (source == btnEliminaDip) {
            eliminaPersonaleSelezionato();
            return;
        }

        if (source == comboFiltroContratto) {
            aggiornaTabellaPersonale();
            return;
        }

        if (source == btnNuovoFor) {
            dialogNuovoFornitore();
            return;
        }

        if (source == btnEliminaFor) {
            eliminaFornitoreSelezionato();
            return;
        }

        if (source == btnCercaFornitore || source == btnResetFornitore || source == txtCercaFornitore) {
            if (source == btnResetFornitore && txtCercaFornitore != null) {
                txtCercaFornitore.setText("");
            }
            aggiornaTabellaFornitori();
            return;
        }

        if (source == comboFiltroPiano) {
            aggiornaTabellaMagazzino();
        }
    }

    private void inizializzaDati() {
        boolean caricato = FilePersistenceService.caricaDati(service, null);
        if (!caricato) {
            service.caricaDatiInizialiDemo();
            lblStatusInfo.setText("Caricati dati dimostrativi aziendali (nessun archivio precedente trovato).");
        } else {
            lblStatusInfo.setText("Archivio caricato con successo dal file locale.");
        }
    }

    /**
     * Struttura e arricchisce i pannelli delle Tab con le tabelle aziendali, 
     * layout ergonomico e meccanismi di Drag and Drop.
     */
    private void costruisciVisteAziendali() {
        // Setup listener pulsanti header
        btnSalvaDati.addActionListener(this);
        btnRicarica.addActionListener(this);

        costruisciTabDashboard();
        costruisciTabProgettiDnD();
        costruisciTabMagazzino3Piani();
        costruisciTabClienti();
        costruisciTabPersonale();
        costruisciTabFornitori();
    }

    // =========================================================================
    // 1. DASHBOARD & KPI
    // =========================================================================
    private void costruisciTabDashboard() {
        tabDashboard.setLayout(new BorderLayout(15, 15));
        tabDashboard.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 15));

        // Pannello KPI in alto
        JPanel pnlKpi = new JPanel(new GridLayout(1, 4, 15, 0));
        pnlKpi.setOpaque(false);

        lblKpiCommesse = creaKpiCard("COMMESSE ATTIVE", "0", new Color(41, 128, 185));
        lblKpiMagazzino = creaKpiCard("VALORE MAGAZZINO", "€ 0.00", new Color(39, 174, 96));
        lblKpiClienti = creaKpiCard("CLIENTI REGISTRATI", "0", new Color(142, 68, 173));
        lblKpiPersonale = creaKpiCard("ORGANICO ATTIVO", "0", new Color(211, 84, 0));

        pnlKpi.add(lblKpiCommesse.getParent());
        pnlKpi.add(lblKpiMagazzino.getParent());
        pnlKpi.add(lblKpiClienti.getParent());
        pnlKpi.add(lblKpiPersonale.getParent());

        tabDashboard.add(pnlKpi, BorderLayout.NORTH);

        // Sezione centrale con tabella allarmi sottoscorta
        JPanel pnlAlert = new JPanel(new BorderLayout(8, 8));
        pnlAlert.setBorder(BorderFactory.createTitledBorder(
                BorderFactory.createLineBorder(new Color(192, 57, 43), 2),
                " ⚠️ ALLARMI SOTTO-SCORTA MAGAZZINO (Articoli da Riordinare) ",
                0, 0, new Font("Segoe UI", Font.BOLD, 14), new Color(192, 57, 43)));

        String[] colsAlert = {"ID", "Nome Materiale", "Categoria", "Giacenza", "U.M.", "Soglia Minima", "Piano", "Scaffale", "Stato"};
        modelAlertSottoscorta = new DefaultTableModel(colsAlert, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tableAlertSottoscorta = new JTable(modelAlertSottoscorta);
        tableAlertSottoscorta.setRowHeight(26);
        tableAlertSottoscorta.getTableHeader().setFont(new Font("Segoe UI", Font.BOLD, 12));

        // Renderer rosso per evidenziare la sottoscorta
        tableAlertSottoscorta.setDefaultRenderer(Object.class, new DefaultTableCellRenderer() {
            @Override
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                if (!isSelected) {
                    c.setBackground(new Color(255, 235, 235));
                    c.setForeground(new Color(192, 57, 43));
                    setFont(getFont().deriveFont(Font.BOLD));
                }
                return c;
            }
        });

        pnlAlert.add(new JScrollPane(tableAlertSottoscorta), BorderLayout.CENTER);
        tabDashboard.add(pnlAlert, BorderLayout.CENTER);
    }

    private JLabel creaKpiCard(String titolo, String valore, Color accent) {
        JPanel card = new JPanel(new BorderLayout(5, 5));
        card.setBackground(Color.WHITE);
        card.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createMatteBorder(0, 5, 0, 0, accent),
                BorderFactory.createCompoundBorder(
                        BorderFactory.createLineBorder(new Color(220, 220, 220)),
                        BorderFactory.createEmptyBorder(12, 15, 12, 15)
                )
        ));

        JLabel lblTitolo = new JLabel(titolo);
        lblTitolo.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        lblTitolo.setForeground(Color.GRAY);

        JLabel lblValore = new JLabel(valore);
        lblValore.setFont(new Font("Segoe UI", Font.BOLD, 22));
        lblValore.setForeground(accent);

        card.add(lblTitolo, BorderLayout.NORTH);
        card.add(lblValore, BorderLayout.CENTER);

        return lblValore;
    }

    // =========================================================================
    // 2. COMMESSE & PROGETTI (CON DRAG AND DROP DIRETTO DAI MATERIALI)
    // =========================================================================
    private void costruisciTabProgettiDnD() {
        tabProgetti.setLayout(new BorderLayout(10, 10));
        tabProgetti.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // Toolbar azioni progetti
        JPanel pnlToolbar = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        btnNuovoProgetto = new JButton("➕ Nuova Commessa / Abito");
        btnNuovoProgetto.setFont(new Font("Segoe UI", Font.BOLD, 12));
        btnNuovoProgetto.addActionListener(this);

        btnEliminaProgetto = new JButton("🗑️ Elimina Selezionato");
        btnEliminaProgetto.addActionListener(this);

        pnlToolbar.add(btnNuovoProgetto);
        pnlToolbar.add(btnEliminaProgetto);
        pnlToolbar.add(new JSeparator(JSeparator.VERTICAL));

        pnlToolbar.add(new JLabel("Avanzamento Stato:"));
        comboNuovoStato = new JComboBox<>(StatoProgetto.values());
        btnAggiornaStato = new JButton("Aggiorna Stato");
        btnAggiornaStato.addActionListener(this);
        pnlToolbar.add(comboNuovoStato);
        pnlToolbar.add(btnAggiornaStato);

        tabProgetti.add(pnlToolbar, BorderLayout.NORTH);

        // Split centrale: Sinistra = Elenco Progetti, Destra = Distinta Base con Target Drag & Drop
        JSplitPane splitProgetti = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
        splitProgetti.setDividerLocation(620);
        splitProgetti.setResizeWeight(0.6);

        // --- SINISTRA: TABELLA PROGETTI ---
        JPanel pnlElencoProgetti = new JPanel(new BorderLayout(5, 5));
        pnlElencoProgetti.setBorder(BorderFactory.createTitledBorder(" Elenco Commesse Sartoriali "));

        String[] colsPrj = {"ID", "Capo", "Cliente", "Sarto Resp.", "Stato", "Inizio", "Consegna", "Prezzo (€)"};
        modelProgetti = new DefaultTableModel(colsPrj, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tableProgetti = new JTable(modelProgetti);
        tableProgetti.setRowHeight(24);
        tableProgetti.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        tableProgetti.getSelectionModel().addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) mostraDettagliProgettoSelezionato();
        });

        pnlElencoProgetti.add(new JScrollPane(tableProgetti), BorderLayout.CENTER);
        splitProgetti.setLeftComponent(pnlElencoProgetti);

        // --- DESTRA: DISTINTA BASE & DRAG AND DROP AREA ---
        JPanel pnlDestra = new JPanel(new BorderLayout(8, 8));

        // Area Distinta Base (DROP TARGET)
        JPanel pnlDistinta = new JPanel(new BorderLayout(5, 5));
        lblDettaglioProgetto = new JLabel("Seleziona una commessa per visualizzare o trascinare materiali");
        lblDettaglioProgetto.setFont(new Font("Segoe UI", Font.BOLD, 12));
        lblDettaglioProgetto.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        pnlDistinta.add(lblDettaglioProgetto, BorderLayout.NORTH);

        String[] colsDistinta = {"ID Comp.", "Nome Materiale", "Qta Usata", "U.M.", "Costo Tot (€)"};
        modelDistintaProgetto = new DefaultTableModel(colsDistinta, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tableDistintaProgetto = new JTable(modelDistintaProgetto);
        tableDistintaProgetto.setRowHeight(22);

        JScrollPane scrollDistinta = new JScrollPane(tableDistintaProgetto);
        pnlDistinta.add(scrollDistinta, BorderLayout.CENTER);

        // Box inferiore Catalogo Materiali (DRAG SOURCE)
        JPanel pnlCatalogoDnD = new JPanel(new BorderLayout(5, 5));
        pnlCatalogoDnD.setPreferredSize(new Dimension(300, 240));
        pnlCatalogoDnD.setBorder(BorderFactory.createTitledBorder(" ⬇️ Trascina Materiale nella distinta sopra (Drag & Drop) "));

        modelCatalogoMateriali = new DefaultListModel<>();
        listCatalogoMaterialiPerDnD = new JList<>(modelCatalogoMateriali);
        listCatalogoMaterialiPerDnD.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        listCatalogoMaterialiPerDnD.setDragEnabled(true);
        listCatalogoMaterialiPerDnD.setTransferHandler(new TransferHandler("selectedValue") {
            @Override
            protected Transferable createTransferable(JComponent c) {
                Componente sel = listCatalogoMaterialiPerDnD.getSelectedValue();
                if (sel != null) {
                    return new ComponenteTransferable(sel);
                }
                return null;
            }
            @Override
            public int getSourceActions(JComponent c) {
                return COPY;
            }
        });

        pnlCatalogoDnD.add(new JScrollPane(listCatalogoMaterialiPerDnD), BorderLayout.CENTER);

        // ABILITA DROP TARGET SULLA TABELLA DISTINTA
        new DropTarget(tableDistintaProgetto, DnDConstants.ACTION_COPY, new DropTargetAdapter() {
            @Override
            public void drop(DropTargetDropEvent dtde) {
                try {
                    int selPrjRow = tableProgetti.getSelectedRow();
                    if (selPrjRow < 0) {
                        JOptionPane.showMessageDialog(FinestraPrincipale.this,
                                "Seleziona prima una commessa a sinistra su cui assegnare il materiale!",
                                "Nessuna commessa selezionata", JOptionPane.WARNING_MESSAGE);
                        dtde.rejectDrop();
                        return;
                    }

                    if (dtde.isDataFlavorSupported(ComponenteTransferable.COMPONENTE_FLAVOR)) {
                        dtde.acceptDrop(DnDConstants.ACTION_COPY);
                        Componente comp = (Componente) dtde.getTransferable().getTransferData(ComponenteTransferable.COMPONENTE_FLAVOR);

                        String qtaStr = JOptionPane.showInputDialog(FinestraPrincipale.this,
                                "Inserisci la quantità da prelevare per " + comp.getNome() + "\n(Disponibili: " +
                                        String.format("%.2f", comp.getQuantitaDisponibile()) + " " + comp.getUnitaMisura() + "):",
                                "Preleva Materiale (Drag & Drop)", JOptionPane.QUESTION_MESSAGE);

                        if (qtaStr != null && !qtaStr.trim().isEmpty()) {
                            double qta = Double.parseDouble(qtaStr.replace(",", "."));
                            String prjId = (String) modelProgetti.getValueAt(selPrjRow, 0);

                            boolean ok = service.assegnaMaterialeAProgetto(prjId, comp.getId(), qta);
                            if (ok) {
                                JOptionPane.showMessageDialog(FinestraPrincipale.this,
                                        "Materiale assegnato con successo e giacenza di magazzino scaricata!",
                                        "Assegnazione Riuscita", JOptionPane.INFORMATION_MESSAGE);
                                aggiornaTutteLeTabelle();
                                mostraDettagliProgettoSelezionato();
                            } else {
                                JOptionPane.showMessageDialog(FinestraPrincipale.this,
                                        "Errore: quantità non valida o giacenza insufficiente.",
                                        "Errore", JOptionPane.ERROR_MESSAGE);
                            }
                        }
                        dtde.dropComplete(true);
                    } else {
                        dtde.rejectDrop();
                    }
                } catch (java.io.IOException | java.awt.datatransfer.UnsupportedFlavorException | NumberFormatException ex) {
                    dtde.rejectDrop();
                }
            }
        });

        pnlDestra.add(pnlDistinta, BorderLayout.CENTER);
        pnlDestra.add(pnlCatalogoDnD, BorderLayout.SOUTH);

        splitProgetti.setRightComponent(pnlDestra);
        tabProgetti.add(splitProgetti, BorderLayout.CENTER);
    }

    // =========================================================================
    // 3. MAGAZZINO A 3 PIANI (PIANO 0, PIANO 1, PIANO 2)
    // =========================================================================
    private void costruisciTabMagazzino3Piani() {
        tabMagazzino.setLayout(new BorderLayout(10, 10));
        tabMagazzino.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JPanel pnlFiltri = new JPanel(new FlowLayout(FlowLayout.LEFT, 15, 5));
        pnlFiltri.add(new JLabel("Visualizza Piano:"));

        comboFiltroPiano = new JComboBox<>(new String[]{
                "Tutti i Piani (Visione Globale)",
                "Piano 0: Ricevimento & Scorte Pesanti",
                "Piano 1: Tessuti Pregiati & Rotoli",
                "Piano 2: Minuteria, Filati & Accessori"
        });
        comboFiltroPiano.setFont(new Font("Segoe UI", Font.BOLD, 12));
        comboFiltroPiano.addActionListener(this);
        pnlFiltri.add(comboFiltroPiano);

        btnNuovoComp = new JButton("➕ Nuovo Componente");
        btnNuovoComp.addActionListener(this);
        pnlFiltri.add(btnNuovoComp);

        btnModificaQta = new JButton("✏️ Modifica Giacenza / Scaffale");
        btnModificaQta.addActionListener(this);
        pnlFiltri.add(btnModificaQta);

        btnEliminaComp = new JButton("🗑️ Elimina Componente");
        btnEliminaComp.addActionListener(this);
        pnlFiltri.add(btnEliminaComp);

        lblValorePianoCorrente = new JLabel("Valore Piano: € 0.00");
        lblValorePianoCorrente.setFont(new Font("Segoe UI", Font.BOLD, 13));
        lblValorePianoCorrente.setForeground(new Color(39, 174, 96));
        pnlFiltri.add(lblValorePianoCorrente);

        tabMagazzino.add(pnlFiltri, BorderLayout.NORTH);

        String[] colsMag = {"ID", "Nome Materiale", "Categoria", "Giacenza", "U.M.", "Costo (€)", "Valore (€)", "Piano", "Scaffale", "Soglia Min.", "Fornitore", "Alert"};
        modelMagazzinoPiani = new DefaultTableModel(colsMag, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tableMagazzinoPiani = new JTable(modelMagazzinoPiani);
        tableMagazzinoPiani.setRowHeight(24);
        tableMagazzinoPiani.getTableHeader().setFont(new Font("Segoe UI", Font.BOLD, 12));

        // Evidenziazione righe sottoscorta
        tableMagazzinoPiani.setDefaultRenderer(Object.class, new DefaultTableCellRenderer() {
            @Override
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
                if (!isSelected) {
                    Object alertVal = table.getValueAt(row, 11);
                    if ("SOTTOSCORTA".equals(alertVal)) {
                        c.setBackground(new Color(255, 230, 230));
                        c.setForeground(new Color(192, 57, 43));
                    } else {
                        c.setBackground(row % 2 == 0 ? Color.WHITE : new Color(248, 249, 250));
                        c.setForeground(Color.BLACK);
                    }
                }
                return c;
            }
        });

        tabMagazzino.add(new JScrollPane(tableMagazzinoPiani), BorderLayout.CENTER);
    }

    // =========================================================================
    // 4. CLIENTI & SCHEDE MISURE
    // =========================================================================
    private void costruisciTabClienti() {
        tabClienti.setLayout(new BorderLayout(10, 10));
        tabClienti.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JPanel pnlTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        btnNuovoCliente = new JButton("➕ Nuovo Cliente");
        btnNuovoCliente.addActionListener(this);

        btnEliminaCliente = new JButton("🗑️ Elimina");
        btnEliminaCliente.addActionListener(this);

        pnlTop.add(btnNuovoCliente);
        pnlTop.add(btnEliminaCliente);
        pnlTop.add(new JLabel("Cerca (CF o Cognome):"));
        txtCercaCliente = new JTextField(15);
        txtCercaCliente.addActionListener(this);
        btnCercaCliente = new JButton("Filtra");
        btnCercaCliente.addActionListener(this);
        btnResetCliente = new JButton("Reset");
        btnResetCliente.addActionListener(this);

        pnlTop.add(txtCercaCliente);
        pnlTop.add(btnCercaCliente);
        pnlTop.add(btnResetCliente);

        tabClienti.add(pnlTop, BorderLayout.NORTH);

        String[] colsCli = {"ID", "Codice Fiscale", "Cognome", "Nome", "Telefono", "Email", "Note Misure Sartoriali"};
        modelClienti = new DefaultTableModel(colsCli, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tableClienti = new JTable(modelClienti);
        tableClienti.setRowHeight(24);
        tabClienti.add(new JScrollPane(tableClienti), BorderLayout.CENTER);
    }

    // =========================================================================
    // 5. PERSONALE & FORME CONTRATTUALI
    // =========================================================================
    private void costruisciTabPersonale() {
        tabPersonale.setLayout(new BorderLayout(10, 10));
        tabPersonale.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JPanel pnlTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        btnNuovoDip = new JButton("➕ Nuovo Dipendente");
        btnNuovoDip.addActionListener(this);

        btnEliminaDip = new JButton("🗑️ Elimina");
        btnEliminaDip.addActionListener(this);

        pnlTop.add(btnNuovoDip);
        pnlTop.add(btnEliminaDip);

        pnlTop.add(new JLabel("Filtra per Contratto:"));
        comboFiltroContratto = new JComboBox<>(new String[]{
                "TUTTI", "Indeterminato", "Determinato", "Apprendistato", "Stage", "Part-Time"
        });
        comboFiltroContratto.addActionListener(this);
        pnlTop.add(comboFiltroContratto);

        tabPersonale.add(pnlTop, BorderLayout.NORTH);

        String[] colsDip = {"ID", "Cognome", "Nome", "Ruolo", "Tipo Contratto", "Stipendio (€)", "Data Assunzione", "Stato"};
        modelPersonale = new DefaultTableModel(colsDip, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tablePersonale = new JTable(modelPersonale);
        tablePersonale.setRowHeight(24);
        tabPersonale.add(new JScrollPane(tablePersonale), BorderLayout.CENTER);
    }

    // =========================================================================
    // 6. FORNITORI TESSILI & ACCESSORI
    // =========================================================================
    private void costruisciTabFornitori() {
        tabFornitori.setLayout(new BorderLayout(10, 10));
        tabFornitori.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JPanel pnlTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        btnNuovoFor = new JButton("➕ Nuovo Fornitore");
        btnNuovoFor.addActionListener(this);

        btnEliminaFor = new JButton("🗑️ Elimina");
        btnEliminaFor.addActionListener(this);

        pnlTop.add(btnNuovoFor);
        pnlTop.add(btnEliminaFor);

        pnlTop.add(new JLabel("Cerca (Ragione Sociale / P.IVA):"));
        txtCercaFornitore = new JTextField(15);
        txtCercaFornitore.addActionListener(this);
        btnCercaFornitore = new JButton("Filtra");
        btnCercaFornitore.addActionListener(this);
        btnResetFornitore = new JButton("Reset");
        btnResetFornitore.addActionListener(this);

        pnlTop.add(txtCercaFornitore);
        pnlTop.add(btnCercaFornitore);
        pnlTop.add(btnResetFornitore);

        tabFornitori.add(pnlTop, BorderLayout.NORTH);

        String[] colsFor = {"ID", "Ragione Sociale / Nome", "Cognome", "Partita IVA", "Telefono", "Email", "Sede Operativa"};
        modelFornitori = new DefaultTableModel(colsFor, 0) {
            @Override public boolean isCellEditable(int r, int c) { return false; }
        };
        tableFornitori = new JTable(modelFornitori);
        tableFornitori.setRowHeight(24);
        tabFornitori.add(new JScrollPane(tableFornitori), BorderLayout.CENTER);
    }

    // =========================================================================
    // AGGIORNAMENTO DATI & TABELLE
    // =========================================================================
    public void aggiornaTutteLeTabelle() {
        aggiornaDashboard();
        aggiornaTabellaProgetti();
        aggiornaTabellaMagazzino();
        aggiornaTabellaClienti();
        aggiornaTabellaPersonale();
        aggiornaTabellaFornitori();
        aggiornaCatalogoMaterialiDnD();

        double valTot = service.getValoreEconomicoTotaleMagazzino();
        lblValoreMagazzinoStatus.setText(String.format("Valore Magazzino: € %.2f", valTot));
    }

    private void aggiornaDashboard() {
        lblKpiCommesse.setText(String.valueOf(service.getProgetti().size()));
        lblKpiMagazzino.setText(String.format("€ %.2f", service.getValoreEconomicoTotaleMagazzino()));
        lblKpiClienti.setText(String.valueOf(service.getClienti().size()));
        lblKpiPersonale.setText(String.valueOf(service.getPersonale().size()));

        modelAlertSottoscorta.setRowCount(0);
        for (Componente c : service.getComponentiSottoscorta()) {
            modelAlertSottoscorta.addRow(new Object[]{
                    c.getId(), c.getNome(), c.getCategoria(),
                    String.format("%.2f", c.getQuantitaDisponibile()), c.getUnitaMisura(),
                    String.format("%.2f", c.getSogliaMinima()), "Piano " + c.getPianoMagazzino(),
                    c.getScaffale(), "SOTTOSCORTA!"
            });
        }
    }

    private void aggiornaTabellaProgetti() {
        modelProgetti.setRowCount(0);
        for (Progetto p : service.getProgetti()) {
            modelProgetti.addRow(new Object[]{
                    p.getId(), p.getTipoCapo(), p.getIdCliente(),
                    p.getIdSartoAssegnato(), p.getStatoAvanzamento().getLabel(),
                    p.getDataInizioFormattata(), p.getDataConsegnaPrevistaFormattata(),
                    String.format("%.2f", p.getPrezzoFinale())
            });
        }
    }

    private void mostraDettagliProgettoSelezionato() {
        int sel = tableProgetti.getSelectedRow();
        modelDistintaProgetto.setRowCount(0);
        if (sel >= 0) {
            String idPrj = (String) modelProgetti.getValueAt(sel, 0);
            service.cercaProgettoPerId(idPrj).ifPresent(p -> {
                lblDettaglioProgetto.setText("Distinta Base per: " + p.getId() + " - " + p.getTipoCapo() +
                        " | Costo Materiali: € " + String.format("%.2f", p.calcolaCostoMateriali()) +
                        " | Preventivo Cliente: € " + String.format("%.2f", p.getPrezzoFinale()));
                comboNuovoStato.setSelectedItem(p.getStatoAvanzamento());

                for (ComponenteUsato cu : p.getComponentiUsati()) {
                    modelDistintaProgetto.addRow(new Object[]{
                            cu.getIdComponente(), cu.getNomeComponente(),
                            String.format("%.2f", cu.getQuantitaUsata()), cu.getUnitaMisura(),
                            String.format("%.2f", cu.getCostoTotale())
                    });
                }
            });
        } else {
            lblDettaglioProgetto.setText("Seleziona una commessa per visualizzare o trascinare materiali");
        }
    }

    private void aggiornaCatalogoMaterialiDnD() {
        modelCatalogoMateriali.clear();
        for (Componente c : service.getComponenti()) {
            modelCatalogoMateriali.addElement(c);
        }
    }

    private void aggiornaTabellaMagazzino() {
        modelMagazzinoPiani.setRowCount(0);
        int idxPiano = comboFiltroPiano.getSelectedIndex(); // 0 = tutti, 1 = p0, 2 = p1, 3 = p2

        List<Componente> lista;
        if (idxPiano == 0) {
            lista = service.getComponenti();
            lblValorePianoCorrente.setText(String.format("Valore Totale Tutti i Piani: € %.2f", service.getValoreEconomicoTotaleMagazzino()));
        } else {
            int piano = idxPiano - 1;
            lista = service.getComponentiPerPiano(piano);
            lblValorePianoCorrente.setText(String.format("Valore Piano %d: € %.2f", piano, service.getValoreEconomicoPiano(piano)));
        }

        for (Componente c : lista) {
            modelMagazzinoPiani.addRow(new Object[]{
                    c.getId(), c.getNome(), c.getCategoria(),
                    String.format("%.2f", c.getQuantitaDisponibile()), c.getUnitaMisura(),
                    String.format("%.2f", c.getCostoUnitario()), String.format("%.2f", c.getValoreTotale()),
                    c.getPianoMagazzino(), c.getScaffale(), String.format("%.2f", c.getSogliaMinima()),
                    c.getIdFornitore(), c.isSottoscorta() ? "SOTTOSCORTA" : "OK"
            });
        }
    }

    private void aggiornaTabellaClienti() {
        modelClienti.setRowCount(0);
        String q = (txtCercaCliente != null) ? txtCercaCliente.getText() : "";
        for (Cliente c : service.filtraClienti(q)) {
            modelClienti.addRow(new Object[]{
                    c.getId(), c.getCodiceFiscale(), c.getCognome(), c.getNome(),
                    c.getTelefono(), c.getEmail(), c.getNoteMisure()
            });
        }
    }

    private void aggiornaTabellaPersonale() {
        modelPersonale.setRowCount(0);
        String f = (comboFiltroContratto != null) ? (String) comboFiltroContratto.getSelectedItem() : "TUTTI";
        for (Personale p : service.filtraPersonalePerContratto(f)) {
            modelPersonale.addRow(new Object[]{
                    p.getId(), p.getCognome(), p.getNome(), p.getRuolo(),
                    p.getTipoContratto(), String.format("%.2f", p.getRetribuzioneMensile()),
                    p.getDataAssunzioneFormattata(), p.getStato()
            });
        }
    }

    private void aggiornaTabellaFornitori() {
        modelFornitori.setRowCount(0);
        String q = (txtCercaFornitore != null) ? txtCercaFornitore.getText() : "";
        for (Fornitore f : service.filtraFornitori(q)) {
            modelFornitori.addRow(new Object[]{
                    f.getId(), f.getNome(), f.getCognome(), f.getPartitaIva(),
                    f.getTelefono(), f.getEmail(), f.getIndirizzo()
            });
        }
    }

    // =========================================================================
    // AZIONI CRUD DIALOGHI
    // =========================================================================
    private void dialogNuovoProgetto() {
        JTextField txtCapo = new JTextField();
        JComboBox<Cliente> cbClienti = new JComboBox<>(service.getClienti().toArray(new Cliente[0]));
        JComboBox<Personale> cbSarti = new JComboBox<>(service.getPersonale().toArray(new Personale[0]));
        JTextField txtPrezzo = new JTextField("500.00");
        JTextField txtGiorniConsegna = new JTextField("14");

        JPanel form = new JPanel(new GridLayout(5, 2, 8, 8));
        form.add(new JLabel("Tipo di Capo / Abito:")); form.add(txtCapo);
        form.add(new JLabel("Cliente Committente:")); form.add(cbClienti);
        form.add(new JLabel("Sarto Incaricato:")); form.add(cbSarti);
        form.add(new JLabel("Prezzo Finale Preventivo (€):")); form.add(txtPrezzo);
        form.add(new JLabel("Giorni previsti per consegna:")); form.add(txtGiorniConsegna);

        int res = JOptionPane.showConfirmDialog(this, form, "Crea Nuova Commessa Sartoriale", JOptionPane.OK_CANCEL_OPTION);
        if (res == JOptionPane.OK_OPTION) {
            try {
                Cliente c = (Cliente) cbClienti.getSelectedItem();
                Personale p = (Personale) cbSarti.getSelectedItem();
                double prezzo = Double.parseDouble(txtPrezzo.getText().replace(",", "."));
                int giorni = Integer.parseInt(txtGiorniConsegna.getText().trim());

                Progetto prj = new Progetto(
                        service.generaProssimoIdProgetto(),
                        (c != null) ? c.getId() : "NON_SPECIFICATO",
                        txtCapo.getText().trim(),
                        (p != null) ? p.getId() : "NON_ASSEGNATO",
                        StatoProgetto.IN_ATTESA,
                        LocalDate.now(),
                        LocalDate.now().plusDays(giorni),
                        prezzo
                );
                service.aggiungiProgetto(prj);
                aggiornaTutteLeTabelle();
                lblStatusInfo.setText("Commessa " + prj.getId() + " creata con successo.");
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "Dati non validi: " + ex.getMessage(), "Errore", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void aggiornaStatoProgettoSelezionato() {
        int sel = tableProgetti.getSelectedRow();
        if (sel >= 0) {
            String prjId = (String) modelProgetti.getValueAt(sel, 0);
            StatoProgetto nuovo = (StatoProgetto) comboNuovoStato.getSelectedItem();
            service.cercaProgettoPerId(prjId).ifPresent(p -> {
                p.setStatoAvanzamento(nuovo);
                aggiornaTabellaProgetti();
                lblStatusInfo.setText("Stato commessa " + p.getId() + " aggiornato a: " + nuovo.getLabel());
            });
        } else {
            JOptionPane.showMessageDialog(this, "Seleziona una commessa dall'elenco!", "Attenzione", JOptionPane.WARNING_MESSAGE);
        }
    }

    private void eliminaProgettoSelezionato() {
        int sel = tableProgetti.getSelectedRow();
        if (sel >= 0) {
            String id = (String) modelProgetti.getValueAt(sel, 0);
            int ok = JOptionPane.showConfirmDialog(this, "Confermi l'eliminazione della commessa " + id + "?", "Elimina", JOptionPane.YES_NO_OPTION);
            if (ok == JOptionPane.YES_OPTION) {
                service.eliminaProgetto(id);
                aggiornaTutteLeTabelle();
            }
        }
    }

    private void dialogNuovoComponente() {
        JTextField txtNome = new JTextField();
        JComboBox<String> cbCat = new JComboBox<>(new String[]{"TESSUTO", "MINUTERIA", "FODERA", "ACCESSORIO", "FILATO"});
        JTextField txtQta = new JTextField("10.0");
        JComboBox<String> cbUm = new JComboBox<>(new String[]{"m", "cm", "pezzi", "rocchetti"});
        JTextField txtCosto = new JTextField("15.00");
        JTextField txtSoglia = new JTextField("5.0");
        JComboBox<String> cbPiano = new JComboBox<>(new String[]{"Piano 0: Scorte Pesanti", "Piano 1: Tessuti & Rotoli", "Piano 2: Minuteria & Filati"});
        JTextField txtScaffale = new JTextField("A-01");
        JComboBox<Fornitore> cbFor = new JComboBox<>(service.getFornitori().toArray(new Fornitore[0]));

        JPanel form = new JPanel(new GridLayout(9, 2, 6, 6));
        form.add(new JLabel("Nome Materiale:")); form.add(txtNome);
        form.add(new JLabel("Categoria:")); form.add(cbCat);
        form.add(new JLabel("Quantità Iniziale:")); form.add(txtQta);
        form.add(new JLabel("Unità di Misura:")); form.add(cbUm);
        form.add(new JLabel("Costo Unitario (€):")); form.add(txtCosto);
        form.add(new JLabel("Soglia Minima Allarme:")); form.add(txtSoglia);
        form.add(new JLabel("Piano Magazzino:")); form.add(cbPiano);
        form.add(new JLabel("Codice Scaffale:")); form.add(txtScaffale);
        form.add(new JLabel("Fornitore di Origine:")); form.add(cbFor);

        int res = JOptionPane.showConfirmDialog(this, form, "Aggiungi Nuovo Materiale al Magazzino", JOptionPane.OK_CANCEL_OPTION);
        if (res == JOptionPane.OK_OPTION) {
            try {
                Fornitore f = (Fornitore) cbFor.getSelectedItem();
                Componente c = new Componente(
                        service.generaProssimoIdComponente(),
                        txtNome.getText().trim(),
                        (String) cbCat.getSelectedItem(),
                        Double.parseDouble(txtQta.getText().replace(",", ".")),
                        (String) cbUm.getSelectedItem(),
                        Double.parseDouble(txtCosto.getText().replace(",", ".")),
                        (f != null) ? f.getId() : "NON_SPECIFICATO",
                        cbPiano.getSelectedIndex(),
                        txtScaffale.getText().trim(),
                        Double.parseDouble(txtSoglia.getText().replace(",", "."))
                );
                service.aggiungiComponente(c);
                aggiornaTutteLeTabelle();
                lblStatusInfo.setText("Componente " + c.getId() + " inserito con successo.");
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "Dati errati: " + ex.getMessage(), "Errore", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void modificaComponenteSelezionato() {
        int sel = tableMagazzinoPiani.getSelectedRow();
        if (sel >= 0) {
            String id = (String) modelMagazzinoPiani.getValueAt(sel, 0);
            service.cercaComponentePerId(id).ifPresent(c -> {
                JTextField txtQta = new JTextField(String.valueOf(c.getQuantitaDisponibile()));
                JTextField txtScaffale = new JTextField(c.getScaffale());
                JComboBox<String> cbPiano = new JComboBox<>(new String[]{"Piano 0", "Piano 1", "Piano 2"});
                cbPiano.setSelectedIndex(c.getPianoMagazzino());

                JPanel form = new JPanel(new GridLayout(3, 2, 6, 6));
                form.add(new JLabel("Nuova Giacenza (" + c.getUnitaMisura() + "):")); form.add(txtQta);
                form.add(new JLabel("Piano Magazzino:")); form.add(cbPiano);
                form.add(new JLabel("Nuovo Scaffale:")); form.add(txtScaffale);

                int res = JOptionPane.showConfirmDialog(this, form, "Modifica " + c.getNome(), JOptionPane.OK_CANCEL_OPTION);
                if (res == JOptionPane.OK_OPTION) {
                    try {
                        c.setQuantitaDisponibile(Double.parseDouble(txtQta.getText().replace(",", ".")));
                        c.setPianoMagazzino(cbPiano.getSelectedIndex());
                        c.setScaffale(txtScaffale.getText().trim());
                        aggiornaTutteLeTabelle();
                    } catch (NumberFormatException ex) {
                        JOptionPane.showMessageDialog(this, "Valore non valido.", "Errore", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });
        }
    }

    private void eliminaComponenteSelezionato() {
        int sel = tableMagazzinoPiani.getSelectedRow();
        if (sel >= 0) {
            String id = (String) modelMagazzinoPiani.getValueAt(sel, 0);
            int ok = JOptionPane.showConfirmDialog(this, "Eliminare " + id + "?", "Conferma", JOptionPane.YES_NO_OPTION);
            if (ok == JOptionPane.YES_OPTION) {
                service.eliminaComponente(id);
                aggiornaTutteLeTabelle();
            }
        }
    }

    private void dialogNuovoCliente() {
        JTextField txtCf = new JTextField();
        JTextField txtNome = new JTextField();
        JTextField txtCognome = new JTextField();
        JTextField txtTel = new JTextField();
        JTextField txtMail = new JTextField();
        JTextField txtMisure = new JTextField("Spalle 44, Torace 98, Vita 82");

        JPanel form = new JPanel(new GridLayout(6, 2, 6, 6));
        form.add(new JLabel("Codice Fiscale:")); form.add(txtCf);
        form.add(new JLabel("Cognome:")); form.add(txtCognome);
        form.add(new JLabel("Nome:")); form.add(txtNome);
        form.add(new JLabel("Telefono:")); form.add(txtTel);
        form.add(new JLabel("Email:")); form.add(txtMail);
        form.add(new JLabel("Note Misure:")); form.add(txtMisure);

        int res = JOptionPane.showConfirmDialog(this, form, "Nuovo Cliente Committente", JOptionPane.OK_CANCEL_OPTION);
        if (res == JOptionPane.OK_OPTION) {
            Cliente c = new Cliente(service.generaProssimoIdCliente(), txtCf.getText().trim(),
                    txtNome.getText().trim(), txtCognome.getText().trim(),
                    txtTel.getText().trim(), txtMail.getText().trim(), txtMisure.getText().trim());
            service.aggiungiCliente(c);
            aggiornaTabellaClienti();
            aggiornaDashboard();
        }
    }

    private void eliminaClienteSelezionato() {
        int sel = tableClienti.getSelectedRow();
        if (sel >= 0) {
            String id = (String) modelClienti.getValueAt(sel, 0);
            if (JOptionPane.showConfirmDialog(this, "Eliminare il cliente " + id + "?", "Elimina", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                service.eliminaCliente(id);
                aggiornaTabellaClienti();
                aggiornaDashboard();
            }
        }
    }

    private void dialogNuovoPersonale() {
        JTextField txtCognome = new JTextField();
        JTextField txtNome = new JTextField();
        JTextField txtRuolo = new JTextField("Sarto Specializzato");
        JComboBox<String> cbContratto = new JComboBox<>(new String[]{"Indeterminato", "Determinato", "Apprendistato", "Stage", "Part-Time"});
        JTextField txtStipendio = new JTextField("1800.00");

        JPanel form = new JPanel(new GridLayout(5, 2, 6, 6));
        form.add(new JLabel("Cognome:")); form.add(txtCognome);
        form.add(new JLabel("Nome:")); form.add(txtNome);
        form.add(new JLabel("Ruolo:")); form.add(txtRuolo);
        form.add(new JLabel("Forma Contrattuale:")); form.add(cbContratto);
        form.add(new JLabel("Stipendio Mensile (€):")); form.add(txtStipendio);

        int res = JOptionPane.showConfirmDialog(this, form, "Nuovo Dipendente Organico", JOptionPane.OK_CANCEL_OPTION);
        if (res == JOptionPane.OK_OPTION) {
            try {
                Personale p = new Personale(
                        service.generaProssimoIdPersonale(),
                        txtNome.getText().trim(),
                        txtCognome.getText().trim(),
                        txtRuolo.getText().trim(),
                        (String) cbContratto.getSelectedItem(),
                        Double.parseDouble(txtStipendio.getText().replace(",", ".")),
                        LocalDate.now(),
                        "ATTIVO"
                );
                service.aggiungiPersonale(p);
                aggiornaTabellaPersonale();
                aggiornaDashboard();
            } catch (NumberFormatException ex) {
                JOptionPane.showMessageDialog(this, "Dati non validi.", "Errore", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void eliminaPersonaleSelezionato() {
        int sel = tablePersonale.getSelectedRow();
        if (sel >= 0) {
            String id = (String) modelPersonale.getValueAt(sel, 0);
            if (JOptionPane.showConfirmDialog(this, "Rimuovere dipendente " + id + "?", "Elimina", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                service.eliminaPersonale(id);
                aggiornaTabellaPersonale();
                aggiornaDashboard();
            }
        }
    }

    private void dialogNuovoFornitore() {
        JTextField txtNome = new JTextField();
        JTextField txtCognome = new JTextField();
        JTextField txtPiva = new JTextField();
        JTextField txtTel = new JTextField();
        JTextField txtMail = new JTextField();
        JTextField txtIndirizzo = new JTextField();

        JPanel form = new JPanel(new GridLayout(6, 2, 6, 6));
        form.add(new JLabel("Ragione Sociale / Nome:")); form.add(txtNome);
        form.add(new JLabel("Cognome Referente (opz):")); form.add(txtCognome);
        form.add(new JLabel("Partita IVA / CF:")); form.add(txtPiva);
        form.add(new JLabel("Telefono:")); form.add(txtTel);
        form.add(new JLabel("Email:")); form.add(txtMail);
        form.add(new JLabel("Indirizzo Sede:")); form.add(txtIndirizzo);

        int res = JOptionPane.showConfirmDialog(this, form, "Nuovo Fornitore Aziendale", JOptionPane.OK_CANCEL_OPTION);
        if (res == JOptionPane.OK_OPTION) {
            Fornitore f = new Fornitore(service.generaProssimoIdFornitore(),
                    txtNome.getText().trim(), txtCognome.getText().trim(),
                    txtPiva.getText().trim(), txtTel.getText().trim(),
                    txtMail.getText().trim(), txtIndirizzo.getText().trim());
            service.aggiungiFornitore(f);
            aggiornaTabellaFornitori();
        }
    }

    private void eliminaFornitoreSelezionato() {
        int sel = tableFornitori.getSelectedRow();
        if (sel >= 0) {
            String id = (String) modelFornitori.getValueAt(sel, 0);
            if (JOptionPane.showConfirmDialog(this, "Eliminare fornitore " + id + "?", "Elimina", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                service.eliminaFornitore(id);
                aggiornaTabellaFornitori();
            }
        }
    }

    private void salvaDatiSuFile() {
        boolean ok = FilePersistenceService.salvaDati(service, null);
        if (ok) {
            JOptionPane.showMessageDialog(this, "Dati archiviati con successo su file!", "Salvataggio", JOptionPane.INFORMATION_MESSAGE);
            lblStatusInfo.setText("Salvataggio completato alle ore: " + java.time.LocalTime.now().format(DateTimeFormatter.ofPattern("HH:mm:ss")));
        } else {
            JOptionPane.showMessageDialog(this, "Errore durante il salvataggio dei dati.", "Errore", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void ricaricaDati() {
        int ok = JOptionPane.showConfirmDialog(this, "Ricaricare l'archivio salvato annullando le modifiche non salvate?", "Conferma Ricarica", JOptionPane.YES_NO_OPTION);
        if (ok == JOptionPane.YES_OPTION) {
            FilePersistenceService.caricaDati(service, null);
            aggiornaTutteLeTabelle();
            lblStatusInfo.setText("Dati ricaricati dall'archivio persistente.");
        }
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroupPiani = new javax.swing.ButtonGroup();
        panelHeader = new javax.swing.JPanel();
        lblTitoloApp = new javax.swing.JLabel();
        lblSottotitolo = new javax.swing.JLabel();
        btnSalvaDati = new javax.swing.JButton();
        btnRicarica = new javax.swing.JButton();
        tabbedPanePrincipale = new javax.swing.JTabbedPane();
        tabDashboard = new javax.swing.JPanel();
        tabProgetti = new javax.swing.JPanel();
        tabMagazzino = new javax.swing.JPanel();
        tabClienti = new javax.swing.JPanel();
        tabPersonale = new javax.swing.JPanel();
        tabFornitori = new javax.swing.JPanel();
        panelStatusBar = new javax.swing.JPanel();
        lblStatusInfo = new javax.swing.JLabel();
        lblValoreMagazzinoStatus = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Sistema Gestione Sartoria Digitale - V2 (Java Desktop Enterprise)");
        setMinimumSize(new java.awt.Dimension(1150, 750));

        panelHeader.setBackground(new java.awt.Color(26, 45, 70));

        lblTitoloApp.setFont(new java.awt.Font("Segoe UI", 1, 20)); // NOI18N
        lblTitoloApp.setForeground(new java.awt.Color(255, 255, 255));
        lblTitoloApp.setText("\uD83E\uDDF5 GESTIONE SARTORIA DIGITALE - V2");

        lblSottotitolo.setFont(new java.awt.Font("Segoe UI", 0, 12)); // NOI18N
        lblSottotitolo.setForeground(new java.awt.Color(204, 204, 204));
        lblSottotitolo.setText("Commesse su misura, Magazzino a 3 Piani con Drag&Drop, Dipendenti, Clienti e Fornitori");

        btnSalvaDati.setFont(new java.awt.Font("Segoe UI", 1, 12)); // NOI18N
        btnSalvaDati.setText("\uD83D\uDCBE Salva Archivio");

        btnRicarica.setFont(new java.awt.Font("Segoe UI", 0, 12)); // NOI18N
        btnRicarica.setText("\u21BB Ricarica Dati");

        javax.swing.GroupLayout panelHeaderLayout = new javax.swing.GroupLayout(panelHeader);
        panelHeader.setLayout(panelHeaderLayout);
        panelHeaderLayout.setHorizontalGroup(
            panelHeaderLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelHeaderLayout.createSequentialGroup()
                .addGap(24, 24, 24)
                .addGroup(panelHeaderLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lblTitoloApp, javax.swing.GroupLayout.PREFERRED_SIZE, 450, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lblSottotitolo, javax.swing.GroupLayout.PREFERRED_SIZE, 550, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(btnSalvaDati, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(btnRicarica, javax.swing.GroupLayout.PREFERRED_SIZE, 140, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(24, 24, 24))
        );
        panelHeaderLayout.setVerticalGroup(
            panelHeaderLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelHeaderLayout.createSequentialGroup()
                .addGap(14, 14, 14)
                .addGroup(panelHeaderLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(panelHeaderLayout.createSequentialGroup()
                        .addComponent(lblTitoloApp)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(lblSottotitolo))
                    .addGroup(panelHeaderLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(btnSalvaDati, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(btnRicarica, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(14, Short.MAX_VALUE))
        );

        tabbedPanePrincipale.setFont(new java.awt.Font("Segoe UI", 1, 13)); // NOI18N

        tabDashboard.setLayout(new java.awt.BorderLayout());
        tabbedPanePrincipale.addTab("\uD83D\uDCCA Panoramica & Alert", tabDashboard);

        tabProgetti.setLayout(new java.awt.BorderLayout());
        tabbedPanePrincipale.addTab("\u2702\uFE0F Commesse & Progetti (Drag&Drop)", tabProgetti);

        tabMagazzino.setLayout(new java.awt.BorderLayout());
        tabbedPanePrincipale.addTab("\uD83D\uDCE6 Magazzino a 3 Piani", tabMagazzino);

        tabClienti.setLayout(new java.awt.BorderLayout());
        tabbedPanePrincipale.addTab("\uD83D\uDC65 Clienti & Misure", tabClienti);

        tabPersonale.setLayout(new java.awt.BorderLayout());
        tabbedPanePrincipale.addTab("\uD83D\uDC54 Personale & Contratti", tabPersonale);

        tabFornitori.setLayout(new java.awt.BorderLayout());
        tabbedPanePrincipale.addTab("\uD83C\uDFED Fornitori", tabFornitori);

        panelStatusBar.setBorder(javax.swing.BorderFactory.createEtchedBorder());

        lblStatusInfo.setFont(new java.awt.Font("Segoe UI", 0, 12)); // NOI18N
        lblStatusInfo.setText("Pronto. Sistema avviato con successo.");

        lblValoreMagazzinoStatus.setFont(new java.awt.Font("Segoe UI", 1, 12)); // NOI18N
        lblValoreMagazzinoStatus.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        lblValoreMagazzinoStatus.setText("Valore Magazzino: \u20AC 0.00");

        javax.swing.GroupLayout panelStatusBarLayout = new javax.swing.GroupLayout(panelStatusBar);
        panelStatusBar.setLayout(panelStatusBarLayout);
        panelStatusBarLayout.setHorizontalGroup(
            panelStatusBarLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelStatusBarLayout.createSequentialGroup()
                .addGap(14, 14, 14)
                .addComponent(lblStatusInfo, javax.swing.GroupLayout.PREFERRED_SIZE, 600, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(lblValoreMagazzinoStatus, javax.swing.GroupLayout.PREFERRED_SIZE, 350, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(14, 14, 14))
        );
        panelStatusBarLayout.setVerticalGroup(
            panelStatusBarLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(panelStatusBarLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                .addComponent(lblStatusInfo, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addComponent(lblValoreMagazzinoStatus, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(panelHeader, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(tabbedPanePrincipale, javax.swing.GroupLayout.DEFAULT_SIZE, 1180, Short.MAX_VALUE)
            .addComponent(panelStatusBar, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(panelHeader, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(tabbedPanePrincipale, javax.swing.GroupLayout.DEFAULT_SIZE, 640, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(panelStatusBar, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        pack();
        setLocationRelativeTo(null);
    }// </editor-fold>//GEN-END:initComponents

    /**
     * Entry point principale dell'applicazione aziendale
     */
    public static void main(String args[]) {
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (Exception ex) {
            // Se Nimbus non e' disponibile usa il look and feel predefinito
        }

        java.awt.EventQueue.invokeLater(() -> {
            new FinestraPrincipale().setVisible(true);
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnRicarica;
    private javax.swing.JButton btnSalvaDati;
    private javax.swing.ButtonGroup buttonGroupPiani;
    private javax.swing.JLabel lblSottotitolo;
    private javax.swing.JLabel lblStatusInfo;
    private javax.swing.JLabel lblTitoloApp;
    private javax.swing.JLabel lblValoreMagazzinoStatus;
    private javax.swing.JPanel panelHeader;
    private javax.swing.JPanel panelStatusBar;
    private javax.swing.JPanel tabClienti;
    private javax.swing.JPanel tabDashboard;
    private javax.swing.JPanel tabFornitori;
    private javax.swing.JPanel tabMagazzino;
    private javax.swing.JPanel tabPersonale;
    private javax.swing.JPanel tabProgetti;
    private javax.swing.JTabbedPane tabbedPanePrincipale;
    // End of variables declaration//GEN-END:variables
}
