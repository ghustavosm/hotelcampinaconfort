#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gmodule.h>
#include <locale.h>
#include <windows.h>
#include "structs.h"

static GtkWidget *criarJanela(GtkWidget *window, gchar *titulo, int largura, int altura);
static GtkWidget *criarBotao(GtkWidget *botao, gchar *titulo, GtkWidget *box, GCallback callback, gpointer data);
static void janelaPrincipal(GtkApplication *app, gpointer data);
static void barraDeMenu(GApplication *app, gpointer data);
static void janelaSobre(GSimpleAction *simple, GVariant *parameter, gpointer data);
static void fecharDialogo(GtkDialog *dialog, gint response_id, gpointer data);
static void fecharJanela(GtkWidget *widget, gpointer data);
static void sairPrograma(GSimpleAction *simple, GVariant *parameter, gpointer data);

static void janelaClientes(GtkApplication *app, gpointer data);
static void renderizarListaClientes();
static void janelaCadastroCliente(GtkApplication *app, gpointer data);
static void acaoSalvarCliente(GtkApplication *app, gpointer data);
static void acaoRemoverCliente(GtkWidget *widget, gpointer selection);

static void janelaTipoQuartos(GtkApplication *app, gpointer data);
static void renderizarListaTipoQuartos();
static void janelaCadastroTipoQuarto(GtkApplication *app, gpointer entry);
static void acaoSalvarTipoQuarto(GtkApplication *app, gpointer data);
static void acaoRemoverTipoQuarto(GtkWidget *widget, gpointer selection);

static void janelaServicos(GtkApplication *app, gpointer data);
static void renderizarListaServicos();
static void janelaCadastroServico(GtkApplication *app, gpointer selection);
static void acaoSalvarServico(GtkApplication *app, gpointer data);
static void acaoRemoverServico(GtkWidget *widget, gpointer selection);

static void janelaQuartos(GtkApplication *app, gpointer data);
static void renderizarListaQuartos();
static void janelaCadastroQuarto(GtkApplication *app, gpointer selection);
static void acaoSalvarQuarto(GtkApplication *app, gpointer data);
static void acaoRemoverQuarto(GtkWidget *widget, gpointer selection);

static void janelaCheckin(GtkApplication *app, gpointer data);
static void renderizarListaCheckin();
static void janelaFazerCheckin(GtkApplication *app, gpointer selection);
static void acaoFazerCheckin(GtkApplication *app, gpointer data);

static void janelaCheckout(GtkApplication *app, gpointer data);
static void janelaFazerCheckout(GtkApplication *app, gpointer selection);
static void acaoFazerCheckout(GtkApplication *app, gpointer data);

static GtkWidget *janela_cadastro_cliente = NULL;
static GtkWidget *lista_clientes = NULL;

static GtkWidget *janela_cadastro_tipo_quartos = NULL;
static GtkWidget *lista_tipo_quartos = NULL;

static GtkWidget *janela_cadastro_quarto = NULL;
static GtkWidget *lista_quartos = NULL;

static GtkWidget *janela_cadastro_servico = NULL;
static GtkWidget *lista_servicos = NULL;

static GtkWidget *janela_checkin = NULL;
static GtkWidget *lista_checkin = NULL;

static GtkWidget *janela_checkout = NULL;

typedef enum {
    COL_NOME_C = 0,
    COL_CPF_C,
    COL_ENDERECO_C,
    N_COLUMNS
} COL_CLIENTES;

typedef enum {
    COL_NOME_R = 0,
    COL_CPF_R,
    COL_QUARTO_R,
    COL_INICIO_R,
    COL_FIM_R,
    N_COLUMNS_R,
} COL_RESERVAS;

typedef enum {
    COL_TIPO_TQ = 0,
    COL_PRECO_TQ,
    N_COLUMNS_TQ
} COL_TIPO_QUARTOS;

typedef enum {
    COL_NUMERO_Q = 0,
    COL_TIPO_Q,
    COL_PRECO_Q,
    N_COLUMNS_Q
} COL_QUARTOS;

