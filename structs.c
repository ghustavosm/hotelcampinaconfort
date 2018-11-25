#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gmodule.h>
#define NOME_ARQUIVO_CLIENTES "clientes.dat"
#define NOME_ARQUIVO_TIPO_QUARTOS "tipo_quartos.dat"
#define NOME_ARQUIVO_QUARTOS "quartos.dat"
#define NOME_ARQUIVO_SERVICOS "servicos.dat"
#define NOME_ARQUIVO_CONTRATOS "contratos.dat"

typedef struct {
    gchar nome[60], cpf[20], endereco[200];
} CLIENTE;

typedef struct {
    gchar tipo[30];
    gfloat valor;
} TIPO_QUARTO;

typedef struct {
    gchar numero[10];
    gchar tipo[30];
    gchar reservas[740][11];
} QUARTO;

typedef struct {
    gchar tipo[30];
    gfloat valor;
} SERVICO;

typedef struct {
    gchar cpf[20];
    gchar quartos[100][10];
    gchar servicos[2][100][30];
} CONTRATO;

GSList *clientes = NULL;
GSList *tipo_quartos = NULL;
GSList *quartos = NULL;
GSList *servicos = NULL;
GSList *contratos = NULL;

CLIENTE *pegarCliente(gchar cpf[]) {
    GSList *c = NULL;
    CLIENTE *cliente = NULL;
    CLIENTE *aux;
    for(c = clientes; c != NULL; c = c->next) {
        aux = (CLIENTE*) c->data;
        if(strcmp(aux->cpf, cpf) == 0) {
            cliente = aux;
            break;
        }
    }
    return cliente;
}

gint compararClientes(CLIENTE *a, CLIENTE *b) {
    return strcmp(a->nome, b->nome);
}

gint posicaoCliente(gchar nome[]) {
    GSList *c = NULL;
    CLIENTE *cliente;
    gint posicao = 0;
    for(c = clientes; c != NULL; c = c->next) {
        cliente = (CLIENTE*) c->data;
        if(strcmp(cliente->nome, nome) >= 0) {
            break;
        }
        posicao++;
    }
    return posicao;
}

/* -1 se o cliente existe
    0 se houve problema ao ler ou gravar no arquivo
    1 se adicionou o cliente com sucesso */
gint adicionarCliente(gchar nome[], gchar cpf[], gchar endereco[]) {
    gint retorno = pegarCliente(cpf) ? -1 : 1;
    if(retorno == 1) {
        CLIENTE *cliente = g_new(CLIENTE, 1);
        sprintf(cliente->nome, "%s", nome);
        sprintf(cliente->cpf, "%s", cpf);
        sprintf(cliente->endereco, "%s", endereco);
        clientes = g_slist_insert_sorted(clientes, cliente, (GCompareFunc) compararClientes);
        retorno = salvarClientes();
    }
    return retorno;
}

gint removerCliente(gchar *cpf) {
    gint retorno = pegarCliente(cpf) ? 1 : -1;
    if(retorno == 1) {
        GSList *c = NULL;
        CLIENTE *cliente;
        for(c = clientes; c != NULL; c = c->next) {
            cliente = (CLIENTE*) c->data;
            if(strcmp(cliente->cpf, cpf) == 0) {
                clientes = g_slist_delete_link(clientes, c);
            }
        }
        g_slist_free(c);
        retorno = salvarClientes();
    }
    return retorno;
}

gint salvarClientes(void) {
    FILE *arquivo;
    GSList *c = NULL;
    CLIENTE *cliente = g_new(CLIENTE, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_CLIENTES, "w")) != NULL) {
        for(c = clientes; c != NULL; c = c->next) {
            cliente = (CLIENTE*) c->data;
            if(fwrite(cliente, sizeof(CLIENTE), 1, arquivo) > 0) {
                retorno = 1;
            }
        }
    }

    fclose(arquivo);
    g_slist_free(c);
    return retorno;
}

