#include "interface.c"

static gchar *utf8(char *string);
static GtkWidget *criarJanela(GtkWidget *window, gchar *titulo, int largura, int altura);
static GtkWidget *criarBotao(GtkWidget *botao, gchar *titulo, GtkWidget *box, GCallback callback, gpointer data);
static void janelaPrincipal(GtkApplication *app, gpointer data);
static void barraDeMenu(GApplication *app, gpointer data);
static void janelaSobre(GSimpleAction *simple, GVariant *parameter, gpointer data);
static void janelaClientes(GtkApplication *app, gpointer data);
static void janelaCadastroCliente(GtkApplication *app, gpointer data);
static void renderizarListaClientes(GtkWidget *lista_clientes);
static void visualizarCliente(GtkApplication *app, gpointer data);
static void removerCliente(GtkWidget *widget, gpointer selection);
static void salvarCliente(GtkApplication *app, gpointer data);
static void fecharDialogo(GtkDialog *dialog, gint response_id, gpointer data);
static void fecharJanela(GtkWidget *widget, gpointer data);
static void sairPrograma(GSimpleAction *simple, GVariant *parameter, gpointer data);

static GtkWidget *janela_cadastro_cliente;
static GtkWidget *lista_clientes;
