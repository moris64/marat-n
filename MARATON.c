# include <stdio.h>
# include <stdlib.h>
# include <string.h>

#define PREGUNTAS 250
#define RESPUESTAS 4
#define PUNTOS 50

int MAXJUGA = 5;

struct jugador{//datos del jugador
	char nombre[50];
	int puntaje ;
};
struct reactivo{//datos de los reactivos
	char categoria[20];
	char pregunta[200];
	char respCorrecta[200];
	char respuestas[RESPUESTAS][200]; // almacena las respuestas
	int contestado ; // es decir si ya paso la pregunta
};

typedef struct jugador jgdr;
typedef struct reactivo reac; //crea el tipo de dato reactivo

reac *PreguntasMaraton(int *numRe);
void gestionarJuego(reac reactivos[], int numReac);
void jugar();
int preguntaAleatoria(int rangoMax);
void instrucciones();

int main(){
	system("COLOR 17");
	printf("\n\n");
	printf("\t\t********************************************\n");
	printf("\t\t**                                        **\n");	
	printf("\t\t**               __       ___  __         **\n");	
	printf("\t\t**   |\\ /|  /\\  |  \\  /\\   |  |  | |\\ |   **\n");	
	printf("\t\t**   | V | |__| |__/ |__|  |  |  | | \\|   **\n");
	printf("\t\t**   |   | |  | |  \\ |  |  |  |__| |  |   **\n");
	printf("\t\t**                                        **\n");	
	printf("\t\t**                                        **\n");	 
	printf("\t\t********************************************\n\n");
	printf("\n\n\t\t");
	system("pause");
	int opc;
	do{
		system("COLOR 0F");		
		system("cls");
		printf("\n\t\t***MENU DEL JUEGO***\n");
		printf(" 1. Maraton de 2 jugadores.\n");
		printf(" 2. Maraton de 4 jugadores.\n");
		printf(" 3. Instrucciones\n");
		printf(" 4. Salir.\n");		
		printf("\n\n elija su opcion: ");
		scanf("%i",&opc);		
		switch(opc){
			case 1://para 2 jugadores y la ignorancia
				MAXJUGA = 3;
				jugar();
				getchar();
				break;
			case 2://para 4 jugadores y la ignorancia
				MAXJUGA = 5;
				jugar();
				getchar();
				break;
			case 3://instrucciones
				system("cls");
				instrucciones();
				system("pause");			
				getchar();
				break;			
			case 4:
				break;
			default:
				system("cls");
				printf("\n opcion no valida \n");
				system("pause");
				break;
		}
	}while( opc != 4);

	return 0;
}

reac *PreguntasMaraton(int *numRe){//carga las preguntas
	char cadena[200];
	char delimitadores[6] = ",\n"; // separa el renglon en categoria,pregunta,respuesta y opciones
	char *ptrTok;//es donde se manda los fracmentos de las preguntas
	FILE *archivo;
	int bandera = 0;//marcar que el proceso de obtener preguntas funciono
	reac preguntas[PREGUNTAS]; // crea arreglo de reactivos con la cantidad de PREGUNTAS
	int indiceP = 0;//cuenta cada pregunta
	int indiceSeparador = 1; // comenzamos a contar desde la pregunta no desde la categoría

	archivo = fopen("preguntas.txt","r");//abre el archivo
	if(archivo == NULL){
		printf("No hay archivo");//avisa que no hay archivo
		return 0;
	}
	while(fgets(cadena, 200 , archivo)!= NULL){
		ptrTok = strtok(cadena, delimitadores ); // strtok framenta los renglones y los almacena en ptrTok
		strcpy(preguntas[indiceP].categoria, ptrTok );//strcpy copia cadenas completas

		while( (ptrTok = strtok( NULL, delimitadores )) != NULL ){
			switch(indiceSeparador){
				case 1: // es una pregunta
				strcpy(preguntas[indiceP].pregunta, ptrTok );
				break;
				case 2: // carga la respuesta correcta
				strcpy(preguntas[indiceP].respCorrecta,ptrTok);
				break;
				case 3: // es la respuesta a la opcion a
				strcpy(preguntas[indiceP].respuestas[0] ,ptrTok );
				break;
				case 4: // es la respuesta a la opcion b
				strcpy(preguntas[indiceP].respuestas[1],ptrTok );
				break;
				case 5: // es la respuesta a la opcion c
				strcpy(preguntas[indiceP].respuestas[2], ptrTok );
				break;
				case 6: // es la respuesta a la opcion c
				strcpy(preguntas[indiceP].respuestas[3], ptrTok );
				break;				
				default:
				printf("\nError en la pregunta %i",indiceP);//dice que pregunta esta mal hecha
				bandera = 0;
			}
			indiceSeparador++;
		}indiceSeparador = 1; // regresamos el indice a 1 para el siguiente reactivo
		indiceP++;
		bandera = 1;
	}
	if(bandera=1){
	system("cls");	
//	printf("\n Numero de preguntas: %d \n ", indiceP);//numero totla de preguntas
	*numRe = indiceP;return &preguntas[0];
	}
return NULL;

}

