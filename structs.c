#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gmodule.h>
#define NOME_ARQUIVO_CLIENTES "clientes.dat"
#define NOME_ARQUIVO_TIPO_QUARTOS "tipo_quartos.dat"
#define NOME_ARQUIVO_QUARTOS "quartos.dat"
#define NOME_ARQUIVO_RESERVAS "reservas.dat"
#define NOME_ARQUIVO_SERVICOS "servicos.dat"
#define NOME_ARQUIVO_SERVICOS_CONTRATADOS "servicos_contratados.dat"
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
} QUARTO;

typedef struct {
    gint dia;
    gint mes;
    gint ano;
} DATA;

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

GSList *clientes = NULL;
GSList *tipo_quartos = NULL;
GSList *quartos = NULL;
GSList *reservas = NULL;
GSList *servicos = NULL;
GSList *servicos_contratados = NULL;
GSList *contratos = NULL;

gint quebra_string(const gchar *str, gchar c, gchar ***arr) {
    gint count = 1;
    gint token_len = 1;
    gint i = 0;
    gchar *p;
    gchar *t;

    p = (gchar*) str;
    while(*p != '\0') {
        if(*p == c) {
            count++;
		}
        p++;
    }

    *arr = (gchar**) malloc(sizeof(gchar*) * count);
    if(*arr == NULL) {
		exit(1);
	}

    p = (gchar*) str;
    while(*p != '\0') {
        if(*p == c) {
            (*arr)[i] = (gchar*) malloc(sizeof(gchar) * token_len);
            if((*arr)[i] == NULL) {
                exit(1);
			}
            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (gchar*) malloc(sizeof(gchar) * token_len);
    if((*arr)[i] == NULL) {
        exit(1);
	}

    i = 0;
    p = (gchar*) str;
    t = ((*arr)[i]);
    while(*p != '\0') {
        if(*p != c && *p != '\0') {
            *t = *p;
            t++;
        }
        else {
			*t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

	*t = '\0';
    return count;
}

gint anoBissexto(gint ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

gint diasDoMes(gint mes, gint ano) {
  gint dias_dos_meses[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  dias_dos_meses[2] = anoBissexto(ano) ? dias_dos_meses[2] + 1: dias_dos_meses[2];
  return dias_dos_meses[mes];
}

gint diaDoAno(DATA data) {
    gint i = 0, dia_do_ano = 0;
    for (i = 0; i < data.mes; i++) {
        dia_do_ano += diasDoMes(i, data.ano);
    }
    return (dia_do_ano + data.dia);
}

GSList *gerarDatas(DATA data_inicio, DATA data_fim) {
    GSList *datas = NULL;

    gint ano_aux = data_inicio.ano;
    gint mes_aux = data_inicio.mes;
    gint dia_aux = data_inicio.dia;

    gint continuar = 1;
    while(ano_aux <= data_fim.ano && continuar) {
        while(mes_aux <= 12 && continuar) {
            while(dia_aux <= diasDoMes(mes_aux, ano_aux) && continuar) {
                DATA *data = g_new(DATA, 1);
                data->dia = dia_aux;
                data->mes = mes_aux;
                data->ano = ano_aux;
                datas = g_slist_append(datas, data);
                //printf("%d/%d/%d\n", dia_aux, mes_aux, ano_aux);
                if(dia_aux == data_fim.dia && mes_aux == data_fim.mes && ano_aux == data_fim.ano) {
                    continuar = 0;
                }
                dia_aux++;
            }
            dia_aux = 1;
            mes_aux++;
        }
        mes_aux = 1;
        ano_aux++;
    }
    return datas;
}

gint totalDias(DATA data_inicio, DATA data_fim) {
    GSList *datas = gerarDatas(data_inicio, data_fim);
    return g_slist_length(datas);
}

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

gchar *pegarCpfCliente(gchar nome[]) {
    GSList *c = NULL;
    CLIENTE *cliente = NULL;
    CLIENTE *aux;
    for(c = clientes; c != NULL; c = c->next) {
        aux = (CLIENTE*) c->data;
        if(strcmp(aux->nome, nome) == 0) {
            cliente = aux;
            break;
        }
    }
    return cliente->cpf;
}

gchar *pegarNomeCliente(gchar cpf[]) {
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
    return cliente->nome;
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
            fwrite(cliente, sizeof(CLIENTE), 1, arquivo);
        }
        retorno = 1;
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
        retorno = salvarTipoQuartos();
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
            retorno = salvarTipoQuartos();
        }
    }
    return retorno;
}

gfloat valorTipoQuarto(gchar tipo[]) {
	TIPO_QUARTO *tipo_quarto = pegarTipoQuarto(tipo);
    return tipo_quarto->valor;
}

gint salvarTipoQuartos(void) {
    FILE *arquivo;
    GSList *tq = NULL;
    TIPO_QUARTO *tipo_quarto = g_new(TIPO_QUARTO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_TIPO_QUARTOS, "w")) != NULL) {
        for(tq = tipo_quartos; tq != NULL; tq = tq->next) {
            tipo_quarto = (TIPO_QUARTO*) tq->data;
            fwrite(tipo_quarto, sizeof(TIPO_QUARTO), 1, arquivo);
        }
        retorno = 1;
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

gint posicaoQuarto(gchar numero[]) {
    GSList *q = NULL;
    QUARTO *quarto;
    gint posicao = 0;
    for(q = quartos; q != NULL; q = q->next) {
        quarto = (QUARTO*) q->data;
        if(strcmp(quarto->numero, numero) >= 0) {
            break;
        }
        posicao++;
    }
    return posicao;
}

gint adicionarQuarto(gchar numero[], gchar tipo[]) {
    gint retorno = pegarQuarto(numero) ? -1 : 1;
    if(retorno == 1) {
        QUARTO *quarto = g_new(QUARTO, 1);
        sprintf(quarto->numero, "%s", numero);
        sprintf(quarto->tipo, "%s", tipo);
        quartos = g_slist_insert_sorted(quartos, quarto, (GCompareFunc) compararQuartos);
        retorno = salvarQuartos();
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
        retorno = salvarQuartos();
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

gfloat valorQuarto(gchar numero[]) {
    QUARTO *quarto = pegarQuarto(numero);
    TIPO_QUARTO *tipo_quarto = pegarTipoQuarto(quarto->tipo);
    return tipo_quarto->valor;
}

gint salvarQuartos(void) {
    FILE *arquivo;
    GSList *q = NULL;
    QUARTO *quarto = g_new(QUARTO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_QUARTOS, "w")) != NULL) {
        for(q = quartos; q != NULL; q = q->next) {
            quarto = (QUARTO*) q->data;
            fwrite(quarto, sizeof(QUARTO), 1, arquivo);
        }
        retorno = 1;
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

RESERVA *pegarReserva(gchar cpf[]) {
    GSList *r = NULL;
    RESERVA *reserva = NULL;
    RESERVA *aux;
    for(r = reservas; r != NULL; r = r->next) {
        aux = (RESERVA*) r->data;
        if((strcmp(aux->cpf, cpf) == 0)) {
            reserva = aux;
            break;
        }
    }
    return reserva;
}

gint compararReservas(RESERVA *a, RESERVA *b) {
    return strcmp(pegarNomeCliente(a->cpf), pegarNomeCliente(b->cpf));
}

gint posicaoReserva(gchar cpf[]) {
    GSList *r = NULL;
    RESERVA *reserva;
    gint posicao = 0;
    for(r = reservas; r != NULL; r = r->next) {
        reserva = (RESERVA*) r->data;
        if(strcmp(pegarNomeCliente(reserva->cpf), pegarNomeCliente(cpf)) >= 0) {
            break;
        }
        posicao++;
    }
    return posicao;
}

gint reservarQuarto(gchar cpf[], gchar quarto[], DATA inicio, DATA fim) {
    gint retorno = pegarReserva(cpf) ? -1 : 1;
    if(retorno == 1) {
        retorno = disponibilidadeQuarto(quarto, inicio, fim) ? 1 : -2;
    }
    if(retorno == 1) {
        RESERVA *reserva = g_new(RESERVA, 1);
        sprintf(reserva->cpf, "%s", cpf);
        sprintf(reserva->quarto, "%s", quarto);
        reserva->inicio = inicio;
        reserva->fim = fim;
        reservas = g_slist_insert_sorted(reservas, reserva, (GCompareFunc) compararReservas);
        retorno = salvarReservas();
    }
    return retorno;
}

gboolean disponibilidadeQuarto(gchar quarto[], DATA inicio, DATA fim) {
	gboolean retorno = TRUE;
	RESERVA *reserva;
	DATA *data_reserva;
	DATA *data_checagem;
	GSList *r = NULL;
	GSList *d = NULL;
	GSList *d2 = NULL;
	for(r = reservas; r != NULL && retorno; r = r->next) {
		reserva = (RESERVA*) r->data;
		if(strcmp(reserva->quarto, quarto) == 0 && retorno) {
            for(d = gerarDatas(reserva->inicio, reserva->fim); d != NULL && retorno; d = d->next) {
                data_reserva = (DATA*) d->data;
                for(d2 = gerarDatas(inicio, fim); d2 != NULL && retorno; d2 = d2->next) {
                    data_checagem = (DATA*) d2->data;
                    if(data_reserva->dia == data_checagem->dia && data_reserva->mes == data_checagem->mes && data_reserva->ano == data_checagem->ano) {
                        retorno = FALSE;
                    }
                }
            }
		}
	}
	return retorno;
}

gint liberarReserva(gchar cpf[]) {
    gboolean retorno = 0;
    GSList *r = NULL;
    RESERVA *reserva = NULL;
    RESERVA *aux;
    for(r = reservas; r != NULL; r = r->next) {
        aux = (RESERVA*) r->data;
        if((strcmp(aux->cpf, cpf) == 0)) {
            reservas = g_slist_delete_link(reservas, r);
            retorno = salvarReservas();
            break;
        }
    }
    return retorno;
}

gint salvarReservas(void) {
    FILE *arquivo;
    GSList *r = NULL;
    RESERVA *reserva = g_new(RESERVA, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_RESERVAS, "w")) != NULL) {
        for(r = reservas; r != NULL; r = r->next) {
            reserva = (RESERVA*) r->data;
            fwrite(reserva, sizeof(RESERVA), 1, arquivo);
        }
        retorno = 1;
    }

    fclose(arquivo);
    g_slist_free(r);

    return retorno;
}

gint carregarReservas(void) {
    FILE *arquivo;
    RESERVA *reserva;
    gint retorno = 1;

    if((arquivo = fopen(NOME_ARQUIVO_RESERVAS, "r")) == NULL) {
        if((arquivo = fopen(NOME_ARQUIVO_RESERVAS, "w")) == NULL) {
            retorno = 0;
        }
    }

    while(TRUE) {
        reserva = g_new(RESERVA, 1);
        if(fread(reserva, sizeof(RESERVA), 1, arquivo)) {
            reservas = g_slist_append(reservas, reserva);
        } else {
            break;
        }
    }
    fclose(arquivo);
    return retorno;
}

SERVICO *pegarServico(gchar tipo[]) {
    GSList *s = NULL;
    SERVICO *servico = NULL;
    SERVICO *aux;
    for(s = servicos; s != NULL; s = s->next) {
        aux = (SERVICO*) s->data;
        if(strcmp(aux->tipo, tipo) == 0) {
            servico = aux;
            break;
        }
    }
    return servico;
}

gint compararServicos(SERVICO *a, SERVICO *b) {
    return strcmp(a->tipo, b->tipo);
}

gint posicaoServico(gchar tipo[]) {
    GSList *s = NULL;
    SERVICO *servico;
    gint posicao = 0;
    for(s = servicos; s != NULL; s = s->next) {
        servico = (SERVICO*) s->data;
        if(strcmp(servico->tipo, tipo) >= 0) {
            break;
        }
        posicao++;
    }
    return posicao;
}

gint adicionarServico(gchar tipo[], gfloat valor) {
    gint retorno = pegarServico(tipo) ? -1 : 1;
    gint i;
    if(retorno == 1) {
        SERVICO *servico = g_new(SERVICO, 1);
        sprintf(servico->tipo, "%s", tipo);
        servico->valor = valor;
        servicos = g_slist_insert_sorted(servicos, servico, (GCompareFunc) compararServicos);
        retorno = salvarServicos();
    }
    return retorno;
}

gint removerServico(gchar tipo[]) {
    gint retorno = pegarServico(tipo) ? 1 : -1;
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
        retorno = salvarServicos();
    }
    return retorno;
}

gfloat valorServico(gchar tipo[]) {
	SERVICO *servico = pegarServico(tipo);
    return servico->valor;
}

gint salvarServicos(void) {
    FILE *arquivo;
    GSList *s = NULL;
    SERVICO *servico = g_new(SERVICO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_SERVICOS, "w")) != NULL) {
        for(s = servicos; s != NULL; s = s->next) {
            servico = (SERVICO*) s->data;
            fwrite(servico, sizeof(SERVICO), 1, arquivo);
        }
        retorno = 1;
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








SERVICO_CONTRATADO *pegarServicoContratado(gchar cpf[], gchar tipo[]) {
    GSList *s = NULL;
    SERVICO_CONTRATADO *servico = NULL;
    SERVICO_CONTRATADO *aux;
    for(s = servicos_contratados; s != NULL; s = s->next) {
        aux = (SERVICO_CONTRATADO*) s->data;
        if((strcmp(aux->cpf, cpf) == 0) && (strcmp(aux->tipo, tipo) == 0)) {
            servico = aux;
            break;
        }
    }
    return servico;
}

gint compararServicosContratados(SERVICO_CONTRATADO *a, SERVICO_CONTRATADO *b) {
    return strcmp(a->tipo, b->tipo);
}

gint posicaoServicoContratado(gchar tipo[]) {
    GSList *s = NULL;
    SERVICO_CONTRATADO *servico;
    gint posicao = 0;
    for(s = servicos_contratados; s != NULL; s = s->next) {
        servico = (SERVICO_CONTRATADO*) s->data;
        if(strcmp(servico->tipo, tipo) >= 0) {
            break;
        }
        posicao++;
    }
    return posicao;
}

gint adicionarServicoContratado(gchar cpf[], gchar tipo[], gint quantidade) {
    gint retorno = pegarServicoContratado(cpf, tipo) ? -1 : 1;
    gint i;
    if(retorno == 1) {
        SERVICO_CONTRATADO *servico = g_new(SERVICO_CONTRATADO, 1);
        sprintf(servico->cpf, "%s", cpf);
        sprintf(servico->tipo, "%s", tipo);
        servico->quantidade = quantidade;
        servicos_contratados = g_slist_insert_sorted(servicos_contratados, servico, (GCompareFunc) compararServicosContratados);
        retorno = salvarServicosContratados();
    }
    return retorno;
}

gint alterarServicoContratado(gchar cpf[], gchar tipo[], gint quantidade) {
    gint retorno = pegarServicoContratado(cpf, tipo) ? 1 : -1;
    if(retorno == 1) {
        GSList *s = NULL;
        SERVICO_CONTRATADO *servico;
        for(s = servicos_contratados; s != NULL; s = s->next) {
            servico = (SERVICO_CONTRATADO*) s->data;
            if((strcmp(servico->cpf, cpf) == 0) && (strcmp(servico->tipo, tipo) == 0)) {
                servico->quantidade = quantidade;
            }
        }
        g_slist_free(s);
        retorno = salvarServicosContratados();
    }
    return retorno;
}

gint removerServicoContratado(gchar cpf[], gchar tipo[]) {
    gint retorno = pegarServicoContratado(cpf, tipo) ? 1 : -1;
    if(retorno == 1) {
        GSList *s = NULL;
        SERVICO_CONTRATADO *servico;
        for(s = servicos_contratados; s != NULL; s = s->next) {
            servico = (SERVICO_CONTRATADO*) s->data;
            if((strcmp(servico->cpf, cpf) == 0) && (strcmp(servico->tipo, tipo) == 0)) {
                servicos_contratados = g_slist_delete_link(servicos_contratados, s);
            }
        }
        g_slist_free(s);
        retorno = salvarServicosContratados();
    }
    return retorno;
}

gint salvarServicosContratados(void) {
    FILE *arquivo;
    GSList *s = NULL;
    SERVICO_CONTRATADO *servico = g_new(SERVICO_CONTRATADO, 1);
    gint retorno = 0;

    if ((arquivo = fopen(NOME_ARQUIVO_SERVICOS_CONTRATADOS, "w")) != NULL) {
        for(s = servicos_contratados; s != NULL; s = s->next) {
            servico = (SERVICO_CONTRATADO*) s->data;
            fwrite(servico, sizeof(SERVICO_CONTRATADO), 1, arquivo);
        }
        retorno = 1;
    }

    fclose(arquivo);
    g_slist_free(s);
    return retorno;
}

gint carregarServicosContratados(void) {
    FILE *arquivo;
    SERVICO_CONTRATADO *servico;
    gint retorno = 1;

    if((arquivo = fopen(NOME_ARQUIVO_SERVICOS_CONTRATADOS, "r")) == NULL) {
        if((arquivo = fopen(NOME_ARQUIVO_SERVICOS_CONTRATADOS, "w")) == NULL) {
            retorno = 0;
        }
    }

    while(TRUE) {
        servico = g_new(SERVICO_CONTRATADO, 1);
        if(fread(servico, sizeof(SERVICO_CONTRATADO), 1, arquivo)) {
            servicos_contratados = g_slist_append(servicos_contratados, servico);
        } else {
            break;
        }
    }
    fclose(arquivo);
    return retorno;
}
