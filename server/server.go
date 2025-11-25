package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os/exec"
	"runtime"
	"strings"
)

type Rotolo struct {
	ID                string  `json:"id"`
	Tipo              string  `json:"tipo"`
	Colore            string  `json:"colore"`
	Fantasia          string  `json:"fantasia"`
	Lunghezza_totale  float64 `json:"lunghezza_totale"`
	Lunghezza_attuale float64 `json:"lunghezza_attuale"`
	Costo_metro       float64 `json:"costo_metro"`
	Fornitore         string  `json:"fornitore"`
	Lotto             string  `json:"lotto"`
	Data              string  `json:"data"`
	Stato             string  `json:"stato"`
	NoteAggiuntive    string  `json:"noteAggiuntive"`
}

type Prelievo struct {
	ID                  string  `json:"id"`
	ID_rotolo           string  `json:"id_rotolo"`
	Metraggio_prelevato float64 `json:"metraggio_prelevato"`
	Data                string  `json:"data"`
	Operatore           string  `json:"operatore"`
	Cliente             string  `json:"cliente"`
}

type Ritaglio struct {
	IdRitaglio string  `json:"idRitaglio"`
	ID_rotolo  string  `json:"id_rotolo"`
	Lunghezza  float64 `json:"lunghezza"`
	Data       string  `json:"data"`
}

type Fornitore struct {
	Nome        string `json:"nome"`
	Partita_iva string `json:"partita_iva"`
	Indirizzo   string `json:"indirizzo"`
	Telefono    string `json:"telefono"`
	Email       string `json:"email"`
}

type Progetto struct {
	ID            string `json:"id"`
	IdCliente     string `json:"idCliente"`
	Tipo_capo     string `json:"tipo_capo"`
	IdRotolo      string `json:"idRotolo"`
	Tessuto_usato string `json:"tessuto_usato"`
	Data          string `json:"data"`
}

type Response struct {
	Status  string `json:"status"`
	Message string `json:"message"`
}

type Inventario struct {
	NumeroRotoli      int     `json:"numeroRotoli"`
	MetraggioTotale   float64 `json:"metraggioTotale"`
	ValoreTotale      float64 `json:"valoreTotale"`
	RotoliDisponibili int     `json:"rotoliDisponibili"`
}