typedef enum {
    COL_TIPO_S = 0,
    COL_PRECO_S,
    N_COLUMNS_S
} COL_SERVICOS;

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");

    int status;
    GtkApplication *app;
    GList *icons = NULL;

    printf("%s\n", carregarClientes() == 1 ? "Clientes carregados com sucesso!" : "Não foi possível carregar os clientes!");
    printf("%s\n", carregarTipoQuartos() == 1 ? "Tipo quartos carregados com sucesso!" : "Não foi possível carregar os tipos de quartos!");
    printf("%s\n", carregarQuartos() == 1 ? "Quartos carregados com sucesso!" : "Não foi possível carregar os quartos!");
    printf("%s\n", carregarReservas() == 1 ? "Reservas carregadas com sucesso!" : "Não foi possível carregar as reservas!");
    printf("%s\n", carregarServicos() == 1 ? "Servicos carregados com sucesso!" : "Não foi possível carregar os servicos!");
    printf("%s\n", carregarServicosContratados() == 1 ? "Servicos contratados carregados com sucesso!" : "Não foi possível carregar os servicos contratados!");
    //printf("%s\n", carregarContratos() == 1 ? "Contratos carregados com sucesso!" : "Não foi possível carregar os contratos!");

    /*adicionarCliente("Gustavo Silva", "75429843067", "Rua Manuel Leonardo Gomes");
    adicionarCliente("Larissa Guimarães", "52901248020", "Rua Henry Leandro Leonardo Dias");
    adicionarCliente("Tiago Medeiros", "17745320003", "Rua Levi César Aparício");
    adicionarCliente("João Augusto", "91646177002", "Rua Martin Caio Souza");
    adicionarCliente("Irisnete Maria", "04513040041", "Rua Bruno Pedro Caleb Campos");
    adicionarCliente("José Samuel", "72389066003", "Rua Luís Bernardo da Mota");
    adicionarCliente("Lucas Gabriel", "89773759083", "Rua Yago Otávio Lopes");
    adicionarCliente("Fátima Passos", "19753735043", "Rua Aprígio Nepomuceno");
    adicionarCliente("Thamara Sales", "29753735043", "Rua Aprígio Nepomuceno");

    adicionarTipoQuarto("Bronze", 90);
    adicionarTipoQuarto("Prata", 130);
    adicionarTipoQuarto("Ouro", 180);
    adicionarTipoQuarto("Platina", 250);
    adicionarTipoQuarto("Diamante", 250);

    adicionarQuarto("301", "Platina");
    adicionarQuarto("101", "Bronze");
    adicionarQuarto("207", "Platina");
    adicionarQuarto("001", "Prata");
    adicionarQuarto("405", "Ouro");
    adicionarQuarto("302", "Bronze");
    adicionarQuarto("507", "Bronze");

    adicionarServico("Aluguel de carro de luxo", 300);
    adicionarServico("Seguro para carro de luxo", 100);
    adicionarServico("Aluguel de carro executivo", 150);
    adicionarServico("Seguro para carro executivo", 100);
    adicionarServico("Tanque cheio", 300);
    adicionarServico("Babysitter", 100);*/

    DATA reserva_inicio = {2, 3, 2013};
    DATA reserva_fim = {5, 3, 2013};

    //reservarQuarto("75429843067", "101", reserva_inicio, reserva_fim);
    //reservarQuarto("75429843067", "101", (DATA) {28, 12, 2011}, (DATA) {1, 3, 2012});

    RESERVA *reserva = pegarReserva("75429843067");
    printf("%d/%d/%d\n", reserva->inicio.dia, reserva->inicio.mes, reserva->inicio.ano);
    printf("%d/%d/%d\n", reserva->fim.dia, reserva->fim.mes, reserva->fim.ano);

    //liberarReserva("75429843067");
    printf("%s\n", disponibilidadeQuarto("101", reserva_inicio, reserva_fim) ? "Disponivel" : "Nao disponivel");

    /*GSList *datas = gerarDatas(reserva_inicio, reserva_fim);
    DATA *data_aux;
    GSList *d = NULL;
    for(d = datas; d != NULL; d = d->next) {
        data_aux = (DATA*) d->data;
        printf("%d/%d/%d\n", data_aux->dia, data_aux->mes, data_aux->ano);
    }*/

    //printf("%s\n", criarContrato("75429843067", "101", reserva_inicio, reserva_fim) == 1 ? "Contrato criado com sucesso!" : "Erro ao criar contrato!");


    //adicionarServicoContratado("75429843067", "Aluguel de carro de luxo", 1);
    //adicionarServicoContratado("75429843067", "Tanque cheio", 1);

    //SERVICO_CONTRATADO *servico_c = pegarServicoContratado("75429843067", "Tanque cheio");
    //alterarServicoContratado("75429843067", "Tanque cheio", 2);
    //printf("%s %s %d\n", servico_c->cpf, servico_c->tipo, servico_c->quantidade);

    //printf("%d\n", totalDias(reserva_inicio, reserva_fim));

    gchar **data = NULL;
    quebra_string("18/05/1989", '/', &data);
    printf("%d/%d/%d\n", atoi(data[0]), atoi(data[1]), atoi(data[2]));

    printf("%s\n", pegarCpfCliente("Gustavo Silva"));

    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-16.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-32.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-48.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-64.png", NULL));
    icons = g_list_append(icons, gdk_pixbuf_new_from_file("./img/icon-128.png", NULL));
    gtk_window_set_default_icon_list(icons);

    app = gtk_application_new("hotel.campina.confort", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(janelaPrincipal), NULL);
    g_signal_connect(app, "startup", G_CALLBACK(barraDeMenu), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}

static GtkWidget *criarJanela(GtkWidget *window, gchar *titulo, int largura, int altura) {
    if(window == NULL) {
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    }

    gtk_window_set_title(GTK_WINDOW(window), titulo);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    if(largura != 0 || altura != 0) {
        gtk_window_set_default_size(GTK_WINDOW(window), largura, altura);
    }
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    return window;
}

static GtkWidget *criarBotao(GtkWidget *botao, gchar *titulo, GtkWidget *box, GCallback callback, gpointer data) {
    botao = gtk_button_new_with_label(titulo);
    gtk_box_pack_start(GTK_BOX(box), botao, TRUE, TRUE, 0);
    g_signal_connect(botao, "clicked", callback, data);
    return botao;
}

static void fecharDialogo(GtkDialog *dialog, gint response_id, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void mostrarMensagem(gchar *mensagem) {
    GtkWidget *dialogo;
    GtkWidget *window_parent = gdk_screen_get_active_window(gdk_screen_get_default());
    dialogo = gtk_message_dialog_new(window_parent, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, mensagem);
    gtk_window_set_modal(GTK_WINDOW(dialogo), TRUE);
    gtk_window_set_position(GTK_WINDOW(dialogo), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_title(GTK_WINDOW(dialogo), "Aviso!");
    gtk_message_dialog_set_image(dialogo, gtk_image_new_from_icon_name("dialog-warning", GTK_ICON_SIZE_DIALOG));
    gtk_widget_show_all(dialogo);
    g_signal_connect(GTK_DIALOG(dialogo), "response", G_CALLBACK(fecharDialogo), NULL);
}

static void fecharJanela(GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data));
}

static void sairPrograma(GSimpleAction *simple, GVariant *parameter, gpointer data) {
    GApplication *application = data;
    g_application_quit(application);
}

static void janelaPrincipal(GtkApplication *app, gpointer data) {
    GtkWidget *window = NULL;
    GtkWidget *box = NULL;
    GtkWidget *btn_checkin = NULL;
    GtkWidget *btn_checkout = NULL;
    GtkWidget *btn_servicos = NULL;
    GtkWidget *btn_sair = NULL;

    window = gtk_application_window_new(app);
    window = criarJanela(window, "Sistema Hotel Campina Confort Premium", 500, 450);

    box = gtk_box_new(TRUE, 5);
    gtk_container_set_border_width (GTK_CONTAINER(box), 50);

    btn_checkin = criarBotao(btn_checkin, "Checkin", box, G_CALLBACK(janelaCheckin), NULL);
    btn_checkout = criarBotao(btn_checkout, "Checkout", box, G_CALLBACK(janelaCheckout), NULL);
    btn_servicos = criarBotao(btn_servicos, "Serviços", box, G_CALLBACK(janelaServicos), NULL);
    btn_sair = criarBotao(btn_sair, "Sair", box, G_CALLBACK(fecharJanela), window);

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
}

