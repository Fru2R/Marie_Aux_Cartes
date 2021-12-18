//
//  main.c
//  Marie aux Cartes
//
//  Created by David Laguesse on 12/12/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define NBR_COUPS_MAX 50 // pour limiter le nombre de coups d'une partie

// Structure pour les cartes. Label contient le libélé de la carte.
// Valeur contient la valeur de la carte (de 2 à 14 pour l'as)
typedef struct{
    char* couleur;
    int valeur;
    char* label;
}carte_t;

// Structure pour les paquests de cartes. Elle contient le nombre de cartes du paquet
// et les cartes qui le composent.
typedef struct{
    int nombre_cartes;
    carte_t carte[52];
}paquet_t;

// Structure des joueurs. Id contient le numéro du joueur,
// paquet contient les cartes dans la main du joueur
typedef struct{
    int id;
    paquet_t paquet;
    char* atout;
    int score;
}joueur_t;

// PRE: recoit identite, qui sert à définir le numéro de joueur,
// PRE: ainsi que le nombre de cartes qui lui seront distribuées en début de partie.
//POST: retourne un nouveau joueur, dont l'identité a été passée en argument
joueur_t creer_joueur(int identite, int nombre_cartes){
    joueur_t nouveau_joueur;
    nouveau_joueur.id=identite;     // l'identité du joueur est celle passée en argument
    nouveau_joueur.paquet.nombre_cartes=nombre_cartes;
    nouveau_joueur.paquet.carte[nombre_cartes];
    nouveau_joueur.score=0;
    char* couleurs[4]={"Pique","Trèfle","Coeur","Carreau"};
    int i=5;
    while(i!=1 && i!=2 && i!=3 && i!=4){
        printf("J%i - Choisissez votre atout: 1=Pique, 2=Trèfle, 3=Coeur, 4=Carreau\n",identite);
        scanf(" %i",&i);
    }
    nouveau_joueur.atout=couleurs[i-1];
    return nouveau_joueur;
}

// PRE: recoit une couleur et une valeur en entrée
//POST: retourne une carte cree, avec sa valeur, sa couleur et un label
carte_t nouvelle_carte(int valeur, char* couleur){
    carte_t carte;
    carte.couleur = couleur;
    carte.valeur = valeur;
    if(valeur==2) carte.label="2";
    else if(valeur==3) carte.label="3";
    else if(valeur==4) carte.label="4";
    else if(valeur==5) carte.label="5";
    else if(valeur==6) carte.label="6";
    else if(valeur==7) carte.label="7";
    else if(valeur==8) carte.label="8";
    else if(valeur==9) carte.label="9";
    else if(valeur==10) carte.label="10";
    else if(valeur==11) carte.label="Valet";
    else if(valeur==12) carte.label="Dame";
    else if(valeur==13) carte.label="Roi";
    else carte.label="As";
    return carte;
}

// PRE: recoit une taille de paquet en entrée
//POST: retourne un paquet, contenant autant de cartes que demandé
paquet_t creer_paquet(int taille){
    paquet_t paquet;
    paquet.carte[taille];
    paquet.nombre_cartes=taille;
    int i;
    char* couleurs[4]={"Pique","Trèfle","Coeur","Carreau"};
    for(i=0;i<taille;i++){
        paquet.carte[i]=nouvelle_carte((i%13)+2,couleurs[i%4]);
    }
    return paquet;
}

