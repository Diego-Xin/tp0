#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	
	// Usando el logger creado previamente
	log_info(logger,"Hola! Soy un log");
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	// Usando el config creado previamente, leemos los valores del config y los 
	
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip=config_get_string_value(config,"IP"); //la  config funciona como un diccionario?
	puerto=config_get_string_value(config,"PUERTO");
	valor=config_get_string_value(config,"CLAVE");
	// Loggeamos el valor de config
	log_info(logger,"lei el puerto: %s,lei la ip: %s, lei la clave: %s",puerto, ip,valor);
	//se pone %s para poder leer un string
	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion); //el mensaje que enviamos es valor
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	printf("TERMINO EL PROCESO");
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	if((nuevo_logger=log_create("tp0.log","TP0",1,LOG_LEVEL_INFO))== NULL)
		{	//perror("algo paso con el log");
			printf("no se pudo crear el logger");
			exit(1);
		}
	return nuevo_logger;
}

t_config* iniciar_config(void) //leer_config
{
	/*t_config* nuevo_config=config_create("./cliente.config");
	if(nuevoconfig==NULL){
		perror("no se pudo leer config");
		exit(EXIT_FAILURE);
	}*/
	t_config* nuevo_config;
	if((nuevo_config=config_create("./cliente.config"))==NULL){ //path/ruta:cliente.config
		printf("no se pudo leer la configuración");
		exit(1);//exit(2) para saber cual fue el error?
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido; // lo que esribes lo guarda en el char*

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger,">> %s", leido);
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	//while(1) o while(true)
	while(strcmp(leido, "") != 0){//el primer string es: leido
		free(leido); //liberamos la memoria
		leido = readline("> ");
		log_info(logger,">> %s", leido);
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido); //liberamos la memoria
}	
void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	// Leemos y esta vez agregamos las lineas al paquete
	leido = NULL;
	paquete = crear_paquete();
	leido = readline("> ");
	while(strcmp(leido, "") != 0){ //el primer string es: leido
		agregar_a_paquete(paquete,leido,strlen(leido) + 1);
		free(leido); //liberamos la memoria
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	//enviar paquete
	enviar_paquete(paquete,conexion);//el socket_cliente es conexion???
	//eliminar paquete
	eliminar_paquete(paquete);
}
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger!=NULL){
		log_destroy(logger);
	}
	if(config!=NULL){
		config_destroy(config); //destruye la estuctura en memoria, no el archivo 
	}
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
}