static void barraDeMenu(GApplication *app, gpointer data) {
    GMenu *menu;
    GMenu *cadastro;
    GSimpleAction *btn_clientes;
    GSimpleAction *btn_tipo_quartos;
    GSimpleAction *btn_quartos;
    GSimpleAction *btn_servicos;
    GSimpleAction *btn_sobre;
    GSimpleAction *btn_sair;

    menu = g_menu_new();
    cadastro = g_menu_new();
    g_menu_append_submenu(menu, "Cadastro", cadastro);
    g_menu_append(cadastro, "Clientes", "app.clientes");
    g_menu_append(cadastro, "Tipo de Quartos", "app.tipo_quartos");
    g_menu_append(cadastro, "Quartos", "app.quartos");
    g_menu_append(cadastro, "Serviços", "app.servicos");
    g_menu_append(menu, "Sobre", "app.sobre");
    g_menu_append(menu, "Sair", "app.sair");

    btn_clientes = g_simple_action_new("clientes", NULL);
    g_signal_connect(btn_clientes, "activate", G_CALLBACK(janelaClientes), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(btn_clientes));

    btn_tipo_quartos = g_simple_action_new("tipo_quartos", NULL);
    g_signal_connect(btn_tipo_quartos, "activate", G_CALLBACK(janelaTipoQuartos), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(btn_tipo_quartos));

    btn_quartos = g_simple_action_new("quartos", NULL);
    g_signal_connect(btn_quartos, "activate", G_CALLBACK(janelaQuartos), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(btn_quartos));

    btn_servicos = g_simple_action_new("servicos", NULL);
    g_signal_connect(btn_servicos, "activate", G_CALLBACK(janelaServicos), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(btn_servicos));

    btn_sobre = g_simple_action_new("sobre", NULL);
    g_signal_connect(btn_sobre, "activate", G_CALLBACK(janelaSobre), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(btn_sobre));

    btn_sair = g_simple_action_new("sair", NULL);
    g_signal_connect(btn_sair, "activate", G_CALLBACK(sairPrograma), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(btn_sair));

    gtk_application_set_app_menu(GTK_APPLICATION(app), G_MENU_MODEL(menu));
}

static void janelaSobre(GSimpleAction *simple, GVariant *parameter, gpointer data) {
    GtkWidget *dialogo_sobre = gtk_about_dialog_new();
    const gchar *autores[] = {"Gustavo Silva Medeiros", NULL};

    gtk_window_set_modal(GTK_WINDOW(dialogo_sobre), TRUE);
    gtk_window_set_position(GTK_WINDOW(dialogo_sobre), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialogo_sobre), gdk_pixbuf_new_from_file("./img/icon-128.png", NULL));
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialogo_sobre), "Sistema Hotel Campina Confort Premium");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialogo_sobre), "1.0.0");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialogo_sobre), "Projeto de conclusão da disciplina de Laboratório de Programação I, ministrada pelo professor Danilo Abreu, do curso de Ciências da Computação da Universidade Estadual da Paraíba - UEPB.");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialogo_sobre), "Copyright \xc2\xa9 2018 Gustavo Silva Medeiros");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialogo_sobre), "Github");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialogo_sobre), "https://github.com/ghustavosm/hotelcampinaconfort");
    gtk_about_dialog_add_credit_section(GTK_ABOUT_DIALOG(dialogo_sobre), "Criado por:", autores);
    g_signal_connect(GTK_DIALOG(dialogo_sobre), "response", G_CALLBACK(fecharDialogo), NULL);

    gtk_widget_show(dialogo_sobre);
}

static void janelaClientes(GtkApplication *app, gpointer data) {
    GtkTreeSelection *selection;
    GtkWidget *window = NULL;
    GtkWidget *scroll_window;
    GtkWidget *vbox, *hbox;
    GtkWidget *btn_adicionar = NULL, *btn_remover = NULL, *btn_editar = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Clientes", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_clientes = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_clientes), TRUE);
    renderizarListaClientes();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_clientes));

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll_window), GTK_SHADOW_ETCHED_IN);
    gtk_container_add(GTK_CONTAINER(scroll_window), lista_clientes);

    vbox = gtk_box_new(FALSE, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 5);

    hbox = gtk_box_new(FALSE, 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(hbox), GTK_ORIENTATION_HORIZONTAL);

    btn_adicionar = criarBotao(btn_adicionar, "Adicionar", hbox, G_CALLBACK(janelaCadastroCliente), NULL);
    btn_remover = criarBotao(btn_remover, "Remover", hbox, G_CALLBACK(acaoRemoverCliente), selection);
    btn_editar = criarBotao(btn_editar, "Editar", hbox, G_CALLBACK(janelaCadastroCliente), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void renderizarListaClientes() {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col_nome, *col_cpf, *col_endereco;
    GtkListStore *store;
    GSList *aux = NULL;

    renderer = gtk_cell_renderer_text_new();
    col_nome = gtk_tree_view_column_new_with_attributes("Nome", renderer, "text", COL_NOME_C, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_clientes), col_nome);

    renderer = gtk_cell_renderer_text_new();
    col_cpf = gtk_tree_view_column_new_with_attributes("CPF", renderer, "text", COL_CPF_C, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_clientes), col_cpf);

    renderer = gtk_cell_renderer_text_new();
    col_endereco = gtk_tree_view_column_new_with_attributes("Endereço", renderer, "text", COL_ENDERECO_C, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_clientes), col_endereco);

    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    CLIENTE *cliente;
    for(aux = clientes; aux != NULL; aux = aux->next) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        cliente = (CLIENTE*) aux->data;
        gtk_list_store_set(store, &iter,
                           COL_NOME_C, cliente->nome,
                           COL_CPF_C, cliente->cpf,
                           COL_ENDERECO_C, cliente->endereco, -1);
    }
    g_slist_free(aux);

    gtk_tree_view_set_model(GTK_TREE_VIEW(lista_clientes), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

static void janelaCadastroCliente(GtkApplication *app, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *table;
    GtkWidget *label_nome;
    GtkWidget *label_cpf;
    GtkWidget *label_endereco;
    GtkWidget *label_salvar;
    GtkWidget *nome;
    GtkWidget *cpf;
    GtkWidget *endereco;
    gchar *nome_value;
    gchar *cpf_value;
    gchar *endereco_value;

    janela_cadastro_cliente = NULL;

    janela_cadastro_cliente = criarJanela(janela_cadastro_cliente, "Cadastro de Cliente", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_cadastro_cliente), 10);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(janela_cadastro_cliente), table);

    label_nome = gtk_label_new("Nome");
    label_cpf = gtk_label_new("CPF");
    label_endereco = gtk_label_new("Endereço");
    label_salvar = gtk_label_new("");

    gtk_table_attach(GTK_TABLE(table), label_nome, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_cpf, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_endereco, 0, 1, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_salvar, 0, 1, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    nome = gtk_entry_new();
    cpf = gtk_entry_new();
    endereco = gtk_entry_new();

    if(selection != NULL) {
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes)));
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_NOME_C, &nome_value,  -1);
            gtk_tree_model_get(store, &iter, COL_CPF_C, &cpf_value,  -1);
            gtk_tree_model_get(store, &iter, COL_ENDERECO_C, &endereco_value,  -1);
        }
        gtk_entry_set_text(nome, nome_value);
        gtk_entry_set_text(cpf, cpf_value);
        gtk_entry_set_text(endereco, endereco_value);
    }

    GtkWidget *salvar;
    salvar = gtk_button_new_with_label("Salvar");

    gtk_table_attach(GTK_TABLE(table), nome, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), cpf, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), endereco, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), salvar, 1, 2, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    GSList *fields = NULL;
    fields = g_slist_append(fields, selection);
    fields = g_slist_append(fields, nome);
    fields = g_slist_append(fields, cpf);
    fields = g_slist_append(fields, endereco);

    g_signal_connect(G_OBJECT(salvar), "clicked", G_CALLBACK(acaoSalvarCliente), fields);

    gtk_widget_show_all(janela_cadastro_cliente);
}

