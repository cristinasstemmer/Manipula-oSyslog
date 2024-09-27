#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define o caminho do arquivo
#define LOG_FILE "/var/log/syslog"
#define BUFFER_SIZE 1024

// Estrutura para armazenar eventos de rede
typedef struct Event {
    int id;                // Código identificador do evento
    char dateTime[30];     // Horário do evento
    char type[20];         // Tipo do evento: CONNECTED ou DISCONNECTED
    struct Event *next;    // Ponteiro para o próximo nó
} Event;

// Ponteiro global para a cabeça da lista
Event *head = NULL;
int currentCode = 1;  // Código incremental para cada evento

// Função para adicionar evento à lista
void add_event(const char *dateTime, const char *type) {
    Event *newEvent = (Event *)malloc(sizeof(Event));
    newEvent->id = currentCode++;
    strcpy(newEvent->dateTime, dateTime);
    strcpy(newEvent->type, type);
    newEvent->next = head;
    head = newEvent;
}

// Função para listar todos os eventos
void list_events() {
    Event *current = head;
    if (current == NULL) {
        printf("Nenhum evento registrado.\n");
        return;
    }

    printf("Lista de eventos:\n");
    while (current != NULL) {
        printf("Código: %d | Data e Hora: %s | Tipo: %s\n", current->id, current->dateTime, current->type);
        current = current->next;
    }
}

// Função para listar os eventos por tipo
void list_type(const char *type) {
    Event *current = head;
    if (current == NULL) {
        printf("Nenhum evento registrado.\n");
        return;
    }

    printf("Lista de eventos do tipo '%s':\n", type);
    int found = 0;

    while (current != NULL) {
        if (strcasecmp(current->type, "CONNECTED") == 0 && strcasecmp(type, "connection") == 0) {
            printf("Código: %d | Data e Hora: %s | Tipo: %s\n", current->id, current->dateTime, current->type);
            found = 1;
        } else if (strcasecmp(current->type, "DISCONNECTED") == 0 && strcasecmp(type, "disconnection") == 0) {
            printf("Código: %d | Data e Hora: %s | Tipo: %s\n", current->id, current->dateTime, current->type);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("Nenhum evento encontrado do tipo '%s'.\n", type);
    }
}

// Função para adicionar evento manualmente
void add_event_manually() {
    char dateTime[30];
    char type[20];

    printf("Digite a data e hora do evento (formato: YYYY-MM-DD HH:MM:SS): ");
    scanf(" %[^\n]s", dateTime);  // Lê a data e hora com espaços

    printf("Digite o tipo de evento (CONNECTED ou DISCONNECTED): ");
    scanf("%s", type);  // Lê o tipo do evento

    // Adiciona o evento à estrutura
    add_event(dateTime, type);
    printf("Evento adicionado com sucesso!\n");
}

// Função para converter uma string para minúsculas
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Função que filtra eventos do arquivo de log
void filter_events(const char *line) {
    char dateTime[30];
    strncpy(dateTime, line, 15);
    dateTime[15] = '\0';

    // Cria uma cópia da linha para manipulação
    char line_copy[BUFFER_SIZE];
    strncpy(line_copy, line, BUFFER_SIZE);
    to_lowercase(line_copy); // Converte para minúsculas

    // Verifica se "networkmanager" está presente na linha
    if (strstr(line_copy, "networkmanager")) {
        char type[20];
        if (strstr(line_copy, "link connected")) {
            strcpy(type, "CONNECTED");
            add_event(dateTime, type);
        } else if (strstr(line_copy, "networkmanager state is now disconnected")) {
            strcpy(type, "DISCONNECTED");
            add_event(dateTime, type);
        }
    }
}

// Função para remover um evento pelo código
void remove_event(int id) {
    Event *current = head;
    Event *previous = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                head = current->next;
            }
            free(current);
            printf("Evento %d removido com sucesso.\n", id);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Evento com o código %d não encontrado.\n", id);
}

int main() {
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL) {
        perror("Erro ao abrir o syslog");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    printf("Lendo o arquivo %s...\n", LOG_FILE);

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        filter_events(buffer);
    }

    fclose(file);

    int option, id;
    char type[20];
    do {
        printf("\nMenu de Opções:\n");
        printf("1. Listar todos os eventos\n");
        printf("2. Filtrar e listar eventos por tipo\n");
        printf("3. Remover um evento\n");
        printf("4. Adicionar evento manualmente\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                list_events();
                break;
            case 2:
                printf("Digite o tipo de evento que deseja listar (connection ou disconnection): ");
                scanf("%s", type);
                list_type(type);
                break;
            case 3:
                printf("Digite o código do evento para remover: ");
                scanf("%d", &id);
                remove_event(id);
                break;
            case 4:
                add_event_manually();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (option != 5);

    return 0;
}