// PRE: reçoit la référence d'un paquet de cartes qu'il faut mélanger
//POST: ne retourne aucune valeur, mais ré-arrange aléatoirement le paquet de cartes passé en arguments
void melanger_paquet(paquet_t *paquet_a_melanger){
    // creation d'une variable temporaire pour copier les valeurs vers le paquet à mélanger
    paquet_t copie_paquet;
    // creation d'un tableau d'entiers indiquant 1 si une carte n'a pas encore été sélectionnée
    int check_disponible[paquet_a_melanger->nombre_cartes];
    copie_paquet.nombre_cartes=paquet_a_melanger->nombre_cartes;
    for(int i=0;i<copie_paquet.nombre_cartes;i++){
        copie_paquet.carte[i]=paquet_a_melanger->carte[i];
        check_disponible[i]=1;
    }
    
    // pour chaque élément du paquet à mélanger, on identifie un élément de la copie de manière aléatoire
    // et on vérifie s'il a déjà été sélectionné (check_disponible==0) auquel cas, on essaie une autre carte aléatoire
    for(int j=0;j<paquet_a_melanger->nombre_cartes;j++){
        int tmp=rand()%copie_paquet.nombre_cartes;
        while(check_disponible[tmp]==0){
            tmp=rand()%copie_paquet.nombre_cartes;
        }
        // on copie la carte sélectionnée aléatoirement dans la pile finale et on flag la carte comme plus disponible
        paquet_a_melanger->carte[j]=copie_paquet.carte[tmp];
        check_disponible[tmp]=0;
    }
}

// PRE: reçoit la référence vers un paquet de cartes, et de deux joueurs
//POST: ne renvoie aucun resultat, distribue le paquet de cartes entre les 2 joueurs, une carte à la fois, chacun à son tour
void distribuer_cartes(paquet_t *paquet, joueur_t *joueur1, joueur_t *joueur2){
    int i=0;
    int compte_carte_joueur1=0;
    int compte_carte_joueur2=0;
    while(i<paquet->nombre_cartes){
        joueur1->paquet.carte[compte_carte_joueur1]=paquet->carte[i];
        joueur2->paquet.carte[compte_carte_joueur2]=paquet->carte[i+1];
        compte_carte_joueur1++;
        compte_carte_joueur2++;
        i=i+2;
    }
    paquet->carte[0];
    paquet->nombre_cartes=0;
}

// PRE: reçoit en entrée la référence vers les deux joueurs, ainsi que la couleur de la dernière carte qu'ils ont jouée,
// PRE: la référence vers le paquet de cartes en jeu, le type de partie jouée (pour calcul score mode atout)
// PRE: et le nombre de cartes que le paquet en jeu contient
//POST: ne renvoie aucune valeur, mais assigne les cartes en jeu au gagnant et vide le paquet de cartes en jeu
void gagner_coup(joueur_t *gagnant, char* couleur_gagnant, joueur_t *perdant, char* couleur_perdant, paquet_t *cartes_en_jeu, int nbr_cartes, int type_partie){
    // on stocke le nombre de cartes du gagnant avant de distribuer les gains
    int nbr_cartes_avant=gagnant->paquet.nombre_cartes;
    
    // on augmente le nombre de cartes du gagnant en tenant compte de ses gains
    gagnant->paquet.nombre_cartes+=nbr_cartes;
    gagnant->paquet.carte[gagnant->paquet.nombre_cartes];
    
    // on assigne une à une les cartes mises en jeu au gagnant
    for(int i=0;i<nbr_cartes;i++){
        gagnant->paquet.carte[nbr_cartes_avant+i]=cartes_en_jeu->carte[i];
    }
    
    // on vide le paquet de cartes mises en jeu
    cartes_en_jeu->nombre_cartes=0;
    cartes_en_jeu->carte[0];
    
    // dans le cas d'une partie de mode 'ATOUT', on calcule le score
    if(type_partie==3){
        if(gagnant->atout==couleur_gagnant){
            gagnant->score++;
        }
        if(perdant->atout==couleur_perdant && perdant->score>0){
            perdant->score--;
        }
    }
}

// PRE: -
//POST: renvoit une chaine de caractères, correspondant à l'une des 4 couleurs
//POST: cette fonction sert à déterminer quelle sera la couleur dominante du coup; (+1 à la valeur carte)
char* couleur_dominante(){
    char* resultat;
    char* couleurs[4]={"Pique","Trèfle","Coeur","Carreau"};
    resultat=couleurs[rand()%4];
    return resultat;
}