static void acaoSalvarCliente(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *cpf_atual;
    gint retorno;

    gpointer *selection = g_slist_nth_data(data, 0);
    gchar *nome = gtk_entry_get_text(g_slist_nth_data(data, 1));
    gchar *cpf = gtk_entry_get_text(g_slist_nth_data(data, 2));
    gchar *endereco = gtk_entry_get_text(g_slist_nth_data(data, 3));

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes)));

    if(selection != NULL) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_CPF_C, &cpf_atual, -1);
            gtk_list_store_remove(store, &iter);
            removerCliente(cpf_atual);
        }
    }

    if((retorno = adicionarCliente(nome, cpf, endereco)) == 1) {
        gtk_list_store_insert(store, &iter, posicaoCliente(nome));
        gtk_list_store_set(store, &iter, COL_NOME_C, nome, -1);
        gtk_list_store_set(store, &iter, COL_CPF_C, cpf, -1);
        gtk_list_store_set(store, &iter, COL_ENDERECO_C, endereco, -1);
        if(selection != NULL) {
            gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        }
    } else {
        mostrarMensagem("Não foi possível cadastrar o cliente, pois já existe um cliente com o mesmo CPF.");
    }
    fecharJanela(NULL, janela_cadastro_cliente);
}

static void acaoRemoverCliente(GtkWidget *widget, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter  iter;
    gchar *cpf_cliente;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes));

    if(gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
        return;
    }

    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(store, &iter, COL_CPF_C, &cpf_cliente,  -1);
        gtk_list_store_remove(store, &iter);
        removerCliente(cpf_cliente);
    }
}

static void janelaTipoQuartos(GtkApplication *app, gpointer data) {
    GtkTreeSelection *selection;
    GtkWidget *window = NULL;
    GtkWidget *scroll_window;
    GtkWidget *vbox, *hbox;
    GtkWidget *btn_adicionar = NULL, *btn_remover = NULL, *btn_editar = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Tipo de Quartos", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_tipo_quartos = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_tipo_quartos), TRUE);
    renderizarListaTipoQuartos();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_tipo_quartos));

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll_window), GTK_SHADOW_ETCHED_IN);
    gtk_container_add(GTK_CONTAINER(scroll_window), lista_tipo_quartos);

    vbox = gtk_box_new(FALSE, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 5);

    hbox = gtk_box_new(FALSE, 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(hbox), GTK_ORIENTATION_HORIZONTAL);

    btn_adicionar = criarBotao(btn_adicionar, "Adicionar", hbox, G_CALLBACK(janelaCadastroTipoQuarto), NULL);
    btn_remover = criarBotao(btn_remover, "Remover", hbox, G_CALLBACK(acaoRemoverTipoQuarto), selection);
    btn_editar = criarBotao(btn_editar, "Editar", hbox, G_CALLBACK(janelaCadastroTipoQuarto), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void renderizarListaTipoQuartos() {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col_tipo, *col_preco;
    GtkListStore *store;
    GSList *aux = NULL;
    TIPO_QUARTO *tipo_quarto;

    renderer = gtk_cell_renderer_text_new();
    col_tipo = gtk_tree_view_column_new_with_attributes("Tipo", renderer, "text", COL_TIPO_TQ, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_tipo_quartos), col_tipo);

    renderer = gtk_cell_renderer_text_new();
    col_preco = gtk_tree_view_column_new_with_attributes("Preço", renderer, "text", COL_PRECO_TQ, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_tipo_quartos), col_preco);

    store = gtk_list_store_new(N_COLUMNS_TQ, G_TYPE_STRING, G_TYPE_STRING);

    gchar preco[10] = "";
    for(aux = tipo_quartos; aux != NULL; aux = aux->next) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        tipo_quarto = (TIPO_QUARTO*) aux->data;
        sprintf(preco, "R$ %.2f", tipo_quarto->valor);
        gtk_list_store_set(store, &iter,
                           COL_TIPO_TQ, tipo_quarto->tipo,
                           COL_PRECO_TQ, preco, -1);
    }

    g_slist_free(aux);
    gtk_tree_view_set_model(GTK_TREE_VIEW(lista_tipo_quartos), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

static void janelaCadastroTipoQuarto(GtkApplication *app, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *table;
    GtkWidget *label_tipo;
    GtkWidget *label_preco;
    GtkWidget *label_salvar;
    GtkWidget *tipo;
    GtkWidget *preco;
    gchar *tipo_value;
    gchar preco_value[15];

    janela_cadastro_tipo_quartos = NULL;

    janela_cadastro_tipo_quartos = criarJanela(janela_cadastro_tipo_quartos, "Cadastro Tipo de Quarto", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_cadastro_tipo_quartos), 10);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(janela_cadastro_tipo_quartos), table);

    label_tipo = gtk_label_new("Tipo");
    label_preco = gtk_label_new("Preço");
    label_salvar = gtk_label_new("");

    gtk_table_attach(GTK_TABLE(table), label_tipo, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_preco, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_salvar, 0, 1, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    tipo = gtk_entry_new();
    preco = gtk_entry_new();

    if(selection != NULL) {
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_tipo_quartos)));
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_tipo_quartos));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_TIPO_TQ, &tipo_value,  -1);
        }
        sprintf(preco_value, "%.2f", pegarTipoQuarto(tipo_value)->valor);
        gtk_entry_set_text(tipo, tipo_value);
        gtk_entry_set_text(preco, preco_value);
    }

    GtkWidget *salvar;
    salvar = gtk_button_new_with_label("Salvar");

    gtk_table_attach(GTK_TABLE(table), tipo, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), preco, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), salvar, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    GSList *fields = NULL;
    fields = g_slist_append(fields, selection);
    fields = g_slist_append(fields, tipo);
    fields = g_slist_append(fields, preco);

    g_signal_connect(G_OBJECT(salvar), "clicked", G_CALLBACK(acaoSalvarTipoQuarto), fields);

    gtk_widget_show_all(janela_cadastro_tipo_quartos);
}

