#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gtk/gtk.h>
#include <gmodule.h>
#include <glib.h>
#define TAM_STR 100
#define NOME_ARQUIVO_CLIENTES "clientes.dat"
#define NOME_ARQUIVO_TIPO_QUARTOS "tipo_quartos.dat"
#define NOME_ARQUIVO_QUARTOS "quartos.dat"
#define NOME_ARQUIVO_SERVICOS "servicos.dat"
#define NOME_ARQUIVO_CONTRATOS "contratos.dat"

typedef struct {
    gchar nome[TAM_STR], cpf[TAM_STR], endereco[TAM_STR * 2];
} CLIENTE;

typedef struct {
    gchar tipo[TAM_STR];
    gfloat valor;
} TIPO_QUARTO;

typedef struct {
    gchar numero[TAM_STR];
    gchar tipo[TAM_STR];
    gboolean disponivel;
} QUARTO;

typedef struct {
    gchar tipo[TAM_STR];
    gfloat valor;
} SERVICO;

typedef struct {
    gchar cpf[TAM_STR];
    GList quartos[TAM_STR];
    GList servicos[TAM_STR];
} CONTRATO;

static gboolean existeCliente(gchar cpf[]);
static gint compararClientes(CLIENTE *a, CLIENTE *b);
static gint adicionarCliente(gchar nome[], gchar cpf[], gchar endereco[]);
static gint editarCliente(gchar cpf[], gchar novo_nome[], gchar novo_cpf[], gchar novo_endereco[]);
static gint removerCliente(gchar cpf[]);
static gint salvarClientes(void);
static gint carregarClientes(void);
static void imprimirClientes(void);
static gboolean existeTipoQuarto(gchar tipo[]);
static gint compararTipoQuartos(TIPO_QUARTO *a, TIPO_QUARTO *b);
static gint adicionarTipoQuarto(gchar tipo[], gfloat valor);
static gint editarTipoQuarto(gchar tipo[], gchar novo_tipo[], gfloat novo_valor);
static gint removerTipoQuarto(gchar tipo[]);
static gint salvarTipoQuarto(void);
static gint carregarTipoQuartos(void);
static void imprimirTipoQuartos(void);
static gboolean existeQuarto(gchar numero[]);
static gint compararQuartos(QUARTO *a, QUARTO *b);
static gint adicionarQuarto(gchar numero[], gchar tipo[]);
static gint editarQuarto(gchar numero[], gchar novo_numero[], gchar novo_tipo[]);
static gint removerQuarto(gchar numero[]);
static gint totalQuartosTipo(gchar tipo[]);
static gfloat valorQuarto(gchar tipo[]);
static void reservarQuarto(gchar numero[]);
static void liberarQuarto(gchar numero[]);
static gint salvarQuarto(void);
static gint carregarQuartos(void);
static void imprimirQuartos(void);
static gboolean existeServico(gchar tipo[]);
static gint compararServicos(SERVICO *a, SERVICO *b);
static gint adicionarServico(gchar tipo[], gfloat valor);
static gint editarServico(gchar tipo[], gchar novo_tipo[], gfloat novo_valor);
static gint removerServico(gchar tipo[]);
static gint salvarServico(void);
static gint carregarServicos(void);
static void imprimirServicos(void);

static GSList *clientes = NULL;
static GSList *tipo_quartos = NULL;
static GSList *quartos = NULL;
static GSList *servicos = NULL;
static GSList *contratos = NULL;

static gboolean existeCliente(gchar cpf[]) {
    GSList *c = NULL;
    CLIENTE *cliente;
    gboolean existe = FALSE;
    for(c = clientes; c != NULL; c = c->next) {
        cliente = (CLIENTE*) c->data;
        if(strcmp(cliente->cpf, cpf) == 0) {
            existe = TRUE;
        }
    }
    g_slist_free(c);
    return existe;
}

static gint compararClientes(CLIENTE *a, CLIENTE *b) {
    return strcmp(a->nome, b->nome);
}

/* -1 se o cliente existe
    0 se houve problema ao ler ou gravar no arquivo
    1 se adicionou o cliente com sucesso */