void gestionarJuego(reac reactivos[], int numReac){
	jgdr participantes[MAXJUGA];
	int indJugador = 0;
	int pregunta;
	int indice;
	char respuesta[200];

	// generamos el jugador por defecto que se llama ignorancia
	strcpy(participantes[0].nombre, "Ignorancia");
	participantes[0].puntaje = 0; // asigna puntaje inicial


	for(indJugador=1;indJugador < MAXJUGA;indJugador++){//carga los nombres de los jugadores
		printf("\n Por favor ingrese el nombre del jugador numero %d \n ", indJugador );
		do{
			gets(participantes[indJugador].nombre);
		}while(strcmp(participantes[indJugador].nombre , " ") == 0 || strcmp(participantes[indJugador].nombre , "\0") == 0 );
		participantes[indJugador].puntaje = 0; // asigna puntaje inicial
	}
	system("cls");
	printf("\n El juego esta listo\n");
	printf("\n Los participantes son: \n");//presenta los jugadores
	for( indJugador = 0 ; indJugador < MAXJUGA ; indJugador++){
		printf("\n El participante numero %d es : %s", indJugador + 1, participantes[indJugador].nombre );
	}
	printf("\n");
	system("pause");
	system("cls");
	do{
		pregunta = preguntaAleatoria(numReac);
		if(reactivos[pregunta].contestado != 1){//indica que la pregunta aun no ha sido contestada
			int inciso = 49; // 1 del ascii	
			for(indJugador=0;indJugador<MAXJUGA;indJugador++){//tabla de puntaje
				printf("\n Participante %s ",participantes[indJugador].nombre);
				printf("\n\t|"); 
				printf("\n\t*----> Puntaje: %d\n",participantes[indJugador].puntaje);
			}
			indJugador = 0;//aqui empieza el juego
			printf("\n La categoria es: %s\n",reactivos[pregunta].categoria);
			printf(" pregunta: %c%s",168, reactivos[pregunta].pregunta);
			printf("\n posibles respuestas: \n");
			for(indice = 0; indice < RESPUESTAS; indice++){
				printf(" %c ) %s \n", inciso, reactivos[pregunta].respuestas[indice]);
				 ++inciso;
			}

			inciso = 49; // vuelve a poner el inciso en 1
		}

		do{ 
			indJugador++;
			printf("\n El turno es para: %s\n", participantes[indJugador].nombre);
			do{
				printf("\n Escriba la letra del inciso para responder\n ");
				scanf("%s",&respuesta);
				getchar();
			}while(respuesta == (int) " " || respuesta == (int) "\0");
			 if(strcmp(reactivos[pregunta].respCorrecta,respuesta)== 0){//compara las cadenas, si es correcto su resultado es 0
				printf("\n respuesta correcta, mas un punto!!");
				printf("\n Punto para %s", participantes[indJugador].nombre);
				participantes[indJugador].puntaje++;
				break; // rompe el cliclo por que la respuesta es correcta y no hay mas que hacer 
			}else{
				if(indJugador == MAXJUGA - 1 ){// el ultimo jugador, quiere decir que nadie contesto bien
					printf("\n respuesta incorrecta!!\n");
					participantes[0].puntaje++; // le suma un punto a ignorancia
					printf(" Punto para %s", participantes[0].nombre);
					break; // rompe el ciclo porque gano el punto la ignorancia
				}
			}				 			
		}while(indJugador < MAXJUGA);
			printf("\n\n ");
			system("pause");
			system("cls");
			if(participantes[indJugador].puntaje == PUNTOS || participantes[0].puntaje == PUNTOS ){
				break;
			}
	}while(1);// al llegar al puntaje maximo se termina el juego
		if(participantes[0].puntaje == PUNTOS) {
			printf("\n El gandador es %s  :(  \n", participantes[0].nombre);	//ignorancia

		}
		else{
			printf("\n\n\t\t !!!El ganador es : %s !!!\n\n",participantes[indJugador].nombre);//cualquier otro
			system("COLOR A0");			
		}
}

void jugar(){//carga el juego
	int numReactivos = 0;
	reac *reactivos;
	if ( (reactivos = PreguntasMaraton( &numReactivos)) != NULL)
	gestionarJuego(reactivos , numReactivos );
}
int preguntaAleatoria(int rangoMax){//selecciona una pregunta al azar
	int M = rangoMax;
	int num;
	srand (time(NULL)* getpid());
	num = 0 + rand() % M ;
return num;
}

void instrucciones(){
	printf("\n ***INSTRUCCIONES***");
	printf("\n - Elige el modo de juego ( de 2 o 4 jugadores).");
	printf("\n - Una vez seleccionado el modo de juego escriba su nombre.");
	printf("\n - Al momento de elegir la opcion coloque el digito de la respuesta.");
	printf("\n - Para saltar la pregunta escriba 'saltar' y presione ENTER.");// en realidad sirve con cualquier palabra pero se escucha mas profesional :v//
	printf("\n - el jugador que llegue primero a %d puntos, gana!!!.\n\n ",PUNTOS);
}
