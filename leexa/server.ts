import express, { Request, Response } from "express";
import { exec } from "child_process";

const app = express();
app.use(express.json());

// Função auxiliar para executar comandos do Termux:API
const executarComando = (comando: string): Promise<string> => {
  return new Promise((resolve, reject) => {
    exec(comando, (error, stdout) => {
      if (error) reject(error);
      else resolve(stdout);
    });
  });
};

// ROTA 1: Falar (Text-to-Speech)
app.post("/falar", async (req: Request, res: Response) => {
  const { texto } = req.body;
  if (!texto)
    return res
      .status(400)
      .json({ erro: "Envie um 'texto' no corpo da requisição." });

  try {
    await executarComando(`termux-tts-speak "${texto}"`);
    res.json({ status: "Sucesso", mensagem: `O celular falou: ${texto}` });
  } catch (err) {
    res.status(500).json({ erro: "Falha ao acionar áudio", detalhes: err });
  }
});

// ROTA 2: Consultar Bateria
app.get("/bateria", async (_req: Request, res: Response) => {
  try {
    const info = await executarComando("termux-battery-status");
    res.json(JSON.parse(info));
  } catch (err) {
    res.status(500).json({ erro: "Falha ao ler bateria" });
  }
});

// ROTA 3: Lanterna (Opcional - para testar hardware)
app.post("/lanterna", async (req: Request, res: Response) => {
  const { ligado } = req.body; // espera um booleano: true ou false
  const acao = ligado ? "on" : "off";

  try {
    await executarComando(`termux-torch ${acao}`);
    res.json({ status: "Sucesso", lanterna: acao });
  } catch (err) {
    res.status(500).json({ erro: "Falha ao controlar lanterna" });
  }
});

const executar = (cmd: string): Promise<string> => {
  return new Promise((resolve, reject) => {
    exec(cmd, (err, stdout) => (err ? reject(err) : resolve(stdout)));
  });
};

app.post("/comando", async (req: Request, res: Response) => {
  const msg = req.body.msg?.toLowerCase();
  if (!msg) return res.status(400).json({ erro: "Mande uma 'msg'" });

  try {
    if (msg.includes("luz") || msg.includes("lanterna")) {
      const estado = msg.includes("liga") ? "on" : "off";
      await executar(`termux-torch ${estado}`);
      return res.json({ resposta: `Entendido, lanterna ${estado}.` });
    }

    if (msg.includes("bateria") || msg.includes("status")) {
      const raw = await executar("termux-battery-status");
      const data = JSON.parse(raw);
      const ampere = (data.current / 1000).toFixed(0);
      const frase = `Bateria em ${data.percentage} por cento. Corrente de ${ampere} miliamperes.`;
      await executar(`termux-tts-speak -l pt-BR -r 1.0 "${frase}"`);
      return res.json({ resposta: frase });
    }

    if (msg.includes("vibrar")) {
      await executar("termux-vibrate -d 500");
      return res.json({ resposta: "Vibrando!" });
    }

    await executar(`termux-tts-speak "Não entendi o comando: ${msg}"`);
    res.status(404).json({ erro: "Comando desconhecido" });
  } catch (err) {
    res.status(500).json({ erro: "Erro ao processar", detalhes: err });
  }
});

const PORTA = 3001;
app.listen(PORTA, () => {
  console.log(`🚀 Servidor TS rodando em http://192.168.3.23:${PORTA}`);
});