gint carregarClientes(void) {
    FILE *arquivo;
    CLIENTE *cliente;
    gint retorno = 1;

    if((arquivo = fopen(NOME_ARQUIVO_CLIENTES, "r")) == NULL) {
        if((arquivo = fopen(NOME_ARQUIVO_CLIENTES, "w")) == NULL) {
            retorno = 0;
        }
    }

    while(TRUE) {
        cliente = g_new(CLIENTE, 1);
        if(fread(cliente, sizeof(CLIENTE), 1, arquivo)) {
            clientes = g_slist_append(clientes, cliente);
        } else {
            break;
        }
    }
    fclose(arquivo);
    return retorno;
}

TIPO_QUARTO *pegarTipoQuarto(gchar tipo[]) {
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto = NULL;
    TIPO_QUARTO *aux;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        aux = (TIPO_QUARTO*) tq->data;
        if(strcmp(aux->tipo, tipo) == 0) {
            tipo_quarto = aux;
            break;
        }
    }
    return tipo_quarto;
}

gint posicaoTipoQuarto(gchar tipo[]) {
    GSList *c = NULL;
    TIPO_QUARTO *tipo_quarto;
    gint posicao = 0;
    for(c = tipo_quartos; c != NULL; c = c->next) {
        tipo_quarto = (TIPO_QUARTO*) c->data;
        if(strcmp(tipo_quarto->tipo, tipo) >= 0) {
            break;
        }
        posicao++;
    }
    return posicao;
}

gint compararTipoQuartos(TIPO_QUARTO *a, TIPO_QUARTO *b) {
    return strcmp(a->tipo, b->tipo);
}

gint adicionarTipoQuarto(gchar tipo[], gfloat valor) {
    gint retorno = pegarTipoQuarto(tipo) ? -1 : 1;
    if(retorno == 1) {
        TIPO_QUARTO *tipo_quarto = g_new(TIPO_QUARTO, 1);
        sprintf(tipo_quarto->tipo, "%s", tipo);
        tipo_quarto->valor = valor;
        tipo_quartos = g_slist_insert_sorted(tipo_quartos, tipo_quarto, (GCompareFunc) compararTipoQuartos);
        retorno = salvarTipoQuarto();
    }
    return retorno;
}

gint removerTipoQuarto(gchar tipo[]) {
    gint retorno = pegarTipoQuarto(tipo) ? 1 : -2;
    if(retorno == 1) {
        retorno = totalQuartosTipo(tipo) > 0 ? -1 : 1;
        if(retorno == 1) {
            GSList *tq = NULL;
            TIPO_QUARTO *tipo_quarto;
            for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
                tipo_quarto = (TIPO_QUARTO*) tq->data;
                if(strcmp(tipo_quarto->tipo, tipo) == 0) {
                    tipo_quartos = g_slist_delete_link(tipo_quartos, tq);
                }
            }
            g_slist_free(tq);
            retorno = salvarTipoQuarto();
        }
    }
    return retorno;
}

gint salvarTipoQuarto(void) {
    FILE *arquivo;
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto = g_new(TIPO_QUARTO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_TIPO_QUARTOS, "w")) != NULL) {
        for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
            tipo_quarto = (TIPO_QUARTO*) tq->data;
            if(fwrite(tipo_quarto, sizeof(TIPO_QUARTO), 1, arquivo) > 0) {
                retorno = 1;
            }
        }
    }

    fclose(arquivo);
    g_slist_free(tq);
    return retorno;
}

gint carregarTipoQuartos(void) {
    FILE *arquivo;
    TIPO_QUARTO *tipo_quarto;
    gint retorno = 1;

    if((arquivo = fopen(NOME_ARQUIVO_TIPO_QUARTOS, "r")) == NULL) {
        if((arquivo = fopen(NOME_ARQUIVO_TIPO_QUARTOS, "w")) == NULL) {
            retorno = 0;
        }
    }

    while(TRUE) {
        tipo_quarto = g_new(TIPO_QUARTO, 1);
        if(fread(tipo_quarto, sizeof(TIPO_QUARTO), 1, arquivo)) {
            tipo_quartos = g_slist_append(tipo_quartos, tipo_quarto);
        } else {
            break;
        }
    }
    fclose(arquivo);
    return retorno;
}