static gint adicionarCliente(gchar nome[], gchar cpf[], gchar endereco[]) {
    gint retorno = existeCliente(cpf) ? -1 : 1;
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

/* -2 se o novo cpf já existe
   -1 se o cpf antigo não existe
    0 se houve problema ao ler ou gravar no arquivo
    1 se editou o cliente com sucesso */
static gint editarCliente(gchar cpf[], gchar novo_nome[], gchar novo_cpf[], gchar novo_endereco[]) {
    gint retorno = novo_cpf != cpf && existeCliente(novo_cpf) ? -2 : 1;
    if(retorno == 1) {
        retorno = existeCliente(cpf) ? 1 : -1;
        if(retorno == 1) {
            GSList *c = NULL;
            CLIENTE *cliente;
            for(c = clientes; c != NULL; c = c->next) {
                cliente = (CLIENTE*) c->data;
                if(strcmp(cliente->cpf, cpf) == 0) {
                    sprintf(cliente->nome, "%s", novo_nome);
                    sprintf(cliente->cpf, "%s", novo_cpf);
                    sprintf(cliente->endereco, "%s", novo_endereco);
                    retorno = salvarClientes();
                }
            }
            g_slist_free(c);
        }
    }
    return retorno;
}

static gint removerCliente(gchar *cpf) {
    gint retorno = existeCliente(cpf) ? 1 : -1;
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

static gint salvarClientes(void) {
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

static gint carregarClientes(void) {
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

static void imprimirClientes(void) {
    GSList *c = NULL;
    CLIENTE *cliente;
    for(c = clientes; c != NULL; c = c->next) {
        cliente = (CLIENTE*) c->data;
        printf("%s %s %s\n", cliente->nome, cliente->cpf, cliente->endereco);
    }
    g_slist_free(c);
}

static gboolean existeTipoQuarto(gchar tipo[]) {
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto;
    gboolean existe = FALSE;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        tipo_quarto = (TIPO_QUARTO*) tq->data;
        if(strcmp(tipo_quarto->tipo, tipo) == 0) {
            existe = TRUE;
        }
    }
    g_slist_free(tq);
    return existe;
}

static gint compararTipoQuartos(TIPO_QUARTO *a, TIPO_QUARTO *b) {
    return strcmp(a->tipo, b->tipo);
}

static gint adicionarTipoQuarto(gchar tipo[], gfloat valor) {
    gint retorno = existeTipoQuarto(tipo) ? -1 : 1;
    if(retorno == 1) {
        TIPO_QUARTO *tipo_quarto = g_new(TIPO_QUARTO, 1);
        sprintf(tipo_quarto->tipo, "%s", tipo);
        tipo_quarto->valor = valor;
        tipo_quartos = g_slist_insert_sorted(tipo_quartos, tipo_quarto, (GCompareFunc) compararTipoQuartos);
        retorno = salvarTipoQuarto();
    }
    return retorno;
}

static gint editarTipoQuarto(gchar tipo[], gchar novo_tipo[], gfloat novo_valor) {
    gint retorno = novo_tipo != tipo && existeTipoQuarto(novo_tipo) ? -2 : 1;
    if(retorno == 1) {
        retorno = existeTipoQuarto(tipo) ? 1 : -1;
        if(retorno == 1) {
            GSList *tq = NULL;
            TIPO_QUARTO *tipo_quarto;
            for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
                tipo_quarto = (TIPO_QUARTO*) tq->data;
                if(strcmp(tipo_quarto->tipo, tipo) == 0) {
                    sprintf(tipo_quarto->tipo, "%s", novo_tipo);
                    tipo_quarto->valor = novo_valor;
                    retorno = salvarTipoQuarto();
                }
            }
            g_slist_free(tq);
        }
    }
    return retorno;
}

static gint removerTipoQuarto(gchar tipo[]) {
    gint retorno = existeTipoQuarto(tipo) ? 1 : -2;
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

static gint salvarTipoQuarto(void) {
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

static gint carregarTipoQuartos(void) {
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

static void imprimirTipoQuartos() {
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        tipo_quarto = (TIPO_QUARTO*) tq->data;
        printf("%s %.2f\n", tipo_quarto->tipo, tipo_quarto->valor);
    }
    g_slist_free(tq);
}

static gboolean existeQuarto(gchar numero[]) {
    GSList *q = NULL;
    QUARTO *quarto;
    gboolean existe = FALSE;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        if(strcmp(quarto->numero, numero) == 0) {
            existe = TRUE;
        }
    }
    g_slist_free(q);
    return existe;
}

static gint compararQuartos(QUARTO *a, QUARTO *b) {
    return strcmp(a->numero, b->numero);
}

static gint adicionarQuarto(gchar numero[], gchar tipo[]) {
    gint retorno = existeQuarto(numero) ? -1 : 1;
    if(retorno == 1) {
        QUARTO *quarto = g_new(QUARTO, 1);
        sprintf(quarto->numero, "%s", numero);
        sprintf(quarto->tipo, "%s", tipo);
        quarto->disponivel = TRUE;
        quartos = g_slist_insert_sorted(quartos, quarto, (GCompareFunc) compararQuartos);
        retorno = salvarQuarto();
    }
    return retorno;
}

static gint editarQuarto(gchar numero[], gchar novo_numero[], gchar novo_tipo[]) {
    gint retorno = novo_numero != numero && existeQuarto(novo_numero) ? -2 : 1;
    if(retorno == 1) {
        retorno = existeQuarto(numero) ? 1 : -1;
        if(retorno == 1) {
            GSList *q = NULL;
            QUARTO *quarto;
            for(q = quartos; q != NULL; q = q->next) {
                quarto = (QUARTO*) q->data;
                if(strcmp(quarto->numero, numero) == 0) {
                    sprintf(quarto->numero, "%s", novo_numero);
                    sprintf(quarto->tipo, "%s", novo_tipo);
                    retorno = salvarQuarto();
                }
            }
            g_slist_free(q);
        }
    }
    return retorno;
}

static gint removerQuarto(gchar numero[]) {
    gint retorno = existeQuarto(numero) ? 1 : -1;
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

static gint totalQuartosTipo(gchar tipo[]) {
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

static gfloat valorQuarto(gchar tipo[]) {
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

static void reservarQuarto(gchar numero[]) {
    GSList *q = NULL;
    QUARTO *quarto;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        if(strcmp(quarto->numero, numero) == 0) {
            quarto->disponivel = FALSE;
        }
    }
    g_slist_free(q);
}

static void liberarQuarto(gchar numero[]) {
    GSList *q = NULL;
    QUARTO *quarto;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        if(strcmp(quarto->numero, numero) == 0) {
            quarto->disponivel = TRUE;
        }
    }
    g_slist_free(q);
}

static gint salvarQuarto(void) {
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

static gint carregarQuartos(void) {
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

static void imprimirQuartos(void) {
    GSList *q = NULL;
    QUARTO *quarto;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        printf("%s %s %.2f %s\n", quarto->numero, quarto->tipo, valorQuarto(quarto->tipo), quarto->disponivel ? "Disponível" : "Ocupado");
    }
    g_slist_free(q);
}

static gboolean existeServico(gchar tipo[]) {
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

static gint compararServicos(SERVICO *a, SERVICO *b) {
    return strcmp(a->tipo, b->tipo);
}

static gint adicionarServico(gchar tipo[], gfloat valor) {
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

static gint editarServico(gchar tipo[], gchar novo_tipo[], gfloat novo_valor) {
    gint retorno = novo_tipo != tipo && existeServico(novo_tipo) ? -2 : 1;
    if(retorno == 1) {
        retorno = existeServico(tipo) ? 1 : -1;
        if(retorno == 1) {
            GSList *s = NULL;
            SERVICO *servico;
            for(s = servicos; s != NULL; s = s->next) {
                servico = (SERVICO*) s->data;
                if(strcmp(servico->tipo, tipo) == 0) {
                    sprintf(servico->tipo, "%s", novo_tipo);
                    servico->valor = novo_valor;
                    retorno = salvarServico();
                }
            }
            g_slist_free(s);
        }
    }
    return retorno;
}

static gint removerServico(gchar tipo[]) {
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

static gint salvarServico(void) {
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

static gint carregarServicos(void) {
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

static void imprimirServicos(void) {
    GSList *s = NULL;
    SERVICO *servico;
    for(s = servicos; s != NULL; s = s->next) {
        servico = (SERVICO*) s->data;
        printf("%s %.2f\n", servico->tipo, servico->valor);
    }
    g_slist_free(s);
}
