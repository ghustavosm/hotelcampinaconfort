typedef struct {
    gchar nome[60], cpf[20], endereco[200];
} CLIENTE;

typedef struct {
    gchar tipo[30];
    gfloat valor;
} TIPO_QUARTO;

typedef struct {
    gint dia;
    gint mes;
    gint ano;
} DATA;

typedef struct {
    gchar numero[10];
    gchar tipo[30];
    gchar reservas[740][11];
} QUARTO;

typedef struct {
    gchar quarto[10];
    gchar cpf[20];
    DATA inicio;
    DATA fim;
} RESERVA;

typedef struct {
    gchar tipo[30];
    gfloat valor;
} SERVICO;

typedef struct {
    gchar cpf[20];
    gchar tipo[30];
    gint quantidade;
} SERVICO_CONTRATADO;

typedef struct {
    gchar cpf[20];
    gchar quarto;
    DATA inicio;
    DATA fim;
} CONTRATO;

gint quebra_string(const gchar *str, gchar c, gchar ***arr);
gint anoBissexto(gint ano);
gint diasDoMes(gint mes, gint ano);
gint diaDoAno(DATA data);
GSList *gerarDatas(DATA data_inicio, DATA data_fim);
gint totalDias(DATA data_inicio, DATA data_fim);

CLIENTE *pegarCliente(gchar cpf[]);
gchar *pegarCpfCliente(gchar nome[]);
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
gfloat valorTipoQuarto(gchar tipo[]);
gint salvarTipoQuartos(void);
gint carregarTipoQuartos(void);

QUARTO *pegarQuarto(gchar numero[]);
gint compararQuartos(QUARTO *a, QUARTO *b);
gint posicaoQuarto(gchar numero[]);
gint adicionarQuarto(gchar numero[], gchar tipo[]);
gint removerQuarto(gchar numero[]);
gint totalQuartosTipo(gchar tipo[]);
gfloat valorQuarto(gchar numero[]);
gint salvarQuartos(void);
gint carregarQuartos(void);

RESERVA *pegarReserva(gchar cpf[]);
gint reservarQuarto(gchar cpf[], gchar quarto[], DATA inicio, DATA fim);
gboolean disponibilidadeQuarto(gchar numero[], DATA inicio, DATA fim);
gint liberarReserva(gchar cpf[]);
gint salvarReservas(void);
gint carregarReservas(void);

SERVICO *pegarServico(gchar tipo[]);
gint adicionarServico(gchar tipo[], gfloat valor);
gint removerServico(gchar tipo[]);
gfloat valorServico(gchar tipo[]);
gint salvarServicos(void);
gint carregarServicos(void);
void imprimirServicos(void);

/*CONTRATO *pegarContrato(gchar cpf[]);
gint criarContrato(gchar cpf[], gchar quarto[], DATA inicio, DATA fim);
gint terminarContrato(gchar cpf[], gchar quarto[], DATA inicio, DATA fim);
gint salvarContratos(void);
gint carregarContratos(void);*/

GSList *clientes;
GSList *tipo_quartos;
GSList *quartos;
GSList *reservas;
GSList *servicos;
GSList *servicos_contratados;
GSList *contratos;