// PRE: reçoit la référence vers un joueur et vers le paquet de cartes en jeu,
// PRE: ainsi que le nombre de cartes dans ce paquet
//POST: retourne 0 si le joueur n'a plus de carte en main, sinon
//POST: retourne 1 et retire la première carte du paquet du joueur pour l'ajouter au paquet de cartes en jeu
int jouer_carte(joueur_t *joueur, paquet_t *cartes_en_jeu, int id_carte){
    int resultat=1;
    if(joueur->paquet.nombre_cartes<0){
        resultat=0;
    }
    else{
        // on diminue la taille du paquet du joueur et on ajoute sa première carte au paquet mis en jeu
        joueur->paquet.nombre_cartes-=1;
        cartes_en_jeu->carte[id_carte]=joueur->paquet.carte[0];
        for(int i=0;i<joueur->paquet.nombre_cartes;i++){
            joueur->paquet.carte[i]=joueur->paquet.carte[i+1];
        }
    }
    return resultat;
}

// PRE: reçoit la référence vers un joueur et vers le paquet de cartes en jeu,
// PRE: ainsi que le nombre de cartes dans ce paquet et l'id du coup
//POST: retourne 0 si le joueur n'a plus de carte en main,
//POST: retourne 1 et retire la carte du paquet du joueur pour l'ajouter au paquet de cartes en jeu quand le joueur n'a qu'une carte
//POST: retourne 2 et demande au joueur de choisir s'il lui reste au moins 2 cartes
int jouer_carte_multi(joueur_t *joueur, paquet_t *cartes_en_jeu, int id_carte, int id_coup){
    int resultat;
    if(joueur->paquet.nombre_cartes<=0){ // si le joueur n'a plus de carte
        resultat=0; // le joueur a perdu la partie
    }
    else if(joueur->paquet.nombre_cartes==1){ // s'il ne lui reste qu'une carte, elle est sélectionnée d'office
        // on diminue la taille du paquet du joueur et on ajoute sa première carte au paquet mis en jeu
        printf("Coup %i | J%i: Il ne vous reste plus qu'une carte | %s de %s \n",id_coup,joueur->id,joueur->paquet.carte[0].label,joueur->paquet.carte[0].couleur);
        joueur->paquet.nombre_cartes-=1;
        cartes_en_jeu->carte[id_carte]=joueur->paquet.carte[0];
        resultat=1;
    }
    else{
        // on affiche les 2 premières cartes du paquet et on demande au joueur son choix
        int carte_choisie=2;
        while(carte_choisie!=0 && carte_choisie!=1){
            printf("Coup %i | J%i: Choisissez une carte ('1' ou '2'):\n",id_coup,joueur->id);
            printf("-1- %s de %s | -2- %s de %s\n",joueur->paquet.carte[0].label,joueur->paquet.carte[0].couleur,joueur->paquet.carte[1].label,joueur->paquet.carte[1].couleur);
            scanf(" %i",&carte_choisie);
            if(carte_choisie==123){ // CHEAT CODE si un joueur entre 123, les cartes sont mélangées et 2 nouvelles seront tirées
                melanger_paquet(&joueur->paquet);
            }
            carte_choisie-=1; // par simplicité, on stocke 0 ou 1, soit l'id d'un des 2 premiers éléments du tableau de cartes
        }
        
        // on ajoute la carte choisie au paquet mis en jeu.
        cartes_en_jeu->carte[id_carte]=joueur->paquet.carte[carte_choisie];
        // La carte non choisie est remise au bas de sa main
        joueur->paquet.carte[joueur->paquet.nombre_cartes]=joueur->paquet.carte[1-carte_choisie];
        // on diminue la taille du paquet et on recalcule la main du joueur
        joueur->paquet.nombre_cartes-=1;
        for(int i=0;i<joueur->paquet.nombre_cartes;i++){
            joueur->paquet.carte[i]=joueur->paquet.carte[i+2]; // on décalle de 2, les deux premières cartes ayant été soit utilisée soit placée en bas de main
        }
        resultat=2;
    }
    return resultat;
}