static void acaoSalvarTipoQuarto(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *tipo_tmp;
    gint retorno;

    gpointer *selection = g_slist_nth_data(data, 0);
    gchar *tipo = gtk_entry_get_text(g_slist_nth_data(data, 1));
    gfloat preco = atof(gtk_entry_get_text(g_slist_nth_data(data, 2)));

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_tipo_quartos)));

    if(selection != NULL) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_tipo_quartos));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_TIPO_TQ, &tipo_tmp,  -1);
            gtk_list_store_remove(store, &iter);
            removerTipoQuarto(tipo_tmp);
        }
    }

    gchar preco_str[10] = "";
    if((retorno = adicionarTipoQuarto(tipo, preco)) == 1) {
        sprintf(preco_str, "R$ %.2f", preco);
        gtk_list_store_insert(store, &iter, posicaoTipoQuarto(tipo));
        gtk_list_store_set(store, &iter, COL_TIPO_TQ, tipo, -1);
        gtk_list_store_set(store, &iter, COL_PRECO_TQ, preco_str, -1);
        if(selection != NULL) {
            gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        }
    } else {
        mostrarMensagem("Não foi possível cadastrar o tipo de quarto, pois já existe outro do mesmo tipo.");
    }

    fecharJanela(NULL, janela_cadastro_tipo_quartos);
}

static void acaoRemoverTipoQuarto(GtkWidget *widget, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *tipo;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_tipo_quartos)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_tipo_quartos));

    if(gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
        return;
    }

    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(store, &iter, COL_TIPO_TQ, &tipo,  -1);
        if(removerTipoQuarto(tipo) == 1) {
            gtk_list_store_remove(store, &iter);
        } else {
            mostrarMensagem("Não foi possível remover o tipo de quarto, pois existem quartos cadastrados com este tipo.");
        }
    }
}

static void janelaServicos(GtkApplication *app, gpointer data) {
    GtkTreeSelection *selection;
    GtkWidget *window = NULL;
    GtkWidget *scroll_window;
    GtkWidget *vbox, *hbox;
    GtkWidget *btn_adicionar = NULL, *btn_remover = NULL, *btn_editar = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Serviços", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_servicos = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_servicos), TRUE);
    renderizarListaServicos();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_servicos));

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll_window), GTK_SHADOW_ETCHED_IN);
    gtk_container_add(GTK_CONTAINER(scroll_window), lista_servicos);

    vbox = gtk_box_new(FALSE, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 5);

    hbox = gtk_box_new(FALSE, 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(hbox), GTK_ORIENTATION_HORIZONTAL);

    btn_adicionar = criarBotao(btn_adicionar, "Adicionar", hbox, G_CALLBACK(janelaCadastroServico), NULL);
    btn_remover = criarBotao(btn_remover, "Remover", hbox, G_CALLBACK(acaoRemoverServico), selection);
    btn_editar = criarBotao(btn_editar, "Editar", hbox, G_CALLBACK(janelaCadastroServico), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void renderizarListaServicos() {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col_tipo, *col_preco;
    GtkListStore *store;
    GSList *aux = NULL;
    SERVICO *servico;

    renderer = gtk_cell_renderer_text_new();
    col_tipo = gtk_tree_view_column_new_with_attributes("Tipo", renderer, "text", COL_TIPO_S, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_servicos), col_tipo);

    renderer = gtk_cell_renderer_text_new();
    col_preco = gtk_tree_view_column_new_with_attributes("Preço", renderer, "text", COL_PRECO_S, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_servicos), col_preco);

    store = gtk_list_store_new(N_COLUMNS_S, G_TYPE_STRING, G_TYPE_STRING);

    gchar preco[10] = "";
    for(aux = servicos; aux != NULL; aux = aux->next) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        servico = (SERVICO*) aux->data;
        sprintf(preco, "R$ %.2f", servico->valor);
        gtk_list_store_set(store, &iter,
                           COL_TIPO_S, servico->tipo,
                           COL_PRECO_S, preco, -1);
    }

    g_slist_free(aux);
    gtk_tree_view_set_model(GTK_TREE_VIEW(lista_servicos), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

static void janelaCadastroServico(GtkApplication *app, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *table;
    GtkWidget *label_tipo;
    GtkWidget *label_preco;
    GtkWidget *label_salvar;
    GtkWidget *tipo;
    GtkWidget *preco;
    gchar *tipo_value;
    gchar preco_value[15];

    janela_cadastro_servico = NULL;

    janela_cadastro_servico = criarJanela(janela_cadastro_servico, "Cadastro de Serviços", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_cadastro_servico), 10);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(janela_cadastro_servico), table);

    label_tipo = gtk_label_new("Tipo");
    label_preco = gtk_label_new("Preço");
    label_salvar = gtk_label_new("");

    gtk_table_attach(GTK_TABLE(table), label_tipo, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_preco, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_salvar, 0, 1, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    tipo = gtk_entry_new();
    preco = gtk_entry_new();

    if(selection != NULL) {
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_servicos)));
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_servicos));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_TIPO_S, &tipo_value,  -1);
        }
        sprintf(preco_value, "%.2f", pegarServico(tipo_value)->valor);
        gtk_entry_set_text(tipo, tipo_value);
        gtk_entry_set_text(preco, preco_value);
    }

    GtkWidget *salvar;
    salvar = gtk_button_new_with_label("Salvar");

    gtk_table_attach(GTK_TABLE(table), tipo, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), preco, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), salvar, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    GSList *fields = NULL;
    fields = g_slist_append(fields, selection);
    fields = g_slist_append(fields, tipo);
    fields = g_slist_append(fields, preco);

    g_signal_connect(G_OBJECT(salvar), "clicked", G_CALLBACK(acaoSalvarServico), fields);

    gtk_widget_show_all(janela_cadastro_servico);
}

static void acaoSalvarServico(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *tipo_tmp;
    gint retorno;

    gpointer *selection = g_slist_nth_data(data, 0);
    gchar *tipo = gtk_entry_get_text(g_slist_nth_data(data, 1));
    gfloat preco = atof(gtk_entry_get_text(g_slist_nth_data(data, 2)));

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_servicos)));

    if(selection != NULL) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_servicos));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_TIPO_S, &tipo_tmp,  -1);
            gtk_list_store_remove(store, &iter);
            removerServico(tipo_tmp);
        }
    }

    gchar preco_str[10] = "";
    if((retorno = adicionarServico(tipo, preco)) == 1) {
        sprintf(preco_str, "R$ %.2f", preco);
        gtk_list_store_insert(store, &iter, posicaoServico(tipo));
        gtk_list_store_set(store, &iter, COL_TIPO_S, tipo, -1);
        gtk_list_store_set(store, &iter, COL_PRECO_S, preco_str, -1);
        if(selection != NULL) {
            gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        }
    } else {
        mostrarMensagem("Não foi possível cadastrar o tipo de quarto, pois já existe outro do mesmo tipo.");
    }

    fecharJanela(NULL, janela_cadastro_servico);
}

static void acaoRemoverServico(GtkWidget *widget, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *tipo;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_servicos)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_servicos));

    if(gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
        return;
    }

    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(store, &iter, COL_TIPO_S, &tipo,  -1);
        gtk_list_store_remove(store, &iter);
        removerServico(tipo);
    }
}