QUARTO *pegarQuarto(gchar numero[]) {
    GSList *q = NULL;
    QUARTO *quarto = NULL;
    QUARTO *aux;
    for(q = quartos; q != NULL; q = q->next) {
        aux = (QUARTO*) q->data;
        if(strcmp(aux->numero, numero) == 0) {
            quarto = aux;
            break;
        }
    }
    return quarto;
}

gint compararQuartos(QUARTO *a, QUARTO *b) {
    return strcmp(a->numero, b->numero);
}

gint adicionarQuarto(gchar numero[], gchar tipo[]) {
    gint retorno = pegarQuarto(numero) ? -1 : 1;
    gint i;
    if(retorno == 1) {
        QUARTO *quarto = g_new(QUARTO, 1);
        sprintf(quarto->numero, "%s", numero);
        sprintf(quarto->tipo, "%s", tipo);
        for(i = 0; i < 740; i++) {
            sprintf(quarto->reservas[i], "%s", "");
        }
        quartos = g_slist_insert_sorted(quartos, quarto, (GCompareFunc) compararQuartos);
        retorno = salvarQuarto();
    }
    return retorno;
}

gint removerQuarto(gchar numero[]) {
    gint retorno = pegarQuarto(numero) ? 1 : -1;
    if(retorno == 1) {
        GSList *q = NULL;
        QUARTO *quarto;
        for(q = quartos; q != NULL; q = q->next) {
            quarto = (QUARTO*) q->data;
            if(strcmp(quarto->numero, numero) == 0) {
                quartos = g_slist_delete_link(quartos, q);
            }
        }
        g_slist_free(q);
        retorno = salvarQuarto();
    }
    return retorno;
}

gboolean disponibilidadeQuarto(gchar numero[], gchar data[]) {
    QUARTO *quarto = pegarQuarto(numero);
    gboolean retorno = TRUE;
    gint i;
    for(i = 0; i < 740; i++) {
        if(strcmp(quarto->reservas[i], data) == 0) {
            retorno = FALSE;
            break;
        }
    }
    return retorno;
}

gint totalQuartosTipo(gchar tipo[]) {
    GSList *q = NULL;
    QUARTO *quarto;
    int total = 0;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        if(strcmp(quarto->tipo, tipo) == 0) {
            total++;
        }
    }
    g_slist_free(q);
    return total;
}

gfloat valorQuarto(gchar tipo[]) {
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto;
    gfloat valor = -1.0;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        tipo_quarto = (TIPO_QUARTO*) tq->data;
        if(strcmp(tipo_quarto->tipo, tipo) == 0) {
            valor = tipo_quarto->valor;
        }
    }
    g_slist_free(tq);
    return valor;
}

gboolean reservarQuarto(gchar numero[], gchar data[]) {
    GSList *q = NULL;
    gint i;
    QUARTO *quarto = pegarQuarto(numero);
    gboolean retorno = FALSE;
    if(disponibilidadeQuarto(numero, data)) {
        for(i = 0; i < 740; i++) {
            if(strcmp(quarto->reservas[i], "") == 0) {
                sprintf(quarto->reservas[i], "%s", data);
                retorno = TRUE;
                break;
            }
        }
    }
    return retorno;
}

gboolean liberarQuarto(gchar numero[], gchar data[]) {
    GSList *q = NULL;
    gint i;
    QUARTO *quarto = pegarQuarto(numero);
    gboolean retorno = FALSE;
    for(i = 0; i < 740; i++) {
        if(strcmp(quarto->reservas[i], data) == 0) {
            sprintf(quarto->reservas[i], "%s", "");
            retorno = TRUE;
            break;
        }
    }
    return retorno;
}

