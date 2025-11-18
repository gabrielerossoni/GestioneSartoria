(() => {
	const fileInput = document.getElementById('fileInput');
	const dropArea = document.getElementById('dropArea');
	const rawView = document.getElementById('rawView');
	const tableView = document.getElementById('tableView');
	const logArea = document.getElementById('logArea');
	const wsBtn = document.getElementById('wsConnectBtn');
	const wsStatus = document.getElementById('wsStatus');
	const downloadBtn = document.getElementById('downloadBtn');

	let currentJson = null;
	let ws = null;

	// Utilità
	function log(msg){
		const time = new Date().toLocaleTimeString();
		logArea.textContent = `[${time}] ${msg}\n` + logArea.textContent;
	}

	// Parsing e rendering del JSON ricevuto
	function handleJson(obj, source = 'file'){
		try{
			// se è stringa, proviamo a parsare
			if (typeof obj === 'string'){
				obj = JSON.parse(obj);
			}
			currentJson = obj;
			rawView.textContent = JSON.stringify(obj, null, 2);
			renderStructure(obj);
			downloadBtn.disabled = false;
			log(`JSON ricevuto da ${source}`);
		}catch(e){
			log(`Errore parsing JSON: ${e.message}`);
			rawView.textContent = `Errore parsing JSON: ${e.message}`;
		}
	}

	// Rendering generico: se è array di oggetti mostriamo tabella, altrimenti struttura ricorsiva
	function renderStructure(obj){
		tableView.innerHTML = '';
		if (Array.isArray(obj) && obj.length && typeof obj[0] === 'object'){
			// costruisci tabella con colonne dalle chiavi union
			const cols = Array.from(obj.reduce((s, item) => {
				if (item && typeof item === 'object') Object.keys(item).forEach(k => s.add(k));
				return s;
			}, new Set()));
			const table = document.createElement('table');
			const thead = document.createElement('thead');
			const trh = document.createElement('tr');
			cols.forEach(c => { const th = document.createElement('th'); th.textContent = c; trh.appendChild(th); });
			thead.appendChild(trh);
			table.appendChild(thead);

			const tbody = document.createElement('tbody');
			obj.forEach(row => {
				const tr = document.createElement('tr');
				cols.forEach(c => {
					const td = document.createElement('td');
					let v = row && row.hasOwnProperty(c) ? row[c] : '';
					td.textContent = (typeof v === 'object' && v !== null) ? JSON.stringify(v) : String(v);
					tr.appendChild(td);
				});
				tbody.appendChild(tr);
			});
			table.appendChild(tbody);
			tableView.appendChild(table);
		}else{
			// mostra struttura ricorsiva
			tableView.appendChild(renderTree(obj));
		}
	}

	function renderTree(obj){
		const container = document.createElement('div');
		if (obj === null){
			container.textContent = 'null';
			return container;
		}
		if (typeof obj !== 'object'){
			container.textContent = String(obj);
			return container;
		}
		const ul = document.createElement('ul');
		Object.keys(obj).forEach(key => {
			const li = document.createElement('li');
			const val = obj[key];
			if (typeof val === 'object' && val !== null){
				li.innerHTML = `<strong>${key}:</strong>`;
				li.appendChild(renderTree(val));
			}else{
				li.innerHTML = `<strong>${key}:</strong> ${String(val)}`;
			}
			ul.appendChild(li);
		});
		container.appendChild(ul);
		return container;
	}

	// File input
	fileInput.addEventListener('change', e => {
		const f = e.target.files && e.target.files[0];
		if (!f) return;
		const reader = new FileReader();
		reader.onload = () => handleJson(reader.result, 'file');
		reader.onerror = () => log('Errore lettura file');
		reader.readAsText(f, 'utf-8');
	});

	// Drag & drop
	['dragenter','dragover'].forEach(ev => {
		dropArea.addEventListener(ev, e => {
			e.preventDefault();
			e.stopPropagation();
			dropArea.classList.add('dragover');
		});
	});
	['dragleave','drop'].forEach(ev => {
		dropArea.addEventListener(ev, e => {
			e.preventDefault();
			e.stopPropagation();
			dropArea.classList.remove('dragover');
		});
	});
	dropArea.addEventListener('drop', e => {
		const f = (e.dataTransfer && e.dataTransfer.files && e.dataTransfer.files[0]) || null;
		if (!f){ log('Nessun file nel drop'); return; }
		const reader = new FileReader();
		reader.onload = () => handleJson(reader.result, 'drop');
		reader.onerror = () => log('Errore lettura file drop');
		reader.readAsText(f, 'utf-8');
	});

	// Download JSON corrente
	downloadBtn.addEventListener('click', () => {
		if (!currentJson) return;
		const blob = new Blob([JSON.stringify(currentJson, null, 2)], {type: 'application/json'});
		const url = URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = 'export.json';
		document.body.appendChild(a);
		a.click();
		a.remove();
		URL.revokeObjectURL(url);
		log('JSON scaricato');
	});

	// WebSocket: prova a connettere a ws://localhost:8080
	wsBtn.addEventListener('click', () => {
		if (ws && ws.readyState === WebSocket.OPEN){
			ws.close();
			return;
		}
		try {
			ws = new WebSocket('ws://localhost:8080');
			ws.onopen = () => {
				wsStatus.textContent = 'connesso';
				wsStatus.classList.add('connected');
				log('WebSocket connesso');
				wsBtn.textContent = 'Disconnetti WebSocket';
			};
			ws.onmessage = (ev) => {
				log('Messaggio WebSocket ricevuto');
				// proviamo a parsare JSON dal payload
				try {
					const parsed = JSON.parse(ev.data);
					handleJson(parsed, 'websocket');
				}catch(e){
					log('WebSocket: payload non JSON');
				}
			};
			ws.onclose = () => {
				wsStatus.textContent = 'disconnesso';
				wsStatus.classList.remove('connected');
				wsBtn.textContent = 'Connetti WebSocket (ws://localhost:8080)';
				log('WebSocket disconnesso');
			};
			ws.onerror = (ev) => {
				log('WebSocket errore (controlla che il server sia attivo)');
			};
		}catch(e){
			log('Impossibile aprire WebSocket: ' + e.message);
		}
	});

	// Esporre receiveJson per integrazione con C (ad es. browser automatizzato o webview)
	window.receiveJson = function(payload){
		// payload può essere oggetto o stringa JSON
		handleJson(payload, 'direct');
	};

	// Messaggio iniziale
	log('Interfaccia pronta. Usa file input, drag&drop o window.receiveJson(obj).');
})();