// PRE: reçoit en entrée un pointeur vers joueur1, un pointeur vers joueur2,
// PRE: un pointeur vers le paquet de cartes en train d'être jouées, le numéro du coup, le type de partie
// PRE: et le nombre de cartes DEJA dans le paquet mis en jeu (appel à 0 pour les premiers coups, à 4 ou + pour les batailles)
//POST: retourne comme valeur l'id du joueur qui a gagné le coup
//POST: assigne les cartes mises en jeu au joueur qui tire la carte la plus haute
int jouer_coup(joueur_t *joueur1, joueur_t *joueur2, paquet_t *cartes_en_jeu, int nbr_cartes, int id_coup, int type_partie){
    int resultat=0; //renvoie l'id du joueur qui gagne le coup
    cartes_en_jeu->nombre_cartes=nbr_cartes+2;
    cartes_en_jeu->carte[nbr_cartes+2];
    
    int check_joueur1,check_joueur2; // ces 2 variables servent à identifier si un joueur n'a plus assez de cartes (qd = 0)
    // si on joue une partie multicartes avec ou sans atout, on appelle la fonction permettant au joueur de choisir sa carte
    if(type_partie==2 || type_partie==3){
        check_joueur1=jouer_carte_multi(joueur1,cartes_en_jeu,nbr_cartes,id_coup);
        check_joueur2=jouer_carte_multi(joueur2,cartes_en_jeu,nbr_cartes+1,id_coup);
    }
    else{ // si par défaut on joue une partie standard, il ne faut qu'enlever la première carte de la main de chaque joueur
        check_joueur1=jouer_carte(joueur1,cartes_en_jeu,nbr_cartes);
        check_joueur2=jouer_carte(joueur2,cartes_en_jeu,nbr_cartes+1);
    }
    
    if(check_joueur1==0){
        printf("\n\nJoueur%i n'a plus de cartes\n",joueur1->id);
        resultat=joueur2->id; // on renvoie comme résultat l'id du joueur2
    }
    else if(check_joueur2==0){
        printf("\n\nJoueur%i n'a plus de cartes\n",joueur2->id);
        resultat=joueur1->id; // on renvoie comme résultat l'id du joueur1
    }
    else{
        // on calcule la couleur dominante pour ce coup et on incrémente la valeur des cartes de 1 si elles sont de la couleur dominante. Il faut cependant stocker la valeur des cartes dans de nouvelles variables, pour éviter de changer de manière permanente la valeur de la carte
        char* couleur_dom=couleur_dominante();
        int valeur_carte1, valeur_carte2;
        
        if(cartes_en_jeu->carte[nbr_cartes].couleur==couleur_dom){
            valeur_carte1=cartes_en_jeu->carte[nbr_cartes].valeur+1;
        }
        else{
            valeur_carte1=cartes_en_jeu->carte[nbr_cartes].valeur;
        }
        if(cartes_en_jeu->carte[nbr_cartes+1].couleur==couleur_dom){
            valeur_carte2=cartes_en_jeu->carte[nbr_cartes+1].valeur+1;
        }
        else{
            valeur_carte2=cartes_en_jeu->carte[nbr_cartes+1].valeur;
        }
            
        if(valeur_carte1>valeur_carte2){
            // Joueur 1 gagne, il récupère les cartes du paquet mis en jeu
            printf("Coup %i | J%i: %s de %s | J%i: %s de %s | Dominant: %s | Gagnant du coup: J%i (+%i cartes)\n", id_coup, joueur1->id, cartes_en_jeu->carte[nbr_cartes].label, cartes_en_jeu->carte[nbr_cartes].couleur, joueur2->id, cartes_en_jeu->carte[nbr_cartes+1].label, cartes_en_jeu->carte[nbr_cartes+1].couleur, couleur_dom, joueur1->id, nbr_cartes+2);
            gagner_coup(joueur1, cartes_en_jeu->carte[nbr_cartes].couleur, joueur2, cartes_en_jeu->carte[nbr_cartes+1].couleur, cartes_en_jeu, cartes_en_jeu->nombre_cartes,type_partie);
            resultat=joueur1->id;
        }
        else if(valeur_carte1<valeur_carte2){
            // Joueur 2 gagne, il récupère les cartes du paquet mis en jeu
            printf("Coup %i | J%i: %s de %s | J%i: %s de %s | Dominant: %s | Gagnant du coup: J%i (+%i cartes)\n", id_coup, joueur1->id, cartes_en_jeu->carte[nbr_cartes].label, cartes_en_jeu->carte[nbr_cartes].couleur, joueur2->id, cartes_en_jeu->carte[nbr_cartes+1].label, cartes_en_jeu->carte[nbr_cartes+1].couleur, couleur_dom, joueur2->id, nbr_cartes+2);
            gagner_coup(joueur2, cartes_en_jeu->carte[nbr_cartes+1].couleur,joueur1, cartes_en_jeu->carte[nbr_cartes].couleur, cartes_en_jeu, cartes_en_jeu->nombre_cartes,type_partie);
            resultat=joueur2->id;
        }
        else{
            // En cas de bataille, on ajoute 2 cartes "cachées" au paquet mis en jeu
            printf("Coup %i | J%i: %s de %s | J%i: %s de %s | Dominant: %s |  Bataille!\n",id_coup,joueur1->id,cartes_en_jeu->carte[nbr_cartes].label,cartes_en_jeu->carte[nbr_cartes].couleur,joueur2->id,cartes_en_jeu->carte[nbr_cartes+1].label,cartes_en_jeu->carte[nbr_cartes+1].couleur,couleur_dom);
        
            cartes_en_jeu->nombre_cartes+=2;
            cartes_en_jeu->carte[cartes_en_jeu->nombre_cartes];
            
            // on met la carte du joueur1 dans le paquet mis en jeu et on l'enlève de sa main
            int check2_joueur1=jouer_carte(joueur1,cartes_en_jeu,nbr_cartes+2);
            // on met la carte du joueur2 dans le paquet mis en jeu et on l'enlève de sa main
            int check2_joueur2=jouer_carte(joueur1,cartes_en_jeu,nbr_cartes+3);
            // s'il n'a pas été possible d'enlever une carte au paquet d'un des 2 joueurs, la partie est finie
            if(check2_joueur1==0){
                printf("\n\nJoueur%i n'a plus de cartes\n",joueur1->id);
                resultat=joueur2->id;
            }
            else if(check2_joueur2==0){
                printf("\n\nJoueur%i n'a plus de cartes\n",joueur2->id);
                resultat=joueur1->id;
            }
            else{
                // On rejoue ensuite l'équivalent d'un nouveau tour, en tenant compte qu'il y a maintenant 4 cartes de plus en jeu
                jouer_coup(joueur1, joueur2, cartes_en_jeu, nbr_cartes+4 , id_coup, type_partie);
            }
        }
    }
    return resultat;
}

