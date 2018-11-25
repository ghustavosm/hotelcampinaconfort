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

CLIENTE *pegarCliente(gchar cpf[]);
gint compararClientes(CLIENTE *a, CLIENTE *b);
gint posicaoCliente(gchar nome[]);
gint adicionarCliente(gchar nome[], gchar cpf[], gchar endereco[]);
gint removerCliente(gchar cpf[]);
gint salvarClientes(void);
gint carregarClientes(void);

TIPO_QUARTO *pegarTipoQuarto(gchar tipo[]);
gint compararTipoQuartos(TIPO_QUARTO *a, TIPO_QUARTO *b);
gint posicaoTipoQuarto(gchar tipo[]);
gint adicionarTipoQuarto(gchar tipo[], gfloat valor);
gint removerTipoQuarto(gchar tipo[]);
gint salvarTipoQuarto(void);
gint carregarTipoQuartos(void);

QUARTO *pegarQuarto(gchar numero[]);
gint compararQuartos(QUARTO *a, QUARTO *b);
gint posicaoQuarto(gchar numero[]);
gint adicionarQuarto(gchar numero[], gchar tipo[]);
gint removerQuarto(gchar numero[]);
gint totalQuartosTipo(gchar tipo[]);
gfloat valorQuarto(gchar tipo[]);
gboolean disponibilidadeQuarto(gchar numero[], gchar data[]);
gboolean reservarQuarto(gchar numero[], gchar data[]);
gboolean liberarQuarto(gchar numero[], gchar data[]);
gint salvarQuarto(void);
gint carregarQuartos(void);
void imprimirQuartos(void);

gboolean existeServico(gchar tipo[]);
gint compararServicos(SERVICO *a, SERVICO *b);
gint adicionarServico(gchar tipo[], gfloat valor);
gint removerServico(gchar tipo[]);
gint salvarServico(void);
gint carregarServicos(void);

GSList *clientes;
GSList *tipo_quartos;
GSList *quartos;
GSList *servicosL;
GSList *contratos;
