# 🧵 Produtor-Consumidor com Pthreads em C

Este projeto é uma simulação didática e otimizada do clássico problema da computação concorrente: o **Produtor-Consumidor**. O código foi desenvolvido em C para ambientes Linux e demonstra o uso de threads para manipulação de arquivos assíncrona e segura.

## 💡 Sobre o Projeto

O sistema simula um ambiente onde múltiplas threads geram eventos de log simultaneamente (Produtores) e os enviam para um buffer circular compartilhado. Uma thread dedicada (Consumidor) fica responsável por ler esse buffer e gravar os dados em um arquivo de texto de forma assíncrona, isolando o gargalo de I/O do disco rígido.

### Conceitos Técnicos Aplicados:
- **Concorrência:** Criação e gerenciamento de múltiplas threads usando `<pthread.h>`.
- **Sincronização (Mutex):** Uso de `pthread_mutex_t` para garantir Exclusão Mútua na Seção Crítica e evitar *Race Conditions*.
- **Variáveis de Condição:** Uso de `pthread_cond_t` para evitar espera ocupada (*Busy Waiting*), otimizando o uso da CPU.
- **Buffer Circular:** Estrutura de dados $O(1)$ que evita alocação dinâmica de memória excessiva e fragmentação.
- **I/O Assíncrono:** Minimização da Seção Crítica copiando dados para a *Stack* local (variável privada) antes da gravação em disco.
- **Proteção contra Spurious Wakeups:** Encapsulamento rigoroso dos `cond_wait` em laços `while`.

## 🛠️ Tecnologias e Pré-requisitos

Para compilar e executar este projeto, você precisará de um ambiente Linux (ou WSL no Windows) com o compilador C instalado.

* **GCC** (GNU Compiler Collection)
* Biblioteca **Pthreads** (nativa no Linux)

Se estiver usando o Ubuntu/WSL, você pode instalar as dependências com:
```bash
sudo apt update
sudo apt install build-essential