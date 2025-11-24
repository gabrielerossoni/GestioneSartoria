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
	ID        string  `json:"id"`
	Tipo      string  `json:"tipo"`
	Colore    string  `json:"colore"`
	Lunghezza float64 `json:"lunghezza"`
	Costo     float64 `json:"costo"`
}

type Response struct {
	Status  string `json:"status"`
	Message string `json:"message"`
}

func handleDeleteRotolo(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	if r.Method != "DELETE" {
		http.Error(w, `{"status":"error","message":"Method not allowed"}`, http.StatusMethodNotAllowed)
		return
	}

	// Estrai ID dal path (es: /api/rotoli/delete/R0001)
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

func handleGetPrelievi(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	
	// Per ora ritorna array vuoto
	w.Write([]byte(`[]`))
}

func handleGetRitagli(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	
	// Per ora ritorna array vuoto
	w.Write([]byte(`[]`))
}

func handleGetFornitori(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	
	// Per ora ritorna array vuoto
	w.Write([]byte(`[]`))
}

func handleGetInventario(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	
	// Ritorna statistiche di base
	w.Write([]byte(`{"numeroRotoli":0,"metraggioTotale":0,"valoreTotale":0,"rotoliDisponibili":0}`))
}

func main() {
	// Serve file statici
	fs := http.FileServer(http.Dir("../web"))
	http.Handle("/", fs)

	// API ROTOLI
	http.HandleFunc("/api/rotoli", handleGetRotoli)
	http.HandleFunc("/api/rotoli/add", handleAddRotolo)
	http.HandleFunc("/api/rotoli/delete/", handleDeleteRotolo)  // NUOVO

	// API PRELIEVI (placeholder)
	http.HandleFunc("/api/prelievi", handleGetPrelievi)         // NUOVO

	// API RITAGLI (placeholder)
	http.HandleFunc("/api/ritagli", handleGetRitagli)          // NUOVO

	// API FORNITORI (placeholder)
	http.HandleFunc("/api/fornitori", handleGetFornitori)      // NUOVO

	// API INVENTARIO (placeholder)
	http.HandleFunc("/api/inventario", handleGetInventario)    // NUOVO

	fmt.Println("🚀 Server avviato su http://localhost:8080")
	fmt.Println("📂 Servendo file da: ../web")
	
	openBrowser("http://localhost:8080")
	
	log.Fatal(http.ListenAndServe(":8080", nil))
}

// GET /api/rotoli - Ottieni lista rotoli
func handleGetRotoli(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	w.Header().Set("Access-Control-Allow-Origin", "*")

	// Determina il comando giusto per il sistema operativo
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

// POST /api/rotoli/add - Aggiungi nuovo rotolo
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

	// Determina il comando giusto per il sistema operativo
	cmdName := "../api_handler.exe"
	if runtime.GOOS != "windows" {
		cmdName = "../api_handler"
	}

	cmd := exec.Command(cmdName, "add",
		rotolo.Tipo,
		rotolo.Colore,
		fmt.Sprintf("%.2f", rotolo.Lunghezza),
		fmt.Sprintf("%.2f", rotolo.Costo))

	output, err := cmd.Output()
	
	if err != nil {
		log.Printf("Errore aggiunta rotolo: %v", err)
		http.Error(w, `{"status":"error","message":"Errore aggiunta rotolo"}`, http.StatusInternalServerError)
		return
	}

	w.Write(output)
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