static void janelaQuartos(GtkApplication *app, gpointer data) {
    GtkTreeSelection *selection;
    GtkWidget *window = NULL;
    GtkWidget *scroll_window;
    GtkWidget *vbox, *hbox;
    GtkWidget *btn_adicionar = NULL, *btn_remover = NULL, *btn_editar = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Quartos", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_quartos = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_quartos), TRUE);
    renderizarListaQuartos();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_quartos));

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll_window), GTK_SHADOW_ETCHED_IN);
    gtk_container_add(GTK_CONTAINER(scroll_window), lista_quartos);

    vbox = gtk_box_new(FALSE, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 5);

    hbox = gtk_box_new(FALSE, 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(hbox), GTK_ORIENTATION_HORIZONTAL);

    btn_adicionar = criarBotao(btn_adicionar, "Adicionar", hbox, G_CALLBACK(janelaCadastroQuarto), NULL);
    btn_remover = criarBotao(btn_remover, "Remover", hbox, G_CALLBACK(acaoRemoverQuarto), selection);
    btn_editar = criarBotao(btn_editar, "Editar", hbox, G_CALLBACK(janelaCadastroQuarto), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void renderizarListaQuartos() {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col_numero, *col_tipo, *col_preco;
    GtkListStore *store;
    GSList *aux = NULL;
    QUARTO *quarto;

    renderer = gtk_cell_renderer_text_new();
    col_numero = gtk_tree_view_column_new_with_attributes("Número", renderer, "text", COL_NUMERO_Q, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_quartos), col_numero);

    renderer = gtk_cell_renderer_text_new();
    col_tipo = gtk_tree_view_column_new_with_attributes("Tipo", renderer, "text", COL_TIPO_Q, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_quartos), col_tipo);

    renderer = gtk_cell_renderer_text_new();
    col_preco = gtk_tree_view_column_new_with_attributes("Preço", renderer, "text", COL_PRECO_Q, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_quartos), col_preco);

    store = gtk_list_store_new(N_COLUMNS_Q, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    gchar preco[10] = "";
    for(aux = quartos; aux != NULL; aux = aux->next) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        quarto = (QUARTO*) aux->data;
        sprintf(preco, "R$ %.2f", valorQuarto(quarto->numero));
        gtk_list_store_set(store, &iter,
                           COL_NUMERO_Q, quarto->numero,
                           COL_TIPO_Q, quarto->tipo,
                           COL_PRECO_Q, preco, -1);
    }

    g_slist_free(aux);
    gtk_tree_view_set_model(GTK_TREE_VIEW(lista_quartos), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

static void janelaCadastroQuarto(GtkApplication *app, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *table;
    GtkWidget *label_numero;
    GtkWidget *label_tipo;
    GtkWidget *label_salvar;
    GtkWidget *numero;
    GtkWidget *tipo;
    gchar *numero_value;
    gchar *tipo_value;

    janela_cadastro_quarto = NULL;

    janela_cadastro_quarto = criarJanela(janela_cadastro_quarto, "Cadastro de Quartos", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_cadastro_quarto), 10);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(janela_cadastro_quarto), table);

    label_numero = gtk_label_new("Número");
    label_tipo = gtk_label_new("Tipo");
    label_salvar = gtk_label_new("");

    gtk_table_attach(GTK_TABLE(table), label_numero, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_tipo, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_salvar, 0, 1, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    numero = gtk_entry_new();

    tipo = gtk_combo_box_text_new();

    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto;
    for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
        tipo_quarto = (TIPO_QUARTO*) tq->data;
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(tipo), tipo_quarto->tipo);
    }
    g_slist_free(tq);

    if(selection != NULL) {
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_quartos)));
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_quartos));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_NUMERO_Q, &numero_value,  -1);
            gtk_tree_model_get(store, &iter, COL_TIPO_Q, &tipo_value,  -1);
        }
        gtk_entry_set_text(numero, numero_value);
        gtk_combo_box_set_active(GTK_COMBO_BOX(tipo), posicaoTipoQuarto(tipo_value));
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(tipo), 0);
    }

    GtkWidget *salvar;
    salvar = gtk_button_new_with_label("Salvar");

    gtk_table_attach(GTK_TABLE(table), numero, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), tipo, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), salvar, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    GSList *fields = NULL;
    fields = g_slist_append(fields, selection);
    fields = g_slist_append(fields, numero);
    fields = g_slist_append(fields, tipo);

    g_signal_connect(G_OBJECT(salvar), "clicked", G_CALLBACK(acaoSalvarQuarto), fields);

    gtk_widget_show_all(janela_cadastro_quarto);
}

static void acaoSalvarQuarto(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *numero_tmp;
    gint retorno;

    gpointer *selection = g_slist_nth_data(data, 0);
    gchar *numero = gtk_entry_get_text(g_slist_nth_data(data, 1));
    gchar *tipo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_slist_nth_data(data, 2)));

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_quartos)));

    if(selection != NULL) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_quartos));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_NUMERO_Q, &numero_tmp,  -1);
            gtk_list_store_remove(store, &iter);
            removerQuarto(numero_tmp);
        }
    }

    gchar preco[10] = "";
    sprintf(preco, "R$ %.2f", valorTipoQuarto(tipo));
    if((retorno = adicionarQuarto(numero, tipo)) == 1) {
        gtk_list_store_insert(store, &iter, posicaoQuarto(numero));
        gtk_list_store_set(store, &iter, COL_NUMERO_Q, numero, -1);
        gtk_list_store_set(store, &iter, COL_TIPO_Q, tipo, -1);
        gtk_list_store_set(store, &iter, COL_PRECO_Q, preco, -1);
        if(selection != NULL) {
            gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        }
    } else {
        mostrarMensagem("Não foi possível cadastrar o quarto, pois já existe outro com mesmo número.");
    }

    fecharJanela(NULL, janela_cadastro_quarto);
}

static void acaoRemoverQuarto(GtkWidget *widget, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *numero;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_quartos)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_quartos));

    if(gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
        return;
    }

    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(store, &iter, COL_NUMERO_Q, &numero,  -1);
        gtk_list_store_remove(store, &iter);
        printf("%d\n", removerQuarto(numero));
    }
}

