    //--------------------------------------------------------------
// NOMES DOS RESPONSÁVEIS: Mark Poll Herrmann
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>


// ######### ESCREVA O NROUSP DO PRIMEIRO INTEGRANTE AQUI
int nroUSP1() {
    return 11208291;
}

// ######### ESCREVA O NROUSP DO SEGUNDO INTEGRANTE AQUI (OU DEIXE COM ZERO)
int  nroUSP2() {
    return 0;
}

// ######### ESCREVA O NRO DO GRUPO CADASTRADO
int grupo() {
    return 0;
}

typedef struct aux {
	int bot_lin;
	int bot_col;
	int exit_lin;
	int exit_col;
	int box_col;
	int key_col;
	bool haskey;
	bool escapou;
	bool passou;
	struct aux * prox;
	int pos;
	char caminho;

} ESTADO;
typedef struct {
    ESTADO * arr;
    int livre;
} GRAFO;
bool comparaEstados (ESTADO a, ESTADO b) {
    if(a.bot_lin == b.bot_lin){
        if (a.bot_col == b.bot_col){
            if (a.box_col == b.box_col) {
                if (a.key_col == b.key_col) return true;
            }
        }
    }
    return false;

}
void imprimeGrafo (GRAFO * g) {
    ESTADO * comp;
    for (int i = 0; i < g->livre; i++) {
        comp = &g->arr[i];
        printf("(  %i  ) ",g->arr[i].bot_col);
        comp = comp->prox;
        while (comp != NULL) {
            //if (comp->prox == NULL)printf(" ME MATA ");
            printf("-> (%c) ",comp->caminho);
            comp = comp->prox;

        }
        printf(" \n");

    }
}
void mostraEstado (ESTADO * e) {
    printf("\n------------------");
    printf("\nbot_lin : %i\n",e->bot_lin);
    printf("bot_col : %i\n",e->bot_col);
    printf("box_col : %i\n",e->box_col);
    printf("key_col : %i\n",e->key_col);
    printf("caminho : %c\n",e->caminho);

}
void mostraGrafo (ESTADO ** g) {
    for (int i = 0; i < 50; i++) {
        printf(" septo %i ",g[i]->bot_col);
        if (g[i+1] ==  NULL) {
            mostraEstado(g[i-1]);
            mostraEstado(g[i]);
            printf("\n nenem %i",i);
            break;
        }
    }
}
void printResp (char ** resp, int * livre) {
    printf("\n{");
    for (int i = 0; i < *livre; i++) printf("%c, ",*resp[i]);
    printf("}\n");
}
ESTADO* executaComando (ESTADO comp, char c)
{
    ESTADO * s =  (ESTADO *) malloc(sizeof(ESTADO));
    *s = comp;
    switch (c) {
    case 'l' :
        if (s->bot_col > 1) s->bot_col--;
        if (s->bot_lin == 2)s->bot_lin++;
        break;
    case 'r' :
        if (s->bot_col < 3)s->bot_col++;
        if (s->bot_lin == 2)s->bot_lin++;
        break;
    case 'u' :
        if(s->bot_lin == 3 && s->bot_col == s->box_col)s->bot_lin--;
        break;
    case 'd' :
        if (s->bot_lin==2) s->bot_lin++;
        break;
    case 'L' :
        if (s->bot_lin == 3 && s->bot_col == s->box_col && s->bot_col > 1) {
            s->bot_col--;
            s->box_col--;
        }
        break;
    case 'R' :
        if (s->bot_lin == 3 && s->bot_col== s->box_col && s->bot_col < 3) {
            s->bot_col++;
            s->box_col++;
        }
        break;
    case 'g' :
        if (s->bot_lin == 2 && s->bot_col == s->key_col) {
            s->haskey = true;
            s->key_col = 0;
        }
        break;
    case 'e' :
        if (s->bot_lin == s->exit_lin && s->bot_col == s->exit_col && s->haskey == true) s->escapou = true;

    }
    if (comparaEstados(comp, *s) == true) {
        free(s);
        return NULL;
    }

    //printf("%i",s.bot_col);
    return s;
}
ESTADO * criaVertice (ESTADO * comp) {
    ESTADO * novo = (ESTADO *) malloc(sizeof(ESTADO));
    novo->bot_lin = comp->bot_lin;
    novo->bot_col = comp->bot_col;
    novo->caminho = NULL;
    novo->box_col = comp->box_col;
    novo->escapou = comp->escapou;
    novo->exit_col = comp->exit_col;
    novo->exit_lin = comp->exit_lin;
    novo->haskey = comp->haskey;
    novo->key_col = comp->key_col;
    novo->prox = NULL;
    novo->passou = false;
    return novo;

}
void adicionaVertices (GRAFO * g, ESTADO * atual, int arestas) {
    ESTADO * comp = atual;
    bool existe;
    for (int i = 0; i < arestas; i++) {
        existe = false;
        comp = comp->prox;
        for (int j = 0; j < g->livre; j++) {
            if (comparaEstados(g->arr[j],*comp) == true) {
                existe = true;
                comp->pos = j;
            }
        }
        if (existe == false) {
            comp->pos = g->livre;
            g->arr[g->livre] = *criaVertice(comp);
            g->arr[g->livre].pos = g->livre;
            g->livre = g->livre + 1;
        }
    }
}
int criaArestas (ESTADO * atual) {
    char alfabeto [8] = {'l','r','u','d','L','R','g','e'};
    ESTADO * comp = atual;
    atual->prox =  NULL;
    int a = 0;
    ESTADO * proximo;
    for (int j = 0; j < 8; j++) {
        proximo = executaComando(*atual,alfabeto[j]);
        if (proximo != NULL) {
            comp->prox = proximo;
            proximo->caminho = alfabeto[j];
            proximo->prox = NULL;
            comp = comp->prox;
            a++;
            }
    }
    return a;
}
void InicializaG (ESTADO* s, GRAFO * g){
    g->arr[0] = *s;
    g->livre = 1;
    int arestas;
    for (int i = 0; i < g->livre; i++) {
        arestas = 0;
        arestas = criaArestas(&g->arr[i]);
        adicionaVertices(g,&g->arr[i], arestas);
    }

}
ESTADO * procuraSolucao (GRAFO * g, ESTADO * sol, char ** resp, int * livre) {          //sol->bot_col != sol->exit_col && sol->bot_lin != sol->exit_lin && sol->key_col != 0 && sol->haskey != true
    if (sol ==  NULL) return NULL;
    if (sol->passou ==  true)return NULL;
    if (sol->bot_col == sol->exit_col && sol->bot_lin == sol->exit_lin && sol->key_col == 0 && sol->haskey == true) {
        *resp[*livre] = 'e';
        *livre = *livre + 1;
        sol->escapou = true;
        sol->passou = true;
        return sol;
    }
    sol->passou = true;
    ESTADO * comp;
    sol = sol->prox;

    while (sol != NULL) {
        comp = procuraSolucao(g,&g->arr[sol->pos],resp,livre);
        if (comp != NULL) {
                if (comp->escapou == true){
                    *resp[*livre] = sol->caminho;
                    *livre = *livre + 1;
                    sol->escapou = true;
                    return comp;

                }
        }
        sol = sol->prox;

    }
    return NULL;

}
void arrumaResp (char ** resp, int * livre) {
    char * resp2 = (char *) malloc(sizeof(char)* *livre);
    int j = 0;
    for (int i = *livre - 1; i >=0; i--) {
        resp2[j] = *resp[i];
        j++;
    }
    for (int k = 0; k < *livre; k++) {
        *resp[k] = resp2[k];
    }
}
bool soluciona (GRAFO * g, ESTADO * sol) {
    if (sol->passou == true) return false;
    if (sol->bot_col == sol->exit_col && sol->bot_lin == sol->exit_lin && sol->key_col == 0 && sol->haskey == true) {
        sol->escapou = true;
        sol->passou = true;
        return true;
    }
    bool estado;
    sol->passou = true;
    sol = sol->prox;
    while (sol != NULL) {
        estado = soluciona(g,&g->arr[sol->pos]);
        if (estado == true) {
            sol->escapou = true;
            return true;
        }
        if (sol->prox == NULL) break;
        sol = sol->prox;
    }
    return false;
}