gint salvarQuarto(void) {
    FILE *arquivo;
    GSList *q = NULL;
    QUARTO *quarto = g_new(QUARTO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_QUARTOS, "w")) != NULL) {
        for(q = quartos; q != NULL; q = q->next) {
            quarto = (QUARTO*) q->data;
            if(fwrite(quarto, sizeof(QUARTO), 1, arquivo) > 0) {
                retorno = 1;
            }
        }
    }

    fclose(arquivo);
    g_slist_free(q);
    return retorno;
}

gint carregarQuartos(void) {
    FILE *arquivo;
    QUARTO *quarto;
    gint retorno = 1;

    if((arquivo = fopen(NOME_ARQUIVO_QUARTOS, "r")) == NULL) {
        if((arquivo = fopen(NOME_ARQUIVO_QUARTOS, "w")) == NULL) {
            retorno = 0;
        }
    }

    while(TRUE) {
        quarto = g_new(QUARTO, 1);
        if(fread(quarto, sizeof(QUARTO), 1, arquivo)) {
            quartos = g_slist_append(quartos, quarto);
        } else {
            break;
        }
    }
    fclose(arquivo);
    return retorno;
}

gboolean existeServico(gchar tipo[]) {
    GSList *s = NULL;
    SERVICO *servico;
    gboolean existe = FALSE;
    for(s = servicos; s != NULL; s = s->next) {
        servico = (SERVICO*) s->data;
        if(strcmp(servico->tipo, tipo) == 0) {
            existe = TRUE;
        }
    }
    g_slist_free(s);
    return existe;
}

gint compararServicos(SERVICO *a, SERVICO *b) {
    return strcmp(a->tipo, b->tipo);
}

gint adicionarServico(gchar tipo[], gfloat valor) {
    gint retorno = existeServico(tipo) ? -1 : 1;
    if(retorno == 1) {
        SERVICO *servico = g_new(SERVICO, 1);
        sprintf(servico->tipo, "%s", tipo);
        servico->valor = valor;
        servicos = g_slist_insert_sorted(servicos, servico, (GCompareFunc) compararServicos);
        retorno = salvarServico();
    }
    return retorno;
}

gint removerServico(gchar tipo[]) {
    gint retorno = existeServico(tipo) ? 1 : -1;
    if(retorno == 1) {
        GSList *s = NULL;
        SERVICO *servico;
        for(s = servicos; s != NULL; s = s->next) {
            servico = (SERVICO*) s->data;
            if(strcmp(servico->tipo, tipo) == 0) {
                servicos = g_slist_delete_link(servicos, s);
            }
        }
        g_slist_free(s);
        retorno = salvarServico();
    }
    return retorno;
}

gint salvarServico(void) {
    FILE *arquivo;
    GSList *s = NULL;
    SERVICO *servico = g_new(SERVICO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_SERVICOS, "w")) != NULL) {
        for(s = servicos; s != NULL; s = s->next) {
            servico = (SERVICO*) s->data;
            if(fwrite(servico, sizeof(SERVICO), 1, arquivo) > 0) {
                retorno = 1;
            }
        }
    }

    fclose(arquivo);
    g_slist_free(s);
    return retorno;
}

gint carregarServicos(void) {
    FILE *arquivo;
    SERVICO *servico;
    gint retorno = 1;

    if((arquivo = fopen(NOME_ARQUIVO_SERVICOS, "r")) == NULL) {
        if((arquivo = fopen(NOME_ARQUIVO_SERVICOS, "w")) == NULL) {
            retorno = 0;
        }
    }

    while(TRUE) {
        servico = g_new(SERVICO, 1);
        if(fread(servico, sizeof(SERVICO), 1, arquivo)) {
            servicos = g_slist_append(servicos, servico);
        } else {
            break;
        }
    }
    fclose(arquivo);
    return retorno;
}