static void janelaCheckin(GtkApplication *app, gpointer data) {
    GtkTreeSelection *selection;
    GtkWidget *window = NULL;
    GtkWidget *scroll_window;
    GtkWidget *vbox, *hbox;
    GtkWidget *btn_adicionar = NULL, *btn_editar = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Checkin", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_checkin = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_checkin), TRUE);
    renderizarListaCheckin();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_checkin));

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll_window), GTK_SHADOW_ETCHED_IN);
    gtk_container_add(GTK_CONTAINER(scroll_window), lista_checkin);

    vbox = gtk_box_new(FALSE, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 5);

    hbox = gtk_box_new(FALSE, 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(hbox), GTK_ORIENTATION_HORIZONTAL);

    btn_adicionar = criarBotao(btn_adicionar, "Adicionar", hbox, G_CALLBACK(janelaFazerCheckin), NULL);
    btn_editar = criarBotao(btn_editar, "Editar", hbox, G_CALLBACK(janelaFazerCheckin), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void renderizarListaCheckin() {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col_nome, *col_cpf, *col_quarto, *col_inicio, *col_fim;
    GtkListStore *store;
    GSList *aux = NULL;

    renderer = gtk_cell_renderer_text_new();
    col_nome = gtk_tree_view_column_new_with_attributes("Nome", renderer, "text", COL_NOME_R, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_checkin), col_nome);

    renderer = gtk_cell_renderer_text_new();
    col_cpf = gtk_tree_view_column_new_with_attributes("CPF", renderer, "text", COL_CPF_R, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_checkin), col_cpf);

    renderer = gtk_cell_renderer_text_new();
    col_quarto = gtk_tree_view_column_new_with_attributes("Quarto", renderer, "text", COL_QUARTO_R, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_checkin), col_quarto);

    renderer = gtk_cell_renderer_text_new();
    col_inicio = gtk_tree_view_column_new_with_attributes("Início", renderer, "text", COL_INICIO_R, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_checkin), col_inicio);

    renderer = gtk_cell_renderer_text_new();
    col_fim = gtk_tree_view_column_new_with_attributes("Fim", renderer, "text", COL_FIM_R, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_checkin), col_fim);

    store = gtk_list_store_new(N_COLUMNS_R, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    RESERVA *reserva;
    gchar data_inicio[15] = "";
    gchar data_fim[15] = "";
    CLIENTE *cliente;
    for(aux = reservas; aux != NULL; aux = aux->next) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        reserva = (CLIENTE*) aux->data;
        sprintf(data_inicio, "%d/%d/%d", reserva->inicio.dia, reserva->inicio.mes, reserva->inicio.ano);
        sprintf(data_fim, "%d/%d/%d", reserva->fim.dia, reserva->fim.mes, reserva->fim.ano);
        cliente = pegarCliente(reserva->cpf);
        gtk_list_store_set(store, &iter,
                           COL_NOME_R, cliente->nome,
                           COL_CPF_R, reserva->cpf,
                           COL_QUARTO_R, reserva->quarto,
                           COL_INICIO_R, data_inicio,
                           COL_FIM_R, data_fim, -1);
    }
    g_slist_free(aux);

    gtk_tree_view_set_model(GTK_TREE_VIEW(lista_checkin), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

static void janelaFazerCheckin(GtkApplication *app, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *table;
    GtkWidget *label_cliente;
    GtkWidget *label_quarto;
    GtkWidget *label_inicio;
    GtkWidget *label_fim;
    GtkWidget *label_salvar;
    GtkWidget *cliente;
    GtkWidget *quarto;
    GtkWidget *inicio;
    GtkWidget *fim;
    GtkWidget *salvar;
    gchar *cliente_value;
    gchar *quarto_value;
    gchar *cpf_value;
    gchar *inicio_value;
    gchar *fim_value;

    janela_checkin = NULL;

    janela_checkin = criarJanela(janela_checkin, "Fazer checkin", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_checkin), 10);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(janela_checkin), table);

    label_cliente = gtk_label_new("Cliente");
    label_quarto = gtk_label_new("Quarto");
    label_inicio = gtk_label_new("Início");
    label_fim = gtk_label_new("Fim");
    label_salvar = gtk_label_new("");

    gtk_table_attach(GTK_TABLE(table), label_cliente, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_quarto, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_inicio, 0, 1, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_fim, 0, 1, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_salvar, 0, 1, 4, 5, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    cliente = gtk_combo_box_text_new();
    quarto = gtk_combo_box_text_new();
    inicio = gtk_entry_new();
    fim = gtk_entry_new();

    GSList *aux = NULL;
    CLIENTE *cliente_aux;
    for(aux = clientes; aux != NULL; aux = aux->next) {
        cliente_aux = (CLIENTE*) aux->data;
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cliente), cliente_aux->nome);
    }
    g_slist_free(aux);

    GSList *q = NULL;
    QUARTO *quato_aux;
    for(q = quartos; q != NULL; q = q->next) {
        quato_aux = (QUARTO*) q->data;
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(quarto), quato_aux->numero);
    }
    g_slist_free(q);

    if(selection != NULL) {
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin)));
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin));
        if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
            gtk_tree_model_get(store, &iter, COL_NOME_R, &cliente_value,  -1);
            gtk_tree_model_get(store, &iter, COL_CPF_R, &cpf_value,  -1);
            gtk_tree_model_get(store, &iter, COL_QUARTO_R, &quarto_value,  -1);
            gtk_tree_model_get(store, &iter, COL_INICIO_R, &inicio_value,  -1);
            gtk_tree_model_get(store, &iter, COL_FIM_R, &fim_value,  -1);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(cliente), posicaoCliente(cliente_value));
        gtk_combo_box_set_active(GTK_COMBO_BOX(quarto), posicaoQuarto(quarto_value));
        gtk_entry_set_text(inicio, inicio_value);
        gtk_entry_set_text(fim, fim_value);
    } else {
        gtk_combo_box_set_active(GTK_COMBO_BOX(cliente), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(quarto), 0);
    }

    salvar = gtk_button_new_with_label("Salvar");

    gtk_table_attach(GTK_TABLE(table), cliente, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), quarto, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), inicio, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), fim, 1, 2, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), salvar, 1, 2, 4, 5, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    GSList *fields = NULL;
    fields = g_slist_append(fields, selection);
    fields = g_slist_append(fields, cliente);
    fields = g_slist_append(fields, quarto);
    fields = g_slist_append(fields, inicio);
    fields = g_slist_append(fields, fim);

    g_signal_connect(G_OBJECT(salvar), "clicked", G_CALLBACK(acaoFazerCheckin), fields);

    gtk_widget_show_all(janela_checkin);
}

