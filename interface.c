#include <gtk/gtk.h>

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

static GtkWidget *janela_cadastro_cliente = NULL;
static GtkWidget *lista_clientes = NULL;

typedef enum {
    COL_NOME = 0,
    COL_CPF,
    COL_ENDERECO,
    N_COLUMNS
} COL_CLIENTES;

static gchar *utf8(char *string) {
    return g_convert(string, -1, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
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
    GtkWidget *btn_clientes = NULL;
    GtkWidget *btn_contratos = NULL;
    GtkWidget *btn_quartos = NULL;
    GtkWidget *btn_servicos = NULL;
    GtkWidget *btn_sair = NULL;

    window = gtk_application_window_new(app);
    window = criarJanela(window, "Sistema Hotel Campina Confort Premium", 500, 450);

    box = gtk_box_new(TRUE, 5);
    gtk_container_set_border_width (GTK_CONTAINER(box), 50);

    btn_clientes = criarBotao(btn_clientes, "Clientes", box, G_CALLBACK(janelaClientes), NULL);
    btn_contratos = criarBotao(btn_contratos, "Contratos", box, G_CALLBACK(janelaClientes), NULL);
    btn_quartos = criarBotao(btn_quartos, "Quartos", box, G_CALLBACK(janelaClientes), NULL);
    btn_servicos = criarBotao(btn_servicos, utf8("Serviços"), box, G_CALLBACK(janelaClientes), NULL);
    btn_sair = criarBotao(btn_sair, "Sair", box, G_CALLBACK(fecharJanela), window);

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
}

static void barraDeMenu(GApplication *app, gpointer data) {
    GMenu *menu;
    GSimpleAction *btn_sobre;
    GSimpleAction *btn_sair;

    menu = g_menu_new();
    g_menu_append(menu, "Sobre", "app.sobre");
    g_menu_append(menu, "Sair", "app.sair");

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
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialogo_sobre), utf8("Projeto de conclusão da disciplina de Laboratório de Programação I, ministrada pelo professor Danilo Abreu, do curso de Ciências da Computação da Universidade Estadual da Paraíba - UEPB."));
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
    GtkWidget *btn_adicionar = NULL, *btn_remover = NULL, *btn_visualizar = NULL, *btn_editar = NULL, *btn_fechar = NULL;

    window = criarJanela(window, "Clientes", 470, 370);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    lista_clientes = gtk_tree_view_new();
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(lista_clientes), TRUE);
    renderizarListaClientes(lista_clientes);
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
    btn_remover = criarBotao(btn_remover, "Remover", hbox, G_CALLBACK(removerCliente), selection);
    btn_editar = criarBotao(btn_editar, "Editar", hbox, G_CALLBACK(janelaCadastroCliente), selection);
    btn_visualizar = criarBotao(btn_visualizar, "Visualizar", hbox, G_CALLBACK(visualizarCliente), selection);
    btn_fechar = criarBotao(btn_fechar, "Fechar", hbox, G_CALLBACK(fecharJanela), window);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
}

static void renderizarListaClientes(GtkWidget *lista_clientes) {
    GtkCellRenderer    *renderer;
    GtkTreeViewColumn  *col_nome, *col_cpf, *col_endereco;
    GtkListStore       *store;

    renderer = gtk_cell_renderer_text_new();
    col_nome = gtk_tree_view_column_new_with_attributes("Nome", renderer, "text", COL_NOME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_clientes), col_nome);

    renderer = gtk_cell_renderer_text_new();
    col_cpf = gtk_tree_view_column_new_with_attributes("CPF", renderer, "text", COL_CPF, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_clientes), col_cpf);

    renderer = gtk_cell_renderer_text_new();
    col_endereco = gtk_tree_view_column_new_with_attributes(utf8("Endereço"), renderer, "text", COL_ENDERECO, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(lista_clientes), col_endereco);

    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING); //G_TYPE_UINT

    gtk_tree_view_set_model(GTK_TREE_VIEW(lista_clientes), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

static void visualizarCliente(GtkApplication *app, gpointer data) {
}

static void removerCliente(GtkWidget *widget, gpointer selection) {
  GtkListStore *store;
  GtkTreeModel *model;
  GtkTreeIter  iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes)));
  model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes));

  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
      return;
  }

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection),
         &model, &iter)) {
    gtk_list_store_remove(store, &iter);
  }
}

static void salvarCliente(GtkApplication *app, gpointer data) {
    GtkListStore *store;
    GtkTreeIter iter;

    const gchar *str = gtk_entry_get_text(data);

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_clientes)));

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, COL_NOME, str, -1);
    gtk_list_store_set(store, &iter, COL_CPF, "teste", -1);
    gtk_list_store_set(store, &iter, COL_ENDERECO, "teste", -1);

    fecharJanela(NULL, janela_cadastro_cliente);
}

static void janelaCadastroCliente(GtkApplication *app, gpointer entry) {
    GtkWidget *table;

    GtkWidget *label_nome;
    GtkWidget *label_cpf;
    GtkWidget *label_endereco;
    GtkWidget *label_salvar;

    GtkWidget *nome;
    GtkWidget *cpf;
    GtkWidget *endereco;

    janela_cadastro_cliente = NULL;

    janela_cadastro_cliente = criarJanela(janela_cadastro_cliente, "Cadastro de Cliente", 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(janela_cadastro_cliente), 10);

    table = gtk_table_new(3, 2, FALSE);
    gtk_container_add(GTK_CONTAINER(janela_cadastro_cliente), table);

    label_nome = gtk_label_new("Nome");
    label_cpf = gtk_label_new("CPF");
    label_endereco = gtk_label_new(utf8("Endereço"));
    label_salvar = gtk_label_new("");

    gtk_table_attach(GTK_TABLE(table), label_nome, 0, 1, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_cpf, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_endereco, 0, 1, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), label_salvar, 0, 1, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    nome = gtk_entry_new();
    cpf = gtk_entry_new();
    endereco = gtk_entry_new();

    GtkWidget *salvar;
    salvar = gtk_button_new_with_label("Salvar");

    gtk_table_attach(GTK_TABLE(table), nome, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), cpf, 1, 2, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), endereco, 1, 2, 2, 3, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
    gtk_table_attach(GTK_TABLE(table), salvar, 1, 2, 3, 4, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

    g_signal_connect(G_OBJECT(salvar), "clicked", G_CALLBACK(salvarCliente), nome);

    gtk_widget_show_all(janela_cadastro_cliente);

}
