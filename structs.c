GSList *clientes = NULL;
GSList *tipo_quartos = NULL;
GSList *quartos = NULL;

typedef struct {
    gchar *nome, *cpf, *endereco;
} CLIENTE;

typedef struct {
    gchar *descricao;
    gfloat valor;
} TIPO_QUARTO;

typedef struct {
    gchar *numero;
    gchar *tipo;
    gboolean disponivel;
} QUARTO;

typedef struct {
    gint id;
    gchar *descricao;
    gfloat valor;
    gboolean disponivel;
} SERVICO;

typedef struct {
    gchar *cpf;
    GList *quartos;
    GList *servicos;
} CONTRATO;

gint compararClientes(gpointer *a, gpointer *b) {
    CLIENTE *cliente_a = (CLIENTE*) a;
    CLIENTE *cliente_b = (CLIENTE*) b;
    return strcmp(cliente_a->nome, cliente_b->nome);
}

gint compararTipoQuartos(gpointer *a, gpointer *b) {
    TIPO_QUARTO *tipo_quarto_a = (TIPO_QUARTO*) a;
    TIPO_QUARTO *tipo_quarto_b = (TIPO_QUARTO*) b;
    return strcmp(tipo_quarto_a->descricao, tipo_quarto_b->descricao);
}

gint compararQuartos(gpointer *a, gpointer *b) {
    QUARTO *quarto_a = (QUARTO*) a;
    QUARTO *quarto_b = (QUARTO*) b;
    return strcmp(quarto_a->numero, quarto_b->numero);
}

void adicionarCliente(gchar *nome, gchar *cpf, gchar *endereco) {
    CLIENTE *cliente = g_new(CLIENTE, 1);
    cliente->nome = nome;
    cliente->cpf = cpf;
    cliente->endereco = endereco;
    clientes = g_slist_insert_sorted(clientes, cliente, compararClientes);
}

void adicionarTipoQuarto(gchar *descricao, gfloat valor) {
    TIPO_QUARTO *tipo_quarto = g_new(TIPO_QUARTO, 1);
    tipo_quarto->descricao = descricao;
    tipo_quarto->valor = valor;
    tipo_quartos = g_slist_insert_sorted(tipo_quartos, tipo_quarto, compararTipoQuartos);
}

void adicionarQuarto(gchar *numero, gchar *descricao) {
    QUARTO *quarto = g_new(QUARTO, 1);
    quarto->numero = numero;
    quarto->tipo = descricao;
    quarto->disponivel = TRUE;
    quartos = g_slist_insert_sorted(quartos, quarto, compararQuartos);
}

float valorQuarto(gchar *descricao) {
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto;
    gfloat valor = -1.0;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        tipo_quarto = (TIPO_QUARTO*) tq->data;
        if(strcmp(tipo_quarto->descricao, descricao) == 0) {
            valor = tipo_quarto->valor;
        }
    }
    free(tipo_quarto);
    g_slist_free(tq);
    return valor;
}

float reservarQuarto(gchar *numero) {
    GSList *q = NULL;
    QUARTO *quarto;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        if(strcmp(quarto->numero, numero) == 0) {
            quarto->disponivel = FALSE;
        }
    }
    free(quarto);
    g_slist_free(q);
}

void imprimirClientes() {
    GSList *c = NULL;
    CLIENTE *cliente;
    for(c = clientes; c != NULL; c = c->next) {
        cliente = (CLIENTE*) c->data;
        printf("%s %s %s\n", cliente->nome, cliente->cpf, cliente->endereco);
    }
    free(cliente);
    g_slist_free(c);
}

void imprimirTipoQuartos() {
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        tipo_quarto = (TIPO_QUARTO*) tq->data;
        printf("%s %.2f\n", tipo_quarto->descricao, tipo_quarto->valor);
    }
    free(tipo_quarto);
    g_slist_free(tq);
}

void imprimirQuartos() {
    GSList *q = NULL;
    QUARTO *quarto;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        printf("%s %s %.2f %s\n", quarto->numero, quarto->tipo, valorQuarto(quarto->tipo), quarto->disponivel ? "Disponível" : "Ocupado");
    }
    free(quarto);
    g_slist_free(q);
}