static void acaoFazerCheckin(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;

    gpointer *selection = g_slist_nth_data(data, 0);
    gchar *cliente = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_slist_nth_data(data, 1)));
    gchar *cpf = pegarCpfCliente(cliente);
    gchar *quarto = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_slist_nth_data(data, 2)));
    gchar *inicio = gtk_entry_get_text(g_slist_nth_data(data, 3));
    gchar *fim = gtk_entry_get_text(g_slist_nth_data(data, 4));

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin)));

    gchar **data_inicio_aux = NULL;
    gchar **data_fim_aux = NULL;
    quebra_string(inicio, '/', &data_inicio_aux);
    quebra_string(fim, '/', &data_fim_aux);
    DATA data_inicio = (DATA) {atoi(data_inicio_aux[0]), atoi(data_inicio_aux[1]), atoi(data_inicio_aux[2])};
    DATA data_fim = (DATA) {atoi(data_fim_aux[0]), atoi(data_fim_aux[1]), atoi(data_fim_aux[2])};

    RESERVA *reserva = pegarReserva(cpf);
    if((selection != NULL)) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin));
        gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter);
        liberarReserva(cpf);
    }

    gint retorno = reservarQuarto(cpf, quarto, data_inicio, data_fim);
    if(retorno == 1) {
        gtk_list_store_insert(store, &iter, posicaoReserva(cpf));
        gtk_list_store_set(store, &iter, COL_NOME_R, cliente, -1);
        gtk_list_store_set(store, &iter, COL_CPF_R, cpf, -1);
        gtk_list_store_set(store, &iter, COL_QUARTO_R, quarto, -1);
        gtk_list_store_set(store, &iter, COL_INICIO_R, inicio, -1);
        gtk_list_store_set(store, &iter, COL_FIM_R, fim, -1);
        if(selection != NULL) {
            gtk_list_store_remove(store, &iter);
            gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        }
    } else if(retorno == -1) {
        mostrarMensagem("Não foi possível fazer o checkin, pois já existe outro com o mesmo cpf!");
    } else if(retorno == -2) {
        mostrarMensagem("Não foi possível fazer o checkin, pois o quarto não está disponível para esta data!");
        if((selection != NULL)) {
            reservarQuarto(reserva->cpf, reserva->quarto, reserva->inicio, reserva->fim);
            gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        }
    }

    fecharJanela(NULL, janela_checkin);
}

static void janelaCheckout(GtkApplication *app, gpointer data) {
    GtkTreeSelection *selection;
    GtkWidget *window = NULL;
    GtkWidget *scroll_window;
    GtkWidget *vbox, *hbox;
    GtkWidget *btn_checkout = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Checkout", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_checkin = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_checkin), TRUE);
    renderizarListaCheckin();
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_checkin));

    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll_window), GTK_SHADOW_ETCHED_IN);
    gtk_container_add(GTK_CONTAINER(scroll_window), lista_checkin);

    vbox = gtk_box_new(FALSE, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(vbox), GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(vbox), scroll_window, TRUE, TRUE, 5);

    hbox = gtk_box_new(FALSE, 5);
    gtk_orientable_set_orientation(GTK_ORIENTABLE(hbox), GTK_ORIENTATION_HORIZONTAL);

    btn_checkout = criarBotao(btn_checkout, "Chekout", hbox, G_CALLBACK(janelaFazerCheckout), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void janelaFazerCheckout(GtkApplication *app, gpointer selection) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *box;
    GtkWidget *label_fim;
    GtkWidget *label_salvar;
    GtkWidget *fim;
    GtkWidget *confirmar;
    gchar *cliente_value;
    gchar *quarto_value;
    gchar *cpf_value;
    gchar *inicio_value;
    gchar *fim_value;

    janela_checkout = NULL;

    janela_checkout = criarJanela(janela_checkout, "Fazer checkout", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_checkout), 10);

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin));
    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
        gtk_tree_model_get(store, &iter, COL_NOME_R, &cliente_value,  -1);
        gtk_tree_model_get(store, &iter, COL_CPF_R, &cpf_value,  -1);
        gtk_tree_model_get(store, &iter, COL_QUARTO_R, &quarto_value,  -1);
        gtk_tree_model_get(store, &iter, COL_INICIO_R, &inicio_value,  -1);
        gtk_tree_model_get(store, &iter, COL_FIM_R, &fim_value,  -1);
    }

    box = gtk_box_new(TRUE, 5);
    gtk_container_set_border_width (GTK_CONTAINER(box), 30);

    gchar **data_inicio_aux = NULL;
    gchar **data_fim_aux = NULL;
    quebra_string(inicio_value, '/', &data_inicio_aux);
    quebra_string(fim_value, '/', &data_fim_aux);
    DATA data_inicio = (DATA) {atoi(data_inicio_aux[0]), atoi(data_inicio_aux[1]), atoi(data_inicio_aux[2])};
    DATA data_fim = (DATA) {atoi(data_fim_aux[0]), atoi(data_fim_aux[1]), atoi(data_fim_aux[2])};

    gfloat total = valorQuarto(quarto_value) * totalDias(data_inicio, data_fim);

    char total_str[50] = "";
    sprintf(total_str, "Total a pagar:\nR$ %.2f\n", total);

    label_fim = gtk_label_new(total_str);

    fim = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), label_fim, TRUE, TRUE, 0);

    confirmar = gtk_button_new_with_label("Confirmar");
    gtk_box_pack_start(GTK_BOX(box), confirmar, TRUE, TRUE, 0);

    GSList *fields = NULL;
    fields = g_slist_append(fields, selection);
    fields = g_slist_append(fields, cliente_value);
    fields = g_slist_append(fields, quarto_value);
    fields = g_slist_append(fields, inicio_value);
    fields = g_slist_append(fields, fim_value);

    g_signal_connect(G_OBJECT(confirmar), "clicked", G_CALLBACK(acaoFazerCheckout), fields);

    gtk_container_add(GTK_CONTAINER(janela_checkout), box);
    gtk_widget_show_all(janela_checkout);
}

static void acaoFazerCheckout(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter iter;

    gpointer *selection = g_slist_nth_data(data, 0);
    gchar *cliente = g_slist_nth_data(data, 1);
    gchar *cpf = pegarCpfCliente(cliente);
    gchar *quarto = g_slist_nth_data(data, 2);
    gchar *inicio = g_slist_nth_data(data, 3);
    gchar *fim = g_slist_nth_data(data, 4);

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin)));

    gchar **data_inicio_aux = NULL;
    gchar **data_fim_aux = NULL;
    quebra_string(inicio, '/', &data_inicio_aux);
    quebra_string(fim, '/', &data_fim_aux);
    DATA data_inicio = (DATA) {atoi(data_inicio_aux[0]), atoi(data_inicio_aux[1]), atoi(data_inicio_aux[2])};
    DATA data_fim = (DATA) {atoi(data_fim_aux[0]), atoi(data_fim_aux[1]), atoi(data_fim_aux[2])};

    RESERVA *reserva = pegarReserva(cpf);
    if((selection != NULL)) {
        model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_checkin));
        gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter);
        gtk_list_store_remove(store, &iter);
        gtk_tree_selection_select_iter(GTK_TREE_SELECTION(selection), &iter);
        liberarReserva(cpf);
    }

    fecharJanela(NULL, janela_checkout);
}
