import express from 'express';
import { exec } from 'child_process';
import os from 'os';

const app = express();
app.use(express.json());

// ==========================================
// 1. O "OUVIDO" (Interface Web que roda no celular)
// ==========================================
const htmlOuvido = `
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="utf-8">
    <title>Ouvido do Jarvis</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { background: #111; color: #0f0; font-family: monospace; padding: 20px; text-align: center; }
        button { padding: 20px 40px; font-size: 24px; background: #0f0; color: #000; border: none; border-radius: 10px; cursor: pointer; font-weight: bold; }
        button:active { background: #0a0; }
        #status { margin-top: 20px; font-size: 18px; color: #aaa; }
        #log { margin-top: 30px; font-size: 16px; text-align: left; background: #222; padding: 10px; border-radius: 5px; height: 300px; overflow-y: auto; }
    </style>
</head>
<body>
    <h1>🎙️ Jarvis Web Listener</h1>
    <button id="btn" onclick="iniciarEscuta()">INICIAR ESCUTA</button>
    <div id="status">Aguardando ativação...</div>
    <div id="log"></div>

    <script>
        const logDiv = document.getElementById('log');
        const statusDiv = document.getElementById('status');
        let reconhecedor;

        function logar(msg) {
            logDiv.innerHTML = '<p>' + msg + '</p>' + logDiv.innerHTML;
        }

        function iniciarEscuta() {
            // Usa a API de voz nativa do navegador
            const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
            if (!SpeechRecognition) {
                alert("Seu navegador não suporta reconhecimento de voz. Use o Chrome.");
                return;
            }

            reconhecedor = new SpeechRecognition();
            reconhecedor.lang = 'pt-BR';
            reconhecedor.continuous = true;      // Fica escutando sem parar
            reconhecedor.interimResults = false; // Só retorna a frase quando você terminar de falar

            reconhecedor.onstart = () => {
                statusDiv.innerHTML = "🟢 Escutando continuamente...";
                statusDiv.style.color = "#0f0";
                document.getElementById('btn').style.display = 'none';
            };
            
            reconhecedor.onresult = (event) => {
                const frase = event.results[event.results.length - 1][0].transcript.toLowerCase();
                logar("🗣️ Você: " + frase);
                
                if (frase.includes("jarvis")) {
                    logar("⚡ Jarvis ativado! Enviando comando...");
                    
                    // Envia a frase pro servidor Node no Termux
                    fetch('/comando', {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ fala: frase })
                    });
                }
            };

            reconhecedor.onerror = (event) => logar("⚠️ Erro: " + event.error);
            
            // Se o motor de voz parar por causa do silêncio, nós ligamos ele de novo!
            reconhecedor.onend = () => {
                statusDiv.innerHTML = "🟡 Reiniciando escuta...";
                reconhecedor.start(); 
            };

            reconhecedor.start();
        }
    </script>
</body>
</html>
`;

// Servimos a página HTML na rota principal
app.get('/', (req, res) => {
    res.send(htmlOuvido);
});


// ==========================================
// 2. O "CÉREBRO" (Executa os comandos físicos)
// ==========================================
const executar = (cmd: string): Promise<string> => {
    return new Promise((resolve) => {
        exec(cmd, (err, stdout) => {
            if (err) console.log(`[ERRO TERMUX] ${err.message}`);
            resolve(stdout || ""); 
        });
    });
};

app.post('/comando', async (req, res) => {
    const fala: string = req.body.fala;
    console.log(`\n[⚡] Jarvis Web Acionado! Frase recebida: "${fala}"`);
    
    // Pega tudo que foi dito DEPOIS da palavra "jarvis"
    const partes = fala.split("jarvis");
    const comandoReal = partes.length > 1 ? partes[1].trim() : "";

    // Vibra o celular para confirmar que ouviu
    await executar('termux-vibrate -d 100');

    if (comandoReal.includes("hora") || comandoReal.includes("horas")) {
        const agora = new Date();
        const frase = `Agora são ${agora.getHours()} horas e ${agora.getMinutes()} minutos.`;
        console.log(`[🤖] Respondendo: ${frase}`);
        await executar(`termux-tts-speak -l pt-BR "${frase}"`);
    }
    else if (comandoReal.includes("bateria")) {
        const raw = await executar('termux-battery-status');
        try {
            const data = JSON.parse(raw);
            const frase = `A bateria está em ${data.percentage} por cento.`;
            console.log(`[🤖] Respondendo: ${frase}`);
            await executar(`termux-tts-speak -l pt-BR "${frase}"`);
        } catch (e) {
            console.log("Erro ao ler bateria.");
        }
    }
    else if (comandoReal.includes("luz") || comandoReal.includes("lanterna")) {
        const estado = comandoReal.includes("liga") || comandoReal.includes("acende") ? "on" : "off";
        console.log(`[🔦] Alterando lanterna para: ${estado}`);
        await executar(`termux-torch ${estado}`);
        await executar(`termux-tts-speak -l pt-BR "Lanterna ${estado === 'on' ? 'ligada' : 'desligada'}."`);
    } 
    else if (comandoReal === "") {
        console.log(`[❓] Chamado sem comando específico.`);
        await executar('termux-tts-speak -l pt-BR "Sim, mestre?"');
    }

    res.json({ ok: true }); // Responde pro navegador que deu tudo certo
});


// ==========================================
// 3. INICIALIZAÇÃO
// ==========================================
// Omitindo o '0.0.0.0' permite que o Node resolva automaticamente IPv4 e IPv6
app.listen(3000, () => {
    console.log(`\n======================================================`);
    console.log(`🚀 NOVO SERVIDOR JARVIS WEB INICIADO`);
    console.log(`======================================================`);
    
    // Mostra os IPs corretos para acessar
    const interfaces = os.networkInterfaces();
    let ipRede = '';
    for (const name of Object.keys(interfaces)) {
        for (const net of interfaces[name] || []) {
            if (net.family === 'IPv4' && !net.internal) ipRede = net.address;
        }
    }

    console.log(`Abra o Google Chrome no celular e digite:`);
    console.log(`👉 http://127.0.0.1:3000   (Recomendado para o microfone)`);
    if (ipRede) {
        console.log(`\nOu acesse pelo Mac para testar:`);
        console.log(`👉 http://${ipRede}:3000`);
    }
    console.log(`======================================================\n`);
});