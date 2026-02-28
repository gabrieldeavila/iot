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

const PORTA = 3000;
app.listen(PORTA, () => {
  console.log(`🚀 Servidor TS rodando em http://192.168.3.23:${PORTA}`);
});
