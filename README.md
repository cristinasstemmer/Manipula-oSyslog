# Processador de Eventos de Rede

Este projeto em C processa eventos de rede a partir do arquivo de log do sistema (`/var/log/syslog`), especificamente eventos gerados pelo NetworkManager, como conexões e desconexões. Os eventos são armazenados em uma lista simplesmente encadeada.

## Funcionalidades

- **Listar todos os eventos**: Mostra todos os eventos registrados, com seus detalhes.
- **Filtrar eventos por tipo**: Permite listar apenas eventos de conexão ou desconexão.
- **Adicionar evento manualmente**: O usuário pode inserir manualmente a data, hora e tipo de um evento.
- **Remover evento**: Remove um evento específico, identificado por seu código.
- **Filtrar eventos do log**: Identifica automaticamente eventos de rede no log do sistema.

## Como Compilar

1. Clone o repositório:
   git clone https://github.com/seu-usuario/processador-eventos-rede.git
   
2. Compile o código:
   gcc -o processador main.c

3. Execute o programa:
   ./processador