// funcao principal
void caminhoValido(ESTADO* s, char* resp);


void caminhoValido(ESTADO* s, char* resp)
{
    if (s->bot_lin < 2) return;
    if (s->bot_lin == 2 && s->box_col != s->bot_col) return;
    s->passou = false;
    s->escapou = false;
	s->prox = NULL;
	s->pos = 0;
	s->caminho = NULL;
	GRAFO * g = (GRAFO *) malloc(sizeof(GRAFO));
    g->arr = (ESTADO *)malloc(sizeof(ESTADO)*24);
    g->livre = 0;
    InicializaG(s, g);
    imprimeGrafo(g);
    int livre = 0;
    bool estado = soluciona(g,&g->arr[0]);
    if (estado == false) return;
    ESTADO * comp;
    comp = &g->arr[0];
    while (comp != NULL) {
        if (comp->escapou == true) {
            resp[livre] = comp->caminho;
            livre++;
            comp = &g->arr[comp->pos];
        }
        if (comp->prox == NULL) break;
        comp = comp->prox;
    }
    resp[livre] = 'e';
    livre++;
    printf("{ ");
    for (int i = 0; i < livre; i++) {
        printf("%c ,",resp[i]);
    }
    printf("} \n");
    free(g);
    return;

	// seu codigo AQUI

}




//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
// mas nao entregue o codido de main() nem inclua nada
// abaixo deste ponto
//---------------------------------------------------------
int main()
{
	// exemplo de teste do exemplo 1 da especificacao do EP
	char* resp = (char*) malloc(sizeof(char)*500);;
	ESTADO* s = (ESTADO*) malloc(sizeof(ESTADO));
	s->bot_lin=2;  s->bot_col=1;
	s->exit_lin=2; s->exit_col=1;
	s->box_col=1;
	s->key_col=3;
	s->haskey=false;
	caminhoValido(s, resp);
	printf("chegou");
	return 1;
}