// ===== ROTOLI =====
func handleGetRotoli(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "list")
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore esecuzione comando: %v", err)
		http.Error(w, `{"status":"error","message":"Errore caricamento dati"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleAddRotolo(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "POST" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	var rotolo Rotolo
	if err := json.NewDecoder(r.Body).Decode(&rotolo); err != nil {
		log.Printf("Errore parsing JSON: %v", err)
		http.Error(w, `{"status":"error","message":"Dati non validi"}`, http.StatusBadRequest)
		return
	}

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "add",
		rotolo.Tipo,
		rotolo.Colore,
		rotolo.Fantasia,
		fmt.Sprintf("%.2f", rotolo.Lunghezza_totale),
		fmt.Sprintf("%.2f", rotolo.Costo_metro),
		rotolo.Fornitore,
		rotolo.Lotto,
		rotolo.Stato,
		rotolo.NoteAggiuntive)

	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore aggiunta rotolo: %v", err)
		http.Error(w, `{"status":"error","message":"Errore aggiunta rotolo"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleDeleteRotolo(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "DELETE" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	parts := strings.Split(r.URL.Path, "/")
	if len(parts) < 5 {
		http.Error(w, `{"status":"error","message":"ID mancante"}`, http.StatusBadRequest)
		return
	}
	id := parts[4]

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "delete", id)
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore eliminazione rotolo: %v", err)
		http.Error(w, `{"status":"error","message":"Errore eliminazione"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

// ===== PRELIEVI =====
func handleGetPrelievi(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "list_prelievi")
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore caricamento prelievi: %v", err)
		http.Error(w, `{"status":"error","message":"Errore caricamento dati"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleAddPrelievo(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "POST" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	var prelievo Prelievo
	if err := json.NewDecoder(r.Body).Decode(&prelievo); err != nil {
		log.Printf("Errore parsing JSON: %v", err)
		http.Error(w, `{"status":"error","message":"Dati non validi"}`, http.StatusBadRequest)
		return
	}

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "add_prelievo",
		prelievo.ID_rotolo,
		fmt.Sprintf("%.2f", prelievo.Metraggio_prelevato),
		prelievo.Operatore,
		prelievo.Cliente)

	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore aggiunta prelievo: %v", err)
		http.Error(w, `{"status":"error","message":"Errore aggiunta prelievo"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleDeletePrelievo(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "DELETE" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	parts := strings.Split(r.URL.Path, "/")
	if len(parts) < 5 {
		http.Error(w, `{"status":"error","message":"ID mancante"}`, http.StatusBadRequest)
		return
	}
	id := parts[4]

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "delete_prelievo", id)
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore eliminazione prelievo: %v", err)
		http.Error(w, `{"status":"error","message":"Errore eliminazione"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

// ===== RITAGLI =====
func handleGetRitagli(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "list_ritagli")
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore caricamento ritagli: %v", err)
		http.Error(w, `{"status":"error","message":"Errore caricamento dati"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

// ===== FORNITORI =====
func handleGetFornitori(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "list_fornitori")
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore caricamento fornitori: %v", err)
		http.Error(w, `{"status":"error","message":"Errore caricamento dati"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleAddFornitore(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "POST" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	var fornitore Fornitore
	if err := json.NewDecoder(r.Body).Decode(&fornitore); err != nil {
		log.Printf("Errore parsing JSON: %v", err)
		http.Error(w, `{"status":"error","message":"Dati non validi"}`, http.StatusBadRequest)
		return
	}

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "add_fornitore",
		fornitore.Nome,
		fornitore.Partita_iva,
		fornitore.Indirizzo,
		fornitore.Telefono,
		fornitore.Email)

	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore aggiunta fornitore: %v", err)
		http.Error(w, `{"status":"error","message":"Errore aggiunta fornitore"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleDeleteFornitore(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "DELETE" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	parts := strings.Split(r.URL.Path, "/")
	if len(parts) < 5 {
		http.Error(w, `{"status":"error","message":"Nome mancante"}`, http.StatusBadRequest)
		return
	}
	nome := parts[4]

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "delete_fornitore", nome)
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore eliminazione fornitore: %v", err)
		http.Error(w, `{"status":"error","message":"Errore eliminazione"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

// ===== PROGETTI =====
func handleGetProgetti(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "list_progetti")
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore caricamento progetti: %v", err)
		http.Error(w, `{"status":"error","message":"Errore caricamento dati"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleAddProgetto(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "POST" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	var progetto Progetto
	if err := json.NewDecoder(r.Body).Decode(&progetto); err != nil {
		log.Printf("Errore parsing JSON: %v", err)
		http.Error(w, `{"status":"error","message":"Dati non validi"}`, http.StatusBadRequest)
		return
	}

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "add_progetto",
		progetto.IdCliente,
		progetto.Tipo_capo,
		progetto.IdRotolo,
		progetto.Tessuto_usato)

	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore aggiunta progetto: %v", err)
		http.Error(w, `{"status":"error","message":"Errore aggiunta progetto"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

func handleDeleteProgetto(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "DELETE" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	parts := strings.Split(r.URL.Path, "/")
	if len(parts) < 5 {
		http.Error(w, `{"status":"error","message":"ID mancante"}`, http.StatusBadRequest)
		return
	}
	id := parts[4]

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "delete_progetto", id)
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore eliminazione progetto: %v", err)
		http.Error(w, `{"status":"error","message":"Errore eliminazione"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

// ===== INVENTARIO =====
func handleGetInventario(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "get_inventario")
	output, err := cmd.Output()

	if err != nil {
		log.Printf("Errore caricamento inventario: %v", err)
		http.Error(w, `{"status":"error","message":"Errore caricamento dati"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
}

// ===== MAIN =====
func main() {
	// Serve file statici
	fs := http.FileServer(http.Dir("../web"))
	http.Handle("/", fs)

	// API ROTOLI
	http.HandleFunc("/api/rotoli", handleGetRotoli)
	http.HandleFunc("/api/rotoli/add", handleAddRotolo)
	http.HandleFunc("/api/rotoli/delete/", handleDeleteRotolo)

	// API PRELIEVI
	http.HandleFunc("/api/prelievi", handleGetPrelievi)
	http.HandleFunc("/api/prelievi/add", handleAddPrelievo)
	http.HandleFunc("/api/prelievi/delete/", handleDeletePrelievo)

	// API RITAGLI
	http.HandleFunc("/api/ritagli", handleGetRitagli)

	// API FORNITORI
	http.HandleFunc("/api/fornitori", handleGetFornitori)
	http.HandleFunc("/api/fornitori/add", handleAddFornitore)
	http.HandleFunc("/api/fornitori/delete/", handleDeleteFornitore)

	// API PROGETTI
	http.HandleFunc("/api/progetti", handleGetProgetti)
	http.HandleFunc("/api/progetti/add", handleAddProgetto)
	http.HandleFunc("/api/progetti/delete/", handleDeleteProgetto)

	// API INVENTARIO
	http.HandleFunc("/api/inventario", handleGetInventario)

	fmt.Println("🚀 Server avviato su http://localhost:8080")
	fmt.Println("📂 Servendo file da: ../web")

	openBrowser("http://localhost:8080")

	log.Fatal(http.ListenAndServe(":8080", nil))
}

// Apri browser automaticamente
func openBrowser(url string) {
	var err error
	switch runtime.GOOS {
	case "windows":
		err = exec.Command("cmd", "/c", "start", url).Start()
	case "darwin":
		err = exec.Command("open", url).Start()
	default: // linux
		err = exec.Command("xdg-open", url).Start()
	}
	if err != nil {
		log.Printf("Impossibile aprire browser: %v", err)
	}
}