// PRE: reçoit la référence vers 2 joueurs, un paquet de cartes et le nombre de cartes au total
//POST: ne renvoit aucune valeur, mais reset les cartes de chaque joueur ainsi que leur score et ré-initialise le paquet de cartes à distribuer
void redemarer_partie(joueur_t *joueur1,joueur_t *joueur2,paquet_t *paquet,int nbr_cartes){
    joueur1->paquet.nombre_cartes=nbr_cartes/2;
    joueur1->paquet.carte[nbr_cartes/2];
    joueur1->score=0;
    joueur2->paquet.nombre_cartes=nbr_cartes/2;
    joueur2->paquet.carte[nbr_cartes/2];
    joueur2->score=0;
    
    *paquet=creer_paquet(nbr_cartes);
}

// PRE: recoit un paquet de cartes en argument
//POST: ne retourne aucune valeur, mais affiche l'état du paquet de cartes. Utilisé pour debugging.
void afficher_etat_paquet(paquet_t paquet){
    for(int i=0;i<paquet.nombre_cartes;i++){
        printf("Carte %i - %s de %s, de valeur %i\n",i+1,paquet.carte[i].label,paquet.carte[i].couleur,paquet.carte[i].valeur);
    }
}

int main()
{
    srand(time(NULL));
    
    char restart='y'; // variable utilisée pour recommencer une partie
    int partie_id=1; // variable pour identifier le nombre de parties jouées
    int type_partie=0; // variable indiquant le type de partie qu'on va jouer (1 pour mono carte, 2 pour multi-cartes)
    
    // creation des 2 joueurs
    joueur_t joueur1=creer_joueur(1,26);
    joueur_t joueur2=creer_joueur(2,26);
    paquet_t paquet_depart=creer_paquet(52); // création d'un paquet de 52 cartes
    
    // tant qu'on choisit de jouer des parties, le code s'exécute
    while(restart=='y'){
        melanger_paquet(&paquet_depart);
        distribuer_cartes(&paquet_depart, &joueur1, &joueur2);

    // création d'un paquet temporaire pour garder les cartes temporaires
        paquet_t paquet_mis_en_jeu;
        paquet_mis_en_jeu.carte[0];
        int i=0; // compteur pour le nombre de parties jouées
        while(type_partie!=1 && type_partie!=2 && type_partie!=3){
            printf("Quel type de partie souhaitez-vous jouer? Entrez '1' pour classique, '2' pour multicartes ou '3' pour atouts.\n");
            scanf(" %i",&type_partie);
        }
// C'est ici que la partie commence
        printf("\n\n\n");
        if(type_partie==3){
            printf("------ MODE ATOUTS - DEBUT DE LA PARTIE %i ------\n",partie_id);
        }
        else if(type_partie==2){
            printf("------ MODE MULTI-CARTES - DEBUT DE LA PARTIE %i ------\n",partie_id);
        }
        else{
            printf("------ MODE CLASSIQUE - DEBUT DE LA PARTIE %i ------\n",partie_id);
        }
        
        int premier_joueur=1; // variable indiquant quel joueur a la main. Pour le premier coup, il s'agit de J1
        while(joueur1.paquet.nombre_cartes>0 && joueur2.paquet.nombre_cartes>0 && i<NBR_COUPS_MAX){
            if(premier_joueur==1){ //si le joueur 1 a la main
               premier_joueur=jouer_coup(&joueur1, &joueur2, &paquet_mis_en_jeu, 0,i+1,type_partie);
            }
            else{ //si le joueur 2 a la main
                premier_joueur=jouer_coup(&joueur2, &joueur1, &paquet_mis_en_jeu, 0,i+1,type_partie);
            }
            i++;
        }

// Affichage des résultat de la partie
        printf("\n\n\n");
        printf("------ GAGNANT DE LA PARTIE %i ------\n",partie_id);
        if(type_partie==3){
            if(joueur1.score>joueur2.score){
                printf("Joueur 1 remporte la partie avec le score de %i contre %i\n", joueur1.score, joueur2.score);
            }
            else if(joueur1.score<joueur2.score){
                printf("Joueur 2 remporte la partie avec le score de %i contre %i\n", joueur2.score, joueur1.score);
            }
            else{
                printf("Match nul, les deux joueurs ont le même score: %i\n", joueur1.score);
            }
        }
        else{
            if(joueur1.paquet.nombre_cartes>joueur2.paquet.nombre_cartes){
                printf("Joueur 1 remporte la partie - %i cartes contre %i cartes!\n", joueur1.paquet.nombre_cartes, joueur2.paquet.nombre_cartes);
            }
            else if(joueur1.paquet.nombre_cartes<joueur2.paquet.nombre_cartes){
                printf("Joueur 2 remporte la partie - %i cartes contre %i cartes!\n", joueur2.paquet.nombre_cartes, joueur1.paquet.nombre_cartes);
            }
            else{
                printf("Match nul, les deux joueurs ont le même nombre de cartes: %i\n", joueur1.paquet.nombre_cartes);
            }
        }
        partie_id++; // on incrémente partie_id, au cas où une nouvelle partie serait jouée
        type_partie=0; // on change la valeur de type_partie, pour pouvoir changer de mode dans la partie suivante
        redemarer_partie(&joueur1,&joueur2,&paquet_depart,52); // on vide les paquets des 2 joueurs et on crée un nouveau paquet à mélanger
        printf("Entrez 'y' si vous souhaitez jouer une nouvelle partie.\n");
        scanf(" %c",&restart);
    }
    return 0;
}